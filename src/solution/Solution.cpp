#include "Solution.hpp"

#include <cmath>
#include <cfloat>
#include <fstream>
#include <limits>

void Solution::init() {
    this->tTravels = std::vector<double>(this->instance.getNumVehicles(), 0.0);
    this->tFulfills = std::vector<double>(this->instance.getNumRequests(), 0.0);
    this->totalProfit = 0.0;
    this->sumTFulfill = 0.0;
    this->loads = std::vector<std::vector<double>>(this->instance.getNumVehicles());
    this->times = std::vector<std::vector<double>>(this->instance.getNumVehicles());
    this->vehiclesR = std::vector<unsigned>(this->instance.getNumRequests(),
            this->instance.getNumVehicles());
    this->vehiclesV = std::vector<unsigned>(this->instance.getNumVertices(),
            this->instance.getNumVehicles());
    this->vehiclesE = std::vector<std::vector<unsigned>>(this->instance.getNumVertices());
    this->requestsK = std::vector<std::unordered_set<unsigned>>(this->instance.getNumVehicles());
    this->indexesKV = std::vector<std::vector<unsigned>>(this->instance.getNumVehicles());

    for(unsigned int v = 0; v < this->instance.getNumVertices(); v++) {
        this->vehiclesE[v] = std::vector<unsigned>(this->instance.getNumVertices(),
                this->instance.getNumVehicles());
    }

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        this->loads[k] = std::vector<double>(this->paths[k].size(), 0.0);
        this->times[k] = std::vector<double>(this->paths[k].size(), 0.0);
        this->indexesKV[k] = std::vector<unsigned>(this->instance.getNumVertices(),
                this->paths[k].size());

        for(unsigned i = 0; i < this->paths[k].size(); i++) {
            unsigned v = this->paths[k][i];
            unsigned r = this->instance.getRequestV(v);

            if(i == 0) {
                this->tTravels[k] = this->instance.getTVisit(v);
                this->loads[k][i] = this->instance.getDemandV(v);
                this->times[k][i] = this->instance.getTVisit(v);
            } else {
                unsigned u = this->paths[k][i - 1];

                this->tTravels[k] += this->instance.getLength(u, v)/this->instance.getSpeed(k) +
                    this->instance.getTVisit(v);
                this->loads[k][i] = this->loads[k][i - 1] +
                    this->instance.getDemandV(v);
                this->times[k][i] = this->times[k][i - 1] + this->instance.getLength(u,
                        v)/this->instance.getSpeed(k) + this->instance.getTVisit(v);
                this->vehiclesE[u][v] = k;
            }

            if(r < this->instance.getNumRequests() && this->instance.isATarget(v)) {
                this->tFulfills[r] = this->instance.getTStart(k) + this->times[k][i] -
                    this->instance.getTCreate(r);

                this->totalProfit += this->instance.getProfit(r);
                this->sumTFulfill += this->tFulfills[r];
                this->vehiclesR[r] = k;
                this->requestsK[k].insert(r);
            }

            this->vehiclesV[v] = k;
            this->indexesKV[k][v] = i;
        }
    }
}

