//
// Created by Elahe Amiri on 2026-04-11.
//

#ifndef CPP_CODE_DISTANCEMATRIX_H
#define CPP_CODE_DISTANCEMATRIX_H

#include <unordered_map>
#include <string>

//-----------------------------------------------------------------------------
//  Distance Matrix class
//  Define the distance matrix of the instance
//-----------------------------------------------------------------------------

class DistanceMatrix {
    std::unordered_map<std::string, std::unordered_map<std::string, int>> matrix_;          // matrix of distances between locations

public:
    // Function to get distance based on start and end locations
    int getDistance(const std::string &start, const std::string &end);

    // Function to update distance based on start and end locations
    void updateDistance(const std::string& start, const std::string& end, int distance);

    // Function to insert distance based on start and end locations
    void insertDistance(const std::string& start, const std::string& end, int distance);
};



#endif //CPP_CODE_DISTANCEMATRIX_H
