//
// Created by Elahe Amiri on 2026-04-12.
//

#include "Label.h"


unsigned int Label::labelCount_ = 0;

Label::Label(PNode &source, int nbNodes) : labelID_(labelCount_++) {
    char* name2 = new char[255];
    strncpy(name2, std::to_string(labelID_).c_str(), 255);
    name_ = name2;
    load_ = 0;
    totalDistance_ = 0;
    numVisisted_ = 0;
    pathNode_.push_back(source.get());
    reducedCost_ = 0.0;
    elementaryCheck_.reset();
    elementaryCheck_.resize(nbNodes);
    status_ = ACTIVE;
    LScore_ = 0.0;
}

Label::Label(const Label &label) :labelID_(labelCount_++) {
    char* name2 = new char[255];
    strncpy(name2, std::to_string(labelID_).c_str(), 255);
    name_ = name2;
    load_ = label.load_;
    totalDistance_ = label.totalDistance_;
    numVisisted_ = label.numVisisted_;
    pathNode_ = label.pathNode_;
    reducedCost_ = label.reducedCost_;
    numVisisted_ = label.numVisisted_;
    elementaryCheck_ = label.elementaryCheck_;
    status_ = ACTIVE;
}

Label::~Label() {
    delete[] name_;
}

unsigned int Label::getLabelId() const {
    return labelID_;
}


void Label::extend(Node *outNode, PDistanceMatrix& durationMatrix) {
    load_ += outNode->load_;
    int distance = durationMatrix->getDistance(pathNode_.back()->locationID_, outNode->locationID_);
    totalDistance_ += distance;
    numVisisted_++;
    pathNode_.push_back(outNode);
    reducedCost_ +=  static_cast<float>(distance) - outNode->dual_;
    if (outNode->type_ == DEMAND_NODE)
        elementaryCheck_.set(outNode->nodeIndex_, true);
    LScore_ = reducedCost_ / numVisisted_;
}

// this function check the feasibility of the label before extension
bool Label::isExtendFeasible(Node *outNode, int capacity) const {

    // check tha capacity of vehicle
    if (load_ + outNode->load_ > capacity)
        return false;

    // check elementary condition
    if (elementaryCheck_.test(outNode->nodeIndex_))
        return false;
    return true;
}


bool Label::isDominated(PLabel &otherLabel) const {

    if (this->reducedCost_ >= otherLabel->reducedCost_) {
        if (this->numVisisted_ >= otherLabel->numVisisted_) {
            if (this->load_ >= otherLabel->load_) {
                if ((otherLabel->elementaryCheck_ & this->elementaryCheck_) == otherLabel->elementaryCheck_) {
                    return true;
                }
            }
        }
    }
    return false;
}

PRoute Label::labelToRoute(PDistanceMatrix& distanceMatrix) {
    PRoute newRoute = std::make_shared<Route>();
    newRoute->reducedCost_ = reducedCost_;

    for (Node* raw : pathNode_) {
        PNode nodePtr(raw, [](Node*) {});
        newRoute->addNode(nodePtr, distanceMatrix);
    }

    return newRoute;
}
