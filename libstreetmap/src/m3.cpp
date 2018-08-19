#include "m3.h"
#include "m1.h"
using namespace std;

map<unsigned, nodeInfo> visitedNode; // Nodes that are visited (BFS)
vector<unsigned> rightPath; // result path (DFS)
extern streetmap init_map;

struct nodeInfo 
{
    int edgeID; // edge used to reach this node
    double pathLen; // travel time used to reach this node from source
    double cost; // A* future cost
    nodeInfo () 
    {}
    
    nodeInfo (int id, double len, double c) 
    {
        edgeID = id;
        pathLen = len;
        cost = c;
    }
};

struct waveElem
{
    unsigned ID; // ID of this node (intersection ID)
    nodeInfo info; // edge used to reach this node
    waveElem (unsigned n, int id, double len, double c) 
    {
        ID = n;
        info.edgeID = id;
        info.pathLen = len;
        info.cost = c;
    }
    
    waveElem (unsigned n, nodeInfo i) 
    {
        ID = n;
        info = i;
    }
};


std::vector<unsigned> find_path_between_intersections(unsigned 
                   intersect_id_start, unsigned intersect_id_end)
{
    vector<unsigned> path;
    visitedNode.clear();
    init_map.clearVisited();
    
    bool found = BFS(intersect_id_start, intersect_id_end);
    if (found)
        path = bfsTraceBack(intersect_id_end);
    
    visitedNode.clear();
    return path;
}


double compute_path_travel_time(const std::vector<unsigned>& path)
{
    if (path.size() == 0)
	return 0;
    
    double sum = 0;
    unsigned segID = path[0];
    sum += find_segment_travel_time(segID);
    
    for(unsigned i = 1; i < path.size(); i++)
    {
        unsigned segID = path[i];
        sum += find_segment_travel_time(segID);
        if(isTurn (path[i-1], path[i]))
            sum += TURN_TIME;
    }
    
    return sum;
}


std::vector<unsigned> find_path_to_point_of_interest (unsigned 
                   intersect_id_start, std::string point_of_interest_name)
{
    vector<unsigned> POI_ID= find_closest_POI_from_Interseciton(intersect_id_start, point_of_interest_name);
    vector<unsigned> dest = find_closest_intersections_mutiple_POI(POI_ID);
    vector<unsigned> path;
    
    if(dest.size() == 1)
        path = find_path_between_intersections(intersect_id_start, dest[0]);
    else
    {
        visitedNode.clear();
        init_map.clearVisited();

        int end = BFSMulti (intersect_id_start, dest);
        if (end != -1)
            path = bfsTraceBack(end);
        visitedNode.clear();
    }
    return path;
}


bool BFS (unsigned source, unsigned dest)
{
    vector<waveElem> wavefront;
    LatLon destPos = getIntersectionPosition(dest);
    make_heap(wavefront.begin(), wavefront.end(), &isGreaterThan);
    wavefront.push_back(waveElem(source, NO_EDGE, 0, 0));
    push_heap(wavefront.begin(), wavefront.end(), &isGreaterThan);
    
    while(wavefront.size() != 0)
    {
        pop_heap(wavefront.begin(), wavefront.end(), &isGreaterThan);
        waveElem currentELEM = wavefront.back();
        wavefront.pop_back();
        unsigned current = currentELEM.ID;
        //cerr << current << " ";
        
        if(!init_map.visited[current])
        {
            // visitedNode insertion
            visitedNode.insert(pair<unsigned, nodeInfo> (current, currentELEM.info));
            init_map.visited[current] = true;
             
            if (current == dest)
            {
                return true;
            }

            vector<waveElem> adjacent = find_adjacent_node(current);
            for(unsigned i = 0; i < adjacent.size(); i++)
            {
                waveElem wav = adjacent[i];
                // drawing
                if(DRAW)
                {
                    drawSegment(getIntersectionPosition(current), getIntersectionPosition(wav.ID));
                    flushinput();
                }
                
                double newPathLen = wav.info.pathLen + currentELEM.info.pathLen;
                if(isTurn (currentELEM.info.edgeID, wav.info.edgeID))
                    newPathLen += TURN_TIME;
                double newCost = newPathLen + find_distance_between_two_points
                        (destPos, getIntersectionPosition(wav.ID))/AVG_SPEED * DISTANCE_WEIGHT;
		//cout << newPathLen << " : " << newCost << endl;

                // wavefront insertion
                if(!init_map.visited[wav.ID])
                {
                    wavefront.push_back(waveElem(wav.ID, wav.info.edgeID, newPathLen, newCost));
                    push_heap(wavefront.begin(), wavefront.end(), &isGreaterThan);
                }
            }
        }
    }
    
    return false;
}


