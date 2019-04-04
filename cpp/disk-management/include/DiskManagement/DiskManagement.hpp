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
        LogStream * logStream;
        Disk disk;
        std::vector<QueuedTrack> queue;

        uint_fast64_t operations;
        uint_fast64_t time;

    public:


    };
}




#endif //OPERATING_SYSTEMS_CPP_DISKMANAGEMENT_HPP
