set(LIB_NAME solver)
set(SOURCE_FILES
        solver.cpp
        LabelingSubProblem.cpp)

add_library(${LIB_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${LIB_NAME} PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/.."
        "${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(${LIB_NAME} PRIVATE data utilities)
set_property(TARGET ${LIB_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)
