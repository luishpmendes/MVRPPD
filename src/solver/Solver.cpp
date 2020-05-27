#include "Solver.hpp"

#include <fstream>

Solver::Solver(const Instance & instance, unsigned timeLimit, unsigned seed, 
        unsigned maxNumSolutions) : 
    instance(instance), 
    timeLimit(timeLimit), 
    seed(seed),
    maxNumSolutions(maxNumSolutions), 
    solvingTime(0),
    solutionSet(maxNumSolutions) {}


Solver::Solver(const Instance & instance, unsigned timeLimit, unsigned seed) :
    Solver::Solver(instance, timeLimit, seed, 2 * instance.getNumVertices()) {}


Solver::Solver(const Instance & instance, unsigned timeLimit) : 
    Solver::Solver(instance, timeLimit, 
            std::chrono::system_clock::now().time_since_epoch().count()) {}

Solver::Solver(const Instance & instance) : Solver::Solver(instance, 3600) {}

Solver::Solver() : Solver::Solver(Instance(), 0) {}

bool Solver::areTerminationCriteriaMet() const {
    if(this->getElapsedTime() >= this->timeLimit) {
        return true;
    }

    return false;
}

Instance Solver::getInstance() const {
    return this->instance;
}

unsigned Solver::getMaxNumSolutions() const {
    return this->maxNumSolutions;
}

unsigned Solver::getSolvingTime() const {
    return this->solvingTime;
}

SolutionSet Solver::getSolutionSet() const {
    return this->solutionSet;
}

unsigned Solver::getElapsedTime(const std::chrono::steady_clock::time_point & startTime) {
    std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
    std::chrono::seconds elapsedTime = std::chrono::duration_cast<std::chrono::seconds> (currentTime
            - startTime);
    return elapsedTime.count();
}

unsigned Solver::getElapsedTime() const {
    return Solver::getElapsedTime(this->startTime);
}

unsigned Solver::getRemainingTime(const std::chrono::steady_clock::time_point & startTime,
        unsigned timeLimit) {
    return timeLimit - Solver::getElapsedTime(startTime);
}

unsigned Solver::getRemainingTime(const std::chrono::steady_clock::time_point & startTime) const {
    return Solver::getRemainingTime(startTime, this->timeLimit);
}

unsigned Solver::getRemainingTime(unsigned int timeLimit) const {
    return Solver::getRemainingTime(this->startTime, timeLimit);
}

unsigned Solver::getRemainingTime() const {
    return Solver::getRemainingTime(this->startTime, this->timeLimit);
}

void Solver::write(std::ostream & os) const {
    // TO DO
}

void Solver::write(const char * filename) const {
    std::ofstream ofs;
    ofs.open(filename);

    if(ofs.is_open()) {
        this->write(ofs);
        ofs.close();
    } else {
        throw "File not created.";
    }
}

void Solver::write(const std::string & filename) const {
    this->write(filename.c_str());
}

void Solver::write() const {
    this->write(std::cout);
}

