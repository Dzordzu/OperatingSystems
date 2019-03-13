//
// Created by dzordzu on 09.03.19.
//

#ifndef OPERATING_SYSTEMS_CPP_MANAGER_HPP
#define OPERATING_SYSTEMS_CPP_MANAGER_HPP

#include <ProcessesManagement/Process.hpp>
#include <ProcessesManagement/Logger.hpp>
#include <vector>
#include <utility>
#include <iostream>

class ProcessBuilder {
    ProcessBuilder(){}
    Process lastProcess;

    std::string name;
    uint_fast16_t delay, executionTime;

public:
    ProcessBuilder(ProcessBuilder const&) = delete;
    void operator=(ProcessBuilder const&) = delete;

    inline static ProcessBuilder &getInstance() {
        static ProcessBuilder instance;
        return instance;
    }

    ProcessBuilder &newProcess();
    inline ProcessBuilder &setDelay(uint_fast16_t delay) { this->delay = delay;return *this; }
    inline ProcessBuilder &setName(const std::string &name) {this->name = name; return *this; }
    inline ProcessBuilder &setExecutionTime(uint_fast16_t executionTime) { this->executionTime = executionTime;return *this; }
    inline Process create() { lastProcess = Process(name, delay, executionTime); return lastProcess; }

};

class Manager {
protected:
    std::vector<Process> processes;

    std::vector<QueuedProcess> queue;

    uint_fast16_t nextPid;
    uint_fast64_t currentTime;

    bool finished();
    virtual void addToQueue(Process & process);
public:
    void addProcess(Process process);
    uint_fast32_t getProcessesAmount();
    std::vector<Process> getProcessesVector();
    virtual double simulate();
    virtual double simulate(LogStream * const logStream) = 0;
    virtual void reset();
};

class FCFSManager : public Manager {
public:
    inline FCFSManager() { reset(); }
    double simulate(LogStream * const logStream) override;
    double simulate() override;
};

class SJFManager : public FCFSManager {
public:
    inline SJFManager() { reset(); }
    void addToQueue(Process & process) override;
};

class SRTFManager : public Manager {
public:
    inline SRTFManager() { reset(); }
    void addToQueue(Process & process) override;

};


#endif //OPERATING_SYSTEMS_CPP_MANAGER_HPP
