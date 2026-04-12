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

When you run the solver, it prints a summary of the solution quality, runtime, and the final selected routes. A sample output is shown below:

```text
***************************** Solution Results ****************************
Total Distance:        221.00
simulation time:       1.64 (seconds)
#
============================= Final Routes ================================
#   - Route ID              : 1410
#   - Route Size            : 12
#   - Distance              : 98
#   - Load                  : 93
#   - Reduced Cost          : 0.0000
# ----------------------------------------------------------------------------
#     Node Type    Location ID  Node IDX   Node Load   Cum. Load   Cum. Dist
# ----------------------------------------------------------------------------
#   1  (SOURCE)        0            0           0           0           0
#   2  (DEMAND)        2            2          12          12           9
#   3  (DEMAND)        20          20           9          21          15
#   4  (DEMAND)        15          15          12          33          26
#   5  (DEMAND)        16          16           6          39          33
#   6  (DEMAND)        14          14           9          48          42
#   7  (DEMAND)        19          19          10          58          50
#   8  (DEMAND)        13          13           5          63          56
#   9  (DEMAND)        4            4          15          78          62
#  10  (DEMAND)        3            3           7          85          70
#  11  (DEMAND)        1            1           8          93          86
#  12  ( SINK )        0           21           0          93          98
==============================================================================

#   - Route ID              : 2371
#   - Route Size            : 12
#   - Distance              : 123
#   - Load                  : 97
#   - Reduced Cost          : 0.0000
# ----------------------------------------------------------------------------
#     Node Type    Location ID  Node IDX   Node Load   Cum. Load   Cum. Dist
# ----------------------------------------------------------------------------
#   1  (SOURCE)        0            0           0           0           0
#   2  (DEMAND)        11          11           7           7          14
#   3  (DEMAND)        12          12          13          20          24
#   4  (DEMAND)        8            8          14          34          39
#   5  (DEMAND)        9            9           8          42          52
#   6  (DEMAND)        10          10          10          52          59
#   7  (DEMAND)        6            6          11          63          66
#   8  (DEMAND)        18          18          11          74          75
#   9  (DEMAND)        7            7           6          80          83
#  10  (DEMAND)        17          17           8          88          97
#  11  (DEMAND)        5            5           9          97         108
#  12  ( SINK )        0           21           0          97         123
==============================================================================
```

The code also exports LP/MPS model snapshots such as `ortools_orig.lp` and `ortools_orig.mps`, which can be useful for debugging the master problem.

## Design Decisions and Assumptions

- The implementation is aimed at **practical performance** rather than full exactness.
- A dynamic-programming-based labeling algorithm is used in the pricing step because it can generate multiple useful columns for the master problem.
- Since the vehicles are homogeneous, the pricing subproblem only needs to be solved once per iteration to generate a sufficient pool of candidate routes.
- This choice also makes it easier to handle richer routing constraints while producing naturally integer route paths.
- The main limitation is scalability, since the number of labels can grow rapidly on larger instances.
- To address that trade-off, the method is combined with heuristic acceleration to keep the solution process practical and flexible.

## Future Improvements

Possible next steps for the project include:

- adding benchmark instances beyond the current 20-customer dataset
- reporting detailed iteration statistics
- improving route stabilization and pricing heuristics
