#include <gtest/gtest.h>

#include <iostream>
#include <memory>
#include <string>

#include "data/DistanceMatrix.h"
#include "data/Graph.h"
#include "data/Route.h"

// Use std::cerr so output is visible when you run the binary directly.
// With CTest, use: ctest -V -R cvrp_tests  (verbose shows stderr/stdout).

namespace {

class RouteTest : public ::testing::Test {
protected:
    void SetUp() override { Route::routeCount_ = 0; }
};

TEST_F(RouteTest, Constructor_AssignsSequentialIds) {
    Route a;
    Route b;
    EXPECT_EQ(a.getRouteId(), 0u);
    EXPECT_EQ(b.getRouteId(), 1u);
    EXPECT_EQ(a.routeSize_, 0u);
    EXPECT_EQ(a.load_, 0);
    EXPECT_EQ(a.totalDistance_, 0);

    std::cerr << "[RouteTest::Constructor_AssignsSequentialIds] "
              << "routeId(a)=" << a.getRouteId() << " routeId(b)=" << b.getRouteId() << "\n";
}

TEST_F(RouteTest, AddNode_FirstStop_NoTravelLeg) {
    auto dm = std::make_shared<DistanceMatrix>();
    dm->insertDistance("0", "0", 0);

    auto depot = std::make_shared<Node>(std::string("0"), DEPART_NODE);
    depot->nodeIndex_ = 0;
    depot->load_ = 0;

    Route r;
    PNode n = depot;
    r.addNode(n, dm);

    EXPECT_EQ(r.routeSize_, 1u);
    EXPECT_EQ(r.load_, 0);
    EXPECT_EQ(r.totalDistance_, 0);
    ASSERT_EQ(r.routeNodes_.size(), 1u);
    EXPECT_EQ(r.routeNodes_[0]->locationID_, "0");

    std::cerr << "[RouteTest::AddNode_FirstStop_NoTravelLeg] "
              << "routeSize=" << r.routeSize_ << " load=" << r.load_
              << " totalDistance=" << r.totalDistance_ << "\n";
}

TEST_F(RouteTest, AddNode_AccumulatesLoadAndDistance) {
    auto dm = std::make_shared<DistanceMatrix>();
    dm->insertDistance("0", "1", 10);
    dm->insertDistance("1", "2", 7);

    auto depot = std::make_shared<Node>(std::string("0"), DEPART_NODE);
    depot->nodeIndex_ = 0;
    depot->load_ = 0;

    auto c1 = std::make_shared<Node>(std::string("1"), DEMAND_NODE);
    c1->nodeIndex_ = 1;
    c1->load_ = 4;

    auto c2 = std::make_shared<Node>(std::string("2"), DEMAND_NODE);
    c2->nodeIndex_ = 2;
    c2->load_ = 3;

    Route r;
    PNode p0 = depot;
    PNode p1 = c1;
    PNode p2 = c2;
    r.addNode(p0, dm);
    r.addNode(p1, dm);
    r.addNode(p2, dm);

    EXPECT_EQ(r.routeSize_, 3u);
    EXPECT_EQ(r.load_, 7);
    EXPECT_EQ(r.totalDistance_, 17);

    std::cerr << "[RouteTest::AddNode_AccumulatesLoadAndDistance] "
              << "routeSize=" << r.routeSize_ << " load=" << r.load_
              << " totalDistance=" << r.totalDistance_ << "\n";
}

TEST_F(RouteTest, ToString_ContainsSummaryFields) {
    auto dm = std::make_shared<DistanceMatrix>();
    dm->insertDistance("0", "0", 0);

    auto depot = std::make_shared<Node>(std::string("0"), DEPART_NODE);
    depot->nodeIndex_ = 0;
    depot->load_ = 0;

    Route r;
    PNode p = depot;
    r.addNode(p, dm);

    const std::string s = r.toString(dm);
    EXPECT_NE(s.find("- Route ID"), std::string::npos);
    EXPECT_NE(s.find("- Route Size"), std::string::npos);
    EXPECT_NE(s.find("- Distance"), std::string::npos);
    EXPECT_NE(s.find("- Load"), std::string::npos);
    EXPECT_NE(s.find("(SOURCE )"), std::string::npos);

    std::cerr << "[RouteTest::ToString_ContainsSummaryFields] "
              << "toString length=" << s.size() << " bytes\n";
}

} // namespace
