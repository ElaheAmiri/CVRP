# Capacitated Vehicle Routing Problem (CVRP)

This project solves the **Capacitated Vehicle Routing Problem (CVRP). The goal is to build vehicle routes that start and end at the depot, satisfy truck capacity limits, visit every customer exactly once, and minimize the total travel distance.

The program is a **practical implementation of a column generation framework** with **heuristic accelerations** in the pricing step. In particular, it uses a labeling-based subproblem with truncation controls to speed up route generation and then solves an integer master problem over the generated columns. Because of these accelerations and the restricted column pool, the method is designed to produce a **high-quality workable solution** efficiently, but it should **not be interpreted as an exact global-optimal CVRP solver**.

## Method Overview

The solution process follows a standard decomposition pattern:

1. Build and solve a restricted master problem (LP relaxation).
2. Extract dual values from the master problem.
3. Solve a pricing subproblem with a labeling algorithm to generate negative reduced-cost routes.
4. Add improving columns to the master problem and repeat.
5. Solve an integer master problem on the generated route set to obtain a final route plan.

## Project Structure

```text
CPP_Code/
├── CMakeLists.txt              # Top-level build configuration
├── main.cpp                    # Program entry point
├── dataset/
│   ├── cvrp_problem_data.json  # CVRP instance data
│   └── cvrp_params.json        # Solver parameters
├── src/
│   ├── data/                   # Core problem data structures
│   ├── solver/                 # Column generation and pricing logic
│   └── utilities/              # Auxiliary functions and helpers, I/O
└── tests/                      # Basic GoogleTest-based tests
```

## Main Components

- `main.cpp`: loads the dataset and parameters, creates the solver, and launches the optimization.
- `src/solver/solver.*`: coordinates the column generation loop.
- `src/solver/MP_Solver.*`: defines and solves the master problem with OR-Tools.
- `src/solver/LabelingSubProblem.*`: pricing subproblem solved by a labeling / dynamic-programming style algorithm.
- `src/data/`: instance, graph, route, label, parameter, and distance-matrix models.
- `src/utilities/`: helper utilities such as input paths, timers, and read/write functions.

## Requirements

To build this project, you need:

- A C++17-compatible compiler
- `cmake` 3.16+
- Boost headers
- Google OR-Tools
- A supported MIP backend for OR-Tools such as `SCIP` or `CBC`

## Quick Start

### 1. Configure and build

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```

The main executable is written to `bin/CVRP_main`.

### 2. Run the solver

From the `CPP_Code` directory:

```bash
./bin/CVRP_main
```

### 3. Run tests

If tests are enabled during configuration (default behavior):

```bash
cd build
ctest --output-on-failure
```

## Input Files

The program reads its inputs from the `dataset/` folder:

- `dataset/cvrp_problem_data.json`: defines the nodes, demands, vehicle count, vehicle capacity, and distance matrix.
- `dataset/cvrp_params.json`: defines solver controls such as maximum iterations, column limit, truncation behavior, label cap, MIP gap, and minimum capacity threshold.

The current sample instance describes:

- 1 depot
- 20 delivery customers
- 2 vehicles
- capacity 100 per vehicle

## Output

When you run the solver, it prints:

- instance information
- progress of the column generation loop
- final total distance
- elapsed runtime
- selected routes in the final solution

The code also exports LP/MPS model snapshots such as `ortools_orig.lp` and `ortools_orig.mps`, which can be useful for debugging the master problem.

## Notes on Solution Quality

This implementation is aimed at **practical performance** rather than full exactness. The use of truncated labeling, bounded iteration settings, and final integer optimization over only the generated route set means the reported answer is best viewed as a **good feasible solution from a heuristic-accelerated column generation pipeline**.

## Future Improvements

Possible next steps for the project include:

- adding benchmark instances beyond the current 20-customer dataset
- reporting detailed iteration statistics
- improving route stabilization and pricing heuristics
- adding stronger automated tests around solver behavior and data loading
