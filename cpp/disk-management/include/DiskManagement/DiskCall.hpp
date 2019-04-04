//
// Created by dzordzu on 04.04.19.
//

#ifndef OPERATING_SYSTEMS_CPP_DISK_HPP
#define OPERATING_SYSTEMS_CPP_DISK_HPP

#include <cstdint>

class DiskCall {
    uint_fast32_t position;
    uint_fast32_t accelerationTime;
    bool async;
    uint_fast64_t deadline;
public:
    DiskCall(const uint_fast32_t position, const uint_fast32_t accelerationTime, bool isAsync,
             const uint_fast64_t deadline);

    uint_fast32_t getPosition() const;
    uint_fast32_t getAccelerationTime() const;
    bool isAsync() const;
    uint_fast64_t getTimeToDeadline() const;

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
    void operator=(const DiskCallBuilder &) = delete;


    DiskCallBuilder & setPosition(uint_fast32_t position);
    DiskCallBuilder & setAccelerationTime(uint_fast32_t accelerationTime);
    DiskCallBuilder & setIsAsync(bool isAsync);
    DiskCallBuilder & setTimeToDeadline(uint_fast64_t timeToDeadline);

    const DiskCall build();
};



#endif //OPERATING_SYSTEMS_CPP_DISK_HPP
