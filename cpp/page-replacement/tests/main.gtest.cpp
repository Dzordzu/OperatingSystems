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
    for(Time i=0;i<5;i++) {
        algorithm.addCall(Call{& *pages.begin()+i, 1+i});
    }
    algorithm.addCall(Call{& *pages.begin()+6, 6});
    for(Time i=0;i<=6;i++) {
        algorithm.addCall(Call{& *pages.begin()+(6-i), 7+i});
    }
}

void prepareForTest_2(Algorithm &algorithm, std::vector<Page> & pages ) {
    pages = {};
    for(int i=0;i<10; i++) pages.emplace_back(i);
    for(Time i=0;i<10;i++) {
        algorithm.addCall(Call{& *pages.begin()+i, 1+i});
    }
    for(Time i=0;i<5;i++) {
        algorithm.addCall(Call{& *pages.begin()+(4-i), 10+i});
    }
}

TEST(Algorithms, OPTIMAL_1) {
    std::vector<Page> pages;
    OPTIMAL optimal;
    optimal.setFramesAmount(3);
    prepareForTest_1(optimal, pages);
    EXPECT_EQ(optimal.simulate(), 10);
}

TEST(Algorithms, OPTIMAL_2) {
    StandardOutputLogStream logStream;
    std::vector<Page> pages;
    OPTIMAL optimal(&logStream);
    optimal.setFramesAmount(5);
    prepareForTest_2(optimal, pages);
    EXPECT_EQ(optimal.simulate(), 6);
    logStream.writeAll();
}

TEST(Algorithms, FIFO_1) {
    std::vector<Page> pages;
    FIFO fifo;
    fifo.setFramesAmount(3);
    prepareForTest_1(fifo, pages);
    EXPECT_EQ(fifo.simulate(), 11);
}

//TEST(Algorithms, LRU_2) {
//    std::vector<Page> pages;
//    FIFO fifo;
//    fifo.setFramesAmount(3);
//    prepareForTest_1(fifo, pages);
//    EXPECT_EQ(fifo.simulate(), 11);
//}