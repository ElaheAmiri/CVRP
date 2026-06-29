//
// Created by Elahe Amiri on 2026-04-12.
//

#ifndef CPP_CODE_MP_SOLVER_H
#define CPP_CODE_MP_SOLVER_H

#include "data/Instance.h"
#include "data/Parameters.h"
#include "data/Route.h"
#include "utilities/MyTools.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace operations_research {
class MPSolver;
class MPVariable;
class MPConstraint;
}

class MP_Solver {
public:
    operations_research::MPSolver* model_{nullptr};                // the model of the master problem (MP)
    std::string outputLog_;                                        // the output log of the MP

    float objValue_{};                                             // the objective value of the MP
    float lpObjValue_{};                                           // the objective value of the LP relaxation of the MP
    std::vector<PRoute> mpRoutes_;                                 // the routes of the master problem
    int iterations_{};                                             // the number of iterations of the master problem

    std::vector<operations_research::MPVariable*> routeVar_;       // route variables of the master problem
    std::vector<operations_research::MPVariable*> zVar_;           // penalty variables for providing feasibility

    std::vector<operations_research::MPConstraint*> nodeConstr_;   // the constraints of the MP to conver demand nodes
    operations_research::MPConstraint* vehicleConstr_{nullptr};    // the constraint of the MP for vehicles limit

    MP_Solver();
    ~MP_Solver();

    int nbNodes_{};
    std::vector<PRoute> routesToAdd_;

    // Attaches a new route variable’s coefficients to the current LP
    void createColumn(const PRoute& route, const PInstance& pInst,
                      operations_research::MPVariable* routeVar);

    // Initializes the model of the MP
    void initializeModel(const PInstance& pInst);


    // Adds route variable to the MP
    void addRouteVar(const PRoute& newRoute, const PInstance& pInst);

    // Adds penalty variable to the MP
    void addZVar(const PNode& node);

    // Updates the model of the MP
    void updateModel(PInstance& pInst);

    // Gets the duals of the LP relaxation of the MP
    void getDuals(const PInstance& pInst);

    // Solves the MP
    int solve();
    void solveModelLP(const PInstance& pInst);
    void solveModelInt(const PInstance& pInst, std::vector<PRoute>& routeSolution);

    // Extracts the solution of the MP
    void extractSolution(const PInstance& pInst, std::vector<PRoute>& routeSolution);

    // Dumps the model of the MP
    void dump_gurobi();

private:
    // Writes a string to a file
    static void writeStringToFile(const std::string& path, const std::string& content);

    // Builds visit counts per demand row (same indexing as \ref nodeConstr_).
    static void countDemandVisits(const PRoute& route, const PInstance& pInst, std::vector<int>& visits);

    void wireRouteToMip(operations_research::MPSolver* mip,
                        const std::vector<operations_research::MPConstraint*>& nodeC,
                        operations_research::MPConstraint* vehC,
                        operations_research::MPVariable* x, const PRoute& route, const PInstance& pInst) const;
};

#endif  // CPP_CODE_MP_SOLVER_H
