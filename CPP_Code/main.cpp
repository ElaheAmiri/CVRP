#include <iostream>
#include <memory>
#include <string>

#include "data/Instance.h"
#include "solver/solver.h"
#include "utilities/InputPaths.h"

int main() {
    std::string dataDir = "dataset/";
    InputPaths inputPaths(dataDir);

    // create output files for epoch results
    inputPaths.initializeInputs();

    // create solver and load instance + parameters from JSON
    std::shared_ptr<solver> instanceSolver = std::make_shared<solver>();
    instanceSolver->createInstanceFile(inputPaths.input_InstanceData_, inputPaths.input_paramFile_);
    std::cout << instanceSolver->mainInstance_->toString();

    std::cout << "Solving the CVRP problem..." << std::endl;
    instanceSolver->solveCG();
}