//
// Created by dzordzu on 04.04.19.
//

#ifndef OPERATING_SYSTEMS_CPP_DISKMANAGEMENT_HPP
#define OPERATING_SYSTEMS_CPP_DISKMANAGEMENT_HPP


#include "Disk.hpp"
#include "CppUtils/Logger.hpp"

#include <vector>
#include <algorithm>
#include <functional>
#include <limits>

namespace DiskManagement {

    class Manager {
    protected:
        LogStream * logStream = nullptr;
        Disk disk;
        std::vector<DiskRequest> queue = {};

        uint_fast64_t operations = 0;
        uint_fast64_t time = 0;
        bool alwaysMove = false;

        void moveArmTo(uint_fast64_t next);
        void service(uint_fast32_t initialPosition, uint_fast32_t distance, bool goesRight);
        virtual void init();
        virtual uint_fast32_t findNext() = 0;

        const uint_fast32_t getDistance(DiskRequest const &to) const;
        const uint_fast32_t isDirectedRight(DiskRequest const &to) const;

    public:
        explicit Manager(Disk &disk);

        void enqueueRequest(DiskRequest request);
        void setLogStream(LogStream *logStream);
        void setDisk(const Disk &disk);

        uint_fast64_t simulate();

    };


    class FCFSManager : public Manager {
    public:
        explicit FCFSManager(Disk &disk);
        uint_fast32_t findNext() override;
    };

    class SSTFManager : public Manager {
    public:
        explicit SSTFManager(Disk &disk);
        uint_fast32_t findNext() override;
    };

    class SCANManager : public Manager {
        double CSCANReturnCostProportion = 0.5;
        bool CSCAN = false;
        bool lastRequest = true;

        void CSCANMoveArmToStart();
    public:
        explicit SCANManager(Disk &disk);
        uint_fast32_t findNext() override;

        void useLastRequest(bool useLastRequest);
        void setModeToCSCAN(bool CSCAN);
        void setCSCANReturnCostProportion(double CSCANReturnCost);
    };
}




#endif //OPERATING_SYSTEMS_CPP_DISKMANAGEMENT_HPP
