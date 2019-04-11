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
            .setSingleTrackMovementCost(10)
            .setDataReadCost(1)
            .setArmPosition(50)
            .build();

    StandardOutputLogStream logStream;

    FCFSManager fcfsManager(disk);
    SSTFManager sstfManager(disk);
    CSCANManager cscanManager(disk);
    SCANManager scanManager(disk);

    FCFSManager fcfsManagerEDF(disk);
    SSTFManager sstfManagerEDF(disk);
    CSCANManager cscanManagerEDF(disk);
    SCANManager scanManagerEDF(disk);

    cscanManagerEDF.setCSCANReturnCostProportion(1);
    cscanManager.setCSCANReturnCostProportion(1);

    std::array<manager, 4> managers {
        manager("FCFS", fcfsManager),
        manager("SSTF", sstfManager),
        manager("SCAN", scanManager),
        manager("CSAN", cscanManager)

    };

    std::array<manager, 4> managersEDF {
            manager("FCFS EDF", fcfsManagerEDF),
            manager("SSTF EDF", sstfManagerEDF),
            manager("SCAN EDF", scanManagerEDF),
            manager("CSAN EDF", cscanManagerEDF)

    };

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<uint_fast16_t > timeDist(0, 500);
    std::uniform_int_distribution<uint_fast16_t > positionDist(0, 500);
    std::uniform_int_distribution<uint_fast16_t > probability(0, 1);

    for(uint_fast16_t i = 0; i<sample; i++) {

        uint_fast32_t queuedTime = timeDist(mt);
        uint_fast32_t trackPosition = positionDist(mt);
        uint_fast32_t timeToDeadline = timeDist(mt)/10;

        DiskRequestBuilder::getInstance()
                .setQueuedTime(queuedTime)
                .setTrackPosition(trackPosition);

        if(probability(mt) == 1) DiskRequestBuilder::getInstance().setTimeToDeadline(timeToDeadline);
        auto requestEDF = DiskRequestBuilder::getInstance().build();

        DiskRequestBuilder::getInstance()
                .setQueuedTime(queuedTime)
                .setTrackPosition(trackPosition);
        auto request = DiskRequestBuilder::getInstance().build();


        for(manager & m : managers) {
            m.second.enqueueRequest(request);
        }

        for(manager & m : managersEDF) {
            m.second.enqueueRequest(requestEDF);
        }

    }

    manager * fastestManager = managers.begin();
    manager * slowestManager = managers.begin();
    auto filterFunc = [](const Log & log) -> bool { return log.getTitle() == "Result"; };

    for(manager & m : managers) {

        m.second.setMaxTime(100000000);
        m.second.setLogStream(&logStream);
//        logStream.filter(filterFunc, logStream);

        uint_fast64_t result = m.second.simulate()/10;
        std::cout<<m.first<<" "<<std::to_string(result)<<std::endl;

//        logStream.writeAll();
        logStream.clear();
        fastestManager = fastestManager->second.getOperations() < result*10 ? fastestManager : &m;
        slowestManager = slowestManager->second.getOperations() > result*10 ? slowestManager : &m;
    }

    for(manager & m : managersEDF) {

        m.second.setMaxTime(100000000);
        m.second.setLogStream(&logStream);
//        logStream.filter(filterFunc, logStream);

        uint_fast64_t result = m.second.simulate()/10;
        std::cout<<m.first<<" "<<std::to_string(result)<<std::endl;

//        logStream.writeAll();
        logStream.clear();
        fastestManager = fastestManager->second.getOperations() < result*10 ? fastestManager : &m;
        slowestManager = slowestManager->second.getOperations() > result*10 ? slowestManager : &m;
    }


    std::cout<<"Fastest: "<<fastestManager->first<<std::endl;
    std::cout<<"Slowest: "<<slowestManager->first<<std::endl;





}

