//
// Created by Elahe Amiri on 2026-04-11.
//

#include "Instance.h"

//-----------------------------------------------------------------------------
//  Instance class
//  contains the instance data including vehicle info and demand nodes
//-----------------------------------------------------------------------------

Instance::Instance() {
    nbVehicles_ = 0;
    vehicleCapacity_ = 0;
    graph_ = std::make_shared<Graph>();
    durationMatrix_ = std::make_shared<DistanceMatrix>();
    vehicleDual_ = 0.0;
}


// Display function
std::string Instance::toString() const {
    std::ostringstream repStr;
    repStr << "******************************* Instance Info *****************************" << std::endl;
    repStr << std::left;
    repStr << "#\t" << std::setw(SET_WLENGTH) << "Number of Trucks: " << nbVehicles_ << "\n";
    repStr << "#\t" << std::setw(SET_WLENGTH) << "Truck Capacity: " << vehicleCapacity_ << "\n";
    if (graph_) {
        repStr << "#\t" << std::setw(SET_WLENGTH) << "Customer Nodes: " << graph_->demandNodes_.size() << "\n";
    }
    if (graph_ && durationMatrix_) {
        const int n = static_cast<int>(graph_->demandNodes_.size()) + 1;
        repStr << "#\t" << std::setw(SET_WLENGTH)<< "Distance Matrix: " << n << " x " << n << "\n";
    }
    repStr << "------------------------- PARAMETERS AND OPTIONS ---------- ---------------" << std::endl;
    repStr << parameters_->toString();
    repStr << "***************************************************************************" << std::endl;
    return repStr.str();
}

