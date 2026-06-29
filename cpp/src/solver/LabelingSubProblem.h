//
// Created by Elahe Amiri on 2026-04-12.
//

#ifndef CPP_CODE_LABELINGSUBPROBLEM_H
#define CPP_CODE_LABELINGSUBPROBLEM_H

#include "utilities/MyTools.h"
#include "data/Instance.h"
#include "data/Label.h"

//-----------------------------------------------------------------------------
//  Labelling Sub problem class
//  contains the labelling algorithm to solve the subproblem
//-----------------------------------------------------------------------------

class LabelingSubProblem {

    public:
    std::vector<Node*> activeNodes_;            // list of nodes with active labels
    int nbDominated_;                           // number of labels removed via Domination Rules
    int nbGenerated_;                           // number of generated labels


    // Constructor and Destructor
    LabelingSubProblem();
    ~LabelingSubProblem() = default;

    // main function of solving SP using dynamic programming
    bool solveSP(PInstance & pInst);

    // reset the active lists of the nodes, create the first label at the source
    void initialization(PInstance & pInst);

    // function to extend a label to a new node
    void labelExtend(const PLabel &parentLabel, Node *outNode, PDistanceMatrix& durationMatrix);

    // function to check if a new label should be added to the active list of a node
    bool isLabelAdded(PLabel &newLabel, Node *outNode);


    // function to truncate the label list of a node based on the maximum allowed labels
    void truncateLabelList(Node *node, const PInstance &pInst) const;


    // function to convert the solution to routes
    void solutionToRoutes(std::vector<PRoute> &availableRoutes, const PInstance & pInst);

};



#endif //CPP_CODE_LABELINGSUBPROBLEM_H