int BFSMulti (unsigned source, vector<unsigned> dest)
{
    vector<waveElem> wavefront;
    make_heap(wavefront.begin(), wavefront.end(), &isGreaterThan);
    wavefront.push_back(waveElem(source, NO_EDGE, 0, 0));
    push_heap(wavefront.begin(), wavefront.end(), &isGreaterThan);
    
    while(wavefront.size() != 0)
    {
        pop_heap(wavefront.begin(), wavefront.end(), &isGreaterThan);
        waveElem currentELEM = wavefront.back();
        wavefront.pop_back();
        unsigned current = currentELEM.ID;
        //cerr << current << " ";
        
        if(!init_map.visited[current])
        {
            // visitedNode insertion
            visitedNode.insert(pair<unsigned, nodeInfo> (current, currentELEM.info));
            init_map.visited[current] = true;
            
            auto it = find (dest.begin(), dest.end(), current);
            if (it != dest.end())
            {
                return (*it);
            }

            vector<waveElem> adjacent = find_adjacent_node(current);
            for(unsigned i = 0; i < adjacent.size(); i++)
            {
                waveElem wav = adjacent[i];
                // drawing
                if(DRAW)
                {
                    drawSegment(getIntersectionPosition(current), getIntersectionPosition(wav.ID));
                    flushinput();
                }
                
                double newPathLen = wav.info.pathLen + currentELEM.info.pathLen;
                if(isTurn (currentELEM.info.edgeID, wav.info.edgeID))
                    newPathLen += TURN_TIME;
                double newCost = newPathLen;
		//cout << newPathLen << " : " << newCost << endl;

                // wavefront insertion
                if(!init_map.visited[wav.ID])
                {
                    wavefront.push_back(waveElem(wav.ID, wav.info.edgeID, newPathLen, newCost));
                    push_heap(wavefront.begin(), wavefront.end(), &isGreaterThan);
                }
            }
        }
    }
    
    return -1;
}


// Chen
vector<unsigned> bfsTraceBack (unsigned destID) 
{
    vector<unsigned> path;
    unsigned currNode = destID;
    int prevEdge = visitedNode[currNode].edgeID;
    
    while (prevEdge != NO_EDGE) 
    {
        path.push_back (prevEdge);
        currNode = segmentOtherEnd(prevEdge, currNode);
        prevEdge = visitedNode[currNode].edgeID;
    }
    
    reverse(path.begin(), path.end());
    return path;
}


// Chen
bool isTurn(int current, int to)
{
    return ((current != NO_EDGE) && (getStreetSegmentStreetID(current) != getStreetSegmentStreetID(to)));
}


// Chen
bool isGreaterThan(waveElem a, waveElem b)
{
    return a.info.cost > b.info.cost;
}


// Chen
std::vector<waveElem> find_adjacent_node(unsigned intersection_ID)
{
    vector<waveElem> adjacent;
    
    vector<unsigned> streetsegments=find_intersection_street_segments(intersection_ID);
    for(auto i = streetsegments.begin(); i != streetsegments.end(); i++)
    {
        unsigned segID = *i;
        double time = find_segment_travel_time(segID);
        nodeInfo info = nodeInfo(segID, time, 0);
        unsigned ID = segmentOtherEnd(segID, intersection_ID);
        waveElem elem = waveElem(ID, info);
        
        if(isReachable(segID, intersection_ID))
            adjacent.push_back(elem);
    }
    
    return adjacent;
}


// Chen
bool isReachable (unsigned segID, unsigned intID)
{
    return (!getStreetSegmentOneWay(segID) || getStreetSegmentEnds(segID).from == intID);
}


// Chen
unsigned segmentOtherEnd (unsigned segID, unsigned intID)
{
    StreetSegmentEnds ends = getStreetSegmentEnds(segID);
    if (ends.from == intID)
        return ends.to;
    else
        return ends.from;
}


// Chen
bool DFS (unsigned current, unsigned dest, vector<unsigned> path)
{
    cout << current << " ";
    if (current == dest)
    {
        /*for(auto i = path.begin(); i != path.end(); i++)
            cout << *i << " ";*/
        rightPath = path;
        cout << endl;
        return true;
    }
    
    visitedNode.insert(pair<unsigned, nodeInfo>(current,nodeInfo()));
    
    vector<unsigned> adjacent = find_adjacent_intersections(current);
    for(auto i = adjacent.begin(); i != adjacent.end(); i++)
    {
        unsigned to = *i;
        // drawing
        drawSegment(getIntersectionPosition(current), getIntersectionPosition(to));
        
        if(visitedNode.find(to) == visitedNode.end())
        {
            vector<unsigned> newPath = path;
            newPath.push_back (to);
            
            bool found = DFS(to, dest, newPath);
            if (found)
                return true;
        }
    }
    
    return false;
}

