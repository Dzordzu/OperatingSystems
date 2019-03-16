//
// Created by dzordzu on 09.03.19.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <CppUtils/Logger.hpp>
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