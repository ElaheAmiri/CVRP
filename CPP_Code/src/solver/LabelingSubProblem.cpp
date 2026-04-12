//
// Created by Elahe Amiri on 2026-04-12.
//

#include "LabelingSubProblem.h"


LabelingSubProblem::LabelingSubProblem() {
    nbGenerated_ = 0;
    nbDominated_ = 0;
}


//***************************************************************************************//
//                           P U S H I N G  S T R A T E G Y
//***************************************************************************************//
bool LabelingSubProblem::solveSP(PInstance & pInst) {
    // create initial label
    initialization(pInst);

    while (!activeNodes_.empty()) {

        // select a node to extend active labels
        Node *currentNode = activeNodes_.back();
        activeNodes_.pop_back();

        // decrease the number of active labels if truncated strategy is used
        if (pInst->parameters_->isTruncated_ && currentNode->nbActiveLabels_ > pInst->parameters_->MaxLabel_){
            truncateLabelList(currentNode, pInst);
        }
        for (size_t j = 0; j < currentNode->activeLabels_.size(); j++) {
            if (currentNode->activeLabels_[j]->status_ == ACTIVE) {
                PLabel selectedLabel = currentNode->activeLabels_[j];
                currentNode->nbActiveLabels_--;
                selectedLabel->status_ = INACTIVE;
                if (selectedLabel->load_ < pInst->vehicleCapacity_) {
                    for (auto &neighbourNode: pInst->graph_->demandNodes_) {
                        if (selectedLabel->isExtendFeasible(neighbourNode.get(), pInst->vehicleCapacity_)) {
                            int nbActive = neighbourNode->nbActiveLabels_;
                            labelExtend(selectedLabel, neighbourNode.get(), pInst->durationMatrix_);
                            if (neighbourNode->nbActiveLabels_ == 1 && nbActive == 0) {
                                activeNodes_.push_back(neighbourNode.get());
                            }
                        }
                    }
                    if (selectedLabel->load_ >= pInst->parameters_->minCapacity_) {
                        labelExtend(selectedLabel, pInst->graph_->sink_.get(), pInst->durationMatrix_);
                    }
                }
            }
        }
    }
    return activeNodes_.empty();
}


void LabelingSubProblem::initialization(PInstance & pInst) {

    // create the initial label at the source and add the source to the list active nodes
    PLabel initialLabel;
    initialLabel = std::make_shared<Label>(pInst->graph_->departNode_, pInst->graph_->nbNodes_);
    initialLabel->reducedCost_ -= pInst->vehicleDual_;

    initialLabel->pathNode_.back()->nbActiveLabels_++;
    activeNodes_.clear();
    activeNodes_.push_back(initialLabel->pathNode_.back());
    initialLabel->pathNode_.back()->activeLabels_.push_back(std::move(initialLabel));
}


void LabelingSubProblem::labelExtend(const PLabel &parentLabel, Node *outNode, PDistanceMatrix& durationMatrix) {
    PLabel newLabel = std::make_shared<Label>(*parentLabel);
    newLabel->extend(outNode, durationMatrix);
    nbGenerated_++;
    if (!isLabelAdded(newLabel, outNode))
        nbDominated_++;
}

bool LabelingSubProblem::isLabelAdded(PLabel &newLabel, Node *outNode) {
    // check the dominance state of the new label
    for (auto &labelObj: outNode->activeLabels_) {
        if (newLabel->isDominated(labelObj)) {
            newLabel->status_ = DOMINATED;
            return false;
        }
    }
    // remove previous dominated labels
    for (int i = outNode->activeLabels_.size() - 1; i >= 0; i--) {
        if (outNode->activeLabels_[i]->isDominated(newLabel)) {
            if (outNode->activeLabels_[i]->status_ == ACTIVE) {
                outNode->nbActiveLabels_--;
            }
            outNode->activeLabels_[i]->status_ = DOMINATED;
            outNode->activeLabels_.erase(outNode->activeLabels_.begin() + i);
            this->nbDominated_++;
        }
    }

    if (outNode->type_ == SINK_NODE){
        newLabel->status_ = TERMINATED;
        outNode->activeLabels_.push_back(std::move(newLabel));

    }
    else{
        outNode->activeLabels_.push_back(newLabel);
        outNode->nbActiveLabels_++;
    }
    return true;
}


void LabelingSubProblem::truncateLabelList(Node *node, const PInstance &pInst) const {
    std::stable_sort(node->activeLabels_.begin(),node->activeLabels_.end(),[](const PLabel &lhs, const PLabel &rhs){
                return lhs->LScore_ < rhs->LScore_;});

    for (int i = node->activeLabels_.size()-1; i >=0; i--){
        if (node->nbActiveLabels_ <= pInst->parameters_->MaxLabel_)
            break;
        if (node->activeLabels_[i]->status_ == ACTIVE){
            node->nbActiveLabels_--;
            node->activeLabels_[i]->status_ = DOMINATED;
            node->activeLabels_.erase(node->activeLabels_.begin() + i);
        }
    }
}


void LabelingSubProblem::solutionToRoutes(std::vector<PRoute> &availableRoutes, const PInstance &pInst) {

    for (auto & labelObj : pInst->graph_->sink_->activeLabels_) {
        PRoute newRoute = labelObj->labelToRoute(pInst->durationMatrix_);
        availableRoutes.emplace_back(std::move(newRoute));
    }

    pInst->graph_->resetGraph();
}

