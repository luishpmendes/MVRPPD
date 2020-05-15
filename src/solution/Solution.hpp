#pragma once

#include "../instance/Instance.hpp"
#include <unordered_set>

/***************************************************
 * The Solution class represents a MVRPPD solution.
 ***************************************************/
class Solution {
    private:
        /************************************
         * The MVRPPD instance being solved.
         ************************************/
        Instance instance;

        /*****************************************************
         * A set of paths with a valid path for each vehicle.
         *****************************************************/
        std::vector<std::vector<unsigned>> paths;

        /*************************************************************************************
         * x[i][j][k] == true if and only if the vehicle k ∈ K traverses the edge (i, j) ∈ E.
         *************************************************************************************/
        std::vector<std::vector<std::vector<bool>>> x;

        /*************************************************************************
         * y[r][k] == true if and only if the vehicle k ∈ K serves request r ∈ R.
         *************************************************************************/
        std::vector<std::vector<bool>> y;

        /**********************************************************************
         * t[i][k] is the time when vehicle k ∈ K starts serving vertex i ∈ V.
         **********************************************************************/
        std::vector<std::vector<double>> t;

        /*******************************************************************
         * l[i][k] is the load of vehicle k ∈ K after serving vertex i ∈ V.
         *******************************************************************/
        std::vector<std::vector<double>> l;

        /****************************************************************************************
         * The amount of time each vehicle needs to visit each vertex and traverses each edge in
         * its path.
         ****************************************************************************************/
        std::vector<double> tTravels;

        /*****************************************************
         * The amount of time needed to fulfill each request.
         *****************************************************/
        std::vector<double> tFulfills;

        /********************
         * The total profit.
         ********************/
        double totalProfit;

        /******************************************************
         * The sum of the fulfill time of the served requests.
         ******************************************************/
        double sumTFulfill;

        /******************************************************************
         * The load of each vehicle after serving each vertex in its path.
         ******************************************************************/
        std::vector<std::vector<double>> loads;

        /********************************************************
         * The travelling time of each sub path of each vehicle.
         ********************************************************/
        std::vector<std::vector<double>> times;

        /******************************************
         * The vehicle that fulfills each request.
         ******************************************/
        std::vector<unsigned> vehiclesR;

        /***************************************
         * The vehicle that serves each vertex.
         ***************************************/
        std::vector<unsigned> vehiclesV;

        /*************************************************
         * The vehicle that traverses each edge.
         *************************************************/
        std::vector<std::vector<unsigned>> vehiclesE;

        /**********************************************
         * The requests fulfilled by each vehicle.
         **********************************************/
        std::vector<std::unordered_set<unsigned>> requestsK;

        /*************************************************
         * The indexes of each vertex in each path
         *************************************************/
        std::vector<std::vector<unsigned>> indexesKV;

        /******************************
         * Initializes a new solution.
         ******************************/
        void init();

        /*****************************************
         * Compute the decision variables values.
         *****************************************/
        void computeDecisionVariables();

        /*******************************************
         * Initializes a new solution.
         *
         * @param is the input stream to read from.
         *******************************************/
        void init(std::istream & is);

    public:
        /**************************************************************************************
         * Constructs a new solution.
         *
         * @param instance the instance been solved.
         * @param paths    a set of paths.
         **************************************************************************************/
        Solution(const Instance & instance, const std::vector<std::vector<unsigned>> & paths);

        /**************************************************************
         * Constructs a new solution.
         *
         * @param instance the instance been solved.
         * @param x        the decision variables x.
         * @param y        the decision variables y.
         * @param t        the decision variables t.
         * @param l        the decision variables l.
         **************************************************************/
        Solution(const Instance & instance,
                const std::vector<std::vector<std::vector<bool>>> & x,
                const std::vector<std::vector<bool>> & y,
                const std::vector<std::vector<double>> & t,
                const std::vector<std::vector<double>> & l);

        /*******************************************************
         * Constructs a new solution.
         *
         * @param instance the instance been solved.
         * @param is       the input stream to read from.
         *******************************************************/
        Solution(const Instance & instance, std::istream & is);

