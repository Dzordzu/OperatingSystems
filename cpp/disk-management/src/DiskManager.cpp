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

    while(!queue.empty()) {
        uint_fast64_t next = findNext();

        if(next != disk.getArmPosition()) moveArmTo(next);
        time++;
    }
}

void DiskManagement::Manager::init() {
    this->operations = 0;
    this->time = 0;
    this->disk.moveArmTo(0);

    std::stable_sort(queue.begin(), queue.end(), [](const QueuedTrack &qt1, const QueuedTrack qt2){
        return qt1.getQueuedTime() < qt2.getQueuedTime();
    });
}

void DiskManagement::Manager::moveArmTo(uint_fast64_t next) {

    logStream->add(Log("Move", "Moving to " + std::to_string(next)));

    uint_fast32_t previousArmPosition = disk.getArmPosition(); // before move
    uint_fast32_t servicingDistance = disk.moveArmTo(next);

    if(!disk.isServicingOnRun()) servicingDistance = 0;
    service(previousArmPosition, servicingDistance, disk.goesRight());

}

void DiskManagement::Manager::service(uint_fast32_t initialPosition, uint_fast32_t distance, bool goesRight) {

    for(size_t i = 0; i<queue.size(); i++) {
        auto it = queue.begin() + i;

        uint_fast32_t trackPosition = it->getTrackPosition();
        uint_fast32_t trackDistance = trackPosition - initialPosition;
        bool trackOnTheRight = trackDistance > 0;
        distance = distance > 0 ? distance : -distance;

        if((trackDistance < distance && goesRight == trackOnTheRight) || trackDistance == distance) {
            logStream->add(Log("Service", "Servicing and removing on the position  " + std::to_string(trackPosition)));
            queue.erase(it);
            i--;
        }
    }
}

DiskManagement::Manager::Manager(DiskManagement::Disk &disk) : disk(disk) {}

uint_fast32_t DiskManagement::FCFSManager::findNext() {
    if(queue.begin()->getQueuedTime() > time) return disk.getArmPosition();
    return queue.begin()->getTrackPosition();
}

DiskManagement::FCFSManager::FCFSManager(DiskManagement::Disk &disk) : Manager(disk) {}
