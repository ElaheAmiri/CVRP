//
// Created by Elahe Amiri on 2026-04-11.
//

#ifndef CPP_CODE_GRAPH_H
#define CPP_CODE_GRAPH_H


#include "utilities/MyTools.h"
#include "utilities/types.h"

//-----------------------------------------------------------------------------
//  Node class
//  Define Demand Nodes, Depot Node, and Sink Node
//-----------------------------------------------------------------------------


// Defining the properties of the graph nodes
class Node {
public:
    NodeType type_;                         // node type: depot, demand, sink
    std::string locationID_;                // location ID of the node
    int nodeIndex_;                         // index of the node
    int load_;                              // load of the node

    float dual_;
    int nbActiveLabels_;                    // Number of active labels in labeling approach
    std::vector<PLabel> activeLabels_;      // list of active labels

    // Constructor and Destructor
    Node(std::string locationID, NodeType type);

};


//-----------------------------------------------------------------------------
//  Graph class
//  Define the nodes and the graph of the instance
//-----------------------------------------------------------------------------

class Graph {
public:
    int nbNodes_;                            // number of nodes in the graph
    std::vector<PNode> demandNodes_;         // list of demand nodes
    PNode departNode_;                       // depot node of the vehicles
    PNode sink_;                             // sink node of the vehicles

    // Constructor and Destructor
    Graph();
    Graph(PNode &source, PNode &sink);

    // function for adding node to graph
    void addNewNode(const PNode &node);

    // function to reset the graph after each SP
    void resetGraph();

};



#endif //CPP_CODE_GRAPH_H
