//
// Created by dzordzu on 04.04.19.
//

#ifndef OPERATING_SYSTEMS_CPP_DISK_HPP
#define OPERATING_SYSTEMS_CPP_DISK_HPP

#include <cstdint>

namespace DiskManagement {
    class QueuedTrack {
        uint_fast32_t queuedTime;
        uint_fast32_t trackPosition;

        bool realTime;
        uint_fast32_t timeToDeadline;

    public:
        QueuedTrack(uint_fast32_t queuedTime, uint_fast32_t trackPosition, uint_fast32_t timeToDeadline);
        QueuedTrack(uint_fast32_t queuedTime, uint_fast32_t trackNumber);

        uint_fast32_t getQueuedTime() const;
        uint_fast32_t getTrackPosition() const;
        bool isRealTime() const;
        uint_fast32_t getDeadlineTime() const;
    };

    class QueuedTrackBuilder {
        QueuedTrackBuilder() = default;
        uint_fast32_t queuedTime;
        uint_fast32_t trackPosition;
        uint_fast32_t timeToDeadline;
        bool realTime = false;

    public:
        static QueuedTrackBuilder & getInstance();
        QueuedTrackBuilder(const QueuedTrackBuilder &) = delete;
        void operator=(const QueuedTrackBuilder &) = delete;

        void setQueuedTime(uint_fast32_t queuedTime);
        void setTrackPosition(uint_fast32_t trackNumber);
        void setTimeToDeadline(uint_fast32_t timeToDeadline);

        QueuedTrack build();
    };


    class Disk {
        uint_fast32_t size;
        uint_fast32_t armPosition;
        uint_fast8_t dataReadCost;
        uint_fast8_t singleTrackMovementCost;
        bool goesRightValue = true; //0 if left, 1 if right
        bool servicingOnRun;

    public:
        Disk(uint_fast32_t size, uint_fast32_t armPosition, uint_fast8_t dataReadCost,
             uint_fast8_t singleTrackMovementCost, bool servicingOnRun);

        uint_fast32_t getSize() const;
        uint_fast32_t getArmPosition() const;
        uint_fast8_t getDataReadCost() const;
        uint_fast8_t getSingleTrackMovementCost() const;
        bool goesRight() const;
        bool isServicingOnRun() const;

        uint32_t moveArmTo(uint32_t nextPosition);
    };

    class DiskBuilder {
        DiskBuilder() = default;
        uint_fast32_t size;
        uint_fast32_t armPosition;
        uint_fast8_t dataReadCost = 0;
        uint_fast8_t singleTrackMovementCost = 1;
        bool servicingOnRun = false;

    public:
        DiskBuilder(const DiskBuilder&) = delete;
        void operator=(const DiskBuilder&) = delete;
        inline static DiskBuilder & getInstance() { static DiskBuilder instance; return instance; }

        void setSize(uint_fast32_t size);
        void setArmPosition(uint_fast32_t armPosition);
        void setDataReadCost(uint_fast8_t dataReadCost);
        void setSingleTrackMovementCost(uint_fast8_t singleTrackMovementCost);
        void enableServicingOnRun(bool servicingOnRun);

        Disk build();
    };
}



#endif //OPERATING_SYSTEMS_CPP_DISK_HPP
