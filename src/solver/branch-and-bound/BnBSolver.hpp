#pragma once

#include "gurobi_c++.h"
#include "../Solver.hpp"

/********************************************************************************************
 * The BnBSolver class represents a solver for the MVRPPD using the Branch-and-Bound method.
 ********************************************************************************************/
class BnBSolver : public Solver {
    public :
        /****************************************************************************
         * Constructs a new solver.
         *
         * @param instance        the instance to be solved.
         * @param timeLimit       the time limit in seconds.
         * @param seed            the  seed for the pseudo-random numbers generator.
         * @param maxNumSolutions the maximum number of solutions
         ****************************************************************************/
        BnBSolver(const Instance & instance, unsigned timeLimit, unsigned seed,
                unsigned maxNumSolutions);

        /************************************************************************
         * Constructs a new solver.
         *
         * @param instance  the instance to be solved.
         * @param timeLimit the time limit in seconds.
         * @param seed      the  seed for the pseudo-random numbers generator.
         ************************************************************************/
        BnBSolver(const Instance & instance, unsigned timeLimit, unsigned seed);

        /*********************************************************
         * Constructs a new solver.
         *
         * @param instance  the instance to be solved.
         * @param timeLimit the time limit in seconds.
         *********************************************************/
        BnBSolver(const Instance & instance, unsigned timeLimit);

        /*********************************************
         * Constructs a new solver.
         *
         * @param instance the instance to be solved.
         *********************************************/
        BnBSolver(const Instance & instance);

        /******************************
         * Constructs an empty solver.
         ******************************/
        BnBSolver();

        /**********************
         * Solve the instance.
         **********************/
        void solve();
};

