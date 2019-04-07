//
// Created by dzordzu on 09.03.19.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <CppUtils/Logger.hpp>
#include <ProcessesManagement/Manager.hpp>
#include <string>

namespace OperatingSystems {
    namespace ProcessesManagement {
        class ExampleManager : public Manager {
        public:
            double simulate(LogStream *const logStream) override { return 0; }
        };
    }
}

using namespace OperatingSystems::ProcessesManagement;

TEST(ProcessesManagement, ExampleManager) {
    ExampleManager manager;

    for(int i=0; i<10; i++) {
        Process process = ProcessBuilder::getInstance()
                .newProcess()
                .setDelay(50*i)
                .setExecutionTime(1000)
                .setName("Sample")
                .create();

        manager.addProcess(process);
    }

    EXPECT_EQ(manager.getProcessesAmount(), 10);
    EXPECT_EQ(manager.getProcessesVector()[0].getDelay(), 0);
    EXPECT_EQ(manager.getProcessesVector()[1].getDelay(), 50);

}



TEST(ProcessesManagement, FCFSManager) {
    FCFSManager manager;


    for(int i=0; i<3; i++) {
        Process process = ProcessBuilder::getInstance()
                .newProcess()
                .setDelay(i)
                .setExecutionTime(3-i)
                .setName("Sample " + std::to_string(i))
                .create();

        manager.addProcess(process);
    }
    EXPECT_EQ(manager.simulate(), 5.0/3.0);

    for(int i=0; i<3; i++) {
        Process process = ProcessBuilder::getInstance()
                .newProcess()
                .setDelay(2*(3+i))
                .setExecutionTime(3-i)
                .setName("Sample " + std::to_string(i))
                .create();

        manager.addProcess(process);
    }

    EXPECT_EQ(manager.simulate(), 2.0/3.0);

    for(int i=0; i<3; i++) {
        Process process = ProcessBuilder::getInstance()
                .newProcess()
                .setDelay(0)
                .setExecutionTime(10*i + 1)
                .setName("Sample " + std::to_string(i))
                .create();

        manager.addProcess(process);
    }

    EXPECT_EQ(manager.simulate(), 13.0/3.0);

    for(int i=0; i<5; i++) {
        Process process = ProcessBuilder::getInstance()
                .newProcess()
                .setDelay(0)
                .setExecutionTime(3*i + 1)
                .setName("Sample " + std::to_string(i))
                .create();

        manager.addProcess(process);
    }

    EXPECT_EQ(manager.simulate(), 40.0/5.0);

}

TEST(ProcessesManagement, FCFSManager2) {

    Process process[3];

    for(int i=2; i<5; i++) {
        process[i-2] = ProcessBuilder::getInstance()
                .newProcess()
                .setDelay(0)
                .setExecutionTime(i)
                .setName("P" + std::to_string(i))
                .create();
    }

    FCFSManager manager;

    manager.addProcess(process[0]);
    manager.addProcess(process[2]);
    manager.addProcess(process[1]);

    EXPECT_EQ(manager.simulate(), 8.0/3.0);

    manager.addProcess(process[2]);
    manager.addProcess(process[1]);
    manager.addProcess(process[0]);

    EXPECT_EQ(manager.simulate(), 11.0/3.0);
}


TEST(ProcessesManagement, SJFManager) {
    SJFManager manager;

    for(uint_fast16_t i = 0; i<5; i++) {
        manager.addProcess(ProcessBuilder::getInstance()
          .newProcess()
          .setName("SJF" + std::to_string(i))
          .setDelay(0)
          .setExecutionTime(2*(i+1))
          .create());
    }

    EXPECT_EQ(manager.simulate(), 40.0/5.0);

    for(int_fast16_t i = 4; i>=0; i--) {
        manager.addProcess(ProcessBuilder::getInstance()
            .newProcess()
            .setName("SJF" + std::to_string(i))
            .setDelay(0)
            .setExecutionTime(2*(i+1))
            .create());
    }

    EXPECT_EQ(manager.simulate(), 40.0/5.0);

    for(uint_fast16_t i = 0; i<5; i++) {
        manager.addProcess(ProcessBuilder::getInstance()
             .newProcess()
             .setName("SJF - D(" + std::to_string(3*i) + "), E(" + std::to_string(10-2*i) + ")")
             .setDelay(3*i)
             .setExecutionTime(10-2*i)
             .create());
    }

    EXPECT_EQ(manager.simulate(), 32.0/5.0);

    for(uint_fast16_t i = 0; i<3; i++) {
        manager.addProcess(ProcessBuilder::getInstance()
                                       .newProcess()
                                       .setName("SJF" + std::to_string((uint_fast16_t)(20 / pow(2, i))))
                                       .setDelay((uint_fast16_t)(2.5  * i * (i+1)))
                                       .setExecutionTime((uint_fast16_t)(20 / pow(2, i)))
                                       .create());
    }


    EXPECT_EQ(manager.simulate(), 25.0/3.0);


}

TEST(ProcessesManagement, SRTFmanager) {
    SRTFManager manager;

    for(uint_fast16_t i = 0; i<3; i++) {
        manager.addProcess(ProcessBuilder::getInstance()
                                   .newProcess()
                                   .setName("SRTF" + std::to_string((uint_fast16_t)(20 / pow(2, i))))
                                   .setDelay((uint_fast16_t)(2.5  * i * (i+1)))
                                   .setExecutionTime((uint_fast16_t)(20 / pow(2, i)))
                                   .create());
    }

    EXPECT_EQ(manager.simulate(), 15.0/3.0);
}

TEST(ProcessesManagement, RRManager) {
    RRManager manager;

    for(int i=0; i<2; i++) {
        manager.addProcess(ProcessBuilder::getInstance()
            .newProcess()
            .setName("RR" + std::to_string(i))
            .setExecutionTime(30)
            .setDelay(15*i)
            .create()
            );
    }

    EXPECT_THAT(manager.simulate(), 25.0/2.0);
}
