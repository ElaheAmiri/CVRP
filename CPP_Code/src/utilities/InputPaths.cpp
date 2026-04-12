//
// Created by Elahe Amiri on 2026-04-11.
//

#include "InputPaths.h"

//-----------------------------------------------------------------------------
//  getInputDurationData Path Class
//  Instances of this class contain the paths of the inputs
//-----------------------------------------------------------------------------

InputPaths::InputPaths(std::string  datadir) : dataDir_(std::move(datadir)){
    input_InstanceData_ = "";
    input_paramFile_ = "";
}

void InputPaths::initializeInputs() {

    //initialize the file names for trip records and instance data
    input_InstanceData_ = dataDir_ + "cvrp_problem_data.json";
    input_paramFile_ = dataDir_ + "cvrp_params.json";
}