vector<double> getDistance (unsigned source, vector<unsigned> dest)
{
    /*visitedNode.clear();
    init_map.clearVisited();
    vector<double> distance = getDistanceBFS(source, dest);
    visitedNode.clear();
    return distance;*/
    
    vector <bool> visited;
    for (unsigned long long i =0 ; i < getNumberOfIntersections(); i++)
        visited.push_back(false);
    map<unsigned, nodeInfo> visitedNode;
    
    vector<double> distance(dest.size());
    unsigned numFound = 0;
    
    vector<waveElem> wavefront;
    make_heap(wavefront.begin(), wavefront.end(), &isGreaterThan);
    wavefront.push_back(waveElem(source, NO_EDGE, 0, 0));
    push_heap(wavefront.begin(), wavefront.end(), &isGreaterThan);
    
    while(wavefront.size() != 0)
    {
        pop_heap(wavefront.begin(), wavefront.end(), &isGreaterThan);
        waveElem currentELEM = wavefront.back();
        wavefront.pop_back();
        unsigned current = currentELEM.ID;
        //cerr << current << " ";
        
        if(!visited[current])
        {
            // visitedNode insertion
            visitedNode.insert(pair<unsigned, nodeInfo> (current, currentELEM.info));
            visited[current] = true;
            
            auto it = find (dest.begin(), dest.end(), current);
            if(it != dest.end())
            {
                distance[it-dest.begin()] = currentELEM.info.pathLen;
                numFound++;
            }
            if(numFound == dest.size())
                return distance;

            vector<waveElem> adjacent = find_adjacent_node(current);
            for(unsigned i = 0; i < adjacent.size(); i++)
            {
                waveElem wav = adjacent[i];
                // drawing
                if(DRAW)
                {
                    drawSegment(getIntersectionPosition(current), getIntersectionPosition(wav.ID));
                    flushinput();
                }
                
                double newPathLen = wav.info.pathLen + currentELEM.info.pathLen;
                if(isTurn (currentELEM.info.edgeID, wav.info.edgeID))
                    newPathLen += TURN_TIME;
                double newCost = newPathLen;
		//cout << newPathLen << " : " << newCost << endl;

                // wavefront insertion
                if(!visited[wav.ID])
                {
                    wavefront.push_back(waveElem(wav.ID, wav.info.edgeID, newPathLen, newCost));
                    push_heap(wavefront.begin(), wavefront.end(), &isGreaterThan);
                }
            }
        }
    }
    
    cerr << "not reachable" << endl;
    return distance;
} 

// Chen
vector<double> getDistanceBFS (unsigned source, vector<unsigned> &dest)
{
    vector<double> distance(dest.size());
    unsigned numFound = 0;
    
    vector<waveElem> wavefront;
    make_heap(wavefront.begin(), wavefront.end(), &isGreaterThan);
    wavefront.push_back(waveElem(source, NO_EDGE, 0, 0));
    push_heap(wavefront.begin(), wavefront.end(), &isGreaterThan);
    
    while(wavefront.size() != 0)
    {
        pop_heap(wavefront.begin(), wavefront.end(), &isGreaterThan);
        waveElem currentELEM = wavefront.back();
        wavefront.pop_back();
        unsigned current = currentELEM.ID;
        //cerr << current << " ";
        
        if(!init_map.visited[current])
        {
            // visitedNode insertion
            visitedNode.insert(pair<unsigned, nodeInfo> (current, currentELEM.info));
            init_map.visited[current] = true;
            
            auto it = find (dest.begin(), dest.end(), current);
            if(it != dest.end())
            {
                distance[it-dest.begin()] = currentELEM.info.pathLen;
                numFound++;
            }
            if(numFound == dest.size())
                return distance;

            vector<waveElem> adjacent = find_adjacent_node(current);
            for(unsigned i = 0; i < adjacent.size(); i++)
            {
                waveElem wav = adjacent[i];
                // drawing
                if(DRAW)
                {
                    drawSegment(getIntersectionPosition(current), getIntersectionPosition(wav.ID));
                    flushinput();
                }
                
                double newPathLen = wav.info.pathLen + currentELEM.info.pathLen;
                if(isTurn (currentELEM.info.edgeID, wav.info.edgeID))
                    newPathLen += TURN_TIME;
                double newCost = newPathLen;
		//cout << newPathLen << " : " << newCost << endl;

                // wavefront insertion
                if(!init_map.visited[wav.ID])
                {
                    wavefront.push_back(waveElem(wav.ID, wav.info.edgeID, newPathLen, newCost));
                    push_heap(wavefront.begin(), wavefront.end(), &isGreaterThan);
                }
            }
        }
    }
    
    cerr << "not reachable" << endl;
    return distance;
}
