add_executable(cvrp_tests
        smoke_test.cpp
        route_test.cpp)

target_include_directories(cvrp_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(cvrp_tests PRIVATE GTest::gtest_main data)

include(GoogleTest)
gtest_discover_tests(cvrp_tests)
