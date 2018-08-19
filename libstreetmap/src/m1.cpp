#include "m1.h"
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include "streetmap.h"
using namespace std;

//load the map
streetmap init_map;
bool load_map(std::string map_name) {

    bool load_success = loadStreetDatabaseBIN(map_name);
    init_map.build(load_success);
    // created structures here to speed up API functions
    //street
    return load_success;
}


//close the map
void close_map() {
    closeStreetDatabase();
    
    // destroy any data structures created in load_map
   
}

//Chen
unsigned find_intersection_id_from_name(std::string intersection_name)
{
    return init_map.intersections[intersection_name];
}


//function to return street id for an street name
//Chen
unsigned find_street_id_from_name(std::string street_name)
{
    return init_map.streets[street_name];
}


//function to return the street segments for a given intersection with intersection_name as argument
//Peter
std::vector<unsigned> find_intersection_street_segments(std::string intersection_name)
{
    unsigned intersectionID = find_intersection_id_from_name(intersection_name);
    
    return find_intersection_street_segments(intersectionID);
}

//function to return the street segments for a given intersection with intersectionID as argument
//Peter
std::vector<unsigned> find_intersection_street_segments(unsigned intersectionID)
{
    vector<unsigned> streetsegments;
    unsigned segmentCount = getIntersectionStreetSegmentCount(intersectionID);
 
    for(unsigned i=0; i < segmentCount;i++)
    {
        unsigned segmentID = getIntersectionStreetSegment(intersectionID,i);
        streetsegments.push_back(segmentID);
    }
    
    return streetsegments;
}


//function to return street names at an intersection with intersection_name as argument
//Peter
std::vector<std::string> find_intersection_street_names(std::string intersection_name)
{
    unsigned intersectionID = find_intersection_id_from_name(intersection_name);
    
    return find_intersection_street_names(intersectionID);
}


//function to return street names at an intersection with intersectionID as argument
//Peter
std::vector<std::string> find_intersection_street_names(unsigned intersectionID)
{
    bool same=false;
    vector<unsigned> streetsegments = find_intersection_street_segments(intersectionID);
    vector<string> streetname;
    streetname.push_back(getStreetName(getStreetSegmentStreetID(streetsegments[0])));
    
    if(intersectionID >= getNumberOfIntersections()) //if intersectionID out of range, output error message
    {
        cout << "out of range" << endl;
    }
    else
    {
        for (auto i = streetsegments.begin(); i != streetsegments.end(); i++)
        {
            for(auto j = streetname.begin(); j != streetname.end(); j++)
            {
               if(*j == getStreetName(getStreetSegmentStreetID(*i)))
                same = true;
            }
        
            if(!same)
            {
                streetname.push_back(getStreetName(getStreetSegmentStreetID(*i)));
            }
            same = false;
        }
   }
    
        return streetname;
}


//get from intersection1 to intersection2 using a single street segment (hint: check for 1-way streets too)
//Peter
bool are_directly_connected(std::string intersection_name1, std::string intersection_name2)
{
    bool isConnected = false;
    vector<unsigned> streetsegments = find_intersection_street_segments(intersection_name1);
    
    for(auto i = streetsegments.begin(); i != streetsegments.end(); i++)
    {
        if(!getStreetSegmentOneWay(*i))
        {
            if((getStreetSegmentEnds(*i).from == find_intersection_id_from_name(intersection_name2))
                    ||(getStreetSegmentEnds(*i).to == find_intersection_id_from_name(intersection_name2)))
                isConnected = true;                   
        }
        else
        {
             if((getStreetSegmentEnds(*i).from == find_intersection_id_from_name(intersection_name1))
                    &&(getStreetSegmentEnds(*i).to == find_intersection_id_from_name(intersection_name2)))
                 isConnected = true;                 
        }
    }
    
    return isConnected;
}


//finding adjacent intersections
//Peter
std::vector<unsigned> find_adjacent_intersections(std::string intersection_name)
{
    unsigned intersectionID = find_intersection_id_from_name(intersection_name);
    
    return find_adjacent_intersections(intersectionID);
}


