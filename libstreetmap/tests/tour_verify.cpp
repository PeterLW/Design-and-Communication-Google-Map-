#include <vector>
#include <set>
#include <algorithm>
#include <cassert>
#include "StreetsDatabaseAPI.h"

#include "tour_verify.h"

namespace ece297test {

bool tour_is_legal(const std::vector<unsigned> delivery_ids, const std::vector<unsigned> depot_ids, const std::vector<unsigned> tour) {
    //'depot_ids' is the collection of possible start/end intersection ids
    //'delivery_ids' is the collection of intersection ids that must be traversed by the tour
    //'tour' is a vector street segment id's
    
    //The problem spec guarantees that these are non-empty
    assert(depot_ids.size() > 0);
    assert(delivery_ids.size() > 0);
    
    if (tour.size() < 1) {
        return false; //If it is a valid tour it must contain at-least one segment

    } else {
        //General case
        //We verify the tour by walking along each segment checking:
        //  * That we started at a depot intersection
        //  * That we are going a legal direction (i.e. not against one-ways)
        //  * That each segment is connected to the previous intersection
        //  * We also record the delivery intersections visited
        //At the end we verify:
        //  * That we visited every delivery intersection
        //  * That we ended at a depot intersection
        assert(tour.size() >= 1);
        
        //Initialize some useful sets for simpler semantics
        std::set<unsigned> depots(depot_ids.begin(), depot_ids.end());
        std::set<unsigned> deliveries(delivery_ids.begin(), delivery_ids.end());
        
        //The problem spec guaranteed no duplicates in depots and delivery 
        //locations
        assert(depots.size() == depot_ids.size()); //No overlaps among depots
        assert(deliveries.size() == delivery_ids.size()); //No overlaps among deliveries
        
        //Check the intersection of depots and deliveries to verify no shared locations
        std::set<unsigned> shared_depot_deliveries;
        std::set_intersection(depots.begin(), depots.end(), 
                              deliveries.begin(), deliveries.end(), 
                              std::inserter(shared_depot_deliveries, shared_depot_deliveries.begin()));
        assert(shared_depot_deliveries.size() == 0); //No duplicates across depots and deliveries
        
        //Track deliveries that have been completed
        std::set<unsigned> completed_deliveries; //Initially empty
        

        unsigned curr_intersection = 0; //Where we are currently located, initialized by first segment
        for (int i = 0; i < (int) tour.size(); i++) {
            StreetSegmentEnds seg_endpoints = getStreetSegmentEnds(tour[i]);

            
            if(i == 0) {
                //Figure out out starting intersection by looking for a depot 
                //connected to this segment
                
                //If this assertion fails, then the segment is connected to two depots. We 
                //don't know what way to go on the first segment, so we can't
                //verify the path
                assert(depots.find(seg_endpoints.to) == depots.end() || depots.find(seg_endpoints.from) == depots.end());
               
                //Typical cases, one or zero depot connects
                if(depots.find(seg_endpoints.to) != depots.end()) {
                    //Starting at 'to'
                    curr_intersection = seg_endpoints.to;
                } else if(depots.find(seg_endpoints.from) != depots.end()) {
                    //Starting at 'from'
                    curr_intersection = seg_endpoints.from;
                } else {
                    //First segment not connected to a valid depot
                    return false;
                }
            }
            
            //Check whether the current intersection is a delivery point
            if(deliveries.find(curr_intersection) != deliveries.end()) {
                //It is a delivery point, so mark it as completed
                completed_deliveries.insert(curr_intersection);
            }
            

            //Are we moving forward or back along the segment?
            bool seg_traverse_forward;
            if (seg_endpoints.to == curr_intersection) {
                //Moving backwards
                seg_traverse_forward = false;

            } else if (seg_endpoints.from == curr_intersection) {
                //Moving forwards
                seg_traverse_forward = true;

            } else {
                //This segment isn't connected to the current intersection
                return false;
            }

            //Are we going the wrong way along the segment?
            if (!seg_traverse_forward && getStreetSegmentOneWay(tour[i])) {
                return false;
            }

            //Advance to the next intersection
            curr_intersection = (seg_traverse_forward) ? seg_endpoints.to : seg_endpoints.from;

        }

        //Finished tour, we should be at a valid depot
        if (depots.find(curr_intersection) == depots.end()) {
            //Not at a depot
            return false;
        }
        
        //We should have visited every delivery intersection
        for(unsigned deliver_id : delivery_ids) {
            if(completed_deliveries.find(deliver_id) == completed_deliveries.end()) {
                //Did not visit this delivery intersection
                return false;
            }
        }
    }

    //Everything validated
    return true;
}

}