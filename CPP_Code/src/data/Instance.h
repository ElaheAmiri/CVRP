//
// Created by Elahe Amiri on 2026-04-11.
//

#ifndef CPP_CODE_INSTANCE_H
#define CPP_CODE_INSTANCE_H

#include "data/DistanceMatrix.h"
#include "data/Graph.h"
#include "utilities/MyTools.h"
#include "data/Parameters.h"

//-----------------------------------------------------------------------------
// Instance class
// Represents a problem instance with vehicle and demand node information.
//-----------------------------------------------------------------------------

class Instance {
public:
    int nbVehicles_;                                    // Number of vehicles
    int vehicleCapacity_;                               // Capacity of vehicles
    PGraph graph_;                                      // Graph representation of the instance
    PDistanceMatrix durationMatrix_;                    // Matrix containing distance between locations
    PParameters parameters_;                            // Parameters for solving the instance


public:
    // Constructors and Destructor
    Instance();

    // Display function to convert instance data to string format
    std::string toString();

};



#endif //CPP_CODE_INSTANCE_H
