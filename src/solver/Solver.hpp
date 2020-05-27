#pragma once

#include "../solution/SolutionSet.hpp"
#include <chrono>
#include <random>

/*****************************************************************
 * The Solvers abstract class represents a solver for the MVRPPD.
 *****************************************************************/
class Solver {
    protected:
        /*****************************
         * The instance to be solved.
         *****************************/
        Instance instance;

        /*****************************
         * The time limit in seconds.
         *****************************/
        unsigned timeLimit;

        /****************************************************
         * The seed for the pseudo-random numbers generator.
         ****************************************************/
        unsigned seed;

        /************************************************
         * The start time.
         ************************************************/
        std::chrono::steady_clock::time_point startTime;

        /***************************************
         * The pseudo-random numbers generator.
         ***************************************/
        std::default_random_engine generator;

        /***********************************
         * The maximum number of solutions.
         ***********************************/
        unsigned maxNumSolutions;

        /*******************************
         * The solving time in seconds.
         *******************************/
        unsigned solvingTime;

        /******************************
         * The set of solutions found.
         ******************************/
        SolutionSet solutionSet;

        /***************************************************************************
         * Verifies whether the termination criteria have been met.
         *
         * @return true if the termination criteria have been met; false otherwise.
         ***************************************************************************/
        bool areTerminationCriteriaMet() const;

    public:
        /***********************************************************************************************
         * Constructs a new solver.
         *
         * @param instance        the instance to be solved.
         * @param timeLimit       the time limit in seconds.
         * @param seed            the  seed for the pseudo-random numbers generator.
         * @param maxNumSolutions the maximum number of solutions
         ***********************************************************************************************/
        Solver(const Instance & instance, unsigned timeLimit, unsigned seed, unsigned maxNumSolutions);

        /**********************************************************************
         * Constructs a new solver.
         *
         * @param instance  the instance to be solved.
         * @param timeLimit the time limit in seconds.
         * @param seed      the  seed for the pseudo-random numbers generator.
         **********************************************************************/
        Solver(const Instance & instance, unsigned timeLimit, unsigned seed);

        /******************************************************
         * Constructs a new solver.
         *
         * @param instance  the instance to be solved.
         * @param timeLimit the time limit in seconds.
         ******************************************************/
        Solver(const Instance & instance, unsigned timeLimit);

        /*********************************************
         * Constructs a new solver.
         *
         * @param instance the instance to be solved.
         *********************************************/
        Solver(const Instance & instance);

        /******************************
         * Constructs an empty solver.
         ******************************/
        Solver();

        /*************************************
         * Returns the instance to be solved.
         *
         * @return the instance to be solved.
         *************************************/
        Instance getInstance() const;

        /*******************************************
         * Returns the maximum number of solutions.
         *
         * @return the maximum number of solutions.
         *******************************************/
        unsigned getMaxNumSolutions() const;

        /***************************************
         * Returns the solving time in seconds.
         *
         * @return the solving time in seconds.
         ***************************************/
        unsigned getSolvingTime() const;

        /***********************************
         * Returns the set solutions found.
         *
         * @return the set solutions found.
         ***********************************/
        SolutionSet getSolutionSet() const;

        /*****************************************************************************************
         * Returns the elapsed time.
         *
         * @param startTime the start time
         *
         * @return the elapsed time.
         *****************************************************************************************/
        static unsigned getElapsedTime( const std::chrono::steady_clock::time_point & startTime);

        /********************************
         * Returns the elapsed time.
         *
         * @return the elapsed time.
         ********************************/
        unsigned getElapsedTime() const;

        /*****************************************************************************************
         * Returns the remaining time.
         *
         * @param startTime the start time.
         * @param timeLimit the time limit.
         *
         * @return the remaining time.
         *****************************************************************************************/
        static unsigned getRemainingTime(const std::chrono::steady_clock::time_point & startTime,
                unsigned timeLimit);

        /******************************************************************************************
         * Returns the remaining time.
         *
         * @param startTime the start time.
         *
         * @return the remaining time.
         ******************************************************************************************/
         unsigned getRemainingTime(const std::chrono::steady_clock::time_point & startTime) const;

        /*****************************************************
         * Returns the remaining time.
         *
         * @param timeLimit the time limit.
         *
         * @return the remaining time.
         *****************************************************/
         unsigned getRemainingTime(unsigned timeLimit) const;

         /**************************************
         * Returns the remaining time.
         *
         * @return the remaining time.
         ***************************************/
         unsigned getRemainingTime() const;

        /*************************
         * Solve the instance.
         *************************/
        virtual void solve() = 0;

        /*********************************************************
         * Write the statistics into the specified output stream.
         *
         * @param os the output stream to write into.
         *********************************************************/
        void write(std::ostream & os) const;

        /************************************************
         * Write the statistics into the specified file.
         *
         * @param filename the file to write into.
         ************************************************/
        void write(const char * filename) const;

        /************************************************
         * Write the statistics into the specified file.
         *
         * @param filename the file to write into.
         ************************************************/
        void write(const std::string & filename) const;

        /********************************************************
         * Write the statistics into the standard output stream.
         ********************************************************/
        void write() const;
};

