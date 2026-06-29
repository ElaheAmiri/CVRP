# Capacitated Vehicle Routing Problem (CVRP)

[![build-and-test](https://github.com/ElaheAmiri/CVRP/actions/workflows/build-and-test.yml/badge.svg)](https://github.com/ElaheAmiri/CVRP/actions/workflows/build-and-test.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

This repository studies the **Capacitated Vehicle Routing Problem (CVRP)** through two complementary implementations. Together, they show both the reference solution quality and the more scalable optimization framework used for practical solving.

The project is organized around two main components:

- `python/README.md`: an **exact baseline solver**. This version is mainly used as a benchmark and comparison point. Because it solves the given instance exactly, it provides a reference for validating solution quality, but it is not intended to scale well to larger problems.
- `cpp/README.md`: a **column-generation-based framework**. This version is the main algorithmic framework of the project and is designed around decomposition, route generation, and practical solver performance. It is meant to be an extensible approach than the exact baseline.

In other words, the Python implementation serves as the **ground-truth baseline for comparison**, while the C++ implementation serves as the **optimization framework based on column generation**.

## Repository Structure

```text
CVRP/
├── README.md
├── LICENSE
├── .github/
│   └── workflows/
│       └── build-and-test.yml
├── python/
│   ├── README.md
│   ├── main.py
│   ├── requirements.txt
│   ├── data/
│   └── src/
└── cpp/
    ├── README.md
    ├── CMakeLists.txt
    ├── cmake/
    │   └── FindORTools.cmake
    ├── main.cpp
    ├── dataset/
    ├── src/
    └── tests/
```

The top-level repository connects these two implementations: `python/` contains the exact comparison baseline, while `cpp/` contains the column-generation framework and its supporting solver components.

See the README inside each folder for the full method description, project structure, build or run instructions, and implementation details for that version.

## Continuous Integration

The [`build-and-test`](.github/workflows/build-and-test.yml) GitHub Actions workflow builds the C++ project with CMake on Ubuntu and macOS — installing Boost and Google OR-Tools (which bundles the SCIP MIP solver) — and runs the GoogleTest suite via `ctest`. A separate job installs the Python baseline's `requirements.txt` and runs the solver.

## License

This project is licensed under the [MIT License](LICENSE) © 2026 Elahe Amiri.