        /***********************************************************
         * Constructs a new solution.
         *
         * @param instance the instance been solved.
         * @param filename the file to read from.
         ***********************************************************/
        Solution(const Instance & instance, const char * filename);

        /******************************************************************
         * Constructs a new solution.
         *
         * @param instance the instance been solved.
         * @param filename the file to read from.
         ******************************************************************/
        Solution(const Instance & instance, const std::string & filename);

        /********************************************
         * Constructs a new solution.
         *
         * @param instance the instance been solved.
         ********************************************/
        Solution(const Instance & instance);

        /***********************************
         * Constructs a new empty solution.
         ***********************************/
        Solution();

        /************************************
         * Returns the instance been solved.
         *
         * @return the instance been solved.
         ************************************/
        Instance getInstance() const;

        /****************************************************
         * Returns the path of each vehicle.
         *
         * @return the path of each vehicle.
         ****************************************************/
        std::vector<std::vector<unsigned>> getPaths() const;

        /*****************************************************
         * Returns the path of the specified vehicle.
         *
         * @param k the vehicle whose path is to be returned.
         *
         * @return the path of the specified vehicle.
         *****************************************************/
        std::vector<unsigned> getPath(unsigned k) const;

        /*********************************************************
         * Returns the values of the decision variables x.
         *
         * @return the values of the decision variables x.
         *********************************************************/
        std::vector<std::vector<std::vector<bool>>> getX() const;

        /************************************************************************************
         * Returns the values of the decision variable x associated with the edge (i, j) and
         * vehicle k.
         *
         * @param i the first endpoint of the edge whose decision variable value is to be
         *          returned.
         * @param j the second endpoint of the edge whose decision variable value is to be
         *          returned.
         * @param k the vehicle whose decision variable is to be returned.
         *
         * @return true if the vehicle k traverses the edge (i, j); false otherwise.
         ************************************************************************************/
        bool getX(unsigned i, unsigned j, unsigned k) const;

        /**************************************************
         * Returns the values of the decision variables y.
         *
         * @return the values of the decision variables y.
         **************************************************/
        std::vector<std::vector<bool>> getY() const;

        /**************************************************************************************
         * Returns the values of the decision variable y associated with the request r and the
         * vehicle k.
         *
         * @param r the request whose decision variable value is to be returned.
         * @param k the vehicle whose decision variable value is to be returned.
         *
         * @return true if the vehicle k serves request r; false otherwise.
         **************************************************************************************/
        bool getY(unsigned r, unsigned k) const;

        /**************************************************
         * Returns the values of the decision variables t.
         *
         * @return the values of the decision variables t.
         **************************************************/
        std::vector<std::vector<double>> getT() const;

        /*************************************************************************************
         * Returns the values of the decision variable t associated with the vertex i and the
         * vehicle k.
         *
         * @param i the vertex whose decision variable value is to be returned.
         * @param k the vehicle whose decision variable value is to be returned.
         *
         * @return the time when vehicle k start to serve vertex i.
         *************************************************************************************/
        double getT(unsigned i, unsigned k) const;

        /**************************************************
         * Returns the values of the decision variables l.
         *
         * @return the values of the decision variables l.
         **************************************************/
        std::vector<std::vector<double>> getL() const;

        /**************************************************************************************
         * Returns the values of the decision variables l associated with the vertex i and the
         * vehicle k.
         *
         * @param i the vertex whose decision variable value is to be returned.
         * @param k the vehicle whose decision variable value is to be returned.
         *
         * @return the load of the vehicle k after serving the vertex i.
         **************************************************************************************/
        double getL(unsigned i, unsigned k) const;

        /***********************************************
         * Returns the travelling time of each vehicle.
         *
         * @return the travelling time of each vehicle.
         ***********************************************/
        std::vector<double> getTTravels() const;

        /****************************************************************
         * Returns the travelling time of the specified vehicle.
         *
         * @param k the vehicle whose travelling time is to be returned.
         *
         * @return the travelling time of the specified vehicle.
         ****************************************************************/
        double getTTravel(unsigned k) const;

