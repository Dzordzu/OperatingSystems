//
// Created by dzordzu on 04.04.19.
//

#ifndef OPERATING_SYSTEMS_CPP_DISKMANAGEMENT_HPP
#define OPERATING_SYSTEMS_CPP_DISKMANAGEMENT_HPP


#include "DiskCall.hpp"
#include "CppUtils/Logger.hpp"

#include <vector>
#include <algorithm>
#include <functional>

namespace DiskManagement {

    template<typename T>
    int8_t sgn(const T & a) {
        return (T(0) < a) - (a < T(0));
    }

    class Manager {
    protected:
        std::vector<DiskCall> diskCalls;
        uint_fast64_t operations = 0;
        bool async = false;
        LogStream * logStream = nullptr;

        uint_fast32_t time = 0;
        uint_fast32_t position = 0;
        uint_fast32_t maxSize = 1000;
        bool idle;
        bool capturing = true;

        virtual uint_fast32_t findNextTarget() = 0;
        void moveTo(uint_fast32_t nextTarget);
        void handle();
    public:
        void setMaxSize(uint_fast32_t maxSize);
        void setLogStream(LogStream *stream);
        void setAsync(bool async);
        void setCapturing(bool capturing);
        virtual void addDiskCall(const DiskCall &diskCall);

        uint_fast64_t getOperations() const;
        uint_fast64_t simulate();
        virtual void reset();
    };



    class FCFSManager : public Manager {
    protected:
        uint_fast32_t findNextTarget() override;
    };

    class SSTFManager : public Manager {
    protected:
        uint_fast32_t findNextTarget() override;
    };

    class SCANManager : public Manager {
    protected:
        uint_fast32_t findNextTarget() override;
    };

    class CSCANManager : public  Manager {
    protected:
        uint_fast32_t findNextTarget() override;
    };
}



#endif //OPERATING_SYSTEMS_CPP_DISKMANAGEMENT_HPP
