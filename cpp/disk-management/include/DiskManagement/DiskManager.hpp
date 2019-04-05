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
        std::vector<QueuedTrack> queue = {};

        uint_fast64_t operations;
        uint_fast64_t time;

        void moveArmTo(uint_fast64_t next);

        virtual void init();
    public:
        explicit Manager(Disk &disk);
        void setLogStream(LogStream *logStream);
        void setDisk(const Disk &disk);
        uint_fast64_t simulate();

        void service(uint_fast32_t initialPosition, uint_fast32_t distance, bool goesRight);
        virtual uint_fast32_t findNext() = 0;
    };


    class FCFSManager : public Manager {
    public:
        explicit FCFSManager(Disk &disk);
        uint_fast32_t findNext() override;
    };
}




#endif //OPERATING_SYSTEMS_CPP_DISKMANAGEMENT_HPP
