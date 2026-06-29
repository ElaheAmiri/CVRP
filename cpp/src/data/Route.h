//
// Created by Elahe Amiri on 2026-04-12.
//

#ifndef CPP_CODE_ROUTE_H
#define CPP_CODE_ROUTE_H

#include "Instance.h"

//---------------------------------------------------------------------------------------------
//  Route class
//---------------------------------------------------------------------------------------------

class Route {
    const unsigned int routeID_;                // variable name or route ID
public:
    static unsigned int routeCount_;            // Counter the number of routes generated
    const char* name_;                          // name of the route
    int totalDistance_;                         // total distance of the route
    unsigned int routeSize_;                    // size of the route (number of stops)
    std::vector<PNode> routeNodes_;             // nodes of the route
    int load_;                                  // total load of the route
    float reducedCost_;                        // reduced cost of the route


    // Constructor and Destructor
    explicit Route();
    virtual ~Route();

    // Getters
    [[nodiscard]] unsigned int getRouteId() const;

    // these functions are used to add nodes to the routes
    void addNode(PNode &node, PDistanceMatrix& durationMatrix);

    // Display function
    [[nodiscard]] std::string toString(PDistanceMatrix durationMatrix) const;
};



#endif //CPP_CODE_ROUTE_H
