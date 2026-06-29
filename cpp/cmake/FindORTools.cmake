# FindORTools.cmake
#
# Locates Google OR-Tools and exposes its imported target `ortools::ortools`.
#
# OR-Tools ships a CONFIG package (`ortoolsConfig.cmake`). This MODULE-mode
# finder wraps that config so the project can simply call
# `find_package(ORTools REQUIRED)`. The CamelCase name `ORTools` is deliberately
# distinct from the lowercase CONFIG package `ortools`, so this module never
# shadows the real `ortoolsConfig.cmake` (i.e. no module/config name clash).
#
# It also smooths over a packaging difference around SCIP, the MIP solver
# OR-Tools bundles/depends on: OR-Tools' config does `find_dependency(SCIP)` and
# expects the namespaced target `SCIP::libscip`, but some distributions
# (notably Homebrew) export it only as the unnamespaced `libscip`. We create
# the alias up front when needed.
#
# Search hints:
#   * ORTOOLS_ROOT (CMake variable or environment variable) — root of a
#     prebuilt OR-Tools install (e.g. an extracted release archive).
#   * CMAKE_PREFIX_PATH — honored as usual by the underlying config search.

include(FindPackageHandleStandardArgs)

set(_ortools_hints
    ${ORTOOLS_ROOT}
    $ENV{ORTOOLS_ROOT}
)

# OR-Tools' config requires the SCIP::libscip target. Pull SCIP in first (quietly
# — the config's own find_dependency will locate a bundled copy if the system
# one is absent) and alias the unnamespaced target when that is all we have.
find_package(SCIP QUIET HINTS ${_ortools_hints})
if(TARGET libscip AND NOT TARGET SCIP::libscip)
    add_library(SCIP::libscip ALIAS libscip)
endif()

find_package(ortools CONFIG QUIET HINTS ${_ortools_hints})

find_package_handle_standard_args(ORTools
    REQUIRED_VARS ortools_DIR
    VERSION_VAR ortools_VERSION
)

unset(_ortools_hints)
