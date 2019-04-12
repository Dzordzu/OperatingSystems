//
// Created by Dzordzu on 12.04.2019.
//

#ifndef OPERATING_SYSTEMS_CPP_ALGORITHM_H
#define OPERATING_SYSTEMS_CPP_ALGORITHM_H

#include <vector>
#include <cstdint>
#include <algorithm>

namespace OperatingSystems {
    namespace PageReplacement {

        typedef uint_fast64_t Page;
        typedef uint_fast64_t Time;

        struct Frame {
            Page &page;
            bool callBit = false;
        };

        struct Call {
            Page &page;
            Time time;
        };

        class Algorithm {
            std::vector<Page> pages;
            std::vector<Frame> frames;
            std::vector<Call> calls;

        public:
            void setPagesAmount(uint_fast64_t pagesAmount);
            void setFramesAmount(uint_fast64_t framesAmount);
            void addCall(Call const & call);
            virtual void simulate();
            virtual std::vector<Frame>::iterator findNextVictim(std::vector<Frame> & frames) = 0;
        };


        class OPTIMAL : public Algorithm {};
        class FIFO : public Algorithm {};
        class LRU : public Algorithm {};
        class SCA : public Algorithm {};
        class RAND : public Algorithm {};

    }
}


#endif //OPERATING_SYSTEMS_CPP_ALGORITHM_H
