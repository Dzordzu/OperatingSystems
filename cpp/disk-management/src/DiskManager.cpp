//
// Created by dzordzu on 04.04.19.
//

#include <DiskManagement/DiskManager.hpp>

void DiskManagement::Manager::setLogStream(LogStream *logStream) {
    Manager::logStream = logStream;
}

void DiskManagement::Manager::setDisk(const DiskManagement::Disk &disk) {
    Manager::disk = disk;
}

uint_fast64_t DiskManagement::Manager::simulate() {
    init();
    if(logStream != nullptr) logStream->add(Log("General", "Started algorithm"));

    while(!queue.empty()) {
        uint_fast64_t next = findNext();
        if(disk.getArmPosition() != next) moveArmTo(next);

        time++;
        if(time > 1000) {
            if(logStream != nullptr) logStream->add(Log("General", "Maximum time exceeded"));
            break;
        }
    }

    return operations;
}

void DiskManagement::Manager::init() {
    this->operations = 0;
    this->time = 0;
    this->disk.moveArmTo(0);

    std::stable_sort(queue.begin(), queue.end(), [](const DiskRequest &qt1, const DiskRequest qt2){
        return qt1.getQueuedTime() < qt2.getQueuedTime();
    });
}

void DiskManagement::Manager::moveArmTo(uint_fast64_t next) {

    if(logStream != nullptr) logStream->add(Log("Move", "Moving to " + std::to_string(next)));

    uint_fast32_t previousArmPosition = disk.getArmPosition(); // before move
    uint_fast32_t servicingDistance = disk.moveArmTo(next);

    if(!disk.isServicingOnRun()) {

        operations += disk.getSingleTrackMovementCost() * servicingDistance;
        time += servicingDistance;

        servicingDistance = 0;
        previousArmPosition = disk.getArmPosition();
    }

    service(previousArmPosition, servicingDistance, disk.goesRight());

    operations += disk.getSingleTrackMovementCost() * servicingDistance;
    time += servicingDistance;

}

void DiskManagement::Manager::service(uint_fast32_t initialPosition, uint_fast32_t distance, bool goesRight) {

    if(logStream != nullptr) logStream->add(Log("General", "Started servicing"));

    for(size_t i = 0; i<queue.size(); i++) {
        auto it = queue.begin() + i;

        uint_fast32_t trackPosition = it->getTrackPosition();
        int_fast64_t trackDistance = trackPosition - initialPosition;
        bool trackOnTheRight = trackDistance > 0;
        trackDistance = trackDistance > 0 ? trackDistance : -trackDistance;

        if(it->getQueuedTime() > time + trackDistance) continue;
        if((trackDistance < distance && goesRight == trackOnTheRight) || trackDistance == distance) {
            if(logStream != nullptr) logStream->add(Log("Service", "Servicing " + std::to_string(it->getTrackPosition())));
            queue.erase(it);
            i--;
        }

    }
}

DiskManagement::Manager::Manager(DiskManagement::Disk &disk) : disk(disk) {}

void DiskManagement::Manager::enqueueRequest(DiskRequest request) {
    if(logStream != nullptr) logStream->add(Log("General", "Asked to enqueue " + std::to_string(request.getTrackPosition())));

    if(disk.getSize() > request.getTrackPosition()) {
        queue.emplace_back(request);
        if(logStream != nullptr) logStream->add(Log("General", "Accepted"));
    }
    else {

        DiskRequestBuilder::getInstance()
                .setTrackPosition(request.getTrackPosition() % disk.getSize())
                .setQueuedTime(request.getQueuedTime());
        if(request.isRealTime()) DiskRequestBuilder::getInstance().setTimeToDeadline(request.getDeadlineTime());

        queue.emplace_back(DiskRequestBuilder::getInstance().build());

        if(logStream != nullptr) {

            std::string logMessage = "The original request value (track) has been rejected: Reason ";
            logMessage += std::to_string(disk.getSize());
            logMessage += " < ";
            logMessage += std::to_string(request.getTrackPosition());

            logStream->add(Log("Note", logMessage));
        }
    }

}

uint_fast32_t DiskManagement::FCFSManager::findNext() {
    if(queue.begin()->getQueuedTime() > time) return disk.getArmPosition();
    if(logStream != nullptr) logStream->add(Log("General", "Found next " + std::to_string(queue.begin()->getTrackPosition())));
    return queue.begin()->getTrackPosition();
}

DiskManagement::FCFSManager::FCFSManager(DiskManagement::Disk &disk) : Manager(disk) {}
