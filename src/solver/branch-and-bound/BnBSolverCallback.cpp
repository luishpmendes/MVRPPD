#include "BnBSolverCallback.hpp"

BnBSolverCallback::BnBSolverCallback(const Instance & instance,
        const std::vector<std::vector<std::vector<GRBVar>>> & x,
        const std::vector<std::vector<GRBVar>> & y, const std::vector<std::vector<GRBVar>> & t,
        const std::vector<std::vector<GRBVar>> & l, const std::list<double> minTotalProfits) :
    instance(instance), 
    x(x), 
    y(y), 
    t(t), 
    l(l), 
    minTotalProfits(minTotalProfits) {}

void BnBSolverCallback::callback() {
    try {
        if(this->where == GRB_CB_MIPSOL) {
            std::vector<std::vector<std::vector<bool>>> xAux(this->instance.getNumVertices(),
                    std::vector<std::vector<bool>>(this->instance.getNumVertices(),
                        std::vector<bool>(this->instance.getNumVehicles(), false)));
            std::vector<std::vector<bool>> yAux(this->instance.getNumRequests(),
                    std::vector<bool>(this->instance.getNumVehicles(), false));
            std::vector<std::vector<double>> tAux(this->instance.getNumVertices(),
                    std::vector<double>(this->instance.getNumVehicles(), 0.0));
            std::vector<std::vector<double>> lAux(this->instance.getNumVertices(),
                    std::vector<double>(this->instance.getNumVehicles(), 0.0));

            for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
                for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
                    for(unsigned j = 0; j < this->instance.getNumVertices(); j++) {
                        if(this->getSolution(this->x[i][j][k]) >= 0.5) {
                            xAux[i][j][k] = true;
                        }
                    }
                }
            }

            for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
                for(unsigned r = 0; r < this->instance.getNumRequests(); r++) {
                    if(this->getSolution(this->y[r][k]) >= 0.5) {
                        yAux[r][k] = true;
                    }
                }
            }

            for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
                for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
                    tAux[i][k] = this->getSolution(this->t[i][k]);
                }
            }

            for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
                for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
                    lAux[i][k] = this->getSolution(this->l[i][k]);
                }
            }

            Solution solution (instance, xAux, yAux, tAux, lAux);

            this->solutions.push_back(solution);

            while(!this->minTotalProfits.empty() 
                    && this->minTotalProfits.front() <= solution.getTotalProfit()) {
                this->minTotalProfits.pop_front();
            }

            if(!this->minTotalProfits.empty()) {
                GRBLinExpr lhs = 0.0;

                for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
                    for(unsigned r = 0; r < this->instance.getNumRequests(); r++) {
                        lhs += this->instance.getProfit(r) * this->y[r][k];
                    }
                }

                this->addLazy(lhs >= this->minTotalProfits.front());
            }
        }
    } catch(GRBException e) {
        std::cerr << "Error number: " << e.getErrorCode() << std::endl;
        std::cerr << e.getMessage() << std::endl;
    } catch(...) {
        std::cerr << "Error during callback" << std::endl;
    }
}

std::vector<Solution> BnBSolverCallback::getSolutions() const {
    return this->solutions;
}

