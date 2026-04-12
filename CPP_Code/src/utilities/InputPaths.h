//
// Created by Elahe Amiri on 2026-04-11.
//

#ifndef CPP_CODE_INPUTPATHS_H
#define CPP_CODE_INPUTPATHS_H

#include <string>

//-----------------------------------------------------------------------------
//  InputPaths class
//  Define the input paths of the project
//-----------------------------------------------------------------------------

class InputPaths {
public:
    // input data addresses
    std::string dataDir_;                       // dataset directory
    // input data files
    std::string input_InstanceData_;             // instance data file
    std::string input_paramFile_;                // parameter file

    // output file
    std::string output_finalLog_;

    // Constructors
    explicit InputPaths(std::string datadir);    // constructor

    // Function to initialize input paths
    void initializeInputs();
};




#endif //CPP_CODE_INPUTPATHS_H
