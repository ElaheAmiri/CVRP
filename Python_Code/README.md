# CVRP Assessment Baseline

This repository contains a **baseline exact solver** for the SuperQ CVRP take-home instance, organized in a modular project structure so it can later be extended with a **column-generation master problem** and **pricing subproblem**.

## Current status

Implemented now:
- JSON data loading and validation
- Problem/route/solution data models
- Exact baseline solver for the current 2-vehicle instance
- Independent solution validation
- Simple CLI entry point

Left intentionally as placeholders for later:
- `src/master_problem.py`
- `src/pricing.py`

Those two files are scaffolds for the future column-generation implementation.

## Repository structure

```text
repo/
  README.md
  requirements.txt
  data/
    cvrp_problem_data.json
  src/
    data_loader.py
    models.py
    master_problem.py
    pricing.py
    baseline_solver.py
    validator.py
    utils.py
  main.py
```

## Baseline method

The baseline is an **exact 2-vehicle solver** designed for this assignment instance.

It works by:
1. Enumerating feasible customer partitions for vehicle 1,
2. Assigning the complementary customers to vehicle 2,
3. Solving the optimal depot-to-depot order for each partition using Held-Karp dynamic programming,
4. Returning the minimum combined route distance.

This baseline is useful as a benchmark and validation reference before implementing the more general column-generation approach.

## How to run

From the repository root:

```bash
python main.py --input data/cvrp_problem_data.json
```

## Expected result for the provided instance

```text
Truck 1 route: 0 -> 5 -> 14 -> 6 -> 10 -> 9 -> 19 -> 13 -> 4 -> 3 -> 1 -> 0
Truck 1 load: 92 / 100
Truck 1 distance: 106

Truck 2 route: 0 -> 11 -> 12 -> 17 -> 8 -> 7 -> 18 -> 16 -> 15 -> 20 -> 2 -> 0
Truck 2 load: 98 / 100
Truck 2 distance: 109

Total distance: 215
```

## Notes

- The baseline solver currently supports **exactly 2 vehicles**.
- The modular structure is intended to make it easy to replace the baseline logic with a route-based column-generation implementation later.
- Validation is kept separate from solving to make the code easier to trust and extend.