void Solution::computeDecisionVariables() {
    this->x = std::vector<std::vector<std::vector<bool>>>(this->instance.getNumVertices(),
            std::vector<std::vector<bool>>(this->instance.getNumVertices(),
                std::vector<bool>(this->instance.getNumVehicles(), false)));
    this->y = std::vector<std::vector<bool>>(this->instance.getNumRequests(),
            std::vector<bool>(this->instance.getNumVehicles(), false));
    this->t = std::vector<std::vector<double>>(this->instance.getNumVertices(),
            std::vector<double>(this->instance.getNumVehicles(), 0.0));
    this->l = std::vector<std::vector<double>>(this->instance.getNumVertices(),
            std::vector<double>(this->instance.getNumVehicles(), 0.0));

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        for(unsigned i = 0; i < this->paths[k].size(); i++) {
            unsigned v = this->paths[k][i];
            unsigned r = this->instance.getRequestV(v);

            if(i == 0) {
                this->t[v][k] = this->instance.getTStart(k);
                this->l[v][k] = 0.0;
            } else {
                unsigned u = this->paths[k][i - 1];

                this->x[u][v][k] = true;

                if(i < this->paths[k].size() - 1 && this->instance.isATarget(v) 
                        && r < this->instance.getNumRequests()) {
                    this->y[r][k] = true;
                }

                this->t[v][k] = this->t[u][k] + this->instance.getTVisit(u) +
                    this->instance.getLength(u, v)/this->instance.getSpeed(k);

                if(this->instance.isASource(v) && r < this->instance.getNumRequests() 
                        && this->t[v][k] < this->instance.getTCreate(r)) {
                    this->t[v][k] = this->instance.getTCreate(r);
                }

                this->l[v][k] = this->l[u][k] + this->instance.getDemandV(v);
            }
        }
    }
}

void Solution::init(std::istream & is) {
    this->paths = std::vector<std::vector<unsigned>>(this->instance.getNumVehicles());

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        unsigned size;

        is >> size;

        this->paths[k] = std::vector<unsigned>(size, this->instance.getNumVertices());
    }

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        for(unsigned i = 0; i < this->paths[k].size(); i++) {
            is >> paths[k][i];
        }
    }

    this->computeDecisionVariables();

    this->init();
}

Solution::Solution(const Instance & instance, const std::vector<std::vector<unsigned>> & paths) : 
    instance(instance), 
    paths(paths) {
    this->computeDecisionVariables();
    this->init();
}

Solution::Solution(const Instance & instance, 
        const std::vector<std::vector<std::vector<bool>>> & x, 
        const std::vector<std::vector<bool>> & y, const std::vector<std::vector<double>> & t, 
        const std::vector<std::vector<double>> & l) : 
    instance(instance), 
    x(x), 
    y(y), 
    t(t), 
    l(l) {
    this->paths = std::vector<std::vector<unsigned>>(this->instance.getNumVehicles());

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        this->paths[k].push_back(this->instance.getSourceK(k));

        while(this->paths[k].back() != this->instance.getTargetK(k)) {
            for(unsigned v = 0; v < this->instance.getNumVertices(); v++) {
                if(this->x[this->paths[k].back()][v][k]) {
                    this->paths[k].push_back(v);
                    break;
                }
            }
        }
    }

    this->init();
}

Solution::Solution(const Instance & instance, std::istream & is) : 
    instance(instance) {
    this->init(is);
}

Solution::Solution(const Instance & instance, const char * filename) : 
    instance(instance) {
    std::ifstream ifs;

    ifs.open(filename);

    if(ifs.is_open()) {
        this->init(ifs);
    } else {
        throw "File not found.";
    }
}

Solution::Solution(const Instance & instance, const std::string & filename) :
    Solution::Solution(instance, filename.c_str()) {}

Solution::Solution(const Instance & instance) : instance(instance) {
    this->paths = std::vector<std::vector<unsigned>>(this->instance.getNumVehicles());

    this->computeDecisionVariables();

    this->init();
}

Solution::Solution() : Solution::Solution(Instance()) {}

Instance Solution::getInstance() const {
    return this->instance;
}

std::vector<std::vector<unsigned>> Solution::getPaths() const {
    return this->paths;
}

std::vector<unsigned> Solution::getPath(unsigned k) const {
    return this->paths[k];
}

std::vector<std::vector<std::vector<bool>>> Solution::getX() const {
    return this->x;
}

bool Solution::getX(unsigned i, unsigned j, unsigned k) const {
    return this->x[i][j][k];
}

std::vector<std::vector<bool>> Solution::getY() const {
    return this->y;
}

bool Solution::getY(unsigned r, unsigned k) const {
    return this->y[r][k];
}

std::vector<std::vector<double>> Solution::getT() const {
    return this->t;
}

double Solution::getT(unsigned i, unsigned k) const {
    return this->t[i][k];
}

