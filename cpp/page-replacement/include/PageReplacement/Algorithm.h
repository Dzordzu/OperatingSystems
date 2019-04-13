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
            Frame() = default;
            Frame(Page *page, bool callBit);

            Page * page = nullptr;
            bool callBit = false;
        };

        struct Call {
            Call() = default;
            Call(Page *page, Time time);

            Page * page = nullptr;
            Time time = Time(0);
        };

        class Algorithm {
        protected:
            std::vector<Frame> frames = {Frame{nullptr, false}};
            std::vector<Call> calls;
            uint_fast64_t pageErrors;
            Time time;
            virtual std::vector<Frame>::iterator findNextVictim() = 0;
        public:
            void setFramesAmount(uint_fast64_t framesAmount);
            void addCall(Call const & call);
            virtual uint_fast64_t simulate();
            uint_fast64_t getPageErrors() const;
        };


        class OPTIMAL : public Algorithm {
        protected:
            std::vector<Frame>::iterator findNextVictim() override;
        };
        class FIFO : public Algorithm {};
        class LRU : public Algorithm {};
        class SCA : public Algorithm {};
        class RAND : public Algorithm {};

    }
}


#endif //OPERATING_SYSTEMS_CPP_ALGORITHM_H
