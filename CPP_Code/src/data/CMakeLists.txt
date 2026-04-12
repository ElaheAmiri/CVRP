set(LIB_NAME data)
set(SOURCE_FILES
        DistanceMatrix.cpp
        Graph.cpp
        Instance.cpp
        Parameters.cpp)

add_library(${LIB_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${LIB_NAME} PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/.."
        "${CMAKE_CURRENT_LIST_DIR}")
set_property(TARGET ${LIB_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)
