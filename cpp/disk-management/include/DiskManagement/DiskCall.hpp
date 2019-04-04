//
// Created by dzordzu on 04.04.19.
//

#ifndef OPERATING_SYSTEMS_CPP_DISK_HPP
#define OPERATING_SYSTEMS_CPP_DISK_HPP

#include <cstdint>

class DiskCall {
public:
    DiskCall(const uint_fast32_t position, const uint_fast32_t accelerationTime, bool isAsync,
             const uint_fast64_t deadline);

private:
    const uint_fast32_t position;
    const uint_fast32_t accelerationTime;
    const bool async;
    const uint_fast64_t deadline;
};

class DiskCallBuilder {

    DiskCallBuilder(){}

    uint_fast32_t position;
    uint_fast32_t accelerationTime;
    bool isAsync;
    uint_fast64_t deadline;
public:
    inline static DiskCallBuilder & getInstace() { static DiskCallBuilder instance; return instance; }
    DiskCallBuilder(DiskCallBuilder const&) = delete;
    void operator=(DiskCallBuilder const&) = delete;


    DiskCallBuilder & setPosition(uint_fast32_t position);
    DiskCallBuilder & setAccelerationTime(uint_fast32_t accelerationTime);
    DiskCallBuilder & setIsAsync(bool isAsync);
    DiskCallBuilder & setDeadline(uint_fast64_t deadline);

    DiskCall build();
};



#endif //OPERATING_SYSTEMS_CPP_DISK_HPP
