#pragma once
#include <vector>
#include <algorithm>
#include "m3.h"
#include <thread> 
#include <mutex>

struct reach_node
{
    unsigned reach_from;
    unsigned reach_to;
    unsigned index;
    reach_node () 
    {}
    
    reach_node (unsigned from, unsigned to, unsigned index_) 
    {
        
        reach_from = from;
        reach_to = to;
        index=index_;
    }
};

#define MAXNUM 999999

    // This routine takes in a vector of N intersections that must be 
    // traversed in a path, and another vector of M intersections that 
    // are legal start and end points for the path. The first vector 
    // gives the intersection ids of a set of locations at which 
    // deliveries must be made. The second vector gives the intersection
    // ids of courier company depots containing trucks; you start at any
    // one of these depots and end at any one of the depots.
    // You can assume that N is always at least 1, and M is always at 
    // least one (i.e. both input vectors are non-empty).  You can also
    // assume that no intersection is repeated in either list and that
    // no intersection is both a delivery location and a depot (i.e. no
    // intersection will appear in both vectors).
    // This routine returns a vector of street segment ids that form a 
    // path, where the first street segment id is connected to a depot
    // intersection, and the last street segment id also connects to a
    // depot intersection.  If no path connecting all the delivery locations
    // and a start and end depot exists, this routine should return an 
    // empty (size == 0) vector.

    std::vector<unsigned> traveling_salesman(std::vector<unsigned> intersections_to_traverse, std::vector<unsigned> depot_locations);
        
    // greedy algorithm
    vector<unsigned> greedy(unsigned startDepot, unsigned size);
    // keeps improving until stuck in local minima
    vector<unsigned> iterativeHill(std::vector<unsigned> pathIndex);
    // hill climbing
    vector<unsigned> hillClimbing(std::vector<unsigned> pathIndex);
    // keeps improving until stuck in local minima
    vector<unsigned> iterative(std::vector<unsigned> pathIndex);
    // multiStart
    vector<unsigned> multiStart(unsigned deliverySize, unsigned depotSize);
    // swap two deliveries
    vector<unsigned> multiTwoSwap(std::vector<unsigned> pathIndex);
    // swap i j index
    // changes pathIndex
    void twoSwap (std::vector<unsigned> &pathIndex, unsigned i, unsigned j);
    // multiSwap (not working)
    vector<unsigned> multiSwap(std::vector<unsigned> pathIndex);
    vector<unsigned> swap(double &bestPathDistance, std::vector<unsigned> &pathIndex, vector<unsigned> &index);
    
    // find the closest destination  (greedy)
    // returns index of Inter
    unsigned findClosestDest(unsigned currentIndex, 
        std::vector<unsigned> &destIndexLeft, std::vector<vector<double>> &distance);
    // search all dest
    unsigned findClosestDest(unsigned currentIndex, std::vector<vector<double>> &distance);
    
    // pre-compute all path length
    std::vector<std::vector<double>> findAllPathLen (std::vector<unsigned> &a, std::vector<unsigned> &b);
    // pre-compute all geo distances
    std::vector<std::vector<double>> findAllDistance(std::vector<unsigned> &a, std::vector<unsigned> &b);
    // initialization
    void init(std::vector<unsigned> &intersections_to_traverse, std::vector<unsigned> &depot_locations);
    
    void findDepotInterDistance(std::vector<unsigned> intersections_to_traverse, std::vector<unsigned> depot_locations);
    
    // translates pathIndex to actual path
    std::vector<unsigned> getPath(std::vector<unsigned> pathIndex, std::vector<unsigned> &intersections_to_traverse, std::vector<unsigned> &depot_locations);
    // print pathIndex
    void printPath(std::vector<unsigned> pathIndex);
    // returns total path distance
    double getPathDistance(std::vector<unsigned> &pathIndex);
    
    std::vector<unsigned> two_opt(vector<unsigned> init_path);
    
    vector<unsigned> check_path_valid(vector<reach_node> init_path, int size_of_path);
    
    vector<reach_node> path_reaching_node(vector<unsigned> old_path, int size_of_path);
    
    double total_traval_time(vector<unsigned> init_path);
    
    std::vector<unsigned> three_opt(vector<unsigned> &init_path);
    
    std::vector<unsigned> get_front(vector<unsigned> vec, int number);
