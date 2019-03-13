//
// Created by dzordzu on 09.03.19.
//

#ifndef OPERATING_SYSTEMS_CPP_PROCESS_HPP
#define OPERATING_SYSTEMS_CPP_PROCESS_HPP

#include <string>
#include <cstdint>
#include <algorithm>

class Process {
    std::string name;
    uint_fast16_t delay;
    mutable uint_fast16_t execTime;
public:
    Process(const std::string &name, const uint_fast16_t delay, const uint_fast16_t executionTime);
    Process();
    const std::string &getName() const;
    uint_fast16_t getDelay() const;
    uint_fast16_t getLeftExecutionTime() const;
    void execute(uint_fast8_t times = 1) const;
    bool isDone() const;

};

class QueuedProcess {
    uint_fast16_t pid;
    Process process;
    uint_fast16_t lastActivity;
    uint_fast16_t waitTime;
    bool status;

public:
    QueuedProcess(uint_fast16_t pid, const Process &process, uint_fast16_t lastActivity);

    /*
     * Both functions return true if process is just activated/deactivated
     * Otherwise (it has been already activated/deactivated) it returns false
     */
    bool activate(const uint_fast16_t time);
    bool deactivate(const uint_fast16_t time);

    uint_fast16_t getPid() const;
    const Process &getProcess() const;
    uint_fast16_t getLastActivity() const;
    uint_fast16_t getWaitTime() const;
    bool getStatus() const;
};

#endif //OPERATING_SYSTEMS_CPP_PROCESS_HPP
