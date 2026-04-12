//
// Created by Elahe Amiri on 2026-04-12.
//

#include "Route.h"

#include <cstring>
#include <iomanip>
#include <sstream>

//---------------------------------------------------------------------------------------------
//  Route class
//---------------------------------------------------------------------------------------------
unsigned int Route::routeCount_ = 0;

// Constructor and Destructor
Route::Route() : routeID_(routeCount_++) {
    totalDistance_ = 0;
    routeSize_ = 0;
    char* name2 = new char[255];
    strncpy(name2, std::to_string(routeID_).c_str(), 255);
    name_ = name2;
    load_ = 0;
}

Route::~Route(){
    delete[] name_;
}

// Getters and Setters
unsigned int Route::getRouteId() const {return routeID_;}


void Route::addNode(PNode &node, PDistanceMatrix& durationMatrix) {
    if (!routeNodes_.empty()) {
        totalDistance_ += durationMatrix->getDistance(routeNodes_.back()->locationID_, node->locationID_);
    }
    ++routeSize_;
    routeNodes_.push_back(node);
    load_ += node->load_;
}

std::string Route::toString(PDistanceMatrix durationMatrix) const {
    std::stringstream repStr;
    repStr << std::fixed << std::setprecision(4);

    repStr << std::left;
    repStr << "#\t" << std::setw(SET_WLENGTH) << "- Route ID" << " : " << routeID_ << std::endl;
    repStr << "#\t" << std::setw(SET_WLENGTH) << "- Route Size" << " : " << routeSize_ << std::endl;
    repStr << "#\t" << std::setw(SET_WLENGTH) << "- Distance" << " : " << totalDistance_ << std::endl;
    repStr << "#\t" << std::setw(SET_WLENGTH) << "- Load" << " : " << load_ << std::endl;
    repStr << "#" << std::endl;

    repStr << "# ------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
    repStr << std::left << std::setw(6) << "#   ";
    repStr << std::left << std::setw(15) << " Node Type";
    repStr << std::left << std::setw(12) << " Location ID";
    repStr << std::right << std::setw(10) << "Node IDX";
    repStr << std::right << std::setw(12) << " Node Load";
    repStr << std::right << std::setw(14) << " Cum. Load";
    repStr << std::right << std::setw(14) << " Cum. Disct";
    repStr << "# ------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

    int cumLoad = 0;
    int cumDist = 0;
    for (size_t i = 0; i < routeNodes_.size(); ++i) {
        const PNode& n = routeNodes_[i];
        cumLoad += n->load_;
        if (i > 0 && durationMatrix) {
            cumDist += durationMatrix->getDistance(routeNodes_[i - 1]->locationID_, n->locationID_);
        }
        repStr << "#" << std::setw(4) << (i + 1) << "  ";

        if (n->type_ == DEPART_NODE) {
            repStr << std::left << std::setw(15) << "(SOURCE )";
        } else if (n->type_ == SINK_NODE) {
            repStr << std::left << std::setw(15) << "(SINK   )";
        } else
            repStr << std::left << std::setw(15) << "(DEMAND )";

        repStr << std::left << std::setw(12) << n->locationID_;
        repStr << std::right << std::setw(10) << n->nodeIndex_;
        repStr << std::right << std::setw(12) << n->load_;
        repStr << std::right << std::setw(14) << cumLoad;
        repStr << std::right << std::setw(14) << cumDist;
    }
    repStr << "========================================================================================================================================================" << std::endl;
    return repStr.str();
}