std::vector<unsigned> find_adjacent_intersections(unsigned intersection_ID)
{
    vector<unsigned> streetsegments=find_intersection_street_segments(intersection_ID);
    vector<unsigned> adjacent_intersection;
    
    for(auto i = streetsegments.begin(); i != streetsegments.end(); i++)
    {
        if(!getStreetSegmentOneWay(*i))
        {
            if(getStreetSegmentEnds(*i).from == intersection_ID)
                adjacent_intersection.push_back(getStreetSegmentEnds(*i).to);
            else
                adjacent_intersection.push_back(getStreetSegmentEnds(*i).from);  
        
        }
        else
        {
            if(getStreetSegmentEnds(*i).from == intersection_ID)
                adjacent_intersection.push_back(getStreetSegmentEnds(*i).to);
        }
    }
    
    return adjacent_intersection;
}



//for a given street, return all the street segments
//Chen
std::vector<unsigned> find_street_street_segments(std::string street_name)
{
    unsigned streetID = find_street_id_from_name(street_name);

    return init_map.segments[streetID];
}


//for a given street, find all the intersections
//using find_street_street_segments to find intersections on segments' ends
//Chen
std::vector<unsigned> find_all_street_intersections(std::string street_name) 
{
    vector<unsigned> intersections;
    vector<unsigned> segments = find_street_street_segments(street_name);

    for (auto i = segments.begin(); i != segments.end(); i++) 
    {
        intersections.push_back(getStreetSegmentEnds(*i).from);
        intersections.push_back(getStreetSegmentEnds(*i).to);
    }
    sort(intersections.begin(), intersections.end());
    auto last = unique(intersections.begin(), intersections.end());
    intersections.erase(last, intersections.end());

    return intersections;
}



//find distance between two coordinates
//Zhang
double find_distance_between_two_points(LatLon point1, LatLon point2)
{
    double lat_avg;
    double x1,x2,y1,y2;
    double distance;
    lat_avg = (point1.lat + point2.lat)/2 * DEG_TO_RAD;
    double c = cos(lat_avg);
   
    y1 = point1.lat * DEG_TO_RAD;
    y2 = point2.lat * DEG_TO_RAD;
    x1 = (point1.lon * DEG_TO_RAD) * c;
    x2 = (point2.lon * DEG_TO_RAD) * c;
    
    distance = EARTH_RADIUS_IN_METERS * sqrt((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1));
    
    return distance;
}


//find the length of a given street segments
//Zhang
double find_street_segment_length(unsigned street_segment_id) {
    
        double length = 0;
        
        //get intersection ID
        long startID = getStreetSegmentEnds(street_segment_id).from;
        long endID = getStreetSegmentEnds(street_segment_id).to;
        
        //convert ID into position
        LatLon point1 = getIntersectionPosition(startID);
        LatLon point2 = getIntersectionPosition(endID);
        long curve_point_count = getStreetSegmentCurvePointCount(street_segment_id);
	if (curve_point_count != 0) {
	    
            //create an array and store curve_pt IDs into the array
            LatLon curve_points[curve_point_count];
            for (long i=0 ; i < curve_point_count ; i++) 
                curve_points[i] = getStreetSegmentCurvePoint(street_segment_id,i);    
            for (long j=0 ; j < curve_point_count-1 ; j++) 
                length += find_distance_between_two_points(curve_points[j],curve_points[j+1]);
        
            //compute the length for start/end segments
            length += find_distance_between_two_points(point1,curve_points[0]);
            length += find_distance_between_two_points(curve_points[curve_point_count-1],point2);
            return length;
	}
        else 
            return find_distance_between_two_points(point1, point2);
}


//find the length of a whole street
//Zhang
double find_street_length(std::string street_name) {
    vector<unsigned> streetSegments;
    unsigned thisStreetSegment;
    double distance = 0; 
    
    //compute length by adding segments one by one
    streetSegments = find_street_street_segments(street_name);
    for (unsigned i=0; i < streetSegments.size(); i++) {
        thisStreetSegment = streetSegments.at(i);
        distance += find_street_segment_length(thisStreetSegment);
    }
    
    return distance;
}


