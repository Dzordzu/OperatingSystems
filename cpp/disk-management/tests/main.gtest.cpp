//
// Created by dzordzu on 09.03.19.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <CppUtils/Logger.hpp>
#include <string>
#include <DiskManagement/DiskManager.hpp>

namespace DiskManagement {

    /*
     * Disk names explanations:
     *
     * SD - Simple Disk
     * NSOR - No Service On Run
     * SIZE[NUM] - Size (value of NUM)
     * SS - Small Size (same as SIZE10)
     * NS - Normal Size (same AS SIZE500)
     * HS - Huge Size (same as SIZE10000)
     * ES - Enormous Size (same as SIZE100000)
     * ASYNC - Asynchronous
     * DR[NUM] - Data Read Cost (value of NUM)
     * STM[NUM] - Single Track Movement (value of NUM)
     * AP[NUM] - Arm Position (value of NUM)
     */

    Disk SD = DiskBuilder::getInstance()
            .setArmPosition(0)
            .enableServicingOnRun(true)
            .setDataReadCost(0)
            .setSingleTrackMovementCost(1)
            .setSize(100)
            .build();

    Disk NSOR = DiskBuilder::getInstance()
            .setArmPosition(0)
            .enableServicingOnRun(false)
            .setDataReadCost(0)
            .setSingleTrackMovementCost(1)
            .setSize(100)
            .build();

    Disk SS = DiskBuilder::getInstance()
            .setArmPosition(0)
            .enableServicingOnRun(false)
            .setDataReadCost(0)
            .setSingleTrackMovementCost(1)
            .setSize(10)
            .build();

    Disk SS_NSOR = DiskBuilder::getInstance()
            .setArmPosition(0)
            .enableServicingOnRun(false)
            .setDataReadCost(0)
            .setSingleTrackMovementCost(1)
            .setSize(10)
            .build();

    /*
     * Tests:
     * 1 - 2 SYNC
     * 3 - 4 ASYNC
     */

    void prepareForTest_1(Manager * manager) {
        manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                       .setQueuedTime(8)
                                       .setTrackPosition(7)
                                       .build());

        manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                       .setQueuedTime(9)
                                       .setTrackPosition(4)
                                       .build());

        manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                       .setQueuedTime(9)
                                       .setTrackPosition(6)
                                       .build());

        manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                       .setQueuedTime(4)
                                       .setTrackPosition(4)
                                       .build());
    }

    void prepareForTest_2(Manager * manager) {
        manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                       .setQueuedTime(0)
                                       .setTrackPosition(10)
                                       .build());

        manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                       .setQueuedTime(1)
                                       .setTrackPosition(15)
                                       .build());

        manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                       .setQueuedTime(2)
                                       .setTrackPosition(7)
                                       .build());
    }

    void prepareForTest_3(Manager * manager) {
        manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                        .setQueuedTime(0)
                                        .setTrackPosition(100)
                                        .setTimeToDeadline(200)
                                        .build());

        manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                        .setQueuedTime(49)
                                        .setTrackPosition(60)
                                        .build());

        manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                        .setQueuedTime(49)
                                        .setTrackPosition(150)
                                        .setTimeToDeadline(50)
                                        .build());

        manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                        .setQueuedTime(100)
                                        .setTrackPosition(125)
                                        .build());

        manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                        .setQueuedTime(170)
                                        .setTrackPosition(150)
                                        .build());


    }


}

using namespace DiskManagement;

TEST(Algorithm, FCFS_1) {

    FCFSManager manager(SD);
    prepareForTest_1(&manager);
    EXPECT_EQ(10, manager.simulate());

}

TEST(Algorithm, FCFS_2) {

    FCFSManager manager(SD);
    prepareForTest_2(&manager);
    EXPECT_EQ(15, manager.simulate());

    manager.setDisk(NSOR);
    prepareForTest_2(&manager);
    EXPECT_EQ(23, manager.simulate());

    manager.setDisk(SS);
    prepareForTest_2(&manager);
    EXPECT_EQ(7, manager.simulate());

//    StandardOutputLogStream * logStream = new StandardOutputLogStream();
//    manager.setLogStream(logStream);
//    logStream->filter([](const Log& log){ return log.getTitle() != "General"; }, *logStream);
//    logStream->writeAll();

}




//TEST(Algorithm, SSTF) {
//    SSTFManager manager;
//
//    manager.addDiskCall(DiskCallBuilder::getInstace()
//                                .setAccelerationTime(0)
//                                .setIsAsync(false)
//                                .setTimeToDeadline(45)
//                                .setPosition(24)
//                                .build());
//
//    manager.addDiskCall(DiskCallBuilder::getInstace()
//                                .setAccelerationTime(21)
//                                .setIsAsync(false)
//                                .setTimeToDeadline(45)
//                                .setPosition(5)
//                                .build());
//
//    manager.addDiskCall(DiskCallBuilder::getInstace()
//                                .setAccelerationTime(22)
//                                .setIsAsync(false)
//                                .setTimeToDeadline(2)
//                                .setPosition(28)
//                                .build());
//
//    manager.addDiskCall(DiskCallBuilder::getInstace()
//                                .setAccelerationTime(26)
//                                .setIsAsync(false)
//                                .setTimeToDeadline(2)
//                                .setPosition(300)
//                                .build());
//
//    manager.addDiskCall(DiskCallBuilder::getInstace()
//                                .setAccelerationTime(26)
//                                .setIsAsync(false)
//                                .setTimeToDeadline(2)
//                                .setPosition(1)
//                                .build());
//
//    StandardOutputLogStream * logStream = new StandardOutputLogStream();
//    manager.setLogStream(logStream);
//    EXPECT_EQ(354, manager.simulate());
//
////    logStream->writeAll();
//
//
//}
//
//
//TEST(Algorithm, SCAN) {
//    SCANManager manager;
//    manager.setMaxSize(100);
//
//    manager.addDiskCall(DiskCallBuilder::getInstace()
//                                .setAccelerationTime(0)
//                                .setIsAsync(false)
//                                .setTimeToDeadline(45)
//                                .setPosition(24)
//                                .build());
//
//    manager.addDiskCall(DiskCallBuilder::getInstace()
//                                .setAccelerationTime(21)
//                                .setIsAsync(false)
//                                .setTimeToDeadline(45)
//                                .setPosition(5)
//                                .build());
//
//    manager.addDiskCall(DiskCallBuilder::getInstace()
//                                .setAccelerationTime(22)
//                                .setIsAsync(false)
//                                .setTimeToDeadline(2)
//                                .setPosition(28)
//                                .build());
//
//    manager.addDiskCall(DiskCallBuilder::getInstace()
//                                .setAccelerationTime(26)
//                                .setIsAsync(false)
//                                .setTimeToDeadline(2)
//                                .setPosition(98)
//                                .build());
//
//    manager.addDiskCall(DiskCallBuilder::getInstace()
//                                .setAccelerationTime(26)
//                                .setIsAsync(false)
//                                .setTimeToDeadline(2)
//                                .setPosition(94)
//                                .build());
//
//    EXPECT_EQ(200, manager.simulate());
//}
//
//
