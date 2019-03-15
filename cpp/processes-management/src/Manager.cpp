//
// Created by dzordzu on 09.03.19.
//

#include <ProcessesManagement/Manager.hpp>

ProcessBuilder &ProcessBuilder::newProcess() {

    lastProcess = Process();
    name = "";
    executionTime = 0;
    delay = 0;


    return *this;
}

void Manager::addProcess(Process process) {
    processes.push_back(process);
}

uint_fast32_t Manager::getProcessesAmount() {
    return processes.size();
}

std::vector<Process> Manager::getProcessesVector() {
    return processes;
}

bool Manager::finished() {
    return processes.empty() && queue.empty();
}

void Manager::addToQueue(Process &process) {
    QueuedProcess queuedProcess(nextPid, process, currentTime);
    queue.emplace_back(queuedProcess);
    nextPid++;
}

void Manager::reset() {
    currentTime = uint_fast64_t(-1);
    nextPid = 0;
    queue.erase(queue.begin(), queue.end());
    processes.erase(processes.begin(), processes.end());
}

double Manager::simulate() {
    return simulate(nullptr);
}

double FCFSManager::simulate(LogStream * const logStream) {

    typedef typename std::pair<std::string, uint_fast32_t> Times;
    std::vector<Times> times;
    uint_fast32_t consumedProcesses = processes.size();

    std::stable_sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) -> bool {
        return (a.getDelay() < b.getDelay());
    });

    if(logStream != nullptr) {
        for(auto & p : processes) {
            *logStream >> Log("Process", p.getName() + " with delay(" + std::to_string(p.getDelay()) + ") and exec(" + std::to_string(p.getLeftExecutionTime()) + ")");
        }
    }


    while(!finished()) {
        currentTime++;

        if(!processes.empty()) {
            while(processes[0].getDelay() < currentTime) {

                if(logStream != nullptr) {
                    *logStream >> Log("Added process to the queue", processes[0].getName() + " (time: " + std::to_string(currentTime) + ")");
                }

                addToQueue(processes[0]);
                processes.erase(processes.begin());
                if(processes.empty()) break;
            }
        }

        if(queue.empty()) continue;

        if(queue[0].activate(currentTime)) {

            if(queue.size() > 1) queue[1].deactivate(currentTime);

            if(logStream != nullptr) {
                *logStream >> Log("Started execution of the process", queue[0].getProcess().getName() + " (time: " + std::to_string(currentTime) + ")");
            }
        }
        queue[0].getProcess().execute();

        if(queue[0].getProcess().isDone()) {

            if(logStream != nullptr) {
                *logStream >> Log("Removed process from the queue", queue[0].getProcess().getName() + " (time: " + std::to_string(currentTime) + ")");
            }

            queue[0].deactivate(currentTime);
            times.emplace_back(Times{queue[0].getProcess().getName(), queue[0].getWaitTime()});
            queue.erase(queue.begin());
        }
    }

    double avgTime = 0;
    for(auto t : times) {
        avgTime += t.second;
    }

    if(logStream != nullptr) {

        for(auto t : times) {
            *logStream >> Log("Waiting Time", t.first + " " + std::to_string(t.second));
        }

        *logStream >> Log("Total average time", std::to_string(avgTime / consumedProcesses));
    }

    reset();
    return avgTime / consumedProcesses;
}

double FCFSManager::simulate() {
    return Manager::simulate();
}

void SJFManager::addToQueue(Process &process) {
    QueuedProcess queuedProcess(nextPid, process, currentTime);

    if(queue.size() == 0) {
        queue.emplace_back(queuedProcess);
        nextPid++;
        return;
    }

    u_short indexMovement = 0;

    if(queue[0].getStatus()) {
        indexMovement = 1;
    }

    queue.insert(std::upper_bound(queue.begin() + indexMovement, queue.end(), queuedProcess, [](const QueuedProcess & q1, const QueuedProcess & q2) -> bool {
        return q1.getProcess().getLeftExecutionTime() < q2.getProcess().getLeftExecutionTime();
    }),queuedProcess);
    nextPid++;


}