std::vector<std::vector<double>> Solution::getL() const {
    return this->l;
}

double Solution::getL(unsigned i, unsigned k) const {
    return this->l[i][k];
}

std::vector<double> Solution::getTTravels() const {
    return this->tTravels;
}

double Solution::getTTravel(unsigned k) const {
    return this->tTravels[k];
}

std::vector<double> Solution::getTFulfills() const {
    return this->tFulfills;
}

double Solution::getTFulfill(unsigned r) const {
    return this->tFulfills[r];
}

double Solution::getTotalProfit() const {
    return this->totalProfit;
}

double Solution::getSumTFulfill() const {
    return this->sumTFulfill;
}

std::vector<std::vector<double>> Solution::getLoads() const {
    return this->loads;
}

double Solution::getLoad(unsigned k, unsigned i) const {
    return this->loads[k][i];
}

std::vector<std::vector<double>> Solution::getTimes() const {
    return this->times;
}

double Solution::getTime(unsigned k, unsigned i) const {
    return this->times[k][i];
}

std::vector<unsigned> Solution::getVehiclesR() const {
    return this->vehiclesR;
}

unsigned Solution::getVehicleR(unsigned r) const {
    return this->vehiclesR[r];
}

std::vector<unsigned> Solution::getVehiclesV() const {
    return this->vehiclesV;
}

unsigned Solution::getVehicleV(unsigned v) const {
    return this->vehiclesV[v];
}

std::vector<std::vector<unsigned>> Solution::getVehiclesE() const {
    return this->vehiclesE;
}

unsigned Solution::getVehicleE(unsigned u, unsigned v) const {
    return this->vehiclesE[u][v];
}

std::vector<std::unordered_set<unsigned>> Solution::getRequestsK() const {
    return this->requestsK;
}

std::unordered_set<unsigned> Solution::getRequestsK(unsigned k) const {
    return this->requestsK[k];
}

std::vector<std::vector<unsigned>> Solution::getIndexesKV() const {
    return this->indexesKV;
}

unsigned Solution::getIndexKV(unsigned k, unsigned v) const {
    return this->indexesKV[k][v];
}

bool Solution::isValidPath(unsigned k, unsigned & error) const {
    error = 0;

    if(this->paths[k].front() != this->instance.getSourceK(k)) {
        error = 1;
        return false;
    }

    if(this->paths[k].back() != this->instance.getTargetK(k)) {
        error = 2;
        return false;
    }

    if(this->tTravels[k] > this->instance.getTLimit(k)) {
        error = 3;
        return false;
    }

    for(unsigned j = 0; j < this->paths[k].size(); j++) {
        if(this->loads[k][j] > this->instance.getCapacity(k)) {
            error = 4;
            return false;
        }
    }

    for(auto r : this->requestsK[k]) {
        if(this->indexesKV[k][this->instance.getSourceR(r)] >=
                this->indexesKV[k][this->instance.getTargetR(r)]) {
            error = 5;
            return false;
        }
    }

    return true;
}

bool Solution::isValidPath(unsigned k) const {
    bool result;
    unsigned error;

    result = this->isValidPath(k, error);

    return result;
}

bool Solution::isFeasible(unsigned int & error) const {
    error = 0;

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        if(!this->isValidPath(k)) {
            error = k + 1;
            return false;
        }
    }

    return true;
}

bool Solution::isFeasible() const {
    bool result;
    unsigned error;

    result = this->isFeasible(error);

    return result;
}


