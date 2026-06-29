//
// Created by Elahe Amiri on 2026-04-11.
//

#include "Parameters.h"


// Constructor and Destructor
Parameters::Parameters(int nbIter, int nbColumn, bool isTruncated, int maxLabel, float MIPGap, int minCapacity) :
        nbIter_(nbIter), nbColumn_(nbColumn), isTruncated_(isTruncated), MaxLabel_(maxLabel), MIPGap_(MIPGap),
        minCapacity_(minCapacity) {}

Parameters::~Parameters() = default;

// Display function
std::string Parameters::toString() const {
    std::stringstream repStr;
    repStr << std::left;
    repStr << "#\t" << std::setw(SET_WLENGTH) << "One iter per epoch " << " = " << nbIter_ << std::endl;
    repStr << "#\t" << std::setw(SET_WLENGTH) << "Column added to MP " << " = " << nbColumn_ << std::endl;
    repStr << "#\t" << std::setw(SET_WLENGTH) << "Use Truncated Labeling " << " = " << boolToString(isTruncated_) << std::endl;
    repStr << "#\t" << std::setw(SET_WLENGTH) << "MaxLabel in Truncating " << " = " << MaxLabel_ << std::endl;
    repStr << "#\t" << std::setw(SET_WLENGTH) << "MIP Gap " << " = " << MIPGap_ << std::endl;
    repStr << "#\t" << std::setw(SET_WLENGTH) << "Min capacity " << " = " << minCapacity_ << std::endl;

    return repStr.str();
}


std::string boolToString(bool value) {
    return value ? "True" : "False";
}