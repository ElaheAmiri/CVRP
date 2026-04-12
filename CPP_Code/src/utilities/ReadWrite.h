//
// Created by Elahe Amiri on 2026-04-11.
//

#ifndef CPP_CODE_READWRITE_H
#define CPP_CODE_READWRITE_H

#include "utilities/types.h"

//-----------------------------------------------------------------------------
//  ReadWrite class
//  functions to read input and write output
//-----------------------------------------------------------------------------

class ReadWrite {
public:
    // Function to read instance data
    static void readInstanceData(const std::string& strDataFile, PInstance &pInstance);
};

#endif //CPP_CODE_READWRITE_H
