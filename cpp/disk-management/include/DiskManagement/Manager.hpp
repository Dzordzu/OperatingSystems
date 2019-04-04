//
// Created by dzordzu on 04.04.19.
//

#ifndef OPERATING_SYSTEMS_CPP_MANAGER_HPP
#define OPERATING_SYSTEMS_CPP_MANAGER_HPP

#include <vector>
#include "DiskCall.hpp"


class Manager {
protected:
    std::vector<DiskCall> queue;
    std::vector<DiskCall> diskCalls;

    uint_fast64_t movements;

public:
    virtual void simulate() = 0;

};


#endif //OPERATING_SYSTEMS_CPP_MANAGER_HPP
