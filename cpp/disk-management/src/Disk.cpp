//
// Created by dzordzu on 04.04.19.
//

#include <DiskManagement/Disk.hpp>
#include <iostream>

uint_fast32_t DiskManagement::QueuedTrack::getQueuedTime() const {
    return queuedTime;
}

uint_fast32_t DiskManagement::QueuedTrack::getTrackNumber() const {
    return trackNumber;
}

bool DiskManagement::QueuedTrack::isRealTime() const {
    return realTime;
}

uint_fast32_t DiskManagement::QueuedTrack::getDeadlineTime() const {
    return queuedTime + timeToDeadline;
}

DiskManagement::QueuedTrack::QueuedTrack(uint_fast32_t queuedTime, uint_fast32_t trackNumber,
                                         uint_fast32_t timeToDeadline) : queuedTime(queuedTime),
                                                                         trackNumber(trackNumber),
                                                                         realTime(true),
                                                                         timeToDeadline(timeToDeadline) {}

DiskManagement::QueuedTrack::QueuedTrack(uint_fast32_t queuedTime, uint_fast32_t trackNumber) : queuedTime(queuedTime),
                                                                                                realTime(false),
                                                                                                trackNumber(
                                                                                                        trackNumber) {}

void DiskManagement::QueuedTrackBuilder::setQueuedTime(uint_fast32_t queuedTime) {
    QueuedTrackBuilder::queuedTime = queuedTime;
}

void DiskManagement::QueuedTrackBuilder::setTrackNumber(uint_fast32_t trackNumber) {
    QueuedTrackBuilder::trackNumber = trackNumber;
}

void DiskManagement::QueuedTrackBuilder::setTimeToDeadline(uint_fast32_t timeToDeadline) {
    realTime = true;
    QueuedTrackBuilder::timeToDeadline = timeToDeadline;
}

DiskManagement::QueuedTrackBuilder &DiskManagement::QueuedTrackBuilder::getInstance() {
    static QueuedTrackBuilder instance;
    return instance;
}

DiskManagement::QueuedTrack DiskManagement::QueuedTrackBuilder::build() {
    DiskManagement::QueuedTrack queuedTrack = realTime ? DiskManagement::QueuedTrack(queuedTime, trackNumber, timeToDeadline) : DiskManagement::QueuedTrack(queuedTime, trackNumber);
    realTime = false;
    timeToDeadline = 0;
    trackNumber = 0;
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

DiskManagement::Disk::Disk(uint_fast32_t size, uint_fast32_t armPosition, uint_fast8_t dataReadCost) : size(size),
                                                                                                       armPosition(
                                                                                                               armPosition),
                                                                                                       dataReadCost(
                                                                                                               dataReadCost) {}

void DiskManagement::Disk::moveArmTo(uint32_t position) {
    this->armPosition = position;
}

uint_fast8_t DiskManagement::Disk::getSingleTrackMovementCost() const {
    return singleTrackMovementCost;
}

DiskManagement::Disk::Disk(uint_fast32_t size, uint_fast32_t armPosition, uint_fast8_t dataReadCost,
                           uint_fast8_t singleTrackMovementCost) : size(size), armPosition(armPosition),
                                                                   dataReadCost(dataReadCost),
                                                                   singleTrackMovementCost(singleTrackMovementCost) {}

void DiskManagement::DiskBuilder::setSize(uint_fast32_t size) {
    DiskBuilder::size = size;
}

void DiskManagement::DiskBuilder::setDataReadCost(uint_fast8_t dataReadCost) {
    DiskBuilder::dataReadCost = dataReadCost;
}

void DiskManagement::DiskBuilder::setSingleTrackMovementCost(uint_fast8_t singleTrackMovementCost) {
    DiskBuilder::singleTrackMovementCost = singleTrackMovementCost;
}

DiskManagement::Disk DiskManagement::DiskBuilder::build() {
    DiskManagement::Disk disk(size, armPosition, dataReadCost, singleTrackMovementCost);
    size = 0;
    armPosition = 0;
    dataReadCost = 0;
    singleTrackMovementCost = 1;
}

void DiskManagement::DiskBuilder::setArmPosition(uint_fast32_t armPosition) {
    DiskBuilder::armPosition = armPosition;
}
