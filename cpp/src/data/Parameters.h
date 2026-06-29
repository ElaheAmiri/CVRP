//
// Created by Elahe Amiri on 2026-04-11.
//

#ifndef CPP_CODE_PARAMETERS_H
#define CPP_CODE_PARAMETERS_H

#include "utilities/MyTools.h"

//-----------------------------------------------------------------------------
//  Parameters class
//  Define the parameters of the problem
//-----------------------------------------------------------------------------
class Parameters {
public:
    int nbIter_;                           // number of iterations
    int nbColumn_;                         // number of columns to add in column generation
    bool isTruncated_{};                   // flag for using acceleration strategy
    int MaxLabel_{};                       // maximum number of labels to keep when using truncated labeling
    float MIPGap_{};                       // MIP gap for the optimization
    int minCapacity_{};                    // minimum capacity parameter (solver)

    // Constructor and Destructor
    Parameters(int nbIter, int nbColumn, bool isTruncated, int maxLabel, float MIPGap, int minCapacity);
    virtual ~Parameters();

    // Display functions
    std::string toString() const;  // Converts the parameters to a string
};

// Utility function to convert a boolean value to a string
std::string boolToString(bool value);

#endif //CPP_CODE_PARAMETERS_H
