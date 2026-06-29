//
// Created by Elahe Amiri on 2026-04-11.
//

#include "solver.h"

#include <fstream>
#include <memory>
#include <sstream>
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

std::string solver::solveCG() {
    // define required variables
    simulationTime_->start();
    PMasterModeler masterModel_ = std::make_shared<MP_Solver>();
    double previousObj;
    int iter = 0;
    std::stringstream repStr;
    std::vector<PRoute> availableRoutes_;



    masterModel_->initializeModel(mainInstance_);
    masterModel_->solveModelLP(mainInstance_);
    repStr << std::left << std::fixed << std::setprecision(1) << std::endl;
    repStr << "Solving the CVRP problem..." << std::endl << std::endl ;

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
        repStr << "LP Objective value: " << masterModel_->lpObjValue_ << std::endl;
        masterModel_->updateModel(mainInstance_);
        masterModel_->solveModelLP(mainInstance_);
        std::cout << "IP Objective value: " << masterModel_->lpObjValue_ << std::endl;
        if (previousObj <= masterModel_->lpObjValue_) {
            break;
        }

    }  // end of CG while
    masterModel_->solveModelInt(mainInstance_, routeSolution_);
    repStr << "IP Objective value: " << masterModel_->objValue_ << std::endl;

    simulationTime_->stop();
    repStr << std::left << std::fixed << std::setprecision(2);
    repStr << "***************************** Solution Results ****************************" << std::endl;
    repStr << std::setw(SET_WLENGTH) << "Total Distance: " << masterModel_->objValue_ << std::endl;
    repStr << std::setw(SET_WLENGTH) << "simulation time: " << simulationTime_->dSinceInit().count() << " (seconds) "<< std::endl;
    repStr << "#" << std::endl;
    repStr << "============================= Final Routes ================================" << std::endl;
    for (auto & routeObj : routeSolution_) {
        repStr << routeObj->toString(mainInstance_->durationMatrix_) << std::endl;
    }
    return repStr.str();
}