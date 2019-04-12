//
// Created by Dzordzu on 12.04.2019.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <PageReplacement/Algorithm.h>
#include <vector>

using namespace OperatingSystems::PageReplacement;

void prepareForTest_1(Algorithm &algorithm) {

    std::vector<Page> pages = {0, 1, 2, 3, 4, 5, 6};

    for(Time i=0;i<5;i++) {
        algorithm.addCall(Call{& *pages.begin()+i, 1+i});
    }

    algorithm.addCall(Call{& *pages.begin()+6, 6});

    for(Time i=6;i<0;i--) {
        algorithm.addCall(Call{& *pages.begin()+i, 12-i});
    }
}

TEST(Algorithms, FIFO) {
    OPTIMAL optimal;
    optimal.setFramesAmount(3);
    prepareForTest_1(optimal);

    EXPECT_EQ(optimal.simulate(), 10);
}