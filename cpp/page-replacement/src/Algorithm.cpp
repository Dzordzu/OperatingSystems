//
// Created by Dzordzu on 12.04.2019.
//

#include "PageReplacement/Algorithm.h"

void OperatingSystems::PageReplacement::Algorithm::setPagesAmount(uint_fast64_t pagesAmount) {
    this->pages.clear();
    this->pages.resize(pagesAmount);
    for(uint_fast64_t i = 0; i<pagesAmount; i++) this->pages.emplace_back(i);
}

void OperatingSystems::PageReplacement::Algorithm::setFramesAmount(uint_fast64_t framesAmount) {
    this->frames.clear();
    this->pages.resize(framesAmount);
    for(uint_fast64_t i = 0; i<framesAmount; i++) this->frames.emplace_back(i);
}

void OperatingSystems::PageReplacement::Algorithm::addCall(const OperatingSystems::PageReplacement::Call &call) {
    this->calls.emplace_back(call);
}

void OperatingSystems::PageReplacement::Algorithm::simulate() {
    std::sort(calls.begin(), calls.end(), [=](Call const & c1, Call const & c2){
        return c1.time < c2.time;
    });

    for(Call const & call : calls) {

        bool found = false;
        for(Frame const & frame : frames) {
            if(frame.page == call.page) found = true;
        }

        if(!found) {
            auto it = findNextVictim(frames);
            it->page = call.page;
        }
    }

}
