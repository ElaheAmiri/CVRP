//
// Created by Elahe Amiri on 2026-04-12.
//

#ifndef CPP_CODE_LABEL_H
#define CPP_CODE_LABEL_H

#include <boost/dynamic_bitset.hpp>
#include "utilities/MyTools.h"
#include "Instance.h"
#include "Route.h"

//-----------------------------------------------------------------------------
// Label class
// This class is used to store information about a partial path in label setting.
//-----------------------------------------------------------------------------

class Label {
    const unsigned int labelID_;              // Unique label ID

public:
    static unsigned int labelCount_;          // Counter to keep track of the number of labels created
    const char* name_;                        // Name of the label
    int load_;                                // Consumed capacity of the vehicle
    int totalDistance_;                       // Total distance of the path
    int numVisisted_;                         // Number of visited nodes
    std::vector<Node*> pathNode_;             // Sequence of nodes in the path
    float reducedCost_;                       // Reduced cost of the path
    boost::dynamic_bitset<> elementaryCheck_; // Checks the elementary condition of the path using a bitset
    LabelStatus status_;                      // Current status of the label (ACTIVE, DOMINATED, INACTIVE, TERMINATED)
    float LScore_;


    // Constructor and Destructor
    Label(PNode &source, int nbNodes);        // Constructor initializing with vehicle and source node
    Label(const Label &label);                // Copy constructor
    ~Label();

    // Getters and Setters
    [[nodiscard]] unsigned int getLabelId() const;

    // Method to extend the label to an outgoing node
    void extend(Node *outNode, PDistanceMatrix& durationMatrix);

    // Checks if extension to a new node is feasible
    bool isExtendFeasible(Node *outNode, int capacity) const;

    // Checks if the current label is dominated by another label
    bool isDominated(PLabel &otherLabel) const;

    // Converts the label to a route
    PRoute labelToRoute(PDistanceMatrix& distanceMatrix);
};



#endif //CPP_CODE_LABEL_H
