//
// Created by Elahe Amiri on 2026-04-11.
//

#include "solver.h"

#include <memory>
#include "solver/LabelingSubProblem.h"
#include "data/Instance.h"
#include "utilities/ReadWrite.h"

solver::solver() {
    mainInstance_ = std::make_shared<Instance>();
    simulationTime_ = new myTools::Timer(); simulationTime_->init();
}

solver::~solver() {
    delete simulationTime_;
}

void solver::createInstanceFile(const std::string &instanceDataPath, const std::string &paramFilePath) {
    (void)paramFilePath;
    mainInstance_ = std::make_shared<Instance>();
    ReadWrite::readInstanceData(instanceDataPath, mainInstance_);
    ReadWrite::readParameters(paramFilePath, mainInstance_);
}

void solver::updateReducedCosts(PInstance &pInst, std::vector<PRoute> &availableRoutes) {
    for (auto & routeObj : availableRoutes){
        routeObj->reducedCost_ = routeObj->totalDistance_ - pInst->vehicleDual_;
        for (auto & nodeObj: routeObj->routeNodes_){
            routeObj->reducedCost_ -= nodeObj->dual_;
        }
    }
}

void solver::solveCG() {
    // define required variables
    simulationTime_->start();
    PMasterModeler masterModel_ = std::make_shared<MP_Solver>();
    double previousObj;
    int iter = 0;
    std::stringstream changeStr;
    std::vector<PRoute> availableRoutes_;



    masterModel_->initializeModel(mainInstance_);
    masterModel_->solveModelLP(mainInstance_);
    while (true) {
        iter++;
        previousObj = masterModel_->lpObjValue_;


        //*****************************************************************//
        //                    LABELLING SUBPROBLEM
        //*****************************************************************//
        masterModel_->iterations_++;
        PLabelingSubPro subProSolve = std::make_shared<LabelingSubProblem>();
        subProSolve->solveSP(mainInstance_);
        subProSolve->solutionToRoutes(availableRoutes_, mainInstance_);
        updateReducedCosts(mainInstance_, availableRoutes_);
        masterModel_->routesToAdd_.clear();
        for (auto & routeObj : availableRoutes_) {
            if (routeObj->reducedCost_ < -0.1) {
                masterModel_->routesToAdd_.push_back(routeObj);
            }
        }
        if (masterModel_->routesToAdd_.empty()) {
            break;
        }

        //*****************************************************************//
        //                    MASTER PROBLEM
        //*****************************************************************//
        masterModel_->updateModel(mainInstance_);
        masterModel_->solveModelLP(mainInstance_);
    //    std::cout << "Previous Objective: " << previousObj << " LP Objective: "  << masterModel_->lpObjValue_ << std::endl;

        if (previousObj <= masterModel_->lpObjValue_) {
            break;
        }

    }  // end of CG while
    masterModel_->solveModelInt(mainInstance_, routeSolution_);

    simulationTime_->stop();
    std::cout << "Total Distance: " << masterModel_->objValue_ << std::endl;
    std::cout << "simulation time: " << simulationTime_->dSinceInit().count() << std::endl;
    std::cout << "#" <<  std::endl;
    for (auto & routeObj : routeSolution_) {
        std::cout << routeObj->toString(mainInstance_->durationMatrix_) << std::endl;
    }
}