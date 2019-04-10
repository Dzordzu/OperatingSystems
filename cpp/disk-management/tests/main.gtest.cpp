//
// Created by dzordzu on 09.03.19.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <CppUtils/Logger.hpp>
#include <string>
#include <DiskManagement/DiskManager.hpp>

namespace OperatingSystems {
    namespace DiskManagement {

        /*
         * Disk names explanations:
         *
         * SD - Simple Disk (size: 100, stm: 1, dr: 0, enabled service on run, ap: 0)
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
         *
         * note: SD can be override by any other property
         */

        Disk SD = DiskBuilder::getInstance()
                .setArmPosition(0)
                .enableServicingOnRun(true)
                .setDataReadCost(0)
                .setSingleTrackMovementCost(1)
                .setSize(100)
                .build();

        Disk SD_NSOR = DiskBuilder::getInstance()
                .setArmPosition(0)
                .enableServicingOnRun(false)
                .setDataReadCost(0)
                .setSingleTrackMovementCost(1)
                .setSize(100)
                .build();

        Disk SD_SS_NSOR = DiskBuilder::getInstance()
                .setArmPosition(0)
                .enableServicingOnRun(false)
                .setDataReadCost(0)
                .setSingleTrackMovementCost(1)
                .setSize(10)
                .build();

        Disk NSOR_NS_DR1_STM10_AP5 = DiskBuilder::getInstance()
                .setArmPosition(5)
                .enableServicingOnRun(false)
                .setDataReadCost(1)
                .setSingleTrackMovementCost(10)
                .setSize(500)
                .build();
        Disk SD_SIZE1000 = DiskBuilder::getInstance()
                .setArmPosition(0)
                .enableServicingOnRun(true)
                .setDataReadCost(0)
                .setSingleTrackMovementCost(1)
                .setSize(1000)
                .build();

        /*
         * Tests:
         * 1 - 2 SYNC
         * 3 - 4 ASYNC
         */

        void prepareForTest_1(Manager *manager) {
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

        void prepareForTest_2(Manager *manager) {
            manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                            .setQueuedTime(0)
                                            .setTrackPosition(10)
                                            .build()); // 5 + 3 + 8

            manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                            .setQueuedTime(1)
                                            .setTrackPosition(15)
                                            .build());

            manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                            .setQueuedTime(2)
                                            .setTrackPosition(7)
                                            .build());
        }

        void prepareForTest_3(Manager *manager) {
            manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                            .setQueuedTime(0)
                                            .setTrackPosition(50)
                                            .setTimeToDeadline(75)
                                            .build());

            manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                            .setQueuedTime(0)
                                            .setTrackPosition(30)
                                            .build());

            manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                            .setQueuedTime(0)
                                            .setTrackPosition(75)
                                            .build());

            manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                            .setQueuedTime(49)
                                            .setTrackPosition(20)
                                            .setTimeToDeadline(30)
                                            .build());

            manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                            .setQueuedTime(100)
                                            .setTrackPosition(99)
                                            .setTimeToDeadline(100)
                                            .build());

            manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                            .setQueuedTime(100)
                                            .setTrackPosition(0)
                                            .setTimeToDeadline(101)
                                            .build());


        }


        void prepareForTest_4(Manager *manager) {
            manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                            .setQueuedTime(0)
                                            .setTrackPosition(50)
                                            .build());

            manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                            .setQueuedTime(50)
                                            .setTrackPosition(100)
                                            .setTimeToDeadline(40)
                                            .build());

            manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                            .setQueuedTime(50)
                                            .setTrackPosition(0)
                                            .setTimeToDeadline(50)
                                            .build());

            manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                            .setQueuedTime(400)
                                            .setTrackPosition(50)
                                            .build());

            manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                            .setQueuedTime(1000)
                                            .setTrackPosition(100)
                                            .setTimeToDeadline(1000)
                                            .build());

            manager->enqueueRequest(DiskRequestBuilder::getInstance()
                                            .setQueuedTime(1000)
                                            .setTrackPosition(49)
                                            .build());
        }
    }
}

using namespace OperatingSystems::DiskManagement;

