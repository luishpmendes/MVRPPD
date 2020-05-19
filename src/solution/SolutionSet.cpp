#include "SolutionSet.hpp"
#include <algorithm>
#include <cfloat>
#include <limits>

void SolutionSet::sort() {
    // non-dominated sorting
    std::vector<std::vector<std::pair<Solution, unsigned>>> frontsAux(1);
    std::vector<std::pair<Solution, unsigned>> solutionsVector;

    for(auto solution : this->solutions) {
        solutionsVector.push_back(std::make_pair(solution, solutionsVector.size()));
    }

    std::vector<unsigned> dominationCount(this->solutions.size(), 0);
    std::vector<std::vector<std::pair<Solution,
        unsigned>>>dominatedSolutions(this->solutions.size());

    for(const auto & p : solutionsVector) {
        for(const auto & q : solutionsVector) {
            if(p.first.dominates(q.first)) {
                dominatedSolutions[p.second].push_back(q);
            } else if(q.first.dominates(p.first)) {
                dominationCount[p.second]++;
            }
        }

        if(dominationCount[p.second] == 0) {
            frontsAux.back().push_back(p);
        }
    }

    unsigned i = 0;
    while(!frontsAux[i].empty()) {
        frontsAux.push_back(std::vector<std::pair<Solution, unsigned>>());

        for(const auto & p : frontsAux[i]) {
            for(const auto & q : dominatedSolutions[p.second]) {
                dominationCount[q.second]--;

                if(dominationCount[q.second] == 0) {
                    frontsAux[i + 1].push_back(q);
                }
            }
        }

        i++;
    }

    while(frontsAux.back().empty()) {
        frontsAux.pop_back();
    }

    this->fronts.clear(); 

    for(auto frontAux : frontsAux) {
        this->fronts.emplace_back();

        for(auto solutionPair : frontAux) {
            this->fronts.back().push_back(solutionPair.first);
        }
    }

    this->minTotalProfitsF = std::vector<double>(this->fronts.size(),
            std::numeric_limits<double>::max());
    this->maxTotalProfitsF = std::vector<double>(this->fronts.size(),
            std::numeric_limits<double>::lowest());
    this->minSumTFulfillsF = std::vector<double>(this->fronts.size(),
            std::numeric_limits<double>::max());
    this->maxSumTFulfillsF = std::vector<double>(this->fronts.size(),
            std::numeric_limits<double>::lowest());

    // crowding sorting each front
    for(unsigned int f = 0; f < this->fronts.size(); f++) {
        std::vector<std::pair<std::pair<double, unsigned>, Solution>> front;
        std::vector<double> distance(this->fronts[f].size(), 0.0);

        for(auto solution : this->fronts[f]) {
            front.push_back(std::make_pair(std::make_pair(solution.getTotalProfit(), 
                            front.size()), solution));
        }

        std::sort(front.begin(), front.end());

        this->minTotalProfitsF[f] = front.front().first.first;
        this->maxTotalProfitsF[f] = front.back().first.first;

        distance[front.front().first.second] = std::numeric_limits<double>::max();
        distance[front.back().first.second] = std::numeric_limits<double>::max();

        for(i = 1; i < front.size() - 1; i++) {
            if(distance[front[i].first.second] < std::numeric_limits<double>::max()) {
                distance[front[i].first.second] += 
                    (front[i + 1].second.getTotalProfit() - front[i - 1].second.getTotalProfit())/
                    (this->maxTotalProfitsF[f] - this->minTotalProfitsF[f]);
            }
        }

        for(i = 0; i < front.size(); i++) {
            front[i].first.first = front[i].second.getSumTFulfill();
        }

        std::sort(front.begin(), front.end());

        this->minSumTFulfillsF[f] = front.front().first.first;
        this->maxSumTFulfillsF[f] = front.back().first.first;

        distance[front.front().first.second] = std::numeric_limits<double>::max();
        distance[front.back().first.second] = std::numeric_limits<double>::max();

        for(i = 1; i < front.size() - 1; i++) {
            if(distance[front[i].first.second] < std::numeric_limits<double>::max()) {
                distance[front[i].first.second] += 
                    (front[i + 1].second.getSumTFulfill() - front[i - 1].second.getSumTFulfill())/
                    (this->maxSumTFulfillsF[f] - this->minSumTFulfillsF[f]);
            }
        }

        for(i = 0; i < front.size(); i++) {
            front[i].first.first = distance[front[i].first.second];
        }

        std::sort(front.begin(), front.end(), 
                std::greater<std::pair<std::pair<double, unsigned>, Solution>>());

        for(i = 0; i < front.size(); i++) {
            this->fronts[f][i] = front[i].second;
        }
    }

    while(this->solutions.size() > this->maxSize) {
        if(this->fronts.empty()) {
            break;
        }

        while(this->fronts.back().empty()) {
            this->fronts.pop_back();
        }

        auto solution = this->fronts.back().back();
        this->fronts.back().pop_back();
        this->solutions.erase(solution);
    }

    this->sorted = true;
}

