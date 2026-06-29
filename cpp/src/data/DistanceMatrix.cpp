//
// Created by Elahe Amiri on 2026-04-11.
//

#include "DistanceMatrix.h"

int DistanceMatrix::getDistance(const std::string &start, const std::string &end) {
    return matrix_[start][end];
}

void DistanceMatrix::updateDistance(const std::string &start, const std::string &end, int distance) {
    matrix_[start][end] = distance;
}

void DistanceMatrix::insertDistance(const std::string &start, const std::string &end, int distance) {
    matrix_[start][end] = distance;
}