bool Solution::areConstraintsSatisfied(unsigned & error) const {
    error = 0;

    for(unsigned r = 0; r < this->instance.getNumRequests(); r++) {
        unsigned sumXorik = 0;

        for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
            if(i != this->instance.getSourceR(r)) {
                for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
                    if(this->x[this->instance.getSourceR(r)][i][k]) {
                        sumXorik++;
                    }
                }
            }
        }

        if(sumXorik > 1) {
            error = 1;
            return false;
        }
    }

    for(unsigned r = 0; r < this->instance.getNumRequests(); r++) {
        unsigned sumXidrk = 0;

        for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
            if(i != this->instance.getTargetR(r)) {
                for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
                    if(this->x[i][this->instance.getTargetR(r)][k]) {
                        sumXidrk++;
                    }
                }
            }
        }

        if(sumXidrk > 1) {
            error = 2;
            return false;
        }
    }

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        for(unsigned r = 0; r < this->instance.getNumRequests(); r++) {
            unsigned sumXorik = 0, sumXidrk = 0;

            for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
                if(i != this->instance.getSourceR(r) 
                        && this->x[this->instance.getSourceR(r)][i][k]) {
                    sumXorik++;
                }
                if(i != this->instance.getTargetR(r) 
                        && this->x[i][this->instance.getTargetR(r)][k]) {
                    sumXidrk++;
                }
            }

            if(sumXorik - sumXidrk != 0) {
                error = 3;
                return false;
            }
        }
    }

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        unsigned sumXokik = 0;

        for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
            if(((this->instance.isASource(i) && i != this->instance.getSourceK(k)) 
                        || i == this->instance.getTargetK(k)) 
                    && this->x[this->instance.getSourceK(k)][i][k]) {
                sumXokik++;
            }
        }

        if(sumXokik != 1) {
            error = 4;
            return false;
        }
    }

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        unsigned sumXidkk = 0;

        for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
            if(((this->instance.isATarget(i) && i != this->instance.getTargetK(k)) 
                        || i == this->instance.getSourceK(k)) 
                    && this->x[i][this->instance.getTargetK(k)][k]) {
                sumXidkk++;
            }
        }

        if(sumXidkk != 1) {
            error = 5;
            return false;
        }
    }

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
            if(i != this->instance.getSourceK(k) && i != this->instance.getTargetK(k)) {
                unsigned sumXjik = 0, sumXijk = 0;

                for(unsigned j = 0; j < this->instance.getNumVertices(); j++) {
                    if(j != this->instance.getTargetK(k) && this->x[j][i][k]) {
                        sumXjik++;
                    }

                    if(j != this->instance.getSourceK(k) && this->x[i][j][k]) {
                        sumXijk++;
                    }
                }

                if(sumXjik - sumXijk != 0) {
                    error = 6;
                    return false;
                }
            }
        }
    }

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        for(unsigned r = 0; r < this->instance.getNumRequests(); r++) {
            unsigned int sumXorik = 0;
            unsigned int yrk = 0;

            if(this->y[r][k]) {
                yrk = 1;
            }

            for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
                if(i != this->instance.getSourceR(r) 
                        && this->x[this->instance.getSourceR(r)][i][k]) {
                    sumXorik++;
                }
            }

            if(yrk - sumXorik != 0) {
                error = 7;
                return false;
            }
        }
    }

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        for(unsigned r = 0; r < this->instance.getNumRequests(); r++) {
            unsigned sumXidrk = 0;
            unsigned yrk = 0;

            if(this->y[r][k]) {
                yrk = 1;
            }

            for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
                if(i != this->instance.getTargetR(r) 
                        && this->x[i][this->instance.getTargetR(r)][k]) {
                    sumXidrk++;
                }
            }

            if(yrk - sumXidrk != 0) {
                error = 8;
                return false;
            }
        }
    }

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        if(this->t[this->instance.getSourceK(k)][k] < this->instance.getTStart(k)) {
            error = 9;
            return false;
        }
    }

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        if(this->t[this->instance.getTargetK(k)][k] > this->instance.getTStart(k) +
                this->instance.getTLimit(k) -
                this->instance.getTVisit(this->instance.getTargetK(k))) {
            error = 10;
            return false;
        }
    }

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        for(unsigned r = 0; r < this->instance.getNumRequests(); r++) {
            if(this->y[r][k]) {
                if(this->t[this->instance.getSourceK(k)][k] - this->instance.getTCreate(r) < 0.0) {
                    error = 11;
                    return false;
                }
            } else {
                if(this->t[this->instance.getSourceK(k)][k] < 0.0) {
                    error = 11;
                    return false;
                }
            }
        }
    }

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        for(unsigned r = 0; r < this->instance.getNumRequests(); r++) {
            if(this->y[r][k]) {
                if(this->t[this->instance.getTargetR(r)][k] - this->instance.getTStart(k) -
                        this->instance.getTLimit(k) +
                        this->instance.getTVisit(this->instance.getTargetK(k)) > 0.0) {
                    error = 12;
                    return false;
                }
            } else {
                if(this->t[this->instance.getTargetR(r)][k] > 0.0) {
                    error = 12;
                    return false;
                }
            }
        }
    }

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        for(unsigned r = 0; r < this->instance.getNumRequests(); r++) {
            if(this->y[r][k]) {
                if(this->t[this->instance.getTargetK(k)][k] -
                        this->t[this->instance.getSourceK(k)][k] -
                        this->instance.getLength(this->instance.getSourceR(r),
                            this->instance.getTargetR(r)) / this->instance.getSpeed(k) < 0.0) {
                    error = 13;
                    return false;
                }
            } else {
                if(this->t[this->instance.getTargetK(k)][k] -
                        this->t[this->instance.getSourceK(k)][k] < 0.0) {
                    error = 13;
                    return false;
                }
            }
        }
    }

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
            for(unsigned j = 0; j < this->instance.getNumVertices(); j++) {
                unsigned xijk = 0;

                if(this->x[i][j][k]) {
                    xijk = 1;
                }

                if(this->t[j][k] - this->t[i][k] - (this->instance.getTStart(k) +
                            this->instance.getTLimit(k) -
                            this->instance.getTVisit(this->instance.getTargetK(k)) +
                            this->instance.getTVisit(i) + this->instance.getLength(i, j) /
                            this->instance.getSpeed(k)) * xijk <
                        this->instance.getTVisit(this->instance.getTargetK(k)) -
                        this->instance.getTStart(k) - this->instance.getTLimit(k) -
                        std::numeric_limits<float>::epsilon()) {
                    error = 14;
                    return false;
                }
            }
        }
    }

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        if(fabs(this->l[this->instance.getSourceK(k)][k]) > std::numeric_limits<float>::epsilon()) {
            error = 15;
            return false;
        }
    }

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        if(fabs(this->l[this->instance.getTargetK(k)][k]) > std::numeric_limits<float>::epsilon()) {
            error = 16;
            return false;
        }
    }

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        for(unsigned r = 0; r < this->instance.getNumRequests(); r++) {
            if(this->y[r][k]) {
                if(this->l[this->instance.getSourceR(r)][k] - this->instance.getDemandR(r) < 0.0) {
                    error = 17;
                    return false;
                }
            } else {
                if(this->l[this->instance.getSourceR(r)][k] < 0.0) {
                    error = 17;
                    return false;
                }
            }
        }
    }

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        for(unsigned r = 0; r < this->instance.getNumRequests(); r++) {
            if(this->l[this->instance.getSourceR(r)][k] > this->instance.getCapacity(k)) {
                error = 18;
                return false;
            }
        }
    }

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        for(unsigned r = 0; r < this->instance.getNumRequests(); r++) {
            if(this->l[this->instance.getTargetR(r)][k] > this->instance.getCapacity(k) -
                    this->instance.getDemandR(r)) {
                error = 19;
                return false;
            }
        }
    }

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
            for(unsigned j = 0; j < this->instance.getNumVertices(); j++) {
                unsigned xijk = 0;

                if(this->x[i][j][k]) {
                    xijk = 1;
                }

                if(this->l[i][k] - this->l[j][k] + (this->instance.getCapacity(k) +
                            this->instance.getDemandV(j)) * xijk > this->instance.getCapacity(k) +
                        std::numeric_limits<float>::epsilon()) {
                    error = 20;
                    return false;
                }
            }
        }
    }

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
            if(this->t[i][k] < 0.0) {
                error = 21;
                return false;
            }
        }
    }

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
            if(this->l[i][k] < 0.0) {
                error = 22;
                return false;
            }
        }
    }

    return true;
}