TEST(Algorithm, FCFS_1) {

    FCFSManager manager(SD);
    prepareForTest_1(&manager);
    EXPECT_EQ(10, manager.simulate());

}

TEST(Algorithm, FCFS_2) {

    FCFSManager manager(SD);
    prepareForTest_2(&manager);
    EXPECT_EQ(15, manager.simulate());

    manager.setDisk(SD_NSOR);
    prepareForTest_2(&manager);
    EXPECT_EQ(23, manager.simulate());

    manager.setDisk(SD_SS_NSOR);
    prepareForTest_2(&manager);
    EXPECT_EQ(7, manager.simulate());

    manager.setDisk(NSOR_NS_DR1_STM10_AP5);
    prepareForTest_2(&manager);
    EXPECT_EQ(183, manager.simulate());

//    StandardOutputLogStream * logStream = new StandardOutputLogStream();
//    manager.setLogStream(logStream);
//    logStream->filter([](const Log& log){ return log.getTitle() != "General"; }, *logStream);
//    logStream->writeAll();

}

TEST(Algorithm, FCFS_3) {

    FCFSManager manager(SD);
    prepareForTest_3(&manager);
    EXPECT_EQ(258, manager.simulate());

    manager.setDisk(SD_NSOR);
    prepareForTest_3(&manager);
    EXPECT_EQ(258, manager.simulate());

    manager.setDisk(NSOR_NS_DR1_STM10_AP5);
    prepareForTest_3(&manager);
    EXPECT_EQ(2536, manager.simulate());

}




TEST(Algorithm, SSTF_1) {

    SSTFManager manager(SD);
    prepareForTest_1(&manager);
    EXPECT_EQ(10, manager.simulate());

}

TEST(Algorithm, SSTF_2) {

    SSTFManager manager(SD);
    prepareForTest_2(&manager);
    EXPECT_EQ(15, manager.simulate());

    manager.setDisk(SD_NSOR);
    prepareForTest_2(&manager);
    EXPECT_EQ(21, manager.simulate());

    manager.setDisk(SD_SS_NSOR);
    prepareForTest_2(&manager);
    EXPECT_EQ(7, manager.simulate());

    manager.setDisk(NSOR_NS_DR1_STM10_AP5);
    prepareForTest_2(&manager);
    EXPECT_EQ(163, manager.simulate());

}

TEST(Algorithm, SSTF_3) {

    SSTFManager manager(SD);
    prepareForTest_3(&manager);
    EXPECT_EQ(258, manager.simulate());

    manager.setDisk(SD_NSOR);
    prepareForTest_3(&manager);
    EXPECT_EQ(258, manager.simulate());
}

TEST(Algorithm, SCAN_1) {

    SCANManager manager(SD);
    prepareForTest_1(&manager);
    EXPECT_EQ(10, manager.simulate());

    manager.enableLastFirstMode(false);
    prepareForTest_1(&manager);
    EXPECT_EQ(200, manager.simulate());

}

TEST(Algorithm, SCAN_3) {

    SCANManager manager(SD);
    prepareForTest_3(&manager);
    EXPECT_EQ(258, manager.simulate());

    manager.enableLastFirstMode(false);
    prepareForTest_3(&manager);
    EXPECT_EQ(200, manager.simulate());

}

TEST(Algorithm, SCAN_4) {
    SCANManager manager(SD_SIZE1000);
    prepareForTest_4(&manager);
    EXPECT_EQ(351, manager.simulate());
}


TEST(Algorithm, CSCAN_1) {

    CSCANManager manager(SD);
    prepareForTest_1(&manager);
    EXPECT_EQ(14, manager.simulate());

    manager.enableLastFirstMode(false);
    prepareForTest_1(&manager);
    EXPECT_EQ(250, manager.simulate());


}

TEST(Algorithm, CSCAN_3) {

    CSCANManager manager(SD);
    prepareForTest_3(&manager);
    EXPECT_EQ(258, manager.simulate());

    manager.enableLastFirstMode(false);
    prepareForTest_3(&manager);
    EXPECT_EQ(250, manager.simulate());

}

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
