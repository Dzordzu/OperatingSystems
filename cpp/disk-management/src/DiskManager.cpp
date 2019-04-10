//
// Created by dzordzu on 04.04.19.
//

#include <DiskManagement/DiskManager.hpp>

/**
 * @XXX This smells
 */
using namespace OperatingSystems;

void DiskManagement::Manager::setLogStream(LogStream * const logStream) {
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
        if(queue.begin()->getQueuedTime() > time) continue;
        uint_fast64_t next = findNext();
        if(next != disk.getArmPosition() || alwaysMove) moveArmTo(next);

        if(time > maxTime) {
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

    if(logStream != nullptr) {
        std::string logMessage = "Moving to " + std::to_string(next);
        logMessage += " time(" + std::to_string(time) + ")";
        logStream->add(Log("Move", logMessage));
    }

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

        bool right = goesRight && initialPosition + distance == trackPosition;
        bool left = !goesRight && initialPosition - distance == trackPosition;

        if((trackDistance < distance && goesRight == trackOnTheRight) || left || right) {

            if(logStream != nullptr) {
                std::string logMessage = "Servicing ";
                logMessage += std::to_string(it->getTrackPosition()) + " ";
                logMessage += "that has been queued at " + std::to_string(it->getQueuedTime());
                logMessage += " time(" + std::to_string(time + trackDistance) + ")";
                logMessage += ", "; it->isRealTime() ? logMessage += "real time" : logMessage += "normal";


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
    if(logStream != nullptr) {
        Log log("General", "Asked to enqueue " + std::to_string(request.getTrackPosition()));
        logStream->add(log);
    }

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

const uint_fast32_t DiskManagement::Manager::getDistance(DiskRequest const &to) const {
    uint_fast32_t target = to.getTrackPosition();
    uint_fast32_t from = disk.getArmPosition();
    return target > from ? target - from : from - target;
}

const uint_fast32_t DiskManagement::Manager::isDirectedRight(DiskRequest const &to) const {
    return disk.getArmPosition() < to.getTrackPosition();
}

void DiskManagement::Manager::setMaxTime(uint_fast64_t maxTime) {
    Manager::maxTime = maxTime;
}

uint_fast32_t DiskManagement::FCFSManager::findNext() {
    auto it = queue.rbegin() - 1;
    int_fast64_t shortestRealTimeTrackPosition = -1, shortestRealTimeValue = INT_FAST64_MAX;

    for(int i=0; i<queue.size(); i++) {
        it++;
        if(!it->isRealTime() || it->getQueuedTime() > time) continue;

        if(it->getDeadlineTime() < shortestRealTimeValue)
            shortestRealTimeTrackPosition = it->getTrackPosition();
    }

    if(logStream != nullptr) logStream->add(Log("General", "Found next " + std::to_string(queue.begin()->getTrackPosition())));
    return shortestRealTimeTrackPosition == -1 ? queue.begin()->getTrackPosition() : shortestRealTimeTrackPosition;
}

DiskManagement::FCFSManager::FCFSManager(DiskManagement::Disk &disk) : Manager(disk) {}

uint_fast32_t DiskManagement::SSTFManager::findNext() {

    auto it = std::min_element(queue.begin(), queue.end(), [=](const DiskRequest &d1, const DiskRequest &d2) {

        /**
         * TODO fix this shitty error with time, add methods to calculate it
         */
        if(d1.getQueuedTime() >= time) { // && (c1.getQueuedTime() >= time && c2.getQueuedTime() >= time)
            return false;
        }

        if(d2.getQueuedTime() >= time) {
            return true;
        }

        if(d1.isRealTime() && !d2.isRealTime()) return true;
        if(!d1.isRealTime() && d2.isRealTime()) return false;
        if(d1.isRealTime() && d2.isRealTime()) {
            // not d1<d2, because of equity
            if(d1.getDeadlineTime() < d2.getDeadlineTime()) return true;
            if(d1.getDeadlineTime() > d2.getDeadlineTime()) return false;
        }

        int_fast64_t difference1 = disk.getArmPosition() - d1.getTrackPosition();
        difference1 = difference1 < 0 ? -difference1 : difference1;

        int_fast64_t difference2 = disk.getArmPosition() - d2.getTrackPosition();
        difference2 = difference2 < 0 ? -difference2 : difference2;

        return difference1 < difference2;
    });

    return it->getTrackPosition();



}

DiskManagement::SSTFManager::SSTFManager(DiskManagement::Disk &disk) : Manager(disk) {}

DiskManagement::CSCANManager::CSCANManager(DiskManagement::Disk &disk) : Manager(disk) { alwaysMove = false; }

uint_fast32_t DiskManagement::CSCANManager::findNext() {

    auto it = std::max_element(queue.begin(), queue.end(), [=](const DiskRequest & d1, const DiskRequest & d2) {
        if(d2.getQueuedTime() > time) return false;
        if(d1.getQueuedTime() > time) return true;

        if(d1.isRealTime() && !d2.isRealTime()) return false;
        if(!d1.isRealTime() && d2.isRealTime()) return true;
        if(d1.isRealTime() && d2.isRealTime()) {
            // not d1<d2, because of equity
            if(d1.getDeadlineTime() < d2.getDeadlineTime()) return true;
            if(d1.getDeadlineTime() > d2.getDeadlineTime()) return false;
        }

        return d1.getTrackPosition() < d2.getTrackPosition();
    });

    if(it->getTrackPosition() < disk.getArmPosition()) {
        CSCANMoveArmToStart();
        service(0, 0, false);
    }

    return lastFirstMode ? it->getTrackPosition() : disk.getSize();
}

void DiskManagement::CSCANManager::setCSCANReturnCostProportion(double CSCANReturnCostProportion) {
    CSCANManager::CSCANReturnCostProportion = CSCANReturnCostProportion;
}

void DiskManagement::CSCANManager::CSCANMoveArmToStart() {
    operations += (uint_fast32_t)(CSCANReturnCostProportion * disk.getArmPosition());
    disk.moveArmTo(0);
}

//
//uint_fast32_t DiskManagement::CSCANManager::findNextTarget() {
//    disk.getArmPosition() = 0;
//    return maxSize;
//}
void DiskManagement::SCANLike::enableLastFirstMode(bool lastFirstMode) {
    SCANLike::lastFirstMode = lastFirstMode;
}

DiskManagement::SCANManager::SCANManager(DiskManagement::Disk &disk) : Manager(disk) {}

uint_fast32_t DiskManagement::SCANManager::findNext() {

    uint_fast32_t position;

    if(!lastFirstMode) {
        return disk.goesRight() ? 0 : disk.getSize();
    }

    auto minIt = std::min_element(queue.begin(), queue.end(), [=](const DiskRequest & d1, const DiskRequest & d2){
        if(d2.getQueuedTime() > time) return true;
        if(d1.getQueuedTime() > time) return false;

        if(d1.isRealTime() && !d2.isRealTime()) return false;
        if(!d1.isRealTime() && d2.isRealTime()) return true;
        if(d1.isRealTime() && d2.isRealTime()) {
            // not d1<d2, because of equity
            if(d1.getDeadlineTime() < d2.getDeadlineTime()) return false;
            if(d1.getDeadlineTime() > d2.getDeadlineTime()) return true;
        }

        return d1.getTrackPosition() < d2.getTrackPosition();
    });

    auto maxIt = std::max_element(queue.begin(), queue.end(), [=](const DiskRequest & d1, const DiskRequest & d2) {
        if(d2.getQueuedTime() > time) return false;
        if(d1.getQueuedTime() > time) return true;

        if(d1.isRealTime() && !d2.isRealTime()) return false;
        if(!d1.isRealTime() && d2.isRealTime()) return true;
        if(d1.isRealTime() && d2.isRealTime()) {
            // not d1<d2, because of equity
            if(d1.getDeadlineTime() < d2.getDeadlineTime()) return true;
            if(d1.getDeadlineTime() > d2.getDeadlineTime()) return false;
        }

        return d1.getTrackPosition() < d2.getTrackPosition();
    });

    if(disk.goesRight()) {
        position = minIt->getTrackPosition();
        if(position == disk.getArmPosition()) position = maxIt->getTrackPosition();
    }
    else {
        position = maxIt->getTrackPosition();
        if(position == disk.getArmPosition()) position = minIt->getTrackPosition();
    }


    return position;
}
