#pragma once
#include <string>
#include <iostream>

#include "graphics.h"
#include "easygl_constants.h"
#include "StreetsDatabaseAPI.h"

#include "m1.h"
#include "streetmap.h"
#include <unordered_map>

#include <math.h> 
#include "m3.h"

#define INIT_ZOOM 250
extern unsigned RADIUS;

extern t_point inter_posi;
extern t_point POI_posi;
// Draws the map whose at map_path; this should be a .bin file.
void draw_map(std::string map_path);
// initialize
void init();
// graphics
void drawscreen (void);
// average lat lon
LatLon getAvgLatLon ();

// converts LatLon to XY
t_point LatLon_to_XY (LatLon postion);
// draws a circle on the position
void drawPosition (LatLon position);
// draws a line from-to
void drawSegment (LatLon from, LatLon to);
// draws a single street segment
void drawStreetSegment (unsigned segmentID);
//set different colors and linewidth based on speed limit and if the street is one or two way
void setStreetSegmentColor (unsigned segmentID);

// draws all Points of interest
void drawPointOfInterest();
// draws all intersections
void drawIntersection();
// draws all streets
void drawStreet();

// is feature closed
bool isFeatureClosed (unsigned id);
// draw feature area
void drawFeatureArea(unsigned id, int cindex);
// draw feature area
void drawFeatureLine(unsigned id);
// draws all features
void drawFeature();
//find feature attribute
std::string featurexplorer(unsigned id);
//find closest point of interest, return min_distance
double find_distance_closest_point_of_interest(unsigned ID);
//find closest intersection, return min_distance
double find_distance_closest_intersection(unsigned ID);
//find closest intersection, return min_distance
float find_street_angle(unsigned ID);
//draw street name
void draw_street_name();

//m3

//draws path intersection to intersection
vector<unsigned> findPath (unsigned source, unsigned dest);
//draws path intersection to POI
vector<unsigned> findPOIPath (unsigned inter, std::string POI);
//draws path of intersection
void drawPathIntersection (std::vector<unsigned> path);
//draws path of segments
void drawPath (std::vector<unsigned> path);
// closest POI
vector<unsigned> find_closest_POI_from_Interseciton(string intersection_name, string POI_name);
// cloest intersections
vector<unsigned> find_cloest_intersections_mutiple_POI(vector<unsigned> POI_ID);
// bound_box to square
t_bound_box adjust_t_bound_box(t_bound_box origin);
// set street width
int setStreetWidth();

// m4
void testM4();
void drawDeliveries(std::vector<unsigned> &delivery, std::vector<unsigned> &depot);