        /********************************************
         * Returns the fulfill time of each request.
         *
         * @return the fulfill time of each request.
         ********************************************/
        std::vector<double> getTFulfills() const;

        /*************************************************************
         * Returns the fulfill time of the specified request.
         *
         * @param r the request whose fulfill time is to be returned.
         *
         * @return the fulfill time of the specified request.
         *************************************************************/
        double getTFulfill(unsigned r) const;

        /******************************
         * Returns the total profit.
         *
         * @return the total profit.
         ******************************/
        double getTotalProfit() const;

        /**************************************************************
         * Returns the sum of the fulfill time of the served requests.
         *
         * @return the sum of the fulfill time of the served requests.
         **************************************************************/
        double getSumTFulfill() const;

        /**************************************************************************
         * Returns the load of each vehicle after serving each vertex in its path.
         *
         * @return the load of each vehicle after serving each vertex in its path.
         **************************************************************************/
        std::vector<std::vector<double>> getLoads() const;

        /**************************************************************************************
         * Returns the load of the specified vehicle after serving the specified vertex of its 
         * path.
         *
         * @param k the vehicle whose load is to be returned.
         * @param i the index of the vertex.
         *
         * @return the load of the specified vehicle after serving the specified vertex of its
         * path.
         **************************************************************************************/
        double getLoad(unsigned k, unsigned i) const;

        /****************************************************************
         * Returns the travelling time of each sub path of each vehicle.
         *
         * @return the travelling time of each sub path of each vehicle.
         ****************************************************************/
        std::vector<std::vector<double>> getTimes() const;

        /*************************************************************************************
         * Returns the travelling time of the sub path of the specified vehicle ending at the
         * specified vertex.
         *
         * @param k the vehicle whose travelling time is to be returned.
         * @param i the index of the vertex.
         *
         * @return the travelling time of the sub path of the specified vehicle ending at the
         * specified vertex.
         *************************************************************************************/
        double getTime(unsigned k, unsigned i) const;

        /**************************************************
         * Returns the vehicle that fulfills each request.
         *
         * @return the vehicle that fulfills each request.
         **************************************************/
        std::vector<unsigned> getVehiclesR() const;

        /***********************************************************
         * Returns the vehicle that fulfills the specified request.
         *
         * @param r the request whose vehicle is to be returned.
         *
         * @return the vehicle that fulfills the specified request.
         ***********************************************************/
        unsigned getVehicleR(unsigned r) const;

        /***********************************************
         * Returns the vehicle that serves each vertex.
         *
         * @return the vehicle that serves each vertex.
         ***********************************************/
        std::vector<unsigned> getVehiclesV() const;

        /********************************************************
         * Returns the vehicle that serves the specified vertex.
         *
         * @param v the vertex whose vehicle is to be returned.
         *
         * @return the vehicle that serves the specified vertex.
         ********************************************************/
        unsigned getVehicleV(unsigned v) const;

        /********************************************************
         * Returns the vehicle that traverses each edge.
         *
         * @return the vehicle that traverses each edge.
         ********************************************************/
        std::vector<std::vector<unsigned>> getVehiclesE() const;

        /****************************************************************************
         * Returns the vehicle that serves the specified edge.
         *
         * @param u the first endpoint of the edge whose vehicle is to be returned.
         * @param v the second endpoint of the edge whose vehicle is to be returned.
         *
         * @return the vehicle that serves the specified edge.
         ****************************************************************************/
        unsigned getVehicleE(unsigned u, unsigned v) const;

        /***************************************************************
         * Returns the requests fulfilled by each vehicle.
         *
         * @return the requests fulfilled by each vehicle.
         ***************************************************************/
        std::vector<std::unordered_set<unsigned>> getRequestsK() const;

        /********************************************************************
         * Returns the requests fulfilled by the specified vehicle.
         *
         * @param k the vehicle whose fulfilled requests are to be returned.
         *
         * @return the requests fulfilled by the specified vehicle.
         ********************************************************************/
        std::unordered_set<unsigned> getRequestsK(unsigned k) const;