SolutionSet::SolutionSet(unsigned maxSize, const std::set<Solution> & solutions) :
    maxSize(maxSize),
    solutions(solutions),
    sorted(false) {
    if(this->solutions.size() > this->maxSize) {
        this->sort();
    }
}

SolutionSet::SolutionSet(unsigned maxSize) : 
    maxSize(maxSize),
    sorted(false) {}

SolutionSet::SolutionSet() : 
    maxSize(0), 
    sorted(false) {}

unsigned SolutionSet::getMaxSize() const {
    return this->maxSize;
}

std::set<Solution> SolutionSet::getSolutions() const {
    return this->solutions;
}

std::vector<std::vector<Solution>> SolutionSet::getFronts() {
    if(!this->sorted) {
        this->sort();
    }

    return this->fronts;
}

std::vector<Solution> SolutionSet::getFront(unsigned f) {
    if(!this->sorted) {
        this->sort();
    }

    return this->fronts[f];
}

std::vector<double> SolutionSet::getMinTotalProfitsF() {
    if(!this->sorted) {
        this->sort();
    }

    return this->minTotalProfitsF;
}

double SolutionSet::getMinTotalProfitF(unsigned f) {
    if(!this->sorted) {
        this->sort();
    }

    return this->minTotalProfitsF[f];
}

std::vector<double> SolutionSet::getMaxTotalProfitsF() {
    if(!this->sorted) {
        this->sort();
    }

    return this->maxTotalProfitsF;
}

double SolutionSet::getMaxTotalProfitF(unsigned f) {
    if(!this->sorted) {
        this->sort();
    }

    return this->maxTotalProfitsF[f];
}

std::vector<double> SolutionSet::getMinSumTFulfillsF() {
    if(!this->sorted) {
        this->sort();
    }

    return this->minSumTFulfillsF;
}

double SolutionSet::getMinSumTFulfillF(unsigned f) {
    if(!this->sorted) {
        this->sort();
    }

    return this->minSumTFulfillsF[f];
}

std::vector<double> SolutionSet::getMaxSumTFulfillsF() {
    if(!this->sorted) {
        this->sort();
    }

    return this->maxSumTFulfillsF;
}

double SolutionSet::getMaxSumTFulfillF(unsigned f) {
    if(!this->sorted) {
        this->sort();
    }

    return this->maxSumTFulfillsF[f];
}

void SolutionSet::insert(const Solution & solution) {
    this->solutions.insert(solution);
    this->sorted = false;

    if(this->solutions.size() > this->maxSize) {
        this->sort();
    }
}

void SolutionSet::insert(const std::set<Solution> & solutions) {
    this->solutions.insert(solutions.begin(), solutions.end());
    this->sorted = false;

    if(this->solutions.size() > this->maxSize) {
        this->sort();
    }
}

