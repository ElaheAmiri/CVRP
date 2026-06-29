//
// Created by Elahe Amiri on 2026-04-11.
//

#include "ReadWrite.h"
#include "MyTools.h"
#include <fstream>
#include <string>
#include <vector>
#include <json.hpp>
#include "data/DistanceMatrix.h"
#include "data/Graph.h"
#include "data/Instance.h"

using json = nlohmann::json;

//-----------------------------------------------------------------------------
//  ReadWrite class
//  functions to read input and write output
//-----------------------------------------------------------------------------

void ReadWrite::readInstanceData(const std::string &strDataFile, PInstance &pInstance) {
    std::ifstream file(strDataFile);
    std::cout << "Reading << " << strDataFile << " >>" << std::endl;

    if (!file.is_open()) {
        std::cout << "While trying to read the file " << strDataFile << std::endl;
        throw myTools::myException("The instance data file was not opened properly!", __LINE__);
    }

    json j;
    try {
        file >> j;
    } catch (const json::parse_error &e) {
        std::cout << "JSON parse error: " << e.what() << std::endl;
        throw myTools::myException("Failed to parse instance JSON file!", __LINE__);
    }

    const int nbNodes = j.at("nodes").at("total").get<int>();
    const int depot = j.at("nodes").at("depot").get<int>();
    const int nbVehicles = j.at("vehicles").at("count").get<int>();
    const int capacity = j.at("vehicles").at("capacity_per_vehicle").get<int>();

    const std::vector<int> demands = j.at("demands").get<std::vector<int>>();
    const std::vector<std::vector<int>> distMatrix = j.at("distance_matrix").get<std::vector<std::vector<int>>>();

    if (static_cast<int>(demands.size()) != nbNodes) {
        throw myTools::myException("demands length must match nodes.total", __LINE__);
    }
    if (static_cast<int>(distMatrix.size()) != nbNodes) {
        throw myTools::myException("distance_matrix row count must match nodes.total", __LINE__);
    }
    for (int i = 0; i < nbNodes; ++i) {
        if (static_cast<int>(distMatrix[i].size()) != nbNodes) {
            throw myTools::myException("distance_matrix must be square", __LINE__);
        }
    }

    pInstance->nbVehicles_ = nbVehicles;
    pInstance->vehicleCapacity_ = capacity;

    pInstance->durationMatrix_ = std::make_shared<DistanceMatrix>();
    for (int i = 0; i < nbNodes; ++i) {
        for (int k = 0; k < nbNodes; ++k) {
            const std::string si = std::to_string(i);
            const std::string sk = std::to_string(k);
            pInstance->durationMatrix_->insertDistance(si, sk, distMatrix[i][k]);
        }
    }

    pInstance->graph_ = std::make_shared<Graph>();

    auto depart = std::make_shared<Node>(std::to_string(depot), DEPART_NODE);
    depart->nodeIndex_ = depot;
    depart->load_ = demands[depot];
    pInstance->graph_->addNewNode(depart);

    auto sink = std::make_shared<Node>(std::to_string(depot), SINK_NODE);
    sink->nodeIndex_ = nbNodes;
    pInstance->graph_->addNewNode(sink);

    for (int i = 0; i < nbNodes; ++i) {
        if (i == depot) {
            continue;
        }
        auto cust = std::make_shared<Node>(std::to_string(i), DEMAND_NODE);
        cust->nodeIndex_ = i;
        cust->load_ = demands[i];
        pInstance->graph_->addNewNode(cust);
    }

    std::cout << "Instance data loaded: " << nbNodes << " nodes, " << nbVehicles << " vehicles, capacity "
              << capacity << std::endl;
}

void ReadWrite::readParameters(const std::string& strParamFile, PInstance &pInstance) {
    // open the JSON file
    std::ifstream file(strParamFile);
    std::cout << "Reading << " << strParamFile << " >>" << std::endl;

    if (!file.is_open()) {
        std::cout << "While trying to read the file " << strParamFile << std::endl;
        std::cout << "The input file was not opened properly!" << std::endl;
        throw myTools::myException("The input file was not opened properly!", __LINE__);
    }

    // Parse JSON
    json j;
    try {
        file >> j;
    } catch (const json::parse_error& e) {
        std::cout << "JSON parse error: " << e.what() << std::endl;
        throw myTools::myException("Failed to parse JSON file!", __LINE__);
    }

    // ==================== READ DEFAULT PARAMETERS ====================
    auto defaultParams = j["solverParameters"];

    // Default Parameters (stable parameters that rarely change)
    int nbIter = defaultParams.value("nbIter", 10);
    int nbColumns = defaultParams.value("nb_column", 50);
    bool isTruncated = defaultParams.value("is_truncated", 1) != 0;
    int maxLabel = defaultParams.value("max_label", 15);
    float mipGap = defaultParams.value("mip_gap", 0.001f);
    int minCapacity = defaultParams.value("min_capacity", 0);

    // ==================== CREATE PARAMETERS OBJECT ====================
    pInstance->parameters_ =
            std::make_shared<Parameters>(nbIter, nbColumns, isTruncated, maxLabel, mipGap, minCapacity);


    std::cout << "Parameters loaded successfully "  << std::endl;
}

