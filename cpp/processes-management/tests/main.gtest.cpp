//
// Created by dzordzu on 09.03.19.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <ProcessesManagement/Manager.hpp>
#include <ProcessesManagement/Logger.hpp>
#include <string>


TEST(LoggerTest, StandardOutputLogStream) {
    StandardOutputLogStream stream;
    stream >> Log("Title", "Message") >> Log("Title", "Message 2");
    stream >> Log("Another", "Message with Title");

    auto list = stream.add(Log("Another", "Message"))
            .filter([](const Log &log) -> bool { return log.getTitle() == "Title"; }, stream)
            .map([](const Log &log) -> Log {
                return Log(log.getTitle(), log.getMessage() + " EDITED");
            }, stream)
            .get();

    for(auto & element : list) {
        EXPECT_EQ(element.getTitle(), "Title");
        EXPECT_THAT(element.getMessage(), ::testing::AnyOf("Message 2 EDITED", "Message EDITED"));
    }


    testing::internal::CaptureStdout();
    stream.writeAll();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "Title: Message EDITED\nTitle: Message 2 EDITED\n");

}



class ExampleManager : public Manager {
public:
    double simulate(LogStream *const logStream) override {return 0;}
};

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

    StandardOutputLogStream stream;
    EXPECT_THAT(manager.simulate(&stream), 25.0/2.0);
    stream.writeAll();
}