void SRTFManager::addToQueue(Process &process) {
    QueuedProcess queuedProcess(nextPid, process, currentTime);

    if(queue.size() == 0) {
        queue.emplace_back(queuedProcess);
        nextPid++;
        return;
    }

    queue.insert(std::upper_bound(queue.begin(), queue.end(), queuedProcess, [](const QueuedProcess & q1, const QueuedProcess & q2) -> bool {
        return q1.getProcess().getLeftExecutionTime() < q2.getProcess().getLeftExecutionTime();
    }),queuedProcess);
    nextPid++;
}

double RRManager::simulate() {
    return Manager::simulate();
}

double RRManager::simulate(LogStream *const logStream) {
    typedef typename std::pair<std::string, uint_fast32_t> Times;
    std::vector<Times> times;
    uint_fast32_t consumedProcesses = processes.size();

    std::stable_sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) -> bool {
        return (a.getDelay() < b.getDelay());
    });

    if(logStream != nullptr) {
        for(auto & p : processes) {
            *logStream >> Log("Process", p.getName() + " with delay(" + std::to_string(p.getDelay()) + ") and exec(" + std::to_string(p.getLeftExecutionTime()) + ")");
        }
    }

    uint_fast64_t currentlyExecuted = 0;
    while(!finished()) {
        currentTime++;

        if(!processes.empty()) {
            while(processes[0].getDelay() <= currentTime) {

                if(logStream != nullptr) {
                    *logStream >> Log("Added process to the queue", processes[0].getName() + " (time: " + std::to_string(currentTime) + ")");
                }

                addToQueue(processes[0]);
                processes.erase(processes.begin());
                if(processes.empty()) break;
            }
        }

        if(queue.empty()) continue;

        uint_fast64_t deactivateIndex = (currentlyExecuted) > 0 ? (currentlyExecuted - 1) : queue.size() - 1;

        if(queue[deactivateIndex].deactivate(currentTime) && logStream != nullptr)
            *logStream >> Log("Deactivated process", queue[deactivateIndex].getProcess().getName() + " (time: " + std::to_string(currentTime)+ ", waited: " + std::to_string(queue[currentlyExecuted].getWaitTime()) + ")");


        if(queue[currentlyExecuted].activate(currentTime) && logStream != nullptr)
            *logStream >> Log("Activated process", queue[currentlyExecuted].getProcess().getName() + "(time: " + std::to_string(currentTime) + ", waited: " + std::to_string(queue[currentlyExecuted].getWaitTime()) + ")");


        for(uint_fast16_t jj = 0; jj<execTimes; jj++) {
            queue[currentlyExecuted].getProcess().execute();

            if(!processes.empty()) {
                while(processes[0].getDelay() <= currentTime) {

                    if(logStream != nullptr) {
                        *logStream >> Log("Added process to the queue", processes[0].getName() + " (time: " + std::to_string(currentTime) + ")");
                    }

                    addToQueue(processes[0]);
                    processes.erase(processes.begin());
                    if(processes.empty()) break;
                }
            }

            currentTime++;
            if(queue[currentlyExecuted].getProcess().isDone()) break;
        }
        currentTime--;

        if(queue[currentlyExecuted].getProcess().isDone()) {

            if(logStream != nullptr) {
                *logStream >> Log("Removed process from the queue", queue[currentlyExecuted].getProcess().getName() + " (time: " + std::to_string(currentTime) + ")");
            }

            queue[currentlyExecuted].deactivate(currentTime);
            times.emplace_back(Times{queue[currentlyExecuted].getProcess().getName(), queue[currentlyExecuted].getWaitTime()});
            queue.erase(queue.begin() + currentlyExecuted);
            currentlyExecuted--;
        }
        currentlyExecuted = currentlyExecuted + 1 < queue.size() ? currentlyExecuted + 1 : 0;
    }

    double avgTime = 0;
    for(auto t : times) {
        avgTime += t.second;
    }

    if(logStream != nullptr) {

        for(auto t : times) {
            *logStream >> Log("Waiting Time", t.first + " " + std::to_string(t.second));
        }

        *logStream >> Log("Total average time", std::to_string(avgTime / consumedProcesses));
    }

    reset();
    return avgTime / consumedProcesses;

}
