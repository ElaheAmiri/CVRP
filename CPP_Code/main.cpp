#include <fstream>
#include <iostream>
#include <memory>
#include <string>

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

    // solve and save final outputs
    const std::string finalLog = instanceSolver->solveCG();
    std::ofstream outputFile(inputPaths.output_finalLog_);
    if (outputFile.is_open()) {
        outputFile << instanceSolver->mainInstance_->toString();
        outputFile << finalLog;
    }
    
}