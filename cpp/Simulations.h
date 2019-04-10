//
// Created by Dzordzu on 10.04.2019.
//

#ifndef OPERATING_SYSTEMS_CPP_PROCESSESMANAGEMENTSIMULATION_H
#define OPERATING_SYSTEMS_CPP_PROCESSESMANAGEMENTSIMULATION_H


#include <cstdint>
#include <random>
#include <array>
#include <ProcessesManagement/Manager.hpp>
#include <DiskManagement/DiskManager.hpp>


namespace Simulations {

    template<typename F, typename S>
    struct Pair {
        Pair(F first, S &second) : first(first), second(second) {}
        F first;
        S & second;
    };

    void processesManagement(uint_fast32_t sample);
    void diskManagement(uint_fast32_t sample);

};


#endif //OPERATING_SYSTEMS_CPP_PROCESSESMANAGEMENTSIMULATION_H
