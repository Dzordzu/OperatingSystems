//
// Created by dzordzu on 04.04.19.
//

#include <DiskManagement/DiskCall.hpp>

#include "DiskManagement/DiskCall.hpp"

#include <iostream>

DiskCall::DiskCall(const uint_fast32_t position, const uint_fast32_t accelerationTime, bool isAsync,
                   const uint_fast64_t deadline) : position(position), accelerationTime(accelerationTime),
                                                   async(isAsync), deadline(deadline) {}

uint_fast32_t DiskCall::getPosition() const {
    return position;
}

uint_fast32_t DiskCall::getAccelerationTime() const {
    return accelerationTime;
}

bool DiskCall::isAsync() const {
    return async;
}

uint_fast64_t DiskCall::getTimeToDeadline() const {
    return deadline;
}

DiskCallBuilder & DiskCallBuilder::setPosition(uint_fast32_t position) {
    this->position = position;
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

DiskCallBuilder & DiskCallBuilder::setTimeToDeadline(uint_fast64_t timeToDeadline) {
    DiskCallBuilder::deadline = timeToDeadline;
    return *this;
}

const DiskCall DiskCallBuilder::build() {
    return { position, accelerationTime, isAsync, deadline };
}
