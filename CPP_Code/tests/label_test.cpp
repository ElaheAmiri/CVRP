#include <gtest/gtest.h>

#include <iostream>
#include <memory>
#include <string>

#include "data/DistanceMatrix.h"
#include "data/Graph.h"
#include "data/Label.h"
#include "data/Route.h"

// Prints go to stderr — run ./bin/cvrp_tests to see them, or: ctest -V -R cvrp_tests

namespace {

class LabelTest : public ::testing::Test {
protected:
    void SetUp() override {
        Label::labelCount_ = 0;
        Route::routeCount_ = 0;
    }
};

static std::shared_ptr<Node> makeDepot(const std::string &loc = "0") {
    auto n = std::make_shared<Node>(loc, DEPART_NODE);
    n->nodeIndex_ = 0;
    n->load_ = 0;
    n->dual_ = 0.F;
    return n;
}

static std::shared_ptr<Node> makeCustomer(const std::string &loc, int index, int load, float dual = 0.F) {
    auto n = std::make_shared<Node>(loc, DEMAND_NODE);
    n->nodeIndex_ = index;
    n->load_ = load;
    n->dual_ = dual;
    return n;
}

TEST_F(LabelTest, Constructor_SetsInitialState) {
    constexpr int kBits = 5;
    auto depot = makeDepot();
    PNode src = depot;
    Label lab(src, kBits);

    EXPECT_EQ(lab.getLabelId(), 0u);
    EXPECT_EQ(lab.load_, 0);
    EXPECT_EQ(lab.totalDistance_, 0);
    EXPECT_EQ(lab.numVisisted_, 0);
    EXPECT_EQ(lab.status_, ACTIVE);
    EXPECT_EQ(lab.pathNode_.size(), 1u);
    EXPECT_EQ(lab.pathNode_[0], depot.get());
    EXPECT_EQ(lab.elementaryCheck_.size(), static_cast<size_t>(kBits));
    EXPECT_EQ(lab.reducedCost_, 0.F);

    std::cerr << "[LabelTest::Constructor_SetsInitialState] labelId=" << lab.getLabelId()
              << " pathLen=" << lab.pathNode_.size() << " bitsetSize=" << lab.elementaryCheck_.size()
              << "\n";
}

TEST_F(LabelTest, Constructor_CopyGetsNewIdAndCopiesFields) {
    constexpr int kBits = 4;
    auto depot = makeDepot();
    PNode src = depot;
    Label a(src, kBits);
    a.load_ = 3;
    a.totalDistance_ = 9;
    a.numVisisted_ = 2;
    a.reducedCost_ = 1.5F;

    Label b(a);
    EXPECT_EQ(a.getLabelId(), 0u);
    EXPECT_EQ(b.getLabelId(), 1u);
    EXPECT_EQ(b.load_, a.load_);
    EXPECT_EQ(b.totalDistance_, a.totalDistance_);
    EXPECT_EQ(b.numVisisted_, a.numVisisted_);
    EXPECT_EQ(b.reducedCost_, a.reducedCost_);
    EXPECT_EQ(b.pathNode_.size(), a.pathNode_.size());

    std::cerr << "[LabelTest::Constructor_CopyGetsNewIdAndCopiesFields] id(a)=" << a.getLabelId()
              << " id(b)=" << b.getLabelId() << " load(b)=" << b.load_ << "\n";
}

TEST_F(LabelTest, Extend_UpdatesLoadDistanceAndPath) {
    constexpr int kBits = 4;
    auto dm = std::make_shared<DistanceMatrix>();
    dm->insertDistance("0", "1", 10);

    auto depot = makeDepot();
    auto c1 = makeCustomer("1", 1, 4, 2.F);

    PNode src = depot;
    Label lab(src, kBits);

    lab.extend(c1.get(), dm);

    EXPECT_EQ(lab.load_, 4);
    EXPECT_EQ(lab.totalDistance_, 10);
    EXPECT_EQ(lab.pathNode_.size(), 2u);
    EXPECT_EQ(lab.pathNode_.back(), c1.get());
    EXPECT_FLOAT_EQ(lab.reducedCost_, static_cast<float>(10) - 2.F);
    EXPECT_EQ(lab.numVisisted_, 1);
    EXPECT_TRUE(lab.elementaryCheck_.test(1));

    std::cerr << "[LabelTest::Extend_UpdatesLoadDistanceAndPath] load=" << lab.load_
              << " dist=" << lab.totalDistance_ << " rc=" << lab.reducedCost_
              << " visitedFlag=" << lab.numVisisted_ << "\n";
}

TEST_F(LabelTest, IsExtendFeasible_FailsWhenOverCapacity) {
    constexpr int kBits = 4;
    auto depot = makeDepot();
    auto heavy = makeCustomer("1", 1, 100, 0.F);

    PNode src = depot;
    Label lab(src, kBits);

    const bool ok = lab.isExtendFeasible(heavy.get(), 50);
    EXPECT_FALSE(ok);

    std::cerr << "[LabelTest::IsExtendFeasible_FailsWhenOverCapacity] feasible=" << ok
              << " (expect false)\n";
}

TEST_F(LabelTest, IsExtendFeasible_FailsWhenElementaryViolated) {
    constexpr int kBits = 4;
    auto dm = std::make_shared<DistanceMatrix>();
    dm->insertDistance("0", "1", 5);
    dm->insertDistance("1", "1", 0);

    auto depot = makeDepot();
    auto c1 = makeCustomer("1", 1, 2, 0.F);

    PNode src = depot;
    Label lab(src, kBits);
    lab.extend(c1.get(), dm);

    auto c1Again = makeCustomer("1", 1, 1, 0.F);
    const bool ok = lab.isExtendFeasible(c1Again.get(), 100);
    EXPECT_FALSE(ok);

    std::cerr << "[LabelTest::IsExtendFeasible_FailsWhenElementaryViolated] feasible=" << ok
              << " (expect false)\n";
}

TEST_F(LabelTest, IsDominated_ReturnsTrueWhenConditionsHold) {
    constexpr int kBits = 3;
    auto depot = makeDepot();

    PNode src = depot;
    Label worse(src, kBits);
    worse.reducedCost_ = 10.F;
    worse.numVisisted_ = 3;
    worse.load_ = 8;
    worse.elementaryCheck_.reset();
    worse.elementaryCheck_.resize(kBits);
    worse.elementaryCheck_.set(0, true);
    worse.elementaryCheck_.set(1, true);

    Label better(src, kBits);
    better.reducedCost_ = 5.F;
    better.numVisisted_ = 2;
    better.load_ = 5;
    better.elementaryCheck_.reset();
    better.elementaryCheck_.resize(kBits);
    better.elementaryCheck_.set(1, true);

    PLabel pw = std::make_shared<Label>(worse);
    PLabel pb = std::make_shared<Label>(better);

    const bool dom = pw->isDominated(pb);
    EXPECT_TRUE(dom);

    std::cerr << "[LabelTest::IsDominated_ReturnsTrueWhenConditionsHold] worse_dominated_by_better="
              << dom << " (expect true)\n";
}

TEST_F(LabelTest, LabelToRoute_BuildsRouteWithMatchingStops) {
    constexpr int kBits = 4;
    auto dm = std::make_shared<DistanceMatrix>();
    dm->insertDistance("0", "0", 0);
    dm->insertDistance("0", "1", 3);
    dm->insertDistance("1", "1", 0);

    auto depot = makeDepot();
    auto c1 = makeCustomer("1", 1, 2, 0.F);

    PNode src = depot;
    Label lab(src, kBits);
    lab.extend(c1.get(), dm);
    lab.reducedCost_ = 1.25F;

    PRoute route = lab.labelToRoute(dm);
    ASSERT_TRUE(route);
    EXPECT_EQ(route->routeSize_, 2u);
    EXPECT_EQ(route->load_, lab.load_);
    EXPECT_FLOAT_EQ(route->reducedCost_, lab.reducedCost_);

    std::cerr << "[LabelTest::LabelToRoute_BuildsRouteWithMatchingStops] routeSize="
              << route->routeSize_ << " load=" << route->load_ << " rc=" << route->reducedCost_
              << "\n";
}

} // namespace
