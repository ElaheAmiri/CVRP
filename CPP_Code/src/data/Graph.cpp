//
// Created by Elahe Amiri on 2026-04-11.
//

#include "Graph.h"


//-----------------------------------------------------------------------------
//  Node class
//  Define Demand Nodes, Depot Node, and Sink Node
//-----------------------------------------------------------------------------


Node::Node(std::string locationID, NodeType type) : type_(type),locationID_(std::move(locationID)){
    load_ = 0;
    nodeIndex_ = 0;
    dual_ = 0.0;
    nbActiveLabels_ = 0;
}


//-----------------------------------------------------------------------------
//  Graph class
//  Define the nodes and the graph of the instance
//-----------------------------------------------------------------------------

// Constructor and Destructor
Graph::Graph() {
    nbNodes_ = 0;
}

Graph::Graph(PNode &source, PNode &sink) {
    nbNodes_ = 0;
    addNewNode(source);
    addNewNode(sink);
}

// function for adding node to graph
void Graph::addNewNode(const PNode &node) {
    nbNodes_++;

    switch(node->type_) {
        case DEMAND_NODE:
            demandNodes_.push_back(node);
            break;
        case SINK_NODE :
            sink_ = node;
            break;
        case DEPART_NODE :
            departNode_ = node;
            break;
    }
}
