#pragma once

#include "../../solution/Solution.hpp"
#include "gurobi_c++.h"
#include <list>

/*********************************************************************************************
 * The BnBSolverCallback class represents the BnBSolver callback that adds the constraints on
 * the total profit.
 *********************************************************************************************/
class BnBSolverCallback : public GRBCallback {
    private:
        /****************************
         * The instance been solved.
         ****************************/
        Instance instance;

        /************************************************
         * The x decision variables.
         ************************************************/
        std::vector<std::vector<std::vector<GRBVar>>> x;

        /***********************************
         * The y decision variables.
         ***********************************/
        std::vector<std::vector<GRBVar>> y;

        /***********************************
         * The t decision variables.
         ***********************************/
        std::vector<std::vector<GRBVar>> t;

        /***********************************
         * The l decision variables.
         ***********************************/
        std::vector<std::vector<GRBVar>> l;

        /************************************
         * The minimum total profits.
         ************************************/
        std::list<double> minTotalProfits;

        /********************************
         * The solutions found.
         ********************************/
        std::vector<Solution> solutions;

    protected:
        /*************************
         * The callback function.
         *************************/
        void callback();

    public:
        /****************************************************************
         * Constructs a new callback.
         *
         * @param instance        the instance been solved.
         * @param x               the x decision variables.
         * @param y               the y decision variables.
         * @param t               the t decision variables.
         * @param l               the l decision variables.
         * @param minTotalProfits the minimum total profits.
         ****************************************************************/
        BnBSolverCallback(const Instance & instance, 
                const std::vector<std::vector<std::vector<GRBVar>>> & x, 
                const std::vector<std::vector<GRBVar>> & y, 
                const std::vector<std::vector<GRBVar>> & t, 
                const std::vector<std::vector<GRBVar>> & l, 
                const std::list<double> minTotalProfits);

        std::vector<Solution> getSolutions() const;
};

