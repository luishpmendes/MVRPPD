#pragma once

#include <iostream>
#include <vector>

/***************************************************
 * The Instance class represents a MVRPPD instance.
 ***************************************************/
class Instance {
    private:
        /**************************
         * The number of requests.
         **************************/
        unsigned numRequests;

        /**************************
         * The number of vehicles.
         **************************/
        unsigned numVehicles;

        /**************************
         * The number of vertices.
         **************************/
        unsigned numVertices;

        /**************************
         * The sum of the profits.
         **************************/
        double sumProfit;

        /******************************
         * The profit of each request.
         ******************************/
        std::vector<double> profits;

        /*************************************
         * The creation time of each request.
         *************************************/
        std::vector<double> tCreates;

        /******************************
         * The demand of each request.
         ******************************/
        std::vector<double> demandsR;

        /*************************************
         * The source vertex of each request.
         *************************************/
        std::vector<unsigned> sourcesR;

        /******************************************
         * The target vertex of each request.
         ******************************************/
        std::vector<unsigned> targetsR;

        /**************************************************
         * The starting time of operation of each vehicle.
         **************************************************/
        std::vector<double> tStarts;

        /*******************************************************
         * The maximum time-limit of operation of each vehicle.
         *******************************************************/
        std::vector<double> tLimits;

        /*****************************
         * The speed of each vehicle.
         *****************************/
        std::vector<double> speeds;

        /********************************
         * The capacity of each vehicle.
         ********************************/
        std::vector<double> capacities;

        /*************************************
         * The source vertex of each vehicle.
         *************************************/
        std::vector<unsigned> sourcesK;

        /******************************************
         * The destination vertex of each vehicle.
         ******************************************/
        std::vector<unsigned> targetsK;

        /***********************************
         * The x coordinate of each vertex.
         ***********************************/
        std::vector<double> x;

        /***********************************
         * The y coordinate of each vertex.
         ***********************************/
        std::vector<double> y;

        /**************************************************
         * The amount of time needed to visit each vertex.
         **************************************************/
        std::vector<double> tVisits;

        /*******************************************
         * The request associated with each vertex.
         *******************************************/
        std::vector<unsigned> requestsV;

        /*******************************************
         * The vehicle associated with each vertex.
         *******************************************/
        std::vector<unsigned> vehiclesV;

        /***************************************************************************************
         * Array of flags indicating whether each vertex is the source of a request or vehicle.
         ***************************************************************************************/
        std::vector<bool> isSourceV;

        /***************************************************************************************
         * Array of flags indicating whether each vertex is the target of a request or vehicle.
         ***************************************************************************************/
        std::vector<bool> isTargetV;

        /*****************************
         * The demand of each vertex.
         *****************************/
        std::vector<double> demandsV;

        /****************************************
         * The length of each edge.
         ****************************************/
        std::vector<std::vector<double>> lengths;

        /************************************
         * Initializes a new instance.
         ************************************/
        void init();

        /*******************************************
         * Initializes a new instance.
         *
         * @param is the input stream to read from.
         *******************************************/
        void init(std::istream & is);

    public:
        /***************************************************************************************
         * Constructs a new instance.
         *
         * @param profits    the profit of each request.
         * @param tCreates   the creation time of each request.
         * @param demandsR   the demand of each request.
         * @param sourcesR   the source vertex of each request.
         * @param targetsR   the destination vertex of each request.
         * @param tStarts    the starting time of operation of each vehicle.
         * @param tLimits    the maximum time-limit of operation of each vehicle.
         * @param speeds     the speed of each vehicle.
         * @param capacities the capacity of each vehicle.
         * @param sourcesK   the source vertex of each vehicle.
         * @param targetsK   the destination vertex of each vehicle.
         * @param G          the complete graph.
         * @param tVisits    the amount of time needed to visit each vertex.
         ***************************************************************************************/
        Instance(const std::vector<double> & profits, const std::vector<double> & tCreates,
                const std::vector<double> & demandsR, const std::vector<unsigned> & sourcesR,
                const std::vector<unsigned> & targetsR, const std::vector<double> & tStarts,
                const std::vector<double> & tLimits, const std::vector<double> & speeds,
                const std::vector<double> & capacities, const std::vector<unsigned> & sourcesK,
                const std::vector<unsigned> & targetsK, const std::vector<double> x,
                const std::vector<double> y, const std::vector<double> tVisits);

        /*******************************************
         * Constructs a new instance.
         *
         * @param is the input stream to read from.
         *******************************************/
        Instance(std::istream & is);

        /*****************************************
         * Constructs a new instance.
         *
         * @param filename the file to read from.
         *****************************************/
        Instance(const char * filename);

        /*****************************************
         * Constructs a new instance.
         *
         * @param filename the file to read from.
         *****************************************/
        Instance(const std::string & filename);

        /*
         * Constructs a new empty instance.
         */
        Instance();

