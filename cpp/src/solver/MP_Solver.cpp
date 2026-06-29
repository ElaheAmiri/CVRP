//
// Created by Elahe Amiri on 2026-04-12.
//

#include "MP_Solver.h"

#include <cmath>
#include <fstream>
#include <stdexcept>

#include "ortools/linear_solver/linear_solver.h"

using operations_research::MPConstraint;
using operations_research::MPSolver;
using operations_research::MPVariable;
using operations_research::MPSolverParameters;

namespace {

constexpr double kZObjectivePenalty = 100.0;

bool lpStatusOk(MPSolver::ResultStatus st) {
    return st == MPSolver::OPTIMAL || st == MPSolver::FEASIBLE;
}

bool mipStatusOk(MPSolver::ResultStatus st) {
    return st == MPSolver::OPTIMAL || st == MPSolver::FEASIBLE || st == MPSolver::ABNORMAL;
}

MPSolver* createMipSolver() {
    static const char* kNames[] = {"SCIP", "CBC"};
    for (const char* name : kNames) {
        MPSolver* s = MPSolver::CreateSolver(name);
        if (s != nullptr) {
            return s;
        }
    }
    return nullptr;
}

}  // namespace

MP_Solver::MP_Solver() {
    model_ = MPSolver::CreateSolver("GLOP");
    if (model_ == nullptr) {
        throw std::runtime_error("MP_Solver: OR-Tools GLOP linear solver is not available.");
    }
    model_->MutableObjective()->SetMinimization();
    model_->SuppressOutput();

    nbNodes_ = 0;
    objValue_ = 0;
    lpObjValue_ = 0;
    iterations_ = 0;

    mpRoutes_.clear();
    routesToAdd_.clear();
}

MP_Solver::~MP_Solver() {
    delete model_;
}

void MP_Solver::writeStringToFile(const std::string& path, const std::string& content) {
    std::ofstream out(path);
    if (out) {
        out << content;
    }
}

void MP_Solver::countDemandVisits(const PRoute& route, const PInstance& pInst, std::vector<int>& visits) {
    const int departIdx = pInst->graph_->departNode_->nodeIndex_;
    const int sinkIdx = pInst->graph_->sink_->nodeIndex_;
    const size_t nDemand = pInst->graph_->demandNodes_.size();
    visits.assign(nDemand, 0);

    for (const auto& nodeObj : route->routeNodes_) {
        const int ni = nodeObj->nodeIndex_;
        if (ni == departIdx || ni == sinkIdx) {
            continue;
        }
        for (size_t j = 0; j < nDemand; ++j) {
            if (pInst->graph_->demandNodes_[j]->nodeIndex_ == ni) {
                visits[j]++;
                break;
            }
        }
    }
}

void MP_Solver::createColumn(const PRoute& route, const PInstance& pInst, MPVariable* routeVar) {
    vehicleConstr_->SetCoefficient(routeVar, 1.0);

    std::vector<int> visits;
    countDemandVisits(route, pInst, visits);
    for (size_t j = 0; j < visits.size(); ++j) {
        if (visits[j] > 0) {
            nodeConstr_[j]->SetCoefficient(routeVar, static_cast<double>(visits[j]));
        }
    }
}

void MP_Solver::wireRouteToMip(MPSolver* /*mip*/, const std::vector<MPConstraint*>& nodeC, MPConstraint* vehC,
                               MPVariable* x, const PRoute& route, const PInstance& pInst) const {
    vehC->SetCoefficient(x, 1.0);
    std::vector<int> visits;
    countDemandVisits(route, pInst, visits);
    for (size_t j = 0; j < visits.size(); ++j) {
        if (visits[j] > 0) {
            nodeC[j]->SetCoefficient(x, static_cast<double>(visits[j]));
        }
    }
}

void MP_Solver::initializeModel(const PInstance& pInst) {
    nbNodes_ = static_cast<int>(pInst->graph_->demandNodes_.size());

    nodeConstr_.clear();
    for (int i = 0; i < nbNodes_; ++i) {
        nodeConstr_.push_back(model_->MakeRowConstraint(1.0, 1.0, "node_" + std::to_string(i)));
    }

    vehicleConstr_ = model_->MakeRowConstraint(-MPSolver::infinity(), static_cast<double>(pInst->nbVehicles_),
                                               "vehicles");

    for (int i = 0; i < nbNodes_; ++i) {
        addZVar(pInst->graph_->demandNodes_[i]);
    }

    dump_gurobi();
}

void MP_Solver::addRouteVar(const PRoute& newRoute, const PInstance& pInst) {
    MPVariable* var =
            model_->MakeNumVar(0.0, MPSolver::infinity(), "route_" + std::to_string(newRoute->getRouteId()));
    model_->MutableObjective()->SetCoefficient(var, static_cast<double>(newRoute->totalDistance_));
    createColumn(newRoute, pInst, var);
    routeVar_.push_back(var);
}

