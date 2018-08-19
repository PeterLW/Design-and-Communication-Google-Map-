/* 
 * File:   streetmap.h
 * Author: linwei8
 *
 * Created on January 29, 2015, 1:05 PM
 */

#ifndef STREETMAP_H
#define	STREETMAP_H
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include "m1.h"

using namespace std;

#ifdef	__cplusplus
extern "C" {
#endif
    
    
    class streetmap
    {
    public:
        unordered_map<string,unsigned> intersections;
        unordered_map<string,unsigned> streets;
        unordered_map<unsigned,vector<unsigned>> segments;
        unordered_map<string,vector<unsigned>> POI;
        
        vector< string > intersections_names;
        vector< string > POI_names;
        vector <bool> visited;
        
        streetmap();
        void build(bool sucess);
        void clearVisited();
    };


#ifdef	__cplusplus
}
#endif

#endif	/* STREETMAP_H */