bool Solution::areConstraintsSatisfied() const {
    bool result;
    unsigned error;

    result = this->areConstraintsSatisfied(error);

    return result;
}

void Solution::write(std::ostream & os) const {
    for(unsigned k = 0; k < this->instance.getNumVehicles() - 1; k++) {
        os << this->paths[k].size() << " ";
    }

    os << this->paths[this->paths.size() - 1].size();

    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
        for(unsigned i = 0; i < this->paths[k].size() - 1; i++) {
            os << this->paths[k][i] << " ";
        }

        os << this->paths[k][this->paths[k].size() - 1] << std::endl;
    }
}

void Solution::write(const char * filename) const {
    std::ofstream ofs;
    ofs.open(filename);

    if(ofs.is_open()) {
        this->write(ofs);
        ofs.close();
    } else {
        throw "File not created.";
    }
}

void Solution::write(const std::string & filename) const {
    this->write(filename.c_str());
}

void Solution::write() const {
    this->write(std::cout);
}

bool Solution::operator == (const Solution & solution) const {
    if(this->paths.size() != solution.paths.size()) {
        return false;
    }

    for(unsigned k = 0; k < this->paths.size(); k++) {
        if(this->paths[k].size() != solution.paths[k].size()) {
            return false;
        }
    }

    for(unsigned k = 0; k < this->paths.size(); k++) {
        for(unsigned i = 0; i < this->paths[k].size(); i++) {
            if(this->paths[k][i] != solution.paths[k][i]) {
                return false;
            }
        }
    }

    return true;
}

