//
// Created by Elahe Amiri on 2026-04-11.
//

#ifndef CPP_CODE_TYPES_H
#define CPP_CODE_TYPES_H

#include <memory>
#include <iostream>

// useful types
class DistanceMatrix;
typedef std::shared_ptr<DistanceMatrix> PDistanceMatrix;
class Node;
typedef std::shared_ptr<Node> PNode;
class Graph;
typedef std::shared_ptr<Graph> PGraph;
class Instance;
typedef std::shared_ptr<Instance> PInstance;

// enums
enum NodeType { DEPART_NODE, SINK_NODE, DEMAND_NODE};
static const char *NodeTypeStr[] = {
    "DEPART ",
    "SINK   ",
    "DEMAND"
};

#endif //CPP_CODE_TYPES_H
