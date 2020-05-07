#include "Instance.hpp"

#include <cmath>
#include <cfloat>
#include <climits>
#include <fstream>
#include <limits>
#include <numeric>

void Instance::init() {
    // compute the flags and demand of each vertex
    this->requestsV = std::vector<unsigned int>(this->numVertices, this->numRequests);
    this->vehiclesV = std::vector<unsigned int>(this->numVertices, this->numVehicles);
    this->isSourceV = std::vector<bool>(this->numVertices, false);
    this->isTargetV = std::vector<bool>(this->numVertices, false);
    this->demandsV = std::vector<double>(this->numVertices, 0.0);

    for(unsigned r = 0; r < this->numRequests; r++) {
        this->requestsV[this->sourcesR[r]] = r;
        this->requestsV[this->targetsR[r]] = r;
        this->isSourceV[this->sourcesR[r]] = true;
        this->isTargetV[this->targetsR[r]] = true;
        this->demandsV[this->sourcesR[r]] = this->demandsR[r];
        this->demandsV[this->targetsR[r]] = -this->demandsR[r];
    }

    for(unsigned k = 0; k < this->numVehicles; k++) {
        this->vehiclesV[this->sourcesK[k]] = k;
        this->vehiclesV[this->targetsK[k]] = k;
        this->isSourceV[this->sourcesK[k]] = true;
        this->isTargetV[this->targetsK[k]] = true;
    }

    // compute the length of each edge
    this->lengths = std::vector<std::vector<double>>(this->numVertices,
            std::vector<double>(this->numVertices));

    for(unsigned u = 0; u < this->numVertices; u++) {
        for(unsigned int v = 0; v < this->numVertices; v++) {
            this->lengths[u][v] = sqrt((this->x[u] - this->x[v])*(this->x[u]-this->x[v]) +
                    (this->y[u] - this->y[v])*(this->y[u] - this->y[v]));
        }
    }
}

void Instance::init(std::istream & is) {
    this->numVehicles = this->numRequests = this->numVertices = 0;

    this->sumProfit = 0.0;

    char c;

    while(is >> c) {
        if(c == 'k') {
            this->tStarts.push_back(0.0);
            this->tLimits.push_back(0.0);
            this->speeds.push_back(1.0);
            this->capacities.push_back(0.0);
            this->sourcesK.push_back(UINT_MAX);
            this->targetsK.push_back(UINT_MAX);

            is >> this->tStarts[this->numVehicles]
               >> this->tLimits[this->numVehicles]
               >> this->speeds[this->numVehicles]
               >> this->capacities[this->numVehicles];

            this->sourcesK[this->numVehicles] = this->numVertices;

            this->x.push_back(0.0);
            this->y.push_back(0.0);
            this->tVisits.push_back(0.0);

            is >> this->x[this->numVertices]
               >> this->y[this->numVertices]
               >> this->tVisits[this->numVertices];

            this->numVertices++;

            this->targetsK[this->numVehicles] = this->numVertices;

            this->x.push_back(0.0);
            this->y.push_back(0.0);
            this->tVisits.push_back(0.0);

            is >> this->x[this->numVertices]
               >> this->y[this->numVertices]
               >> this->tVisits[this->numVertices];

            this->numVertices++;
            this->numVehicles++;
        } else if(c == 'r') {
            this->profits.push_back(0.0);
            this->tCreates.push_back(0.0);
            this->demandsR.push_back(0.0);
            this->sourcesR.push_back(UINT_MAX);
            this->targetsR.push_back(UINT_MAX);

            is >> this->profits[this->numRequests]
               >> this->tCreates[this->numRequests]
               >> this->demandsR[this->numRequests];

            this->sumProfit += this->profits[this->numRequests];

            this->sourcesR[this->numRequests] = this->numVertices;

            this->x.push_back(0.0);
            this->y.push_back(0.0);
            this->tVisits.push_back(0.0);

            is >> this->x[this->numVertices]
               >> this->y[this->numVertices]
               >> this->tVisits[this->numVertices];

            this->numVertices++;

            this->targetsR[this->numRequests] = this->numVertices;

            this->x.push_back(0.0);
            this->y.push_back(0.0);
            this->tVisits.push_back(0.0);

            is >> this->x[this->numVertices]
               >> this->y[this->numVertices]
               >> this->tVisits[this->numVertices];

            this->numVertices++;
            this->numRequests++;
        } else {
            break;
        }
    }

    this->init();
}