bool Solution::operator < (const Solution & solution) const {
    if(this->totalProfit > solution.totalProfit) {
        return true;
    }

    if(this->sumTFulfill < solution.sumTFulfill) {
        return true;
    }

    if(this->paths.size() < solution.paths.size()) {
        return true;
    }

    if(this->paths.size() > solution.paths.size()) {
        return false;
    }

    for(unsigned k = 0; k < this->paths.size(); k++) {
        if(this->paths[k].size() < solution.paths[k].size()) {
            return true;
        }

        if(this->paths[k].size() > solution.paths[k].size()) {
            return false;
        }
    }

    for(unsigned k = 0; k < this->paths.size(); k++) {
        for(unsigned i = 0; i < this->paths[k].size(); i++) {
            if(this->paths[k][i] < solution.paths[k][i]) {
                return true;
            }

            if(this->paths[k][i] > solution.paths[k][i]) {
                return false;
            }
        }
    }

    return false;
}

bool Solution::operator > (const Solution & solution) const {
    if(this->totalProfit < solution.totalProfit) {
        return true;
    }

    if(this->sumTFulfill > solution.sumTFulfill) {
        return true;
    }

    if(this->paths.size() > solution.paths.size()) {
        return true;
    }

    if(this->paths.size() > solution.paths.size()) {
        return false;
    }

    for(unsigned k = 0; k < this->paths.size(); k++) {
        if(this->paths[k].size() > solution.paths[k].size()) {
            return true;
        }

        if(this->paths[k].size() < solution.paths[k].size()) {
            return false;
        }
    }

    for(unsigned k = 0; k < this->paths.size(); k++) {
        for(unsigned i = 0; i < this->paths[k].size(); i++) {
            if(this->paths[k][i] > solution.paths[k][i]) {
                return true;
            }

            if(this->paths[k][i] < solution.paths[k][i]) {
                return false;
            }
        }
    }

    return false;
}

bool Solution::dominates(const Solution & solution) const {
    return ((this->totalProfit > solution.totalProfit 
                && this->sumTFulfill <= solution.sumTFulfill) || (this->totalProfit >=
                    solution.totalProfit && this->sumTFulfill < solution.sumTFulfill));
}