//find the travel time to drive a street segment (time(minutes) = distance(Km)/speed_limit(Km/h)*60)
//Zhang
double find_segment_travel_time(unsigned street_segment_id) {
    
    //have to convert the length into km first, than plug in formula
    return  (find_street_segment_length(street_segment_id)/1000) / getStreetSegmentSpeedLimit(street_segment_id)*60;
}



////find the nearest point of interest (by name) to a given position
//Peter
std::string find_closest_point_of_interest(LatLon my_position)
{
    unsigned long long countPointofinterest=getNumberOfPointsOfInterest();
    string closestPoint = getPointOfInterestName(0);
    LatLon otherslocation = getPointOfInterestPosition(0);
    double min_distance = find_distance_between_two_points(my_position, otherslocation);
    for(unsigned i = 1; i < countPointofinterest;i++)        
    {
        otherslocation=getPointOfInterestPosition(i);
        double distance=find_distance_between_two_points(my_position, otherslocation);
        
        if(distance < min_distance)
        {
            min_distance = distance;
            closestPoint = getPointOfInterestName(i);
        }
    }
    
    return closestPoint;
}


vector<unsigned> find_POI_id_from_name(std::string POI_name)
{
    return init_map.POI[POI_name];
}


unsigned find_closest_intersection(unsigned POI_ID)
{
    LatLon POI_Pos = getPointOfInterestPosition(POI_ID);
    LatLon dest = getIntersectionPosition(0);
    double min_distance = find_distance_between_two_points(POI_Pos, dest);
    unsigned closestPoint = 0;
    
    for(unsigned i = 1; i < getNumberOfIntersections(); i++)        
    {
        dest = getIntersectionPosition(i);
        double distance = find_distance_between_two_points(POI_Pos, dest);
        
        if(distance < min_distance)
        {
            min_distance = distance;
            closestPoint = i;
        }
    }
    
    return closestPoint;
}


vector<unsigned> find_closest_POI_from_Interseciton(unsigned intersection_ID, string POI_name)
{
    vector<unsigned> POI_ID = find_POI_id_from_name(POI_name);
    vector<unsigned> Cloest_POI;
    LatLon intersection_position = getIntersectionPosition(intersection_ID);
    double min1=MAXNUM, min2=MAXNUM, min3=MAXNUM;
    int ID1 = -1,ID2 = -1,ID3 = -1;
    
    for(auto i = POI_ID.begin(); i != POI_ID.end(); i++)
    {
        LatLon POI_position=getPointOfInterestPosition(*i);
        double distance=find_distance_between_two_points(intersection_position, POI_position);
        
        if(distance<=min1&&distance<min2&&distance<min3) 
        {
            min3=min2;
            min2=min1;
            min1=distance;
            ID3=ID2;
            ID2=ID1;
            ID1=*i;
        }
        else if(distance>min1&&distance<=min2&&distance<min3)
        {
            min3=min2;
            min2=distance;
            ID3=ID2;
            ID2=*i;
        }
        else if(distance>min1&&distance>min2&&distance<=min3)
        {
            min3=distance;
            ID3=*i;
        }
    }
    
    if(ID1 != -1)
        Cloest_POI.push_back(ID1);
    if(ID2 != -1)
        Cloest_POI.push_back(ID2);
    if(ID3 != -1)
        Cloest_POI.push_back(ID3);
    
    return Cloest_POI;
}

vector<unsigned> find_closest_intersections_mutiple_POI(vector<unsigned> POI_ID)
{
    vector<unsigned> intersection_ID;
    
    for(auto i=POI_ID.begin(); i!=POI_ID.end();i++)
    {
        unsigned closest_intersection=find_closest_intersection(*i);
        intersection_ID.push_back(closest_intersection);
    }
    return intersection_ID;
}