//
// Created by Elahe Amiri on 2026-04-11.
//

#ifndef CPP_CODE_MYTOOLS_H
#define CPP_CODE_MYTOOLS_H

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <exception>
#include <string>
#include "types.h"
#include <iomanip>
#include <memory>

//-----------------------------------------------------------------------------
//  MyTools class
//  Define the general tools of the project
//-----------------------------------------------------------------------------

namespace myTools {

class myException : public std::exception {
    std::string msg_;

public:
    myException(std::string message, int line)
        : msg_(std::move(message) + " (line " + std::to_string(line) + ")") {}

    const char *what() const noexcept override { return msg_.c_str(); }
};

} // namespace myTools

class MyTools {

};



#endif //CPP_CODE_MYTOOLS_H
