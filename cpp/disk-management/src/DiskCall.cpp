//
// Created by dzordzu on 04.04.19.
//

#include <DiskManagement/DiskCall.hpp>

#include "DiskManagement/DiskCall.hpp"

DiskCall::DiskCall(const uint_fast32_t position, const uint_fast32_t accelerationTime, bool isAsync,
                   const uint_fast64_t deadline) : position(position), accelerationTime(accelerationTime),
                                                   async(isAsync), deadline(deadline) {}

DiskCallBuilder & DiskCallBuilder::setPosition(uint_fast32_t position) {
    DiskCallBuilder::position = position;
    return *this;
}

DiskCallBuilder & DiskCallBuilder::setAccelerationTime(uint_fast32_t accelerationTime) {
    DiskCallBuilder::accelerationTime = accelerationTime;
    return *this;
}

DiskCallBuilder & DiskCallBuilder::setIsAsync(bool isAsync) {
    DiskCallBuilder::isAsync = isAsync;
    return *this;
}

DiskCallBuilder & DiskCallBuilder::setDeadline(uint_fast64_t deadline) {
    DiskCallBuilder::deadline = deadline;
    return *this;
}

DiskCall DiskCallBuilder::build() {
    return {position, accelerationTime, isAsync, deadline};
}