        /********************************************************
         * Returns the indexes of each vertex in each path.
         *
         * @return the indices of each vertex in each path.
         ********************************************************/
        std::vector<std::vector<unsigned>> getIndexesKV() const;

        /**********************************************************************************
         * Returns the index of the specified vertex in the path of the specified vehicle.
         *
         * @param k the vehicle of the path.
         * @param v the vertex whose index in the path is to be returned.
         *
         * @return the index of the specified vertex in the path of the specified vehicle.
         **********************************************************************************/
        unsigned getIndexKV(unsigned k, unsigned v) const;

        /*******************************************************************************
         * Verifies whether the path of the specified vehicle is valid.
         *
         * @param k     the vehicle whose path is to be verified.
         * @param error the error code.
         *
         * @return true if the path of the specified vehicle is valid; false otherwise.
         *******************************************************************************/
        bool isValidPath(unsigned k, unsigned & error) const;

        /*******************************************************************************
         * Verifies whether the path of the specified vehicle is valid.
         *
         * @param k the vehicle whose path is to be verified.
         *
         * @return true if the path of the specified vehicle is valid; false otherwise.
         *******************************************************************************/
        bool isValidPath(unsigned k) const;

        /**************************************************************
         * Verifies whether this solution is feasible.
         *
         * @param error the error code.
         *
         * @return true if this instance is feasible; false otherwise.
         **************************************************************/
        bool isFeasible(unsigned & error) const;

        /**************************************************************
         * Verifies whether this solution is feasible.
         *
         * @return true if this instance is feasible; false otherwise.
         **************************************************************/
        bool isFeasible() const;

        /****************************************************************************
         * Verifies whether this solutions satisfies all constraints.
         *
         * @param error the error code.
         *
         * @return true if this instance satisfies all constraints; false otherwise.
         ****************************************************************************/
        bool areConstraintsSatisfied(unsigned & error) const;

        /****************************************************************************
         * Verifies whether this solutions satisfies all constraints.
         *
         * @return true if this instance satisfies all constraints; false otherwise.
         ****************************************************************************/
        bool areConstraintsSatisfied() const;

        /********************************************************
         * Write this solution into the specified output stream.
         *
         * @param os the output stream to write into.
         ********************************************************/
        void write(std::ostream & os) const;

        /***********************************************
         * Write this solution into the specified file.
         *
         * @param filename the file to write into.
         ***********************************************/
        void write(const char * filename) const;

        /***********************************************
         * Write this solution into the specified file.
         *
         * @param filename the file to write into.
         ***********************************************/
        void write(const std::string & filename) const;

        /*******************************************************
         * Write this solution into the standard output stream.
         *******************************************************/
        void write() const;

        /***************************************************************************************
         * Compares this solutions with the specified one for equality, indicating whether this
         * solution is equal to the specified one.
         *
         * @param solution the solution to be compared.
         *
         * @return true if this solution is equal to the specified solution; false otherwise
         ***************************************************************************************/
        bool operator == (const Solution & solution) const;

        /**************************************************************************************
         * Compares this solution with the specified one for order, indicating whether this
         * solution is less than the specified one.
         *
         * @param solution the solution to be compared.
         *
         * @return true if this solution is less than the specified solution; false otherwise.
         **************************************************************************************/
        bool operator < (const Solution & solution) const;

        /***********************************************************************************
         * Compares this solution with the specified one for order, indicating whether this
         * solution is greater than the specified one.
         *
         * @param solution the solution to be compared.
         *
         * @return true if this solution is greater than the specified solution; 
         *         false otherwise.
         ***********************************************************************************/
        bool operator > (const Solution & solution) const;

        /******************************************************************************
         * Verifies whether this solution dominates the specified one.
         *
         * @param solution the solution whose domination is to be verified.
         *
         * @return true if this instance dominated the specified one; false otherwise.
         ******************************************************************************/
        bool dominates(const Solution & solution) const;
};

