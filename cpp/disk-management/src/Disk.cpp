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
