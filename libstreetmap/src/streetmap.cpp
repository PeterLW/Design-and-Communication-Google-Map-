#include <string>
#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include <list>
#include <vector>
#include <bits/hashtable.h>
#include "streetmap.h"

using namespace std;

streetmap::streetmap(){}

void streetmap::build(bool sucess)
{
    if(sucess){
        
        
        for (unsigned long long streetID=0; streetID < getNumberOfStreets(); streetID++)
        {
            string name_of_street=getStreetName(streetID);
            streets[name_of_street]=streetID;
        }

        //intersections
        for (unsigned long long intersectionID=0; intersectionID < getNumberOfIntersections(); intersectionID++)
        {
            string name_of_intersection=getIntersectionName(intersectionID);
            intersections[name_of_intersection]=intersectionID;
            intersections_names.push_back(name_of_intersection);
        }

        //segments
        for (unsigned long long i=0; i < getNumberOfStreetSegments(); i++)
        {
            unsigned long long streetID = getStreetSegmentStreetID(i);
            segments[streetID].push_back(i);
        }
        
        //POI
        for (unsigned long long i=0; i < getNumberOfPointsOfInterest(); i++)
        {
            string name_of_POI=getPointOfInterestName(i);
            POI[name_of_POI].push_back(i);
            POI_names.push_back(name_of_POI);
        }
    
        //intersections
    
        for (unsigned long long intersectionID=0; intersectionID < getNumberOfIntersections(); intersectionID++)
        {
            string name_of_intersection=getIntersectionName(intersectionID);
            intersections[name_of_intersection]=intersectionID;
            intersections_names.push_back(name_of_intersection);
        
        }
    
        //segments
        for (unsigned long long i=0; i < getNumberOfStreetSegments(); i++)
        {
            unsigned long long streetID = getStreetSegmentStreetID(i);
            segments[streetID].push_back(i);
        }
    
        
        // visited intersections
        for (unsigned long long i =0 ; i < getNumberOfIntersections(); i++)
        {
            visited.push_back(false);
        }
    }
}

void streetmap::clearVisited()
{
    for (unsigned long long i =0 ; i < visited.size(); i++)
    {
        visited[i] = false;
    }
}
