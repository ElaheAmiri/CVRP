# Capacitated Vehicle Routing Problem (CVRP)

This repository studies the **Capacitated Vehicle Routing Problem (CVRP)** through two complementary implementations. Together, they show both the reference solution quality and the more scalable optimization framework used for practical solving.

The project is organized around two main components:

- `Python_Code/README.md`: an **exact baseline solver**. This version is mainly used as a benchmark and comparison point. Because it solves the given instance exactly, it provides a reference for validating solution quality, but it is not intended to scale well to larger problems.
- `CPP_Code/README.md`: a **column-generation-based framework**. This version is the main algorithmic framework of the project and is designed around decomposition, route generation, and practical solver performance. It is meant to be an extensible approach than the exact baseline.

In other words, the Python implementation serves as the **ground-truth baseline for comparison**, while the C++ implementation serves as the **optimization framework based on column generation**.

See the README inside each folder for the full method description, project structure, build or run instructions, and implementation details for that version.
