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

namespace DiskManagement {

    class Manager {
    protected:
        LogStream * logStream;
        Disk & disk;
        std::vector<DiskRequest> queue = {};

        uint_fast64_t operations;
        uint_fast64_t time;

        void moveArmTo(uint_fast64_t next);
        void service(uint_fast32_t initialPosition, uint_fast32_t distance, bool goesRight);
        virtual void init();
        virtual uint_fast32_t findNext() = 0;
    public:
        explicit Manager(Disk &disk);

        void enqueueRequest(DiskRequest track);
        void setLogStream(LogStream *logStream);
        void setDisk(const Disk &disk);

        uint_fast64_t simulate();
    };


    class FCFSManager : public Manager {
    public:
        explicit FCFSManager(Disk &disk);
        uint_fast32_t findNext() override;
    };
}




#endif //OPERATING_SYSTEMS_CPP_DISKMANAGEMENT_HPP
