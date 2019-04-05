//
// Created by dzordzu on 04.04.19.
//

#include <DiskManagement/Disk.hpp>
#include <iostream>

uint_fast32_t DiskManagement::DiskRequest::getQueuedTime() const {
    return queuedTime;
}

uint_fast32_t DiskManagement::DiskRequest::getTrackPosition() const {
    return trackPosition;
}

bool DiskManagement::DiskRequest::isRealTime() const {
    return realTime;
}

uint_fast32_t DiskManagement::DiskRequest::getDeadlineTime() const {
    return queuedTime + timeToDeadline;
}

DiskManagement::DiskRequest::DiskRequest(uint_fast32_t queuedTime, uint_fast32_t trackPosition,
                                         uint_fast32_t timeToDeadline) : queuedTime(queuedTime),
                                                                         trackPosition(trackPosition),
                                                                         realTime(true),
                                                                         timeToDeadline(timeToDeadline) {}

DiskManagement::DiskRequest::DiskRequest(uint_fast32_t queuedTime, uint_fast32_t trackNumber) : queuedTime(queuedTime),
                                                                                                realTime(false),
                                                                                                trackPosition(
                                                                                                        trackNumber) {}

DiskManagement::QueuedTrackBuilder & DiskManagement::QueuedTrackBuilder::setQueuedTime(uint_fast32_t queuedTime) {
    QueuedTrackBuilder::queuedTime = queuedTime;
    return *this;
}

DiskManagement::QueuedTrackBuilder & DiskManagement::QueuedTrackBuilder::setTrackPosition(uint_fast32_t trackNumber) {
    QueuedTrackBuilder::trackPosition = trackNumber;
    return *this;
}

DiskManagement::QueuedTrackBuilder & DiskManagement::QueuedTrackBuilder::setTimeToDeadline(uint_fast32_t timeToDeadline) {
    realTime = true;
    QueuedTrackBuilder::timeToDeadline = timeToDeadline;
    return *this;
}

DiskManagement::QueuedTrackBuilder &DiskManagement::QueuedTrackBuilder::getInstance() {
    static QueuedTrackBuilder instance;
    return instance;
}

DiskManagement::DiskRequest DiskManagement::QueuedTrackBuilder::build() {
    DiskManagement::DiskRequest queuedTrack = realTime ? DiskManagement::DiskRequest(queuedTime, trackPosition, timeToDeadline) : DiskManagement::DiskRequest(queuedTime, trackPosition);
    realTime = false;
    timeToDeadline = 0;
    trackPosition = 0;
    queuedTime = 0;
    return queuedTrack;
}

uint_fast32_t DiskManagement::Disk::getSize() const {
    return size;
}

uint_fast32_t DiskManagement::Disk::getArmPosition() const {
    return armPosition;
}

uint_fast8_t DiskManagement::Disk::getDataReadCost() const {
    return dataReadCost;
}

uint32_t DiskManagement::Disk::moveArmTo(uint32_t nextPosition) {
    int_fast64_t result = nextPosition - this->armPosition;
    this->armPosition = nextPosition;
    goesRightValue = result > 0;
    return result > 0 ? result : -result;
}

uint_fast8_t DiskManagement::Disk::getSingleTrackMovementCost() const {
    return singleTrackMovementCost;
}

bool DiskManagement::Disk::goesRight() const {
    return goesRightValue;
}

bool DiskManagement::Disk::isServicingOnRun() const {
    return servicingOnRun;
}

DiskManagement::Disk::Disk(uint_fast32_t size, uint_fast32_t armPosition, uint_fast8_t dataReadCost,
                           uint_fast8_t singleTrackMovementCost, bool servicingOnRun) : size(size),
                                                                                        armPosition(armPosition),
                                                                                        dataReadCost(dataReadCost),
                                                                                        singleTrackMovementCost(
                                                                                                singleTrackMovementCost),
                                                                                        servicingOnRun(
                                                                                                servicingOnRun) {}

DiskManagement::DiskBuilder & DiskManagement::DiskBuilder::setSize(uint_fast32_t size) {
    DiskBuilder::size = size;
    return *this;
}

DiskManagement::DiskBuilder & DiskManagement::DiskBuilder::setDataReadCost(uint_fast8_t dataReadCost) {
    DiskBuilder::dataReadCost = dataReadCost;
    return *this;
}

DiskManagement::DiskBuilder & DiskManagement::DiskBuilder::setSingleTrackMovementCost(
        uint_fast8_t singleTrackMovementCost) {
    DiskBuilder::singleTrackMovementCost = singleTrackMovementCost;
    return *this;
}

DiskManagement::Disk DiskManagement::DiskBuilder::build() {
    DiskManagement::Disk disk(size, armPosition, dataReadCost, singleTrackMovementCost, servicingOnRun);
    size = 0;
    armPosition = 0;
    dataReadCost = 0;
    singleTrackMovementCost = 1;
    servicingOnRun = false;
}

DiskManagement::DiskBuilder & DiskManagement::DiskBuilder::setArmPosition(uint_fast32_t armPosition) {
    DiskBuilder::armPosition = armPosition;
    return *this;
}

DiskManagement::DiskBuilder & DiskManagement::DiskBuilder::enableServicingOnRun(bool servicingOnRun) {
    DiskBuilder::servicingOnRun = servicingOnRun;
    return *this;
}
