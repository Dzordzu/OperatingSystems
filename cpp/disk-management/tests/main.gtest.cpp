//
// Created by dzordzu on 09.03.19.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <CppUtils/Logger.hpp>
#include <string>
#include <DiskManagement/DiskManagement.hpp>

using namespace DiskManagement;

TEST(FCFS, Algoritm) {
    FCFSManager manager;

    manager.addDiskCall(DiskCallBuilder::getInstace()
        .setAccelerationTime(8)
        .setIsAsync(false)
        .setTimeToDeadline(45)
        .setPosition(7)
        .build());

    manager.addDiskCall(DiskCallBuilder::getInstace()
        .setAccelerationTime(9)
        .setIsAsync(false)
        .setTimeToDeadline(45)
        .setPosition(4)
        .build());

    manager.addDiskCall(DiskCallBuilder::getInstace()
        .setAccelerationTime(4)
        .setIsAsync(false)
        .setTimeToDeadline(2)
        .setPosition(4)
        .build());

    StandardOutputLogStream * logStream = new StandardOutputLogStream();
    manager.setLogStream(logStream);
    EXPECT_EQ(10, manager.simulate());

    logStream->writeAll();
}


