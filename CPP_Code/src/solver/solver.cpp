//
// Created by Elahe Amiri on 2026-04-11.
//

#include "solver.h"

#include <memory>
#include "solver/LabelingSubProblem.h"
#include "data/Instance.h"
#include "utilities/ReadWrite.h"

void solver::createInstanceFile(const std::string &instanceDataPath, const std::string &paramFilePath) {
    (void)paramFilePath;
    mainInstance_ = std::make_shared<Instance>();
    ReadWrite::readInstanceData(instanceDataPath, mainInstance_);
    ReadWrite::readParameters(paramFilePath, mainInstance_);
}

void solver::solveCG() {
    std::vector<PRoute> availableRoutes_;

    // test labeling algorithm
    PLabelingSubPro subProSolve = std::make_shared<LabelingSubProblem>();
    subProSolve->solveSP(mainInstance_);
    subProSolve->solutionToRoutes(availableRoutes_, mainInstance_);
    std::cout << availableRoutes_[0]->toString(mainInstance_->durationMatrix_) << std::endl;

}
