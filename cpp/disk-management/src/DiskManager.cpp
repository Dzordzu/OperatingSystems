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

    moveArmTo(findNext());

    while(!queue.empty()) {
        time++;
        uint_fast64_t next = findNext();
        if(next != disk.getArmPosition()) moveArmTo(next);

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
    this->disk.moveArmTo(disk.getInitialArmPosition());

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

            if(logStream != nullptr) {
                std::string logMessage = "Servicing ";
                logMessage += std::to_string(it->getTrackPosition()) + " ";
                //logMessage += "that has been queued at " + std::to_string(it->getQueuedTime());
                //logMessage += " time(" + std::to_string(time) + ")";


                logStream->add(Log("Service", logMessage));
            }

            queue.erase(it);
            operations += disk.getDataReadCost();
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

        DiskRequest dr = DiskRequestBuilder::getInstance().build();
        queue.emplace_back(dr);

        if(logStream != nullptr) {

            std::string logMessage = "The original request value (track) has been rejected: Reason ";
            logMessage += std::to_string(disk.getSize());
            logMessage += " <= ";
            logMessage += std::to_string(request.getTrackPosition());
            logMessage += " . Converting track value to ";
            logMessage += std::to_string(request.getTrackPosition() % disk.getSize());

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

uint_fast32_t DiskManagement::SSTFManager::findNext() {

    if(queue.begin()->getQueuedTime() > time) {
        return disk.getArmPosition();
    }

    auto it = std::min_element(queue.begin(), queue.end(), [=](const DiskRequest &c1, const DiskRequest &c2) {

        if(c1.getQueuedTime() >= time) {
            return false;
        }

        if(c2.getQueuedTime() >= time) {
            return true;
        }

        int_fast64_t difference1 = disk.getArmPosition() - c1.getTrackPosition();
        difference1 = difference1 < 0 ? -difference1 : difference1;

        int_fast64_t difference2 = disk.getArmPosition() - c2.getTrackPosition();
        difference2 = difference2 < 0 ? -difference2 : difference2;

        return difference1 < difference2;
    });

    return it->getTrackPosition();



}

DiskManagement::SSTFManager::SSTFManager(DiskManagement::Disk &disk) : Manager(disk) {}

//uint_fast32_t DiskManagement::SCANManager::findNextTarget() {
//    return disk.getArmPosition() == maxSize ? 0 : maxSize;
//}
//
//uint_fast32_t DiskManagement::CSCANManager::findNextTarget() {
//    disk.getArmPosition() = 0;
//    return maxSize;
//}
