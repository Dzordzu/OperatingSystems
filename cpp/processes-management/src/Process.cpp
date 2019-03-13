//
// Created by dzordzu on 09.03.19.
//

#include <ProcessesManagement/Process.hpp>

Process::Process(const std::string &name, const uint_fast16_t delay, const uint_fast16_t executionTime) : name(name),
                                                                                                 delay(delay),
                                                                                                 execTime(executionTime) {}

Process::Process() : name(""), delay(0), execTime(0) {}

const std::string &Process::getName() const {
    return name;
}

uint_fast16_t Process::getDelay() const {
    return delay;
}

uint_fast16_t Process::getLeftExecutionTime() const {
    return execTime;
}

void Process::execute(uint_fast8_t times) const {
    execTime = execTime > times ? execTime - times : 0;
}

bool Process::isDone() const {
    return execTime == 0;
}

QueuedProcess::QueuedProcess(uint_fast16_t pid, const Process &process, uint_fast16_t lastActivity) : pid(pid),
                                                                                                      process(process),
                                                                                                      lastActivity(
                                                                                                              lastActivity) {
    status = false;
    waitTime = 0;
}

bool QueuedProcess::activate(const uint_fast16_t time) {
    if(status) return false;
    status = true;
    waitTime += time - lastActivity;
    return true;
}

bool QueuedProcess::deactivate(const uint_fast16_t time) {
    if(!status) return false;
    status = false;
    lastActivity = time;
    return true;
}

uint_fast16_t QueuedProcess::getPid() const {
    return pid;
}

const Process &QueuedProcess::getProcess() const {
    return process;
}

uint_fast16_t QueuedProcess::getLastActivity() const {
    return lastActivity;
}

uint_fast16_t QueuedProcess::getWaitTime() const {
    return waitTime;
}

bool QueuedProcess::getStatus() const {
    return status;
}

