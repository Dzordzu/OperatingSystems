//
// Created by Dzordzu on 12.04.2019.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <PageReplacement/Algorithm.h>
#include <vector>

using namespace OperatingSystems::PageReplacement;
using OperatingSystems::CppUtils::StandardOutputLogStream;

void prepareForTest_1(Algorithm &algorithm, std::vector<Page> & pages ) {

    pages = {0, 1, 2, 3, 4, 5, 6};
    std::cout<<(*pages.begin()+2)<<std::endl;

    for(Time i=0;i<5;i++) {
        algorithm.addCall(Call{& *pages.begin()+i, 1+i});
    }

    algorithm.addCall(Call{& *pages.begin()+6, 6});

    for(Time i=0;i<=6;i++) {
        algorithm.addCall(Call{& *pages.begin()+(6-i), 7+i});
    }
}

TEST(Algorithms, FIFO) {
    std::vector<Page> pages;
    StandardOutputLogStream logStream;
    OPTIMAL optimal(&logStream);
    optimal.setFramesAmount(3);
    prepareForTest_1(optimal, pages);
    EXPECT_EQ(optimal.simulate(), 10);
    logStream.writeAll();
}