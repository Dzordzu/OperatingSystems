//
// Created by Dzordzu on 12.04.2019.
//

#include <iostream>
#include "PageReplacement/Algorithm.h"


void OperatingSystems::PageReplacement::Algorithm::setFramesAmount(uint_fast64_t framesAmount) {
    this->frames.clear();
    this->frames.resize(framesAmount);
    for(uint_fast64_t i = 0; i<framesAmount; i++) this->frames.emplace_back(Frame{nullptr, false});

}

void OperatingSystems::PageReplacement::Algorithm::addCall(OperatingSystems::PageReplacement::Call const & call) {
    auto it = std::find_if(calls.begin(), calls.end(), [=](Call const & call1){return call1.time == call.time;});
    if(it == calls.end()) this->calls.emplace_back(call);
}

uint_fast64_t OperatingSystems::PageReplacement::Algorithm::simulate() {
    std::sort(calls.begin(), calls.end(), [=](Call const & c1, Call const & c2){
        return c1.time < c2.time;
    });

    for(Call const & call : calls) {

        bool found = false;
        for(Frame & frame : frames) {
            if(frame.page == nullptr) {
                frame.page = call.page;
                found = true;
                pageErrors++;
                break;
            }
            if(frame.page == call.page) {
                found = true;
                break;
            }
        }

        time = call.time;

        if(!found) {
            auto it = findNextVictim();
            it->page = call.page;
            pageErrors++;
        }
    }

    return pageErrors;

}

uint_fast64_t OperatingSystems::PageReplacement::Algorithm::getPageErrors() const {
    return pageErrors;
}

OperatingSystems::PageReplacement::Call::Call(OperatingSystems::PageReplacement::Page *page,
                                              OperatingSystems::PageReplacement::Time time) : page(page), time(time) {}

OperatingSystems::PageReplacement::Frame::Frame(OperatingSystems::PageReplacement::Page *page, bool callBit) : page(
        page), callBit(callBit) {}


std::vector<OperatingSystems::PageReplacement::Frame, std::allocator<OperatingSystems::PageReplacement::Frame>>::iterator
OperatingSystems::PageReplacement::OPTIMAL::findNextVictim() {
    auto resultFrameIt = frames.begin();
    uint_fast64_t maxDifference = 0;

    auto findNextCall = [=](std::vector<Frame>::iterator const & frame) {

        auto loopCallIterator = calls.begin();
        for(uint_fast64_t i=0; i<calls.size(); i++) {
            if(loopCallIterator->time <= time) {
                loopCallIterator++;
                continue;
            }
            if(loopCallIterator->page == frame->page) return loopCallIterator;

            loopCallIterator++;
        }
        return calls.end();
    };

    auto loopFrameIt = frames.begin();
    for(uint_fast64_t i=0; i<frames.size(); i++) {
        auto nextCall = findNextCall(loopFrameIt);
        if(nextCall == calls.end()) {
            loopFrameIt++;
            continue;
        }
        if(maxDifference < nextCall->page - loopFrameIt->page) {
            maxDifference = nextCall->page - loopFrameIt->page;
            resultFrameIt = loopFrameIt;
        }
        loopFrameIt++;
    }

    return maxDifference == 0 ? resultFrameIt + (pageErrors % frames.size()) : resultFrameIt;
}
