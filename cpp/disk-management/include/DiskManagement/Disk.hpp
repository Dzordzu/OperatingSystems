//
// Created by dzordzu on 04.04.19.
//

#ifndef OPERATING_SYSTEMS_CPP_DISK_HPP
#define OPERATING_SYSTEMS_CPP_DISK_HPP

#include <cstdint>

namespace DiskManagement {
    class QueuedTrack {
        uint_fast32_t queuedTime;
        uint_fast32_t trackNumber;

        bool realTime;
        uint_fast32_t timeToDeadline;

    public:
        QueuedTrack(uint_fast32_t queuedTime, uint_fast32_t trackNumber, uint_fast32_t timeToDeadline);
        QueuedTrack(uint_fast32_t queuedTime, uint_fast32_t trackNumber);

        uint_fast32_t getQueuedTime() const;
        uint_fast32_t getTrackNumber() const;
        bool isRealTime() const;
        uint_fast32_t getDeadlineTime() const;
    };

    class QueuedTrackBuilder {
        QueuedTrackBuilder(){}
        uint_fast32_t queuedTime;
        uint_fast32_t trackNumber;
        uint_fast32_t timeToDeadline;
        bool realTime = false;

    public:
        static QueuedTrackBuilder & getInstance();
        QueuedTrackBuilder(const QueuedTrackBuilder &) = delete;
        void operator=(const QueuedTrackBuilder &) = delete;
        void setQueuedTime(uint_fast32_t queuedTime);
        void setTrackNumber(uint_fast32_t trackNumber);
        void setTimeToDeadline(uint_fast32_t timeToDeadline);

        QueuedTrack build();
    };


    class Disk {
        uint_fast32_t size;
        uint_fast32_t armPosition;
        uint_fast8_t dataReadCost;

    public:
        Disk(uint_fast32_t size, uint_fast32_t armPosition, uint_fast8_t dataReadCost);

        uint_fast32_t getSize() const;
        uint_fast32_t getArmPosition() const;
        uint_fast8_t getDataReadCost() const;
    };
}



#endif //OPERATING_SYSTEMS_CPP_DISK_HPP
