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
struct Parameters;
typedef std::shared_ptr<Parameters> PParameters;
class Route;
typedef std::shared_ptr<Route> PRoute;
class Label;
typedef std::shared_ptr<Label> PLabel;
class LabelingSubProblem;
typedef std::shared_ptr<LabelingSubProblem> PLabelingSubPro;

// enums
enum NodeType { DEPART_NODE, SINK_NODE, DEMAND_NODE};
static const char *NodeTypeStr[] = {
    "DEPART ",
    "SINK   ",
    "DEMAND"
};

enum LabelStatus { ACTIVE = 0, DOMINATED = 1, INACTIVE = 2, TERMINATED = 4};

// constant values
static const int SET_WLENGTH = 23;
#endif //CPP_CODE_TYPES_H
