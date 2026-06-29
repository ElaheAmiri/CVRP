cmake_minimum_required(VERSION 3.16)

# Thin top-level wrapper. The actual build lives in cpp/; this file just
# forwards into it so the project can be configured from the repository root
# (e.g. `cmake -S . -B build`). The Python baseline in python/ is independent
# and is not part of this build.
project(CVRP CXX)

enable_testing()

add_subdirectory(cpp)
