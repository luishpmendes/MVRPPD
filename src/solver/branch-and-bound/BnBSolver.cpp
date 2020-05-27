#include "BnBSolver.hpp"
#include "BnBSolverCallback.hpp"
#include <list>

BnBSolver::BnBSolver(const Instance & instance, unsigned timeLimit, unsigned seed, 
        unsigned maxNumSolutions) : 
    Solver::Solver(instance, timeLimit, seed, maxNumSolutions) {}

BnBSolver::BnBSolver(const Instance & instance, unsigned timeLimit, unsigned seed) :
    Solver::Solver(instance, timeLimit, seed) {}

BnBSolver::BnBSolver(const Instance & instance, unsigned timeLimit) : Solver::Solver(instance,
        timeLimit) {}

BnBSolver::BnBSolver(const Instance & instance) : Solver::Solver(instance) {}

BnBSolver::BnBSolver() : Solver::Solver() {}

void BnBSolver::solve() {
    this->startTime = std::chrono::steady_clock::now();

    GRBEnv * env = 0;

    try {
        env = new GRBEnv();

        std::list<double> minTotalProfits;

        for(unsigned i = 0; i < this->maxNumSolutions; i++) {
            double minTotalProfit = ((double) i)*(((double)
                        this->instance.getSumProfit())/(((double) this->maxNumSolutions) - 1.0));

            minTotalProfits.push_back(minTotalProfit);
        }

        GRBModel model = GRBModel(* env);
        std::vector<std::vector<std::vector<GRBVar>>> x(this->instance.getNumVertices(),
                std::vector<std::vector<GRBVar>>(this->instance.getNumVertices(),
                    std::vector<GRBVar>(this->instance.getNumVehicles())));
        std::vector<std::vector<GRBVar>> y(this->instance.getNumRequests(),
                std::vector<GRBVar>(this->instance.getNumVehicles()));
        std::vector<std::vector<GRBVar>> t(this->instance.getNumVertices(),
                std::vector<GRBVar>(this->instance.getNumVehicles()));
        std::vector<std::vector<GRBVar>> l(this->instance.getNumVertices(),
                std::vector<GRBVar>(this->instance.getNumVehicles()));

        for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
            for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
                for(unsigned j = 0; j < this->instance.getNumVertices(); j++) {
                    x[i][j][k] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x_" + std::to_string(i)
                            + "_" + std::to_string(j) + "_" + std::to_string(k));
                }
            }
        }

        for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
            for(unsigned r = 0; r < this->instance.getNumRequests(); r++) {
                double obj = this->instance.getTVisit(this->instance.getTargetR(r) -
                        this->instance.getTCreate(r));

                y[r][k] = model.addVar(0.0, 1.0, obj, GRB_BINARY, "y_" + std::to_string(r) + "_" +
                        std::to_string(k));
            }
        }

        for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
            double ub = this->instance.getTStart(k) + this->instance.getTLimit(k);

            for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
                double obj = 0;

                if(this->instance.isATarget(i) && this->instance.getRequestV(i) 
                        < this->instance.getNumRequests()) {
                    obj = 1.0;
                }

                t[i][k] = model.addVar(0.0, ub, obj, GRB_CONTINUOUS, "t_" + std::to_string(i) + 
                        "_" + std::to_string(k));
            }
        }

        for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
            for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
                double ub = this->instance.getCapacity(k);
                unsigned r = this->instance.getRequestV(i);

                if(i == this->instance.getSourceK(k) || i == this->instance.getTargetK(k)) {
                    ub = 0.0;
                } else if(this->instance.isATarget(i) && r < this->instance.getNumRequests()) {
                    ub -= this->instance.getDemandR(r);
                }

                l[i][k] = model.addVar(0.0, ub, 0.0, GRB_CONTINUOUS, "l_" + std::to_string(i) + 
                        "_" + std::to_string(k));
            }
        }

        model.update();

        for(unsigned r = 0; r < this->instance.getNumRequests(); r++) {
            GRBLinExpr lhs01 = 0.0;

            for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
                if(i != this->instance.getSourceR(r)) {
                    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
                        lhs01 += x[this->instance.getSourceR(r)][i][k];
                    }
                }
            }

            model.addConstr(lhs01 <= 1.0, "c_01_" + std::to_string(r));
        }

        for(unsigned r = 0; r < this->instance.getNumRequests(); r++) {
            GRBLinExpr lhs02 = 0.0;

            for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
                if(i != this->instance.getTargetR(r)) {
                    for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
                        lhs02 += x[i][this->instance.getTargetR(r)][k];
                    }
                }
            }

            model.addConstr(lhs02 <= 1.0, "c_02_" + std::to_string(r));
        }

        for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
            for(unsigned r = 0; r < this->instance.getNumRequests(); r++) {
                GRBLinExpr lhs03 = 0.0;

                for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
                    if(i != this->instance.getSourceR(r)) {
                        lhs03 += x[this->instance.getSourceR(r)][i][k];
                    }
                }

                for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
                    if(i != this->instance.getTargetR(r)) {
                        lhs03 -= x[i][this->instance.getTargetR(r)][k];
                    }
                }

                model.addConstr(lhs03 == 0.0, "c_03_" + std::to_string(r) + "_" +
                        std::to_string(k));
            }
        }

        for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
            GRBLinExpr lhs04 = 0.0;

            for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
                if((this->instance.isASource(i) && i != this->instance.getSourceK(k)) 
                        || i == this->instance.getTargetK(k)) {
                    lhs04 += x[this->instance.getSourceK(k)][i][k];
                }
            }

            model.addConstr(lhs04 == 1, "c_04_" + std::to_string(k));
        }

        for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
            GRBLinExpr lhs05 = 0.0;

            for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
                if((this->instance.isATarget(i) && i != this->instance.getTargetK(k)) 
                        || i == this->instance.getSourceK(k)) {
                    lhs05 += x[i][this->instance.getTargetK(k)][k];
                }
            }

            model.addConstr(lhs05 == 1, "c_05_" + std::to_string(k));
        }

        for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
            for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
                if(i != this->instance.getSourceK(k) && i != this->instance.getTargetK(k)) {
                    GRBLinExpr lhs06 = 0.0;

                    for(unsigned j = 0; j < this->instance.getNumVertices(); j++) {
                        if(j != this->instance.getTargetK(k)) {
                            lhs06 += x[j][i][k];
                        }
                    }

                    for(unsigned j = 0; j < this->instance.getNumVertices(); j++) {
                        if(j != this->instance.getSourceK(k)) {
                            lhs06 -= x[i][j][k];
                        }
                    }

                    model.addConstr(lhs06 == 0, "c_06_" + std::to_string(i) + "_" +
                            std::to_string(k));
                }
            }
        }

        for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
            for(unsigned r = 0; r < this->instance.getNumRequests(); r++) {
                GRBLinExpr lhs07 = y[r][k];

                for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
                    if(i != this->instance.getSourceR(r)) {
                        lhs07 -= x[this->instance.getSourceR(r)][i][k];
                    }
                }

                model.addConstr(lhs07 == 0.0, "c_07_" + std::to_string(r) + "_" +
                        std::to_string(k));
            }
        }

        for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
            for(unsigned r = 0; r < this->instance.getNumRequests(); r++) {
                GRBLinExpr lhs08 = y[r][k];

                for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
                    if(i != this->instance.getTargetR(r)) {
                        lhs08 -= x[i][this->instance.getTargetR(r)][k];
                    }
                }

                model.addConstr(lhs08 == 0.0, "c_08_" + std::to_string(r) + "_" +
                        std::to_string(k));
            }
        }

        for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
            GRBLinExpr lhs09 = t[this->instance.getSourceK(k)][k];

            model.addConstr(lhs09 >= this->instance.getTStart(k), "c_09_" + std::to_string(k));
        }

        for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
            GRBLinExpr lhs10 = t[this->instance.getTargetK(k)][k];

            model.addConstr(lhs10 <= this->instance.getTStart(k) + this->instance.getTLimit(k) -
                    this->instance.getTVisit(this->instance.getTargetK(k)), "c_10_" +
                    std::to_string(k));
        }

        for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
            for(unsigned r = 0; r < this->instance.getNumRequests(); r++) {
                GRBLinExpr lhs11 = t[this->instance.getSourceR(r)][k] - 
                    this->instance.getTCreate(r) * y[r][k];

                model.addConstr(lhs11 >= 0, "c_11_" + std::to_string(r) + "_" +
                        std::to_string(k));
            }
        }

        for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
            for(unsigned r = 0; r < this->instance.getNumRequests(); r++) {
                GRBLinExpr lhs12 = t[this->instance.getTargetR(r)][k] - 
                    (this->instance.getTStart(k) + this->instance.getTLimit(k) -
                     this->instance.getTVisit(this->instance.getTargetK(k))) * y[r][k];

                model.addConstr(lhs12 <= 0, "c_12_" + std::to_string(r) + "_" + 
                        std::to_string(k));
            }
        }

        for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
            for(unsigned r = 0; r < this->instance.getNumRequests(); r++) {
                GRBLinExpr lhs13 = t[this->instance.getTargetR(r)][k] -
                    t[this->instance.getSourceR(r)][k] -
                    (this->instance.getLength(this->instance.getSourceR(r),
                                              this->instance.getTargetR(r)) /
                     this->instance.getSpeed(k)) * y[r][k];

                model.addConstr(lhs13 >= 0, "c_13_" + std::to_string(r) + "_" + 
                        std::to_string(k));
            }
        }

        for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
            for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
                for(unsigned j = 0; j < this->instance.getNumVertices(); j++) {
                    GRBLinExpr lhs14 = t[j][k] - t[i][k] - (this->instance.getTStart(k) +
                            this->instance.getTLimit(k) -
                            this->instance.getTVisit(this->instance.getTargetK(k)) +
                            this->instance.getTVisit(i) + this->instance.getLength(i, j) /
                            this->instance.getSpeed(k)) * x[i][j][k];
                    double rhs14 = this->instance.getTVisit(this->instance.getTargetK(k)) -
                        this->instance.getTStart(k) - this->instance.getTLimit(k);

                    model.addConstr(lhs14 >= rhs14, "c_14_" + std::to_string(j) + "_" +
                            std::to_string(i) + "_" + std::to_string(k));
                }
            }
        }

        for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
            for(unsigned r = 0; r < this->instance.getNumRequests(); r++) {
                GRBLinExpr lhs15 = l[this->instance.getSourceR(r)][k] - 
                    this->instance.getDemandR(r) * y[r][k];

                model.addConstr(lhs15 >= 0, "c_15_" + std::to_string(r) + "_" + 
                        std::to_string(k));
            }
        }

        for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
            for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
                for(unsigned j = 0; j < this->instance.getNumVertices(); j++) {
                    GRBLinExpr lhs16 = l[i][k] - l[j][k] + (this->instance.getCapacity(k) +
                            this->instance.getDemandV(j)) * x[i][j][k];

                    model.addConstr(lhs16 <= this->instance.getCapacity(k), "c_16_" +
                            std::to_string(j) + "_" + std::to_string(i) + "_" + 
                            std::to_string(k));
                }
            }
        }

        BnBSolverCallback callback(this->instance, x, y, t, l, minTotalProfits);
        model.setCallback(&callback);

        model.set(GRB_DoubleParam_TimeLimit, this->getRemainingTime(timeLimit));
        model.set(GRB_IntParam_OutputFlag, 0);
        model.set(GRB_IntParam_Threads, 1);
        model.set(GRB_IntParam_LazyConstraints, 1);

        model.update();

        model.optimize();

        for(auto solution : callback.getSolutions()) {
            this->solutionSet.insert(solution);
        }

        if(model.get(GRB_IntAttr_Status) == GRB_OPTIMAL) {
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
                        if(x[i][j][k].get(GRB_DoubleAttr_X) >= 0.5) {
                            xAux[i][j][k] = true;
                        }
                    }
                }
            }

            for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
                for(unsigned r = 0; r < this->instance.getNumRequests(); r++) {
                    if(y[r][k].get(GRB_DoubleAttr_X) >= 0.5) {
                        yAux[r][k] = true;
                    }
                }
            }

            for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
                for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
                    tAux[i][k] = t[i][k].get(GRB_DoubleAttr_X);
                }
            }

            for(unsigned k = 0; k < this->instance.getNumVehicles(); k++) {
                for(unsigned i = 0; i < this->instance.getNumVertices(); i++) {
                    lAux[i][k] = l[i][k].get(GRB_DoubleAttr_X);
                }
            }

            this->solutionSet.insert(Solution(instance, xAux, yAux, tAux, lAux));
        }
    } catch(GRBException e) {
        std::cerr << "Error code = " << e.getErrorCode() << std::endl;
        std::cerr << e.getMessage() << std::endl;
    } catch(...) {
        std::cerr << "Error during optimization" << std::endl;
    }

    this->solvingTime = this->getElapsedTime();
}