void MP_Solver::addZVar(const PNode& node) {
    MPVariable* var = model_->MakeNumVar(0.0, MPSolver::infinity(), "z_" + std::to_string(node->nodeIndex_));
    model_->MutableObjective()->SetCoefficient(var, kZObjectivePenalty);
    nodeConstr_[node->nodeIndex_ - 1]->SetCoefficient(var, 1.0);
    zVar_.push_back(var);
}

int MP_Solver::solve() {
    const MPSolver::ResultStatus st = model_->Solve();
    return static_cast<int>(st);
}

void MP_Solver::getDuals(const PInstance& pInst) {
    for (size_t i = 0; i < nodeConstr_.size(); ++i) {
        pInst->graph_->demandNodes_[i]->dual_ = static_cast<float>(nodeConstr_[i]->dual_value());
    }
    pInst->vehicleDual_ = static_cast<float>(vehicleConstr_->dual_value());
}

void MP_Solver::solveModelLP(const PInstance& pInst) {
    (void)pInst;
    for (MPVariable* v : routeVar_) {
        v->SetBounds(0.0, MPSolver::infinity());
    }

    dump_gurobi();

    const MPSolver::ResultStatus status = model_->Solve();

    if (!lpStatusOk(status)) {
        std::cerr << "Failed to optimize the LMP. OR-Tools status: " << status << std::endl;
        throw std::runtime_error("Failed to optimize the LMP");
    }

    lpObjValue_ = static_cast<float>(model_->Objective().Value());
    getDuals(pInst);
}

void MP_Solver::extractSolution(const PInstance& pInst, std::vector<PRoute>& routeSolution) {
    (void)pInst;
    routeSolution.clear();
    for (size_t r = 0; r < routeVar_.size(); ++r) {
        if (routeVar_[r]->solution_value() > 0.5) {
            routeSolution.push_back(mpRoutes_[r]);
        }
    }
}

void MP_Solver::solveModelInt(const PInstance& pInst, std::vector<PRoute>& routeSolution) {
    MPSolver* mip = createMipSolver();
    if (mip == nullptr) {
        throw std::runtime_error("MP_Solver: no MIP backend (SCIP/CBC) available in OR-Tools.");
    }

    mip->MutableObjective()->SetMinimization();
    mip->SuppressOutput();

    MPSolverParameters params;
    params.SetDoubleParam(MPSolverParameters::RELATIVE_MIP_GAP,
                          static_cast<double>(pInst->parameters_->MIPGap_));

    std::vector<MPConstraint*> nc;
    nc.reserve(static_cast<size_t>(nbNodes_));
    for (int i = 0; i < nbNodes_; ++i) {
        nc.push_back(mip->MakeRowConstraint(1.0, 1.0, "mip_node_" + std::to_string(i)));
    }
    MPConstraint* vc =
            mip->MakeRowConstraint(-MPSolver::infinity(), static_cast<double>(pInst->nbVehicles_), "mip_veh");

    for (int i = 0; i < nbNodes_; ++i) {
        MPVariable* z = mip->MakeNumVar(0.0, MPSolver::infinity(), "mip_z_" + std::to_string(i));
        mip->MutableObjective()->SetCoefficient(z, kZObjectivePenalty);
        nc[static_cast<size_t>(i)]->SetCoefficient(z, 1.0);
    }

    std::vector<MPVariable*> mipRouteVars;
    mipRouteVars.reserve(mpRoutes_.size());
    for (const auto& routeObj : mpRoutes_) {
        MPVariable* x = mip->MakeBoolVar("mip_route_" + std::to_string(routeObj->getRouteId()));
        mip->MutableObjective()->SetCoefficient(x, static_cast<double>(routeObj->totalDistance_));
        wireRouteToMip(mip, nc, vc, x, routeObj, pInst);
        mipRouteVars.push_back(x);
    }

    const MPSolver::ResultStatus status = mip->Solve(params);

    if (!mipStatusOk(status)) {
        delete mip;
        throw std::runtime_error("Failed to optimize the MP");
    }

    objValue_ = static_cast<float>(mip->Objective().Value());

    std::vector<MPVariable*> saved = std::move(routeVar_);
    routeVar_ = std::move(mipRouteVars);
    extractSolution(pInst, routeSolution);
    routeVar_ = std::move(saved);

    delete mip;
}

void MP_Solver::updateModel(PInstance& pInst) {
    (void)pInst;
    for (const auto& routeObj : routesToAdd_) {
        addRouteVar(routeObj, pInst);
        mpRoutes_.emplace_back(routeObj);
    }
}

void MP_Solver::dump_gurobi() {
    if (model_ == nullptr) {
        return;
    }
    std::string lp;
    std::string mps;
    if (model_->ExportModelAsLpFormat(false, &lp)) {
        writeStringToFile("ortools_orig.lp", lp);
    }
    if (model_->ExportModelAsMpsFormat(false, false, &mps)) {
        writeStringToFile("ortools_orig.mps", mps);
    }
}