        /**********************************
         * Returns the number of requests.
         *
         * @return the number of requests.
         **********************************/
        unsigned getNumRequests() const;

        /**********************************
         * Returns the number of vehicles.
         *
         * @return the number of vehicles.
         **********************************/
        unsigned getNumVehicles() const;

        /**********************************
         * Returns the number of vertices.
         *
         * @return the number of vertices.
         **********************************/
        unsigned getNumVertices() const;

        /**********************************
         * Returns the sum of the profits.
         *
         * @return the sum of the profits.
         **********************************/
        unsigned getSumProfit() const;

        /**************************************
         * Returns the profit of each request.
         *
         * @return the profit of each request.
         **************************************/
        std::vector<double> getProfits() const;

        /*******************************************************
         * Returns the profit of the specified request.
         *
         * @param r the request whose profit is to be returned.
         *
         * @return the profit of the specified request.
         *******************************************************/
        double getProfit(unsigned r) const;

        /*********************************************
         * Returns the creation time of each request.
         *
         * @return the creation time of each request.
         *********************************************/
        std::vector<double> getTCreates() const;

        /**************************************************************
         * Returns the creation time of the specified request.
         *
         * @param r the request whose creation time if to be returned.
         *
         * @return the creation time of the specified request.
         **************************************************************/
        double getTCreate(unsigned r) const;

        /****************************************
         * Returns the demand of each request.
         *
         * @return the demand of each request.
         ****************************************/
        std::vector<double> getDemandsR() const;

        /*******************************************************
         * Returns the demand of the specified request.
         *
         * @param r the request whose demand is to be returned.
         *
         * @return the demand of the specified request.
         *******************************************************/
        double getDemandR(unsigned r) const;

        /*********************************************
         * Returns the source vertex of each request.
         *
         * @return the source vertex of each request.
         *********************************************/
        std::vector<unsigned> getSourcesR() const;

        /**************************************************************
         * Returns the source vertex of the specified request.
         *
         * @param r the request whose source vertex is to be returned.
         *
         * @return the source vertex of the specified request.
         **************************************************************/
        unsigned getSourceR(unsigned r) const;

        /*********************************************
         * Returns the target vertex of each request.
         *
         * @return the target vertex of each request.
         *********************************************/
        std::vector<unsigned> getTargetsR() const;

        /**************************************************************
         * Returns the target vertex of the specified request.
         *
         * @param r the request whose target vertex is to be returned.
         *
         * @return the target vertex of the specified request.
         **************************************************************/
        unsigned getTargetR(unsigned r) const;

        /**********************************************************
         * Returns the starting time of operation of each vehicle.
         *
         * @return the starting time of operation of each vehicle.
         **********************************************************/
        std::vector<double> getTStarts() const;

        /***************************************************************************
         * Returns the starting time of operation of the specified vehicle.
         *
         * @param k the vehicle whose starting time of operation is to be returned.
         *
         * @return the starting time of operation of the specified vehicle.
         ***************************************************************************/
        double getTStart(unsigned k) const;

        /***************************************************************
         * Returns the maximum time-limit of operation of each vehicle.
         *
         * @return the maximum time-limit of operation of each vehicle.
         ***************************************************************/
        std::vector<double> getTLimits() const;

        /********************************************************************************
         * Returns the maximum time-limit of operation of the specified vehicle.
         *
         * @param k the vehicle whose maximum time-limit of operation is to be returned.
         *
         * @return the maximum time-limit of operation of the specified vehicle.
         ********************************************************************************/
        double getTLimit(unsigned k) const;

        /*************************************
         * Returns the speed of each vehicle.
         *
         * @return the speed of each vehicle.
         *************************************/
        std::vector<double> getSpeeds() const;

        /******************************************************
         * Returns the speed of the specified vehicle.
         *
         * @param k the vehicle whose speed is to be returned.
         *
         * @return the speed of the specified vehicle.
         ******************************************************/
        double getSpeed(unsigned k) const;

        /******************************************
         * Returns the capacity of each vehicle.
         *
         * @return the capacity of each vehicle.
         ******************************************/
        std::vector<double> getCapacities() const;

        /*********************************************************
         * Returns the capacity of the specified vehicle.
         *
         * @param k the vehicle whose capacity is to be returned.
         *
         * @return the capacity of the specified vehicle.
         *********************************************************/
        double getCapacity(unsigned k) const;

        /*********************************************
         * Returns the source vertex of each vehicle.
         *
         * @return the source vertex of each vehicle.
         *********************************************/
        std::vector<unsigned> getSourcesK() const;

        /************************************************************
         * Returns the source vertex of the specified vehicle.
         *
         * @param the vehicle whose source vertex is to be returned.
         *
         * @return the source vertex of the specified vehicle.
         ************************************************************/
        unsigned getSourceK(unsigned k) const;

