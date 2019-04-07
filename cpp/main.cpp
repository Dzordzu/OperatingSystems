#include <iostream>
#include <ProcessesManagement/Manager.hpp>
#include <DiskManagement/DiskManager.hpp>
#include <random>
#include <string>
#include <thread>
#include <mutex>

void showFor(uint_fast64_t sample) {

    using namespace OperatingSystems::ProcessesManagement;
    using OperatingSystems::CppUtils::StandardOutputLogStream;

    StandardOutputLogStream logs;
    FCFSManager fcfsManager;
    SJFManager sjfManager;
    SRTFManager srtfManager;
    RRManager rrManager;

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
        rrManager.addProcess(process);
    }

    rrManager.setExecTimes(500);

    std::cout<<"Sample: "<<sample<<std::endl;

    typedef typename std::pair<double_t, std::string> Record;
    auto filterFunc = [](const Log & log) -> bool { return log.getTitle() == "Total average time"; };

    Record fcfsResult(fcfsManager.simulate(&logs), "FCFS");
    logs.filter(filterFunc, logs);
    std::cout<<"FCFS: "; logs.writeAll();
    logs.clear();

    Record sjfResult(sjfManager.simulate(&logs), "SJF");
    logs.filter(filterFunc, logs);
    std::cout<<"SJF: "; logs.writeAll();
    logs.clear();

    Record srtfResult(srtfManager.simulate(&logs), "SRTF");
    //logs.filter(filterFunc, logs);
    std::cout<<"SRTF: "; logs.writeAll();
    logs.clear();

    Record rrResult(rrManager.simulate(&logs), "RR");
    logs.filter(filterFunc, logs);
    std::cout<<"RR: "; logs.writeAll();
    logs.clear();

    auto fastest = std::min({srtfResult, sjfResult, fcfsResult, rrResult}, [](Record a, Record b) -> bool {
        return a.first < b.first;
    });

    auto slowest = std::max({srtfResult, sjfResult, fcfsResult, rrResult}, [](Record a, Record b) -> bool {
        return a.first < b.first;
    });
    std::cout<<"Winner "<<(fastest.second) << std::endl;
    std::cout<<"Looser "<<(slowest.second) << std::endl;
    std::cout<<"Difference "<< ((double)slowest.first - (double)fastest.first) << " (" << ((double)slowest.first / (double)fastest.first) * 100 << "%)" << std::endl;

    std::cout<<std::endl<<std::endl;
}

int main() {

    //std::thread newThread(showFor, 100000);
    //newThread.join();

//    showFor(5);
//    showFor(100);
//    showFor(500);
//    for(int i=0; i<10; i++) showFor(1000);
//    showFor(5000);
//    showFor(10000);



}