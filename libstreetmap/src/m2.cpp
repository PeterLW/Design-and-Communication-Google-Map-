//#include <bits/hashtable.h>
//#include "m3.h"
//#include "m4.h"
#include "m2.h"
#include <vector>
#include <unordered_map>

using namespace std;
t_point inter_posi(0,0);
t_point POI_posi(0,0);
double AVG_LAT = 0;
double AVG_LON = 0;
extern streetmap init_map;
vector<unsigned> path;
t_bound_box screen_bound;
bool isDraw=false;
extern unsigned path_POI_ID;
unsigned RADIUS;

void draw_map(std::string map_path)
{
    bool success = load_map(map_path);
    if (success)
        init();
    else
        cout << "Load_map Failed!" << endl;
    
    close_map();
}


// chen
LatLon getAvgLatLon ()
{
    LatLon avgLatLon;
    LatLon current;
    double latSum = 0;
    double lonSum = 0;
    for(unsigned i = 0; i < getNumberOfIntersections(); i++)
    {
        current = getIntersectionPosition(i);
        latSum += current.lat;
        lonSum += current.lon;
    }
    avgLatLon.lat = latSum / getNumberOfIntersections();
    avgLatLon.lon = lonSum / getNumberOfIntersections();
    
    return avgLatLon;
}


// chen
void init()
{
    LatLon avgLatLon = getAvgLatLon();
    AVG_LAT = avgLatLon.lat;
    AVG_LON = -avgLatLon.lon;
    
    const double size = EARTH_RADIUS_IN_METERS / INIT_ZOOM;
    const double X_MAP = EARTH_RADIUS_IN_METERS * (AVG_LON*DEG_TO_RAD) * cos(AVG_LAT*DEG_TO_RAD);
    const double Y_MAP = EARTH_RADIUS_IN_METERS * (AVG_LAT*DEG_TO_RAD);
    const t_bound_box initial_coords = t_bound_box(-X_MAP-size,Y_MAP-size, -X_MAP+size,Y_MAP+size); 
    
    std::cout << "About to start graphics.\n";
    init_graphics("Map", BACKGROUND);
    set_visible_world(initial_coords);
    update_message("Map");
    
    event_loop(NULL, NULL, NULL, drawscreen);
    close_graphics ();
    std::cout << "Graphics closed down.\n";
}

// chen, Peter, Tom
void drawscreen () 
{
    screen_bound = get_visible_world();
    cout << "height: " << screen_bound.get_height() << " width: " << screen_bound.get_width() << endl;//output height and width of screen
    
    set_draw_mode (DRAW_NORMAL);  // Should set this if your program does any XOR drawing in callbacks.
    clearscreen();  /* Should precede drawing for all drawscreens */
    
    drawFeature();
    setlinestyle (SOLID);
    drawStreet();
    draw_street_name();
    
    if (POIon == true)
        drawPointOfInterest(); 
    
    path = findPOIPath(134457, "Wendys");
    path = findPath(31504, 63229);
    if (reset_is_pressed == false)
        drawPath(path);
    
    
   testM4();
}


void testM4()
{
    int test = 2;
    vector<unsigned> path;
    vector<unsigned> delivery;
    vector<unsigned> depot;
    if(test == 0)
    {
        delivery = {41999, 103969};
        depot = {138722};
    }
    else if(test == 1)
    {
        delivery = {26713, 111288, 159340};
        depot = {9085, 122230};
    }
    else if(test == 2)
    {
        delivery = {334, 4904, 6868, 9693, 11584, 16073, 19450, 22197, 23666, 24352, 24756, 24837, 26373, 28349, 28760, 29864, 33378, 44271, 45997, 53528, 54101, 59155, 59257, 59823, 63019, 67387, 68370, 69112, 71050, 71395, 75438, 85427, 86482, 97085, 103839, 105650, 109689, 119610, 122014, 129738, 137740, 140247, 141616, 143665, 143872, 144521, 152618, 157705, 159225, 163352};
        depot = {6604, 15321, 30063, 57131, 77774, 97918, 102219, 103141, 160373, 160522};
    }
    drawDeliveries(delivery, depot);
    
    setlinewidth(1);
    setlinestyle(DASHED);
    setcolor(GREEN);
//    path = traveling_salesman(delivery, depot);
    
    drawDeliveries(delivery, depot);
    
    drawPath(path);
}


void drawDeliveries(std::vector<unsigned> &delivery, std::vector<unsigned> &depot)
{   RADIUS = 500;
    setlinestyle(SOLID);
    setlinewidth(4);
    setcolor(BLACK);
    for(unsigned i = 0; i < delivery.size(); i++)
        drawPosition(getIntersectionPosition(delivery[i]));
    setcolor(RED);
    for(unsigned i = 0; i < depot.size(); i++)
        drawPosition(getIntersectionPosition(depot[i]));
}


// Chen
vector<unsigned> findPOIPath (unsigned inter, std::string POI)
{
    setcolor(BLUE);
    drawPosition(getIntersectionPosition(inter));
    vector<unsigned> POI_ID ;
    
    POI_ID= find_closest_POI_from_Interseciton(inter, POI);
    
    POI_ID = find_POI_id_from_name(POI);
    setcolor(RED);
    for (unsigned i = 0; i < POI_ID.size(); i++)
    {
        //cerr << POI_ID[i] << endl;
        drawPosition(getPointOfInterestPosition(POI_ID[i]));
    }
    setcolor(BLUE);
    for (unsigned i = 0; i < POI_ID.size(); i++)
    {
        //cerr << POI_ID[i] << endl;
        drawPosition(getPointOfInterestPosition(POI_ID[i]));
    }
    
    setcolor(BLACK);
    vector<unsigned> path = find_path_to_point_of_interest(inter, POI);
    cout << "Travel time: " << compute_path_travel_time(path) << endl;
    return path;
}


// Chen
vector<unsigned> findPath (unsigned source, unsigned dest)
{
    setcolor(BLUE);
    drawPosition (getIntersectionPosition(source));
    drawPosition (getIntersectionPosition(dest));
    setcolor(GREEN);
    vector<unsigned> path = find_path_between_intersections(source, dest);
    //drawPath(path);
    cout << "Travel time: " << compute_path_travel_time(path) << endl;
    return path;
}



// Chen
void drawPath (vector<unsigned> path)
{
    if(isDraw)
    {
        settextrotation(-45);
        t_point inter_posi=LatLon_to_XY(getIntersectionPosition(start_intersec));
        t_point POI_posi=LatLon_to_XY(getIntersectionPosition(end_intersec));
        if(screen_bound.get_height()>=12749 && screen_bound.get_width()>=17854)
        {
            setcolor(GREEN);
            fillellipticarc(inter_posi.x, inter_posi.y, 400, 400, 0, 360);    
            setcolor(RED);
            fillellipticarc(POI_posi.x, POI_posi.y, 400, 400, 0, 360); 
            setfontsize(15);
            setcolor(BLACK);
            drawtext(inter_posi.x, inter_posi.y, "START", 100000, 100000);
            drawtext(POI_posi.x, POI_posi.y, "END", 100000, 100000);
        }
        else if(screen_bound.get_height()>2754 && screen_bound.get_width()>3857)
        {
            setcolor(GREEN);
            fillellipticarc(inter_posi.x, inter_posi.y, 200, 200, 0, 360);    
            setcolor(RED);
            fillellipticarc(POI_posi.x, POI_posi.y, 200, 200, 0, 360); 
            setfontsize(11);
            setcolor(BLACK);
            drawtext(inter_posi.x, inter_posi.y, "START", 1000, 1000);
            drawtext(POI_posi.x, POI_posi.y, "END", 1000, 1000);
        }
        else
        {
            setcolor(GREEN);
            fillellipticarc(inter_posi.x, inter_posi.y, 50, 50, 0, 360);    
            setcolor(RED);
            fillellipticarc(POI_posi.x, POI_posi.y, 50, 50, 0, 360); 
            setcolor(BLACK);
            setfontsize(8);
            drawtext(inter_posi.x, inter_posi.y, "START", 1000, 1000);
            drawtext(POI_posi.x, POI_posi.y, "END", 1000, 1000);
        } 
    }
    
    setlinewidth(6);
    setcolor(BLUE);
    for(auto i = path.begin(); i != path.end(); i++)
        drawStreetSegment(*i);
}


// Chen
void drawPathIntersection (vector<unsigned> path)
{
    setcolor(BLUE);
    LatLon from = getIntersectionPosition(*(path.begin()));
    for(auto i = path.begin(); i != path.end(); i++)
    {
        LatLon to = getIntersectionPosition(*i);
        drawSegment(from, to);
        from = to;
    }
}


// Chen, Peter
void drawFeatureLine(unsigned id, const t_color& color, int linestyle, int linewidth)
{
    unsigned numPoints = getFeaturePointCount(id);
    setlinewidth (linewidth);
    setcolor(color);
    setlinestyle(linestyle);
    LatLon point1 = getFeaturePoint(id, 0);//the first one is should be id, second one feature point idx
    LatLon point2;
    for(unsigned j = 1; j < numPoints; j++)
    {
        point2 = getFeaturePoint(id, j);
        drawSegment(point1, point2);
        point1 = point2;
    }
}

// Chen, Peter
void drawFeatureArea(unsigned id, const t_color& color, int linestyle)
{
    setcolor(color);
    setlinestyle(linestyle);
    unsigned numPoints = getFeaturePointCount(id);
    t_point points[numPoints];
    for (unsigned j = 0; j < numPoints; j++)
    {
        points[j] = LatLon_to_XY(getFeaturePoint(id, j));
    }
    fillpoly(points, numPoints);
}

// Chen
bool isFeatureClosed (unsigned id)
{
    LatLon start = getFeaturePoint(id, 0);
    LatLon end = getFeaturePoint(id, getFeaturePointCount(id)-1);
    return (start.lat == end.lat && start.lon == end.lon);
}

std::string featurexplorer(unsigned id)
{
    string feature_attri;
    if(getFeatureAttribute(id, "natural") != "")
        feature_attri=getFeatureAttribute(id, "natural");
    else if(getFeatureAttribute(id, "water") != "")
        feature_attri=getFeatureAttribute(id, "water");
    else if(getFeatureAttribute(id, "waterway") != "")
        feature_attri=getFeatureAttribute(id, "waterway");
    else if(getFeatureAttribute(id, "land") != "")
        feature_attri=getFeatureAttribute(id, "land");
    else
        feature_attri=getFeatureAttribute(id, "building"); 

    return feature_attri;
}

//Peter
void drawFeature()
{
    for(unsigned i = 0; i < getFeatureCount(); i++)
    {
        string feature_attri=featurexplorer(i);
        if(isFeatureClosed(i))
        {
            if(feature_attri=="water"||feature_attri=="yes")
                drawFeatureArea(i,LAKEONTARIO,SOLID);
            else if(feature_attri=="wood"|| feature_attri=="forest")
                drawFeatureArea(i,MY_FOREST,SOLID);
            else if(feature_attri=="peak")
                drawFeatureArea(i,LIMEGREEN,SOLID);
            else if(feature_attri=="grassland"||feature_attri=="grass" || feature_attri=="tree_row")
                drawFeatureArea(i,MY_GREEN,SOLID);
            else if(feature_attri=="sand"|| feature_attri=="beach")
                drawFeatureArea(i,KHAKI,SOLID);
            else if(feature_attri=="pond" || feature_attri=="lake" || feature_attri=="lagoon" ||feature_attri=="stream")
                drawFeatureArea(i,LAKEONTARIO,SOLID);
            else if(feature_attri=="wetland") 
                drawFeatureArea(i,BLUE,SOLID);
            else if(feature_attri=="scrub") 
                drawFeatureArea(i,MY_GREEN,SOLID);
            else if(feature_attri=="island") 
                drawFeatureArea(i,MY_ISLAND,SOLID);
            else if(feature_attri=="riverbank" || feature_attri=="conservation") 
                drawFeatureArea(i,BISQUE,SOLID);
            else if(feature_attri=="land") 
                drawFeatureArea(i,LIGHTGREY,SOLID);
            else if(feature_attri=="heath") 
                drawFeatureArea(i,CORAL,SOLID);
            else if(feature_attri=="construction"||feature_attri=="dam"||feature_attri=="breakwater"||feature_attri=="boatyard"||feature_attri=="drain") 
                drawFeatureArea(i,FIREBRICK,SOLID);
            else if(feature_attri=="recreation_ground") 
                drawFeatureArea(i,THISTLE,SOLID);
            else if(feature_attri=="rapids") 
                drawFeatureArea(i,LAKEONTARIO,SOLID);
            else{
                drawFeatureArea(i,MY_GREEN,SOLID);
            //cout<<"id: "<<i<<"  xx"<<feature_attri<<" is unknown"<<endl;
            }
        }
        else
        {
            if(feature_attri=="tree_row"||feature_attri=="tree"||feature_attri=="wood")
                drawFeatureLine(i, DARKGREEN, DASHED, 2);
            else if(feature_attri=="river"||feature_attri=="water"||feature_attri=="yes")
                drawFeatureLine(i, LIGHTSKYBLUE, SOLID, 3);
            else if(feature_attri=="stream")
                drawFeatureLine(i, LIGHTSKYBLUE, SOLID, 1);
            else if(feature_attri=="drain"||feature_attri=="weir"||feature_attri=="breakwater"||feature_attri=="dam")
                drawFeatureLine(i, MY_DRAIN, SOLID, 2);
            else if(feature_attri=="rapids") 
                drawFeatureLine(i,DARKSLATEBLUE,SOLID, 1);
            else if(feature_attri=="cliff"||feature_attri=="ditch") 
                drawFeatureLine(i,CORAL,SOLID, 1);
            else if(feature_attri=="riverbank") 
                drawFeatureLine(i,BISQUE,SOLID, 1);
            else{
               // cout<<"id: "<<i<<"  xx"<<feature_attri<<" is unknown"<<endl;
                drawFeatureLine(i, LIGHTGREY, SOLID, 1);}

        }
    }
}

// chen,Peter
void drawPointOfInterest()
{   
    RADIUS = 5;
    settextrotation(0);
    for(unsigned i = 0; i < getNumberOfPointsOfInterest(); i++)
    {
        LatLon location = getPointOfInterestPosition(i);
        t_point mylocation=LatLon_to_XY(location);
        //only the point of interests that is inside the screen will be presented
        if(screen_bound.intersects(mylocation)&&screen_bound.get_height()<=3315&&screen_bound.get_width()<=4640)
        {
            setcolor(RED);
            drawPosition (location);
                                t_point p1 = LatLon_to_XY(location);
                    setcolor(BLACK);
                    drawtext(p1, getPointOfInterestName(i), 1000, 100);
        //create different layer for the name of point of interests
            /*if(screen_bound.get_height()<=260&&screen_bound.get_width()<=363)
            {
                double distance=find_distance_closest_point_of_interest(i);

                if(distance>=30)
                {
                    t_point p1 = LatLon_to_XY(location);
                    setcolor(BLACK);
                    drawtext(p1, getPointOfInterestName(i), 1000, 100);
                }
                if((distance>=20&&distance<30)&&(screen_bound.get_height()<=158&&screen_bound.get_width()<=221))
                {
                    t_point p1 = LatLon_to_XY(location);
                    setcolor(BLACK);
                    drawtext(p1, getPointOfInterestName(i), 1000, 100); 
                }
                if((distance>=10&&distance<20)&&(screen_bound.get_height()<=95&&screen_bound.get_width()<=135))
                {
                    t_point p1 = LatLon_to_XY(location);
                    setcolor(BLACK);
                    drawtext(p1, getPointOfInterestName(i), 1000, 100); 
                }
                if(distance<10&&(screen_bound.get_height()<=56&&screen_bound.get_width()<=78))
                {
                    t_point p1 = LatLon_to_XY(location);
                    setcolor(BLACK);
                    drawtext(p1, getPointOfInterestName(i), 1000, 100); 
                }
            }*/
        }
    }
    
}

// chen,Peter
void drawIntersection()
{
    for(unsigned i = 0; i < getNumberOfIntersections(); i++)
    {
        LatLon location = getIntersectionPosition(i);
        const t_point mylocation=LatLon_to_XY(location);
        
        if(screen_bound.intersects(mylocation)&&screen_bound.get_height()<=3315&&screen_bound.get_width()<=4640)
        {
            //drawPosition (location);
            //draw the intersections only inside the screen
            if(screen_bound.get_height()<=260&&screen_bound.get_width()<=363)
            {
                double distance=find_distance_closest_intersection(i);
                string intersection_name=getIntersectionName(i);                
                //create different layer for intersection names
                if(distance>=40&&(screen_bound.get_height()>158&&screen_bound.get_width()>221))
                {
                    setfontsize(6);
                    t_point p1 = LatLon_to_XY(location);
                    setcolor(BLACK);
                    if(intersection_name!="(unknown)")
                        drawtext(p1, getIntersectionName(i), 1000, 100);
                }
                if(distance>=20&&(screen_bound.get_height()<=158&&screen_bound.get_width()<=221)&&(screen_bound.get_height()>95&&screen_bound.get_width()>135))
                {
                    setfontsize(8);
                    t_point p1 = LatLon_to_XY(location);
                    setcolor(BLACK);
                    if(intersection_name!="(unknown)")
                        drawtext(p1, getIntersectionName(i), 1000, 100); 
                }
                if(distance>=15&&(screen_bound.get_height()<=95&&screen_bound.get_width()<=135)&&(screen_bound.get_height()>58&&screen_bound.get_width()>82))
                {
                    setfontsize(10);
                    t_point p1 = LatLon_to_XY(location);
                    setcolor(BLACK);
                    if(intersection_name!="(unknown)")                    
                        drawtext(p1, getIntersectionName(i), 1000, 100); 
                }
                if(distance>0&&screen_bound.get_height()<=58&&screen_bound.get_width()<=82)
                {
                    setfontsize(12);
                    t_point p1 = LatLon_to_XY(location);
                    setcolor(BLACK);
                    if(intersection_name!="(unknown)")                    
                        drawtext(p1, getIntersectionName(i), 1000, 100); 
                }
                
            }
        }
    }
    
}

// chen, Peter, Zhang
void drawStreet()
{
    for(unsigned i = 0; i < getNumberOfStreetSegments(); i++)
    {
        setStreetSegmentColor(i);
        drawStreetSegment(i);
    }
}



void setStreetSegmentColor (unsigned i)
{
    if(getStreetSegmentSpeedLimit(i)<90)
    {
        if (getStreetSegmentStreetID(i)==38 ||getStreetSegmentStreetID(i)==233 ||getStreetSegmentStreetID(i)==196 ||getStreetSegmentStreetID(i)==138 ||getStreetSegmentStreetID(i)==133 ||getStreetSegmentStreetID(i)==281 ||getStreetSegmentStreetID(i)==194 ||getStreetSegmentStreetID(i)==390 || getStreetSegmentStreetID(i)==180 ||getStreetSegmentStreetID(i)==145 ||getStreetSegmentStreetID(i)==69 || getStreetSegmentStreetID(i)==202 ||getStreetSegmentStreetID(i)==1431 ||getStreetSegmentStreetID(i)==359 ||getStreetSegmentStreetID(i)==94 ||getStreetSegmentStreetID(i)==2956 ||getStreetSegmentStreetID(i)==284 || getStreetSegmentStreetID(i)==239)
        {   setlinewidth(5);
            setcolor(YELLOW);
        }
        else if ( getStreetName(getStreetSegmentStreetID(i)) == "(unknown)")
        {
            setlinewidth(2);
            setcolor(DARKGREY);
        }
        else
        {    
            setlinewidth(2);
            setcolor(WHITE);
        }
    }
    else //highway
    {
        setlinewidth(8);
        setcolor(ORANGE);
    }

    /*if(screen_bound.get_height() > 30000)
    {
        setlinewidth(1);
    }*/
}


// chen
void drawStreetSegment (unsigned i)
{
    long startID = getStreetSegmentEnds(i).from;
    long endID = getStreetSegmentEnds(i).to;
    t_point start = LatLon_to_XY(getIntersectionPosition(startID));
    t_point end = LatLon_to_XY(getIntersectionPosition(endID));

    //draw the street segments only inside the screen
    if((screen_bound.intersects(start)||screen_bound.intersects(end)))
    {
        LatLon point1 = getIntersectionPosition(startID);
        LatLon point2;

        for(unsigned j = 0; j < getStreetSegmentCurvePointCount(i); j++)
        {                     
            point2 = getStreetSegmentCurvePoint(i, j);
            drawSegment(point1, point2);
            point1 = point2;                  
        }
        point2 = getIntersectionPosition(endID);
        drawSegment(point1, point2);
    }
}


// chen
void drawSegment (LatLon from, LatLon to)
{
    t_point p1 = LatLon_to_XY(from);
    t_point p2 = LatLon_to_XY(to);
    drawline(p1, p2);
}

// chen
void drawPosition (LatLon location)
{
    t_point coord = LatLon_to_XY(location);
    
    drawellipticarc (coord.x, coord.y, RADIUS, RADIUS, 0, 360);
}

// chen
t_point LatLon_to_XY (LatLon location)
{
    t_point result;
    result.x = EARTH_RADIUS_IN_METERS * location.lon * DEG_TO_RAD * cos(AVG_LAT*DEG_TO_RAD);
    result.y = EARTH_RADIUS_IN_METERS * location.lat * DEG_TO_RAD;
    
    return result;
}

//Peter
////find closest point of interest, return min_distance
double find_distance_closest_point_of_interest(unsigned ID)
{
    LatLon location = getPointOfInterestPosition(ID);
    double min_distance = 100000000;

    for(unsigned long long i = 0; i < getNumberOfPointsOfInterest();i++)        
    {
        if(i!=ID)
        {
            LatLon otherslocation = getPointOfInterestPosition(i);
            t_point myotherlocation=LatLon_to_XY(otherslocation);
            if(screen_bound.intersects(myotherlocation)&&screen_bound.get_height()<=3315&&screen_bound.get_width()<=4640)
            {
                double distance=find_distance_between_two_points(location, otherslocation);

                if(distance < min_distance)
                {
                    min_distance = distance;
                }
            }
        }
    }

    return min_distance;
}

//Peter
//find closest intersection, return min_distance
double find_distance_closest_intersection(unsigned ID)
{
    LatLon location = getIntersectionPosition(ID);
    const t_point mylocation=LatLon_to_XY(location);
    double min_distance = 100000000;
    if(screen_bound.intersects(mylocation)&&screen_bound.get_height()<=3315&&screen_bound.get_width()<=4640)
    {
        for(unsigned long long i = 0; i < getNumberOfIntersections();i++)        
        {
            if(i!=ID)
            {
                LatLon otherslocation = getIntersectionPosition(i);
                t_point myotherlocation=LatLon_to_XY(otherslocation);
                if(screen_bound.intersects(myotherlocation)&&screen_bound.get_height()<=3315&&screen_bound.get_width()<=4640)
                {
                    double distance=find_distance_between_two_points(location, otherslocation);       
                    if(distance < min_distance)
                    {
                        min_distance = distance;
                    }
                }
            }
        }
        return min_distance;
    }
    else
        return 0;
}

float find_street_angle(unsigned ID)
{
    float angle;
    StreetSegmentEnds endpoints=getStreetSegmentEnds(ID);
    unsigned long from=endpoints.from;
    unsigned long to=endpoints.to;
    t_point firstpoint=LatLon_to_XY(getIntersectionPosition(from));
    t_point secondpoint=LatLon_to_XY(getIntersectionPosition(to));
    if((secondpoint.x-firstpoint.x) == 0)
        return 0;
    angle=atan((secondpoint.y-firstpoint.y)/(secondpoint.x-firstpoint.x))*180/M_PI;
    //cout<<"xx"<<angle<<endl;
    return angle;
}




void draw_street_name()
{
    setcolor(BLACK);
    for(auto i = init_map.segments.begin(); i != init_map.segments.end(); i++)
    {
        for(auto j=i->second.begin()+1;j!=i->second.end();j++)
        {    
            long startID = getStreetSegmentEnds(*j).from;
            long endID = getStreetSegmentEnds(*j).to;
            t_point start = LatLon_to_XY(getIntersectionPosition(startID));
            t_point end = LatLon_to_XY(getIntersectionPosition(endID));
            string segment_name=getStreetName(getStreetSegmentStreetID(*j));
            
            
            if(screen_bound.get_height()<=166 && screen_bound.get_width()<=156)
                setfontsize(11);
            else
                setfontsize(8);
            
            t_bound_box text_bound(start, end);
            text_bound = adjust_t_bound_box(text_bound);
            //draw the street segments only inside the screen
            if((screen_bound.intersects(start)||screen_bound.intersects(end)) && segment_name != "(unknown)")
            {
                unsigned middle_point=getStreetSegmentCurvePointCount(*j)/2;
                float angle=find_street_angle(*j);
                settextrotation(angle);
                
                if(getStreetSegmentCurvePointCount(*j)==0)
                {
                    t_point mid_coord;
                    mid_coord.x=(start.x+end.x)/2;
                    mid_coord.y=(start.y+end.y)/2;
                    drawtext(mid_coord, segment_name, 100, 100);
                   if (text_bound.intersects(mid_coord))
                        drawtext(mid_coord, segment_name, text_bound);
                }
                else
                {
                    LatLon mid_latlon=getStreetSegmentCurvePoint(*j,middle_point);
                    t_point mid_coord=LatLon_to_XY(mid_latlon);
                    drawtext(mid_coord, segment_name, 100, 100);
                    if (text_bound.intersects(mid_coord))
                        drawtext(mid_coord, segment_name, text_bound);
                }
            }
        }
    }
}

t_bound_box adjust_t_bound_box(t_bound_box origin)
{
    float height=origin.get_height();
    float width=origin.get_width();
    t_point centre=origin.get_center();
    float edge;
    if(height>width) 
        edge=height;
    else 
        edge=width;
    edge=edge/2;
    return t_bound_box(centre.x-edge,centre.y-edge, centre.x+edge,centre.y+edge);// left, bottom, right, top  
}


int setStreetWidth()
{
    return 50 / sqrt(screen_bound.get_height());
}
