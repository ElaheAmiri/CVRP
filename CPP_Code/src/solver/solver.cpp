//
// Created by Elahe Amiri on 2026-04-11.
//

#include "solver.h"

#include <memory>

#include "data/Instance.h"
#include "utilities/ReadWrite.h"

void solver::createInstanceFile(const std::string &instanceDataPath, const std::string &paramFilePath) {
    (void)paramFilePath;
    mainInstance_ = std::make_shared<Instance>();
    ReadWrite::readInstanceData(instanceDataPath, mainInstance_);
}
