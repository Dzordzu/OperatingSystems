//
// Created by dzordzu on 10.03.19.
//

#include "CppUtils/Logger.hpp"

/**
 * @XXX This smells
 */
using namespace OperatingSystems::CppUtils;

Log::Log(const std::string &title, const std::string &name) : title(title), message(name) {}

const std::string &Log::getTitle() const {
    return title;
}

const std::string &Log::getMessage() const {
    return message;
}

LogStream &StandardOutputLogStream::operator>>(const Log &log) {
    logs.emplace_back(log);
    return *this;
}

LogStream &StandardOutputLogStream::add(const Log &log) {
    *this>>log;
    return *this;
}

LogStream &StandardOutputLogStream::filter(bool (*filterFunction)(const Log &), LogStream & stream) {

    auto & castedStream = dynamic_cast<StandardOutputLogStream &>(stream);
    StandardOutputLogStream newStream;

    for(const auto &log : castedStream.logs) {
        if(filterFunction(log)) newStream.logs.emplace_back(log);
    }

    castedStream = newStream;

    return stream;

}

LogStream &StandardOutputLogStream::map(Log (*mapFunction)(const Log &), LogStream & stream) {

    auto & castedStream = dynamic_cast<StandardOutputLogStream &>(stream);
    StandardOutputLogStream newStream;

    for(const auto &log : castedStream.logs) {
        newStream.logs.emplace_back(mapFunction(log));
    }

    castedStream = newStream;

    return stream;
}

void StandardOutputLogStream::writeAll() {
    for(auto  log : logs) {
        std::cout<<log.getTitle()<<": "<<log.getMessage()<<std::endl;
    }
}

const std::list<Log> StandardOutputLogStream::get() const {
    return logs;
}