Instance::Instance(const std::vector<double> & profits, const std::vector<double> & tCreates, 
        const std::vector<double> & demandsR, const std::vector<unsigned> & sourcesR,
        const std::vector<unsigned> & targetsR, const std::vector<double> & tStarts,
        const std::vector<double> & tLimits, const std::vector<double> & speeds,
        const std::vector<double> & capacities, const std::vector<unsigned> & sourcesK,
        const std::vector<unsigned> & targetsK, const std::vector<double> x,
        const std::vector<double> y, const std::vector<double> tVisits) : 
    profits(profits),
    tCreates(tCreates),
    demandsR(demandsR),
    sourcesR(sourcesR),
    targetsR(targetsR),
    tStarts(tStarts),
    tLimits(tLimits),
    speeds(speeds),
    capacities(capacities),
    sourcesK(sourcesK),
    targetsK(targetsK),
    x(x),
    y(y),
    tVisits(tVisits) {
    this->numRequests = this->profits.size();
    this->numVehicles = this->tLimits.size();
    this->numVertices = this->x.size();

    this->sumProfit = std::accumulate(this->profits.begin(), this->profits.end(), 0.0);

    this->init();
}

Instance::Instance(std::istream & is) {
    this->init(is);
}

Instance::Instance(const char * filename) {
    std::ifstream ifs;
    ifs.open(filename);

    if(ifs.is_open()) {
        this->init(ifs);
    } else {
        throw "File not found.";
    }
}

Instance::Instance(const std::string & filename) : Instance::Instance(filename.c_str()) {}

Instance::Instance() {}

unsigned Instance::getNumRequests() const {
    return this->numRequests;
}
        
unsigned Instance::getNumVehicles() const {
    return this->numVehicles;
}
        
unsigned Instance::getNumVertices() const {
    return this->numVertices;
}

unsigned Instance::getSumProfit() const {
    return this->sumProfit;
}

std::vector<double> Instance::getProfits() const {
    return this->profits;
}

double Instance::getProfit(unsigned r) const {
    return this->profits[r];
}

std::vector<double> Instance::getTCreates() const {
    return this->tCreates;
}

double Instance::getTCreate(unsigned r) const {
    return this->tCreates[r];
}

std::vector<double> Instance::getDemandsR() const {
    return this->demandsR;
}

double Instance::getDemandR(unsigned r) const {
    return this->demandsR[r];
}

std::vector<unsigned> Instance::getSourcesR() const {
    return this->sourcesR;
}

unsigned Instance::getSourceR(unsigned r) const {
    return this->sourcesR[r];
}

std::vector<unsigned> Instance::getTargetsR() const {
    return this->targetsR;
}

unsigned Instance::getTargetR(unsigned r) const {
    return this->targetsR[r];
}

std::vector<double> Instance::getTStarts() const {
    return this->tStarts;
}

double Instance::getTStart(unsigned k) const {
    return this->tStarts[k];
}

std::vector<double> Instance::getTLimits() const {
    return this->tLimits;
}

double Instance::getTLimit(unsigned k) const {
    return this->tLimits[k];
}

std::vector<double> Instance::getSpeeds() const {
    return this->speeds;
}

double Instance::getSpeed(unsigned k) const {
    return this->speeds[k];
}

std::vector<double> Instance::getCapacities() const {
    return this->capacities;
}

double Instance::getCapacity(unsigned k) const {
    return this->capacities[k];
}

std::vector<unsigned> Instance::getSourcesK() const {
    return this->sourcesK;
}

unsigned Instance::getSourceK(unsigned k) const {
    return this->sourcesK[k];
}

std::vector<unsigned> Instance::getTargetsK() const {
    return this->targetsK;
}

unsigned Instance::getTargetK(unsigned k) const {
    return this->targetsK[k];
}

std::vector<double> Instance::getX() const {
    return this->x;
}

double Instance::getX(unsigned v) const {
    return this->x[v];
}

std::vector<double> Instance::getY() const {
    return this->y;
}

double Instance::getY(unsigned v) const {
    return this->y[v];
}

std::vector<double> Instance::getTVisits() const {
    return this->tVisits;
}

double Instance::getTVisit(unsigned v) const {
    return this->tVisits[v];
}

std::vector<unsigned> Instance::getRequestsV() const {
    return this->requestsV;
}

unsigned Instance::getRequestV(unsigned v) const {
    return this->requestsV[v];
}

std::vector<unsigned> Instance::getVehiclesV() const {
    return this->vehiclesV;
}

unsigned Instance::getVehicleV(unsigned v) const {
    return this->vehiclesV[v];
}

bool Instance::isASource(unsigned v) const {
    return this->isSourceV[v];
}

bool Instance::isATarget(unsigned v) const {
    return this->isTargetV[v];
}

std::vector<double> Instance::getDemandsV() const {
    return this->demandsV;
}

double Instance::getDemandV(unsigned v) const {
    return this->demandsV[v];
}

std::vector<std::vector<double>> Instance::getLengths() const {
    return this->lengths;
}

double Instance::getLength(unsigned u, unsigned v) const {
    return this->lengths[u][v];
}

bool Instance::isValid(unsigned & error) const {
    error = 0;

    // check if there is 2 vertices for each request and each vehicle
    if(this->numVertices != 2*this->numRequests + 2*this->numVehicles) {
        error = 1;
        return false;
    }

    // check if there is |R| profit values
    if(this->profits.size() != this->numRequests) {
        error = 2;
        return false;
    }

    // check if there is |R| tCreate values
    if(this->tCreates.size() != this->numRequests) {
        error = 3;
        return false;
    }

    // check if there is |R| demand values
    if(this->demandsR.size() != this->numRequests) {
        error = 4;
        return false;
    }

    // check if there is |R| source vertices for requests
    if(this->sourcesR.size() != this->numRequests) {
        error = 5;
        return false;
    }

    // check if there is |R| target vertices for requests
    if(this->targetsR.size() != this->numRequests) {
        error = 6;
        return false;
    }

    // check if there is |K| tStart values
    if(this->tStarts.size() != this->numVehicles) {
        error = 7;
        return false;
    }

    // check if there is |K| tLimit values
    if(this->tLimits.size() != this->numVehicles) {
        error = 8;
        return false;
    }

    // check if there is |K| speed values
    if(this->speeds.size() != this->numVehicles) {
        error = 9;
        return false;
    }

    // check if there is |K| capacity values
    if(this->capacities.size() != this->numVehicles) {
        error = 10;
        return false;
    }

    // check if there is |K| source vertices for vehicles
    if(this->sourcesK.size() != this->numVehicles) {
        error = 11;
        return false;
    }

    // check if there is |K| target vertices for vehicles
    if(this->targetsK.size() != this->numVehicles) {
        error = 12;
        return false;
    }

    // check if there is |V| tVisit values
    if(this->tVisits.size() != this->numVertices) {
        error = 13;
        return false;
    }

    // check if there is a length value for each pair of vertices
    if(this->lengths.size() != this->numVertices) {
        error = 14;
        return false;
    }

    for(unsigned i = 0; i < this->numVertices; i++) {
        if(this->lengths[i].size() != this->numVertices) {
            error = 15;
            return false;
        }
    }

    // check if all profit values are non negative
    for(unsigned r = 0; r < this->numRequests; r++) {
        if(this->profits[r] < 0) {
            error = 16;
            return false;
        }
    }

    // check if all tCreate values are non negative
    for(unsigned r = 0; r < this->numRequests; r++) {
        if(this->tCreates[r] < 0) {
            error = 17;
            return false;
        }
    }

    // check if all demand values are non negative
    for(unsigned r = 0; r < this->numRequests; r++) {
        if(this->demandsR[r] < 0) {
            error = 18;
            return false;
        }
    }

    // check if all tStart values are non negative
    for(unsigned k = 0; k < this->numVehicles; k++) {
        if(this->tStarts[k] < 0) {
            error = 19;
            return false;
        }
    }

    // check if all tLimit values are non negative
    for(unsigned k = 0; k < this->numVehicles; k++) {
        if(this->tLimits[k] < 0) {
            error = 20;
            return false;
        }
    }

    // check if all speed values are positive
    for(unsigned k = 0; k < this->numVehicles; k++) {
        if(this->speeds[k] <= 0) {
            error = 21;
            return false;
        }
    }

    // check if all capacity values are non negative
    for(unsigned k = 0; k < this->numVehicles; k++) {
        if(this->capacities[k] < 0) {
            error = 22;
            return false;
        }
    }

    // check if all source vertices for requests are valid
    for(unsigned r = 0; r < this->numRequests; r++) {
        if(this->sourcesR[r] > this->numVertices) {
            error = 23;
            return false;
        }
    }

    // check if all target vertices for requests are valid
    for(unsigned r = 0; r < this->numRequests; r++) {
        if(this->targetsR[r] > this->numVertices) {
            error = 24;
            return false;
        }
    }

    // check if all source vertices for vehicles are valid
    for(unsigned k = 0; k < this->numVehicles; k++) {
        if(this->sourcesK[k] > this->numVertices) {
            error = 25;
            return false;
        }
    }

    // check if all target vertices for vehicles are valid
    for(unsigned k = 0; k < this->numVehicles; k++) {
        if(this->targetsK[k] > this->numVertices) {
            error = 26;
            return false;
        }
    }

    // check if each vertex is used exactly once
    std::vector<unsigned> isVertexUsed (this->numVertices, 0);

    for(unsigned r = 0; r < this->numRequests; r++) {
        if(isVertexUsed[this->sourcesR[r]] > 0) {
            error = 27;
            return false;
        }

        isVertexUsed[this->sourcesR[r]]++;
    }

    for(unsigned r = 0; r < this->numRequests; r++) {
        if(isVertexUsed[this->targetsR[r]] > 0) {
            error = 28;
            return false;
        }

        isVertexUsed[this->targetsR[r]]++;
    }

    for(unsigned k = 0; k < this->numVehicles; k++) {
        if(isVertexUsed[this->sourcesK[k]] > 0) {
            error = 29;
            return false;
        }

        isVertexUsed[this->sourcesK[k]]++;
    }

    for(unsigned k = 0; k < this->numVehicles; k++) {
        if(isVertexUsed[this->targetsK[k]] > 0) {
            error = 30;
            return false;
        }

        isVertexUsed[this->targetsK[k]]++;
    }

    for(unsigned v = 0; v < this->numVertices; v++) {
        if(isVertexUsed[v] != 1) {
            error = 31;
            return false;
        }
    }

    // check if all tVisit values are non negative
    for(unsigned v = 0; v < this->numVertices; v++) {
        if(this->tVisits[v] < 0) {
            error = 32;
            return false;
        }
    }

    // check if all length values are non negative
    for(unsigned u = 0; u < this->numVertices; u++) {
        for(unsigned v = 0; v < this->numVertices; v++) {
            if(this->lengths[u][v] < 0) {
                error = 33;
                return false;
            }
        }
    }

    // check if the lengths are symmetric
    for(unsigned u = 0; u < this->numVertices; u++) {
        for(unsigned v = 0; v < this->numVertices; v++) {
            if(fabs(this->lengths[u][v] - this->lengths[v][u]) >
                    std::numeric_limits<float>::epsilon()) {
                error = 34;
                return false;
            }
        }
    }

    // check if the lengths respect the triangular inequality
    for(unsigned u = 0; u < this->numVertices; u++) {
        for(unsigned v = 0; v < this->numVertices; v++) {
            for(unsigned w = 0; w < this->numVertices; w++) {
                if(this->lengths[u][w] > this->lengths[u][v] + this->lengths[v][w] +
                        std::numeric_limits<float>::epsilon()) {
                    error = 35;
                    return false;
                }
            }
        }
    }

    return true;
}

