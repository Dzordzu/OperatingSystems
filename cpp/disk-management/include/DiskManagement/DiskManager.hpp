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

namespace OperatingSystems {
    namespace DiskManagement {

        using OperatingSystems::CppUtils::LogStream;
        using OperatingSystems::CppUtils::Log;

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

        /**
        * @IMPORTANT When lastFirstMode is turned off real time has no effect
        */
        class SCANLike {
        protected:
            bool lastFirstMode = true;
        public:
            void enableLastFirstMode(bool lastFirstMode);
        };


        class SCANManager : public Manager, public SCANLike {
        public:
            explicit SCANManager(Disk &disk);
        protected:
            uint_fast32_t findNext() override;
        };

        class CSCANManager : public Manager, public SCANLike {
            double CSCANReturnCostProportion = 0.5;
            void CSCANMoveArmToStart();
        public:
            explicit CSCANManager(Disk &disk);
            uint_fast32_t findNext() override;
            void setCSCANReturnCostProportion(double CSCANReturnCost);
        };
    }
}




#endif //OPERATING_SYSTEMS_CPP_DISKMANAGEMENT_HPP
