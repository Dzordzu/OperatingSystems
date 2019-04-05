//
// Created by dzordzu on 09.03.19.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <CppUtils/Logger.hpp>
#include <string>
#include <DiskManagement/DiskManager.hpp>

using namespace DiskManagement;

TEST(Algorithm, FCFS) {
    FCFSManager manager(DiskBuilder::getInstance()
        .setArmPosition(0));

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

//    logStream->writeAll();
}


TEST(Algorithm, SSTF) {
    SSTFManager manager;

    manager.addDiskCall(DiskCallBuilder::getInstace()
                                .setAccelerationTime(0)
                                .setIsAsync(false)
                                .setTimeToDeadline(45)
                                .setPosition(24)
                                .build());

    manager.addDiskCall(DiskCallBuilder::getInstace()
                                .setAccelerationTime(21)
                                .setIsAsync(false)
                                .setTimeToDeadline(45)
                                .setPosition(5)
                                .build());

    manager.addDiskCall(DiskCallBuilder::getInstace()
                                .setAccelerationTime(22)
                                .setIsAsync(false)
                                .setTimeToDeadline(2)
                                .setPosition(28)
                                .build());

    manager.addDiskCall(DiskCallBuilder::getInstace()
                                .setAccelerationTime(26)
                                .setIsAsync(false)
                                .setTimeToDeadline(2)
                                .setPosition(300)
                                .build());

    manager.addDiskCall(DiskCallBuilder::getInstace()
                                .setAccelerationTime(26)
                                .setIsAsync(false)
                                .setTimeToDeadline(2)
                                .setPosition(1)
                                .build());

    StandardOutputLogStream * logStream = new StandardOutputLogStream();
    manager.setLogStream(logStream);
    EXPECT_EQ(354, manager.simulate());

//    logStream->writeAll();


}


TEST(Algorithm, SCAN) {
    SCANManager manager;
    manager.setMaxSize(100);

    manager.addDiskCall(DiskCallBuilder::getInstace()
                                .setAccelerationTime(0)
                                .setIsAsync(false)
                                .setTimeToDeadline(45)
                                .setPosition(24)
                                .build());

    manager.addDiskCall(DiskCallBuilder::getInstace()
                                .setAccelerationTime(21)
                                .setIsAsync(false)
                                .setTimeToDeadline(45)
                                .setPosition(5)
                                .build());

    manager.addDiskCall(DiskCallBuilder::getInstace()
                                .setAccelerationTime(22)
                                .setIsAsync(false)
                                .setTimeToDeadline(2)
                                .setPosition(28)
                                .build());

    manager.addDiskCall(DiskCallBuilder::getInstace()
                                .setAccelerationTime(26)
                                .setIsAsync(false)
                                .setTimeToDeadline(2)
                                .setPosition(98)
                                .build());

    manager.addDiskCall(DiskCallBuilder::getInstace()
                                .setAccelerationTime(26)
                                .setIsAsync(false)
                                .setTimeToDeadline(2)
                                .setPosition(94)
                                .build());

    EXPECT_EQ(200, manager.simulate());
}


