# Capacitated Vehicle Routing Problem (Baseline)

This repository contains an **exact baseline solver** for the CVRP.  
Although this method provide exact solution, it is **not practically scalable** and is **not intended as the final solution approach**. Instead, it is used as a **benchmark and comparison point** for the later **column-generation implementation**.

## Repository structure

```text
Python_Code/
  README.md
  requirements.txt
  data/
    cvrp_problem_data.json
  src/
    baseline_solver.py
    data_loader.py
    models.py
    utils.py
    validator.py
  main.py
```

## Baseline Method Overview

The baseline is an **exact 2-vehicle solver** with two steps:

1. split customers between the two trucks
2. solve the best route for each truck

With $n$ customers, there are up to $2^n$ possible assignments. For the current instance with $20$ customers, that is $2^{20} = 1,048,576$ candidate partitions. The solver represents each partition with a **bitmask** and prunes many cases early using:

- **symmetry reduction**, since a partition and its complement are equivalent up to truck labels
- **capacity feasibility**, by discarding any partition that overloads a truck

For every remaining feasible partition, the solver computes the optimal depot-to-depot route for each truck using **Held-Karp dynamic programming**. Since it checks all feasible partitions and solves each induced route optimally, the baseline returns a **globally optimal solution** for this 2-truck instance.

## How to run

From the repository root:

```bash
python main.py
```

## Expected result for the provided instance

```text
============================= Final Routes ================================
# ----------------------------------------------------------------------------
#	- Route ID              : 1
#	- Route Size            : 12
#	- Distance              : 106
#	- Load                  : 92
# ----------------------------------------------------------------------------
#     Node Type    Location ID  Node IDX   Node Load   Cum. Load   Cum. Dist
# ----------------------------------------------------------------------------
#   1  (SOURCE)            0         0           0           0           0
#   2  (DEMAND)            5         5           9           9          15
#   3  (DEMAND)           14        14           9          18          28
#   4  (DEMAND)            6         6          11          29          35
#   5  (DEMAND)           10        10          10          39          42
#   6  (DEMAND)            9         9           8          47          49
#   7  (DEMAND)           19        19          10          57          58
#   8  (DEMAND)           13        13           5          62          64
#   9  (DEMAND)            4         4          15          77          70
#  10  (DEMAND)            3         3           7          84          78
#  11  (DEMAND)            1         1           8          92          94
#  12  ( SINK )            0        21           0          92         106
==============================================================================

# ----------------------------------------------------------------------------
#	- Route ID              : 2
#	- Route Size            : 12
#	- Distance              : 109
#	- Load                  : 98
# ----------------------------------------------------------------------------
#     Node Type    Location ID  Node IDX   Node Load   Cum. Load   Cum. Dist
# ----------------------------------------------------------------------------
#   1  (SOURCE)            0         0           0           0           0
#   2  (DEMAND)           11        11           7           7          14
#   3  (DEMAND)           12        12          13          20          24
#   4  (DEMAND)           17        17           8          28          37
#   5  (DEMAND)            8         8          14          42          48
#   6  (DEMAND)            7         7           6          48          59
#   7  (DEMAND)           18        18          11          59          67
#   8  (DEMAND)           16        16           6          65          76
#   9  (DEMAND)           15        15          12          77          83
#  10  (DEMAND)           20        20           9          86          94
#  11  (DEMAND)            2         2          12          98         100
#  12  ( SINK )            0        21           0          98         109
==============================================================================

Total distance: 215
Runtime: 21.1702 seconds
```

## Notes

- This baseline solver currently supports **exactly 2 vehicles**.
- Although the method is **exact**, it is **not scalable** and is therefore not suitable as the final approach for larger CVRP instances.
- The main purpose of this solver is to provide a **reference optimum** for the current instance and a **comparison benchmark** for the future column-generation method.
- Validation is kept separate from solving to make the code easier to verify and compare across methods.