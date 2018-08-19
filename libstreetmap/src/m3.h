#pragma once
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <map>

#include "m1.h"
#include "m2.h"

#define NO_EDGE -1 // Illegal edge ID 
#define TURN_TIME 0.25 // (minutes)
#define AVG_SPEED 30 // (m/s)
#define DISTANCE_WEIGHT 0.02 // A* heuristic weighting
#define DRAW false

struct nodeInfo; // information on node
struct waveElem; // wavefront element


// Returns a path (route) between the start intersection and the end 
// intersection, if one exists. If no path exists, this routine returns 
// an empty (size == 0) vector. If more than one path exists, the path 
// with the shortest travel time is returned. The path is returned as a vector 
// of street segment ids; traversing these street segments, in the given order,
// would take one from the start to the end intersection.
std::vector<unsigned> find_path_between_intersections(unsigned 
                   intersect_id_start, unsigned intersect_id_end);


// Returns the time required to travel along the path specified. The path
// is passed in as a vector of street segment ids, and this function can 
// assume the vector either forms a legal path or has size == 0.
// The travel time is the sum of the length/speed-limit of each street 
// segment, plus 15 seconds per turn implied by the path. A turn occurs
// when two consecutive street segments have different street names.
double compute_path_travel_time(const std::vector<unsigned>& path);


// Returns the shortest travel time path (vector of street segments) from 
// the start intersection to a point of interest with the specified name.
// If no such path exists, returns an empty (size == 0) vector.
std::vector<unsigned> find_path_to_point_of_interest (unsigned 
                   intersect_id_start, std::string point_of_interest_name);

// algorithms
// Depth First Search
bool DFS (unsigned current, unsigned dest, std::vector<unsigned> path);
// Breath First Search (A*)
bool BFS (unsigned source, unsigned dest);
// Breath First Search (Dijkstra) with multiple destination
int BFSMulti (unsigned source, vector<unsigned> dest);
// Trace Back for BFS
std::vector<unsigned> bfsTraceBack (unsigned destID);

// returns adjacent intersection ID and reaching segment ID
std::vector<waveElem> find_adjacent_node(unsigned intersection_ID);
// returns other end of segment
unsigned segmentOtherEnd (unsigned segment_ID, unsigned intersection_ID);
// is it possible to reach the other end
// assume intersection is one end of segment
bool isReachable (unsigned segment_ID, unsigned intersection_ID);
// waveElem comparator
bool isGreaterThan(waveElem a, waveElem b);
// is same street name
bool isTurn(int current, int to);

// m4
// returns distances to every dest
vector<double> getDistance (unsigned source, vector<unsigned> dest);
// Dijkstra
std::vector<double> getDistanceBFS (unsigned source, std::vector<unsigned> &dest);
