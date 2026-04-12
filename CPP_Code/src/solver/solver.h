//
// Created by Elahe Amiri on 2026-04-11.
//

#ifndef CPP_CODE_SOLVER_H
#define CPP_CODE_SOLVER_H


#include "data/Graph.h"
#include "utilities/MyTools.h"
#include "utilities/types.h"
#include "solver/MP_Solver.h"

//-----------------------------------------------------------------------------
//  Solver class
//  Define the solver of the problem
//-----------------------------------------------------------------------------

class solver {
public:
    PInstance mainInstance_;                                // main instance of the problem
    myTools::Timer *simulationTime_;
    std::vector<PRoute> routeSolution_;


    // Constructor and Destructor
    solver();
    ~solver();

    // Function to create instance file
    void createInstanceFile(const std::string &instanceDataPath, const std::string &paramFilePath);

    // Function to update the reduced costs of the available routes
    void updateReducedCosts(PInstance &pInst, std::vector<PRoute> &availableRoutes);

    // Function to solve the CVRP problem with the CG algorithm
    void solveCG();
};



#endif //CPP_CODE_SOLVER_H