bool Instance::isValid() const {
    bool result;
    unsigned error;

    result = this->isValid(error);

    return result;
}

void Instance::write(std::ostream & os) const {
    for(unsigned k = 0; k < this->numVehicles; k++) {
        os << "k" 
           << this->tStarts[k] << " " 
           << this->tLimits[k] << " " 
           << this->speeds[k] << " " 
           << this->capacities[k] << " " 
           << this->x[this->sourcesK[k]] << " " 
           << this->y[this->sourcesK[k]] << " " 
           << this->tVisits[this->sourcesK[k]] << " " 
           << this->x[this->targetsK[k]] << " " 
           << this->y[this->targetsK[k]] << " " 
           << this->tVisits[this->targetsK[k]] << std::endl;
    }

    for(unsigned r = 0; r < this->numRequests; r++) {    
        os << "r" << this->profits[r] << " " 
           << this->tCreates[r] << " " 
           << this->demandsR[r] << " "
           << this->x[this->sourcesR[r]] << " " 
           << this->y[this->sourcesR[r]] << " " 
           << this->tVisits[this->sourcesR[r]] << " " 
           << this->x[this->targetsR[r]] << " " 
           << this->y[this->targetsR[r]] << " " 
           << this->tVisits[this->targetsR[r]] << std::endl;
    }
}

void Instance::write(const char * filename) const {
    std::ofstream ofs;
    ofs.open(filename);

    if(ofs.is_open()) {
        this->write(ofs);
        ofs.close();
    } else {
        throw "File not created.";
    }
}

void Instance::write() const {
    this->write(std::cout);
}

