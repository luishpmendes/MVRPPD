#pragma once

#include "Solution.hpp"
#include <set>

/***************************************************************
 * The SolutionsSet class represents a set of MVRPPD solutions.
 ***************************************************************/
class SolutionSet {
    private:
        /********************
         * The maximum size.
         ********************/
        unsigned maxSize;

        /*****************************
         * The set of solutions.
         *****************************/
        std::set<Solution> solutions;

        /******************************************
         * The fronts of non-dominated solutions.
         ******************************************/
        std::vector<std::vector<Solution>> fronts;

        /******************************************
         * The minimum total profit of each front.
         ******************************************/
        std::vector<double> minTotalProfitsF;

        /******************************************
         * The maximum total profit of each front.
         ******************************************/
        std::vector<double> maxTotalProfitsF;

        /*****************************************************
         * The minimum sum of the fulfill time of each front.
         *****************************************************/
        std::vector<double> minSumTFulfillsF;

        /*****************************************************
         * The maximum sum of the fulfill time of each front.
         *****************************************************/
        std::vector<double> maxSumTFulfillsF;

        /****************************************************
         * Flag indicating whether the solutions are sorted.
         ****************************************************/
        bool sorted;

        /**********************
         * Sort the solutions.
         **********************/
        void sort();

    public:
        /********************************************************************
         * Constructs a new set of mutually non-dominated solutions.
         *
         * @param maxSize  the maximum size.
         * @param solution the solutions.
         ********************************************************************/
        SolutionSet(unsigned maxSize, const std::set<Solution> & solutions);

        /************************************************************
         * Constructs a new set of mutually non-dominated solutions.
         *
         * @param maxSize the maximum size.
         ************************************************************/
        SolutionSet(unsigned maxSize);

        /*******************************************
         * Constructs a new empty set of solutions.
         *******************************************/
        SolutionSet();

        /****************************
         * Returns the maximum size.
         *
         * @return the maximum size.
         ****************************/
        unsigned getMaxSize() const;

        /****************************************
         * Returns the set of solutions.
         *
         * @return the set of solutions.
         ****************************************/
        std::set<Solution> getSolutions() const;

        /*****************************************************
         * Returns the fronts.
         *
         * @return the fronts.
         *****************************************************/
        std::vector<std::vector<Solution>> getFronts();

        /*************************************************
         * Returns the f-th front.
         *
         * @param f the front to be returned.
         *
         * @return the f-th front.
         *************************************************/
        std::vector<Solution> getFront(unsigned f);

        /**************************************************
         * Returns the minimum total profit of each front.
         *
         * @return the minimum total profit of each front.
         **************************************************/
        std::vector<double> getMinTotalProfitsF();

        /*******************************************************************
         * Returns the minimum total profit of the specified front.
         *
         * @param f the front whose minimum total profit is to be returned.
         *
         * @return the minimum total profit of the specified front.
         *******************************************************************/
        double getMinTotalProfitF(unsigned f);

        /**************************************************
         * Returns the maximum total profit of each front.
         *
         * @return the maximum total profit of each front.
         **************************************************/
        std::vector<double> getMaxTotalProfitsF();

        /*******************************************************************
         * Returns the maximum total profit of the specified front.
         *
         * @param f the front whose minimum total profit is to be returned.
         *
         * @return the maximum total profit of the specified front.
         *******************************************************************/
        double getMaxTotalProfitF(unsigned f);

        /*************************************************************
         * Returns the minimum sum of the fulfill time of each front.
         *
         * @return the minimum sum of the fulfill time of each front.
         *************************************************************/
        std::vector<double> getMinSumTFulfillsF();

        /******************************************************************************
         * Returns the minimum sum of the fulfill time of the specified front.
         *
         * @param f the front whose minimum sum of the fulfill time is to be returned.
         *
         * @return the minimum sum of the fulfill time of the specified front.
         ******************************************************************************/
        double getMinSumTFulfillF(unsigned f);

        /*************************************************************
         * Returns the maximum sum of the fulfill time of each front.
         *
         * @return the maximum sum of the fulfill time of each front.
         *************************************************************/
        std::vector<double> getMaxSumTFulfillsF();

        /******************************************************************************
         * Returns the maximum sum of the fulfill time of the specified front.
         *
         * @param f the front whose maximum sum of the fulfill time is to be returned.
         *
         * @return the maximum sum of the fulfill time of the specified front.
         ******************************************************************************/
        double getMaxSumTFulfillF(unsigned f);

        /********************************
         * Returns the size of this set.
         *
         * @return the site of this set.
         ********************************/
        unsigned size() const;

        /**************************************************
         * Inserts the specified solutions.
         *
         * @param solutions the solutions to be inserted.
         **************************************************/
        void insert(const std::set<Solution> & solutions);

        /***********************************************
         * Inserts the specifies solution.
         *
         * @param solution the solution to be inserted.
         ***********************************************/
        void insert(const Solution & solution);
};