        /*********************************************
         * Returns the target vertex of each vehicle.
         *
         * @return the target vertex of each vehicle.
         *********************************************/
        std::vector<unsigned> getTargetsK() const;

        /************************************************************
         * Returns the target vertex of the specified vehicle.
         *
         * @param the vehicle whose target vertex is to be returned.
         *
         * @return the target vertex of the specified vehicle.
         ************************************************************/
        unsigned getTargetK(unsigned k) const;

        /*******************************************
         * Returns the x coordinate of each vertex.
         *
         * @return the x coordinate of each vertex.
         *******************************************/
        std::vector<double> getX() const;

        /************************************************************
         * Returns the x coordinate of the specified vertex.
         *
         * @param v the vertex whose x coordinate is to be returned.
         *
         * @return the x coordinate of the specified vertex.
         ************************************************************/
        double getX(unsigned v) const;

        /*******************************************
         * Returns the y coordinate of each vertex.
         *
         * @return the y coordinate of each vertex.
         *******************************************/
        std::vector<double> getY() const;

        /************************************************************
         * Returns the y coordinate of the specified vertex.
         *
         * @param v the vertex whose y coordinate is to be returned.
         *
         * @return the y coordinate of the specified vertex.
         ************************************************************/
        double getY(unsigned v) const;

        /**********************************************************
         * Returns the amount of time needed to visit each vertex.
         *
         * @return the amount of time needed to visit each vertex.
         **********************************************************/
        std::vector<double> getTVisits() const;

        /***************************************************************************
         * Returns the amount of time needed to visit the specified vertex.
         *
         * @param v the vertex whose amount time needed to visit is to be returned.
         *
         * @return the amount of time needed to visit the specified vertex
         ***************************************************************************/
        double getTVisit(unsigned v) const;

        /***************************************************
         * Returns the request associated with each vertex.
         *
         * @return the request associated with each vertex.
         ***************************************************/
        std::vector<unsigned> getRequestsV() const;

        /******************************************************************
         * Returns the request associated with the specified vertex.
         *
         * @param v the vertex whose associated request is to be returned.
         *
         * @return the request associated with the specified vertex.
         ******************************************************************/
        unsigned getRequestV(unsigned v) const;

        /***************************************************
         * Returns the vehicle associated with each vertex
         *
         * @return the vehicle associated with each vertex.
         ***************************************************/
        std::vector<unsigned> getVehiclesV() const;

        /******************************************************************
         * Returns the vehicle associated with the specified vertex
         *
         * @param v the vertex whose associated vehicle is to be returned.
         *
         * @return the vehicle associated with the specified vertex.
         ******************************************************************/
        unsigned getVehicleV(unsigned v) const;

        /*************************************************************************************
         * Verifies whether the specified vertex is the source of a request or vehicle.
         *
         * @param v the vertex been verified.
         *
         * @return true  if the vertex is the source of a request or vehicle; false otherwise
         *************************************************************************************/
        bool isASource(unsigned v) const;

        /*************************************************************************************
         * Verifies whether the specified vertex is the target of a request or vehicle.
         *
         * @param v the vertex been verified.
         *
         * @return true  if the vertex is the target of a request or vehicle; false otherwise
         *************************************************************************************/
        bool isATarget(unsigned v) const;

        /****************************************
         * Returns the demand of each vertex.
         *
         * @return the demand of each vertex.
         ****************************************/
        std::vector<double> getDemandsV() const;

        /******************************************************
         * Returns the demand of the specified vertex.
         *
         * @param v the vertex whose demand is to be returned.
         *
         * @return the demand of the specified vertex.
         ******************************************************/
        double getDemandV(unsigned v) const;

        /****************************************************
         * Returns the length of each edge.
         *
         * @return the length of each edge.
         ****************************************************/
        std::vector<std::vector<double>> getLengths() const;

        /*************************************************************************
         * Returns the length of the specified edge.
         *
         * @param u the first vertex of the edge whose length is to be returned.
         * @param v the second vertex of the edge whose length is to be returned.
         *
         * @return the length of the specified edge.
         *************************************************************************/
        double getLength(unsigned u, unsigned v) const;

        /***********************************************************
         * Verifies whether this instance is valid.
         *
         * @param error the error code.
         *
         * @return true if this instance is valid; false otherwise.
         ***********************************************************/
        bool isValid(unsigned & error) const;

        /***********************************************************
         * Verifies whether this instance is valid.
         *
         * @return true if this instance is valid; false otherwise.
         ***********************************************************/
        bool isValid() const;

        /********************************************************
         * Write this instance into the specified output stream.
         *
         * @param os the output stream to write into.
         ********************************************************/
        void write(std::ostream & os) const;

        /***********************************************
         * Write this instance into the specified file.
         *
         * @param filename the file to write into.
         ***********************************************/
        void write(const char * filename) const;

        /*******************************************************
         * Write this instance into the standard output stream.
         *******************************************************/
        void write() const;
};

