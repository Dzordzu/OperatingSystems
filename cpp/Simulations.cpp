//
// Created by Dzordzu on 10.04.2019.
//

#include "Simulations.h"

void Simulations::processesManagement(uint_fast32_t sample) {
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
    logs.filter(filterFunc, logs);
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
















void Simulations::diskManagement(uint_fast32_t sample) {
    using namespace OperatingSystems::DiskManagement;
    using OperatingSystems::CppUtils::StandardOutputLogStream;
    typedef typename Simulations::Pair<std::string, Manager> manager;

    Disk disk = DiskBuilder::getInstance()
            .enableServicingOnRun(true)
            .setSize(1000)
            .setSingleTrackMovementCost(1)
            .setDataReadCost(0)
            .setArmPosition(50)
            .build();

    StandardOutputLogStream logStream;

    FCFSManager fcfsManager(disk);
    SSTFManager sstfManager(disk);
    CSCANManager cscanManager(disk);
    SCANManager scanManager(disk);

    std::array<manager, 4> managers {
        manager("FCFS", fcfsManager),
        manager("SSTF", sstfManager),
        manager("SCAN", scanManager),
        manager("CSAN", cscanManager)
    };

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<uint_fast16_t > timeDist(1, 1000);
    std::uniform_int_distribution<uint_fast16_t > positionDist(1, 1000);
    std::uniform_int_distribution<uint_fast16_t > probability(0, 1);

    for(uint_fast16_t i = 0; i<sample; i++) {
        DiskRequestBuilder::getInstance()
                .setQueuedTime(timeDist(mt))
                .setTrackPosition(positionDist(mt));

        if(probability(mt) == 1) DiskRequestBuilder::getInstance().setTimeToDeadline(timeDist(mt)/10);
        auto request = DiskRequestBuilder::getInstance().build();

        for(manager & m : managers) {
            m.second.enqueueRequest(request);
        }

    }

    manager const * fastestManager = managers.begin();
    manager const * slowestManager = managers.begin();
    auto filterFunc = [](const Log & log) -> bool { return log.getTitle() == "Result"; };

    for(manager m : managers) {
        m.second.setLogStream(&logStream);
        std::cout<<m.first<<" "<<std::to_string(m.second.simulate())<<std::endl;
        //logStream.filter(filterFunc, logStream);
        logStream.writeAll();
        std::cout<<std::endl;
        logStream.clear();
    }








}

