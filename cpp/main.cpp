#include <iostream>
#include <ProcessesManagement/Manager.hpp>
#include <random>
#include <string>
#include <thread>
#include <mutex>

void showFor(uint_fast64_t sample) {
    StandardOutputLogStream logs;
    FCFSManager fcfsManager;
    SJFManager sjfManager;
    SRTFManager srtfManager;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<uint_fast16_t > dist(1, 1000);

    for(uint_fast16_t i = 0; i<sample; i++) {
        auto process = ProcessBuilder::getInstance()
                .newProcess()
                .setName(std::to_string(i))
                .setDelay(dist(mt))
                .setExecutionTime(dist(mt))
                .create();

        fcfsManager.addProcess(process);
        sjfManager.addProcess(process);
        srtfManager.addProcess(process);
    }

    std::cout<<"Sample: "<<sample<<std::endl;

    typedef typename std::pair<double_t, std::string> Record;
    auto filterFunc = [](const Log & log) -> bool { return log.getTitle() == "Total average time"; };

    fcfsManager.simulate(&logs);
    logs.filter(filterFunc, logs);
    Record fcfsResult(std::stod((*logs.get().begin()).getMessage()), "FCFS");
    std::cout<<"FCFS: "; logs.writeAll();
    logs.clear();

    sjfManager.simulate(&logs);
    logs.filter(filterFunc, logs);
    Record sjfResult(std::stod((*logs.get().begin()).getMessage()), "SJF");
    std::cout<<"SJF: "; logs.writeAll();
    logs.clear();

    srtfManager.simulate(&logs);
    logs.filter(filterFunc, logs);
    Record srtfResult(std::stod((*logs.get().begin()).getMessage()), "SRTF");
    std::cout<<"SRTF: "; logs.writeAll();

    std::cout<<"Winner "<<std::max({srtfResult, sjfResult, fcfsResult}, [](Record a, Record b) -> bool {
        return a.first < b.first;
    }).second;

    std::cout<<std::endl<<std::endl;
}

int main() {

    //std::thread newThread(showFor, 100000);
    //newThread.join();

    showFor(10);
    showFor(100);
    showFor(500);
    showFor(1000);
    showFor(5000);
    showFor(10000);


}