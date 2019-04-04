//
// Created by dzordzu on 04.04.19.
//

#include <DiskManagement/DiskManagement.hpp>


uint_fast64_t DiskManagement::Manager::simulate() {

    std::stable_sort(diskCalls.begin(), diskCalls.end(), [](const DiskCall &d1, const DiskCall &d2) -> bool {
        return d1.getAccelerationTime() < d2.getAccelerationTime();
    });

    this->position = 0;
    this->idle = true;

    while(!diskCalls.empty()) {


        uint_fast32_t nextTarget = findNextTarget();
        if (nextTarget != position) moveTo(nextTarget);

        time++;
    }

    return operations / 10;

}


uint_fast32_t DiskManagement::FCFSManager::findNextTarget() {
    if(diskCalls.begin()->getAccelerationTime() > time) return position;
    return diskCalls.begin()->getPosition();
}

void DiskManagement::Manager::addDiskCall(const DiskCall &diskCall) {
    if(diskCalls.end() == std::find_if(diskCalls.begin(), diskCalls.end(),[diskCall](DiskCall & call) -> bool {
        return call.getPosition() == diskCall.getPosition() && call.getAccelerationTime() == diskCall.getAccelerationTime();
    })) {
        if(logStream != nullptr) logStream->add({"Initial Add", "Adding to position " + std::to_string(diskCall.getPosition())});
        this->diskCalls.emplace_back(diskCall);
    }
}

void DiskManagement::Manager::setAsync(bool async) {
    this->async = async;
}

void DiskManagement::Manager::setLogStream(LogStream *stream) {
    this->logStream = stream;
}

uint_fast64_t DiskManagement::Manager::getOperations() const {
    return (operations / 10);
}

void DiskManagement::Manager::moveTo(uint_fast32_t nextTarget) {


    int64_t difference = nextTarget - position;
    int64_t distance = difference < 0 ? -difference : difference;

    if(difference == 0) return;

    for(int64_t i = 0; i<diskCalls.size(); i++) {
        auto it = diskCalls.begin() + i;
        int64_t individualDifference = it->getPosition() - position;
        int64_t individualDistance = individualDifference < 0 ? -individualDifference : individualDifference;

        if(it->getAccelerationTime() > time + individualDistance) break;
        if(individualDistance <= distance) {

            if(sgn(individualDifference) == sgn(difference)) {
                if(logStream != nullptr) logStream->add({"Remove", "Removing " + std::to_string(it->getPosition())+ ", overall " + std::to_string(operations/10)});
                diskCalls.erase(it);
                i--;
            }
        }
    }

    time += distance;
    operations += 10*distance;
    if(logStream != nullptr) logStream->add({"Move", "Moving to position " + std::to_string(nextTarget)});
    position = nextTarget;
}

void DiskManagement::Manager::reset() {
    this->position = 0;
    this->operations = 0;
    this->diskCalls = {};
    this->time = 0;
}

void DiskManagement::Manager::setCapturing(bool capturing) {
    Manager::capturing = capturing;
}

void DiskManagement::Manager::setMaxSize(uint_fast32_t maxSize) {
    Manager::maxSize = maxSize;
}

uint_fast32_t DiskManagement::SSTFManager::findNextTarget() {

    if(diskCalls.begin()->getAccelerationTime() > time) return position;

    auto it = std::min_element(diskCalls.begin(), diskCalls.end(), [=](const DiskCall &c1, const DiskCall &c2) {

        if(c1.getAccelerationTime() > time) {
            return false;
        }

        if(c2.getAccelerationTime() > time) {
            return true;
        }


        int_fast64_t difference1 = position - c1.getPosition();
        difference1 = difference1 < 0 ? -difference1 : difference1;

        int_fast64_t difference2 = position - c2.getPosition();
        difference2 = difference2 < 0 ? -difference2 : difference2;

        return difference1 < difference2;
    });

    return it->getPosition();


}

uint_fast32_t DiskManagement::SCANManager::findNextTarget() {
    return position == maxSize ? 0 : maxSize;
}

uint_fast32_t DiskManagement::CSCANManager::findNextTarget() {
    position = 0;
    return maxSize;
}
