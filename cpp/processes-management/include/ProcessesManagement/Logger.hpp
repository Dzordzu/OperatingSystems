//
// Created by dzordzu on 10.03.19.
//

#ifndef OPERATING_SYSTEMS_CPP_LOGGER_HPP
#define OPERATING_SYSTEMS_CPP_LOGGER_HPP

#include <string>
#include <list>
#include <iostream>

class Log {
    std::string title;
    std::string message;

public:
    Log(const std::string &title, const std::string &name);
    const std::string &getTitle() const;
    const std::string &getMessage() const;
};


class LogStream {
public:
    virtual LogStream & operator>>(const Log &log) = 0;
    virtual LogStream & add(const Log & log) = 0;
    virtual LogStream & filter(bool (*filterFunction)(const Log &log), LogStream & stream) = 0;
    virtual LogStream & map(Log (*filterFunction)(const Log &log), LogStream & stream) = 0;
    virtual const std::list<Log> get() const = 0;
};

class StandardOutputLogStream : public LogStream {
    std::list<Log> logs;
public:
    LogStream &operator>>(const Log &log) override;
    LogStream &add(const Log &log) override;
    LogStream &filter(bool (*filterFunction)(const Log &), LogStream & stream) override;
    LogStream &map(Log (*mapFunction)(const Log &), LogStream & stream) override;
    const std::list<Log> get() const override;

    void writeAll();

};


#endif //OPERATING_SYSTEMS_CPP_LOGGER_HPP
