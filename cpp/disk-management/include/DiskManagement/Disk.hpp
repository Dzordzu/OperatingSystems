//
// Created by dzordzu on 04.04.19.
//

#ifndef OPERATING_SYSTEMS_CPP_DISK_HPP
#define OPERATING_SYSTEMS_CPP_DISK_HPP

#include <cstdint>

namespace DiskManagement {
    class DiskRequest {
        uint_fast32_t queuedTime = 0;
        uint_fast32_t trackPosition = 0;

        bool realTime = false;
        uint_fast32_t timeToDeadline = 0;

    public:
        DiskRequest(uint_fast32_t queuedTime, uint_fast32_t trackPosition, uint_fast32_t timeToDeadline);
        DiskRequest(uint_fast32_t queuedTime, uint_fast32_t trackNumber);

        uint_fast32_t getQueuedTime() const;
        uint_fast32_t getTrackPosition() const;
        bool isRealTime() const;
        uint_fast32_t getDeadlineTime() const;
    };

    class DiskRequestBuilder {
        DiskRequestBuilder() {};
        uint_fast32_t queuedTime = 0;
        uint_fast32_t trackPosition = 0;
        uint_fast32_t timeToDeadline = 0;
        bool realTime = false;

    public:
        static DiskRequestBuilder& getInstance();
        DiskRequestBuilder(DiskRequestBuilder const&) = delete;
        void operator=(DiskRequestBuilder const&) = delete;

        DiskRequestBuilder & setQueuedTime(uint_fast32_t queuedTime);
        DiskRequestBuilder & setTrackPosition(uint_fast32_t trackNumber);
        DiskRequestBuilder & setTimeToDeadline(uint_fast32_t timeToDeadline);

        DiskRequest build();
    };


    class Disk {
        uint_fast32_t size;
        uint_fast32_t initialArmPosition;
        uint_fast32_t armPosition;
        uint_fast8_t dataReadCost;
        uint_fast8_t singleTrackMovementCost;
        bool goesRightValue = true; //0 if left, 1 if right
        bool servicingOnRun;

    public:
        Disk(uint_fast32_t size, uint_fast32_t initialArmPosition, uint_fast8_t dataReadCost,
             uint_fast8_t singleTrackMovementCost, bool servicingOnRun);

        uint_fast32_t getSize() const;
        uint_fast32_t getInitialArmPosition() const;
        uint_fast32_t getArmPosition() const;
        uint_fast8_t getDataReadCost() const;
        uint_fast8_t getSingleTrackMovementCost() const;
        bool goesRight() const;
        bool isServicingOnRun() const;

        uint32_t moveArmTo(uint32_t nextPosition);
    };

    class DiskBuilder {
        DiskBuilder() {};
        uint_fast32_t size = 0;
        uint_fast32_t armPosition = 0;
        uint_fast8_t dataReadCost = 0;
        uint_fast8_t singleTrackMovementCost = 1;
        bool servicingOnRun = false;

    public:
        DiskBuilder(const DiskBuilder&) = delete;
        void operator=(const DiskBuilder&) = delete;
        inline static DiskBuilder & getInstance() { static DiskBuilder instance; return instance; }

        DiskBuilder & setSize(uint_fast32_t size);
        DiskBuilder & setArmPosition(uint_fast32_t armPosition);
        DiskBuilder & setDataReadCost(uint_fast8_t dataReadCost);
        DiskBuilder & setSingleTrackMovementCost(uint_fast8_t singleTrackMovementCost);
        DiskBuilder & enableServicingOnRun(bool servicingOnRun);

        Disk build();
    };
}



#endif //OPERATING_SYSTEMS_CPP_DISK_HPP
