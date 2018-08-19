#include "m4.h"

using namespace std;
vector<vector<double>> interInterDistance;
vector<vector<double>> depotInterDistance;
vector<vector<double>> interDepotDistance;
unsigned startDepot;
unsigned endDepot;

bool found;

std::vector<unsigned> traveling_salesman
    (std::vector<unsigned> intersections_to_traverse, std::vector<unsigned> depot_locations)
{
    init(intersections_to_traverse, depot_locations);
    /*vector<unsigned> result=three_opt(test);
    cout<<result[0]<<"|"<<result[1]<<"|"<<result[2]<<endl;
    for(auto i=result.begin(); i!=result.end();i++) {
        
        cout <<*i<< "|" << endl;
    
        cout<< endl;
    }*/
    vector<unsigned> pathIndex = multiStart(intersections_to_traverse.size(), depot_locations.size());
    cerr << "Final path: " << endl;
    printPath(pathIndex);
    cerr << getPathDistance(pathIndex) << endl;
    return getPath(pathIndex, intersections_to_traverse, depot_locations);
}

// Chen
vector<unsigned> multiStart(unsigned deliverySize, unsigned depotSize)
{
    vector<unsigned> bestPathIndex;
    vector<unsigned> pathIndex;
    double bestPathDistance = MAXNUM;
    unsigned bestStartDepot = 0;
    unsigned bestEndDepot = 0;
    
    for(unsigned i = 0; i < depotSize; i++)
    {
        startDepot = i;
        pathIndex = greedy(startDepot, deliverySize);
        //pathIndex = multiTwoSwap(pathIndex);
        //pathIndex = two_opt(pathIndex);
        //pathIndex = three_opt(pathIndex);
        
        pathIndex = iterative(pathIndex);
        //pathIndex = hillClimbing(pathIndex);
        //pathIndex = iterativeHill(pathIndex);
        
        // update
        endDepot = findClosestDest(pathIndex.back(), interDepotDistance);
        double pathDistance = getPathDistance(pathIndex);
        if(pathDistance < bestPathDistance)
        {
            bestStartDepot = startDepot;
            bestEndDepot = endDepot;
            bestPathDistance = pathDistance;
            bestPathIndex = pathIndex;
        }
        //printPath(pathIndex);
        //cerr << pathDistance << endl;
    }
    
    startDepot = bestStartDepot;
    endDepot = bestEndDepot;
    
    return bestPathIndex;
}

// Chen
vector<unsigned> iterativeHill(std::vector<unsigned> pathIndex)
{
    endDepot = findClosestDest(pathIndex.back(), interDepotDistance);
    double pathDistance = getPathDistance(pathIndex);
    bool done = false;
    while (!done)
    {
        double old = pathDistance;
        pathIndex = hillClimbing(pathIndex);
        endDepot = findClosestDest(pathIndex.back(), interDepotDistance);
        pathDistance = getPathDistance(pathIndex);

        if(pathDistance < old)
            old = pathDistance;
        else
            done = true;
        //cerr << pathDistance << endl;
    }

    return pathIndex;
}

// Chen
vector<unsigned> iterative(std::vector<unsigned> pathIndex)
{
    endDepot = findClosestDest(pathIndex.back(), interDepotDistance);
    double pathDistance = getPathDistance(pathIndex);
    bool done = false;
    while (!done)
    {
        double old = pathDistance;
        pathIndex = three_opt(pathIndex); // improvement
        endDepot = findClosestDest(pathIndex.back(), interDepotDistance);
        pathDistance = getPathDistance(pathIndex);

        if(pathDistance < old)
            old = pathDistance;
        else
            done = true;
        //cerr << pathDistance << endl;
    }

    return pathIndex;
}

// Chen
vector<unsigned> hillClimbing(std::vector<unsigned> pathIndex)
{
    vector<unsigned> result = pathIndex;
    unsigned size = pathIndex.size();
    endDepot = findClosestDest(pathIndex.back(), interDepotDistance);
    double bestPathDistance = getPathDistance(pathIndex);
    
    for(unsigned i = 0; i < size; i++)
    {
        for(unsigned j = i+1; j < size; j++)
        {
            twoSwap(pathIndex, i, j);
            
            vector<unsigned> newPathIndex = iterative(pathIndex);
            endDepot = findClosestDest(newPathIndex.back(), interDepotDistance);
            double newPathDistance = getPathDistance(newPathIndex);
            if(newPathDistance < bestPathDistance)
            {
                result = newPathIndex;
                bestPathDistance = newPathDistance;
                //cerr << bestPathDistance << endl;
            }
            
            twoSwap(pathIndex, i, j);
        }
    }
    return result;
}

// Chen
vector<unsigned> multiTwoSwap(std::vector<unsigned> pathIndex)
{
    vector<unsigned> result = pathIndex;
    unsigned size = pathIndex.size();
    double bestPathDistance = getPathDistance(pathIndex);
    //printPath(pathIndex);
    //cerr << bestPathDistance << endl;
    
    for(unsigned i = 0; i < size; i++)
    {
        for(unsigned j = i+1; j < size; j++)
        {
            twoSwap(pathIndex, i, j);
            //if(j == size-1)
                endDepot = findClosestDest(pathIndex.back(), interDepotDistance);
            double newPathDistance = getPathDistance(pathIndex);
            
            //printPath(pathIndex);
            //cerr << newPathDistance << endl;
            
            if (newPathDistance < bestPathDistance)
            {
                result = pathIndex;
                bestPathDistance = newPathDistance;
            }
            
            twoSwap(pathIndex, i, j);
        }
    }
    return result;
}

// Chen
void twoSwap (std::vector<unsigned> &pathIndex, unsigned i, unsigned j)
{
    unsigned temp = pathIndex[i];
    pathIndex[i] = pathIndex[j];
    pathIndex[j] = temp;
}

// Chen
vector<unsigned> multiSwap(std::vector<unsigned> pathIndex)
{
    vector<unsigned> result = pathIndex;
    unsigned size = pathIndex.size();
    double bestPathDistance = getPathDistance(pathIndex);
    //printPath(pathIndex);
    //cerr << bestPathDistance << endl;
    
    for(unsigned i = 0; i < size; i++)
    {
        for(unsigned j = i+1; j < size; j++)
        {
            for(unsigned k = j+1; k < size; k++)
            {
                for(unsigned l = k+1; l < size; l++)
                {
                    vector<unsigned> index = {i,j,k,l};
                    double old = bestPathDistance;
                    vector<unsigned> newPathIndex = swap(bestPathDistance, pathIndex, index);
                    
                    if(bestPathDistance < old)
                        result = newPathIndex;
                }
            }
        }
    }
    return result;
}

// Chen
vector<unsigned> swap(double &bestPathDistance, std::vector<unsigned> &pathIndex, vector<unsigned> &index)
{
    vector<unsigned> result = pathIndex;
    unsigned size = index.size();
    
    for(unsigned i = 0; i < size; i++)
    {
        for(unsigned j = i+1; j < size; j++)
        {
            twoSwap(pathIndex, index[i], index[j]);
            //if(j == size-1)
                endDepot = findClosestDest(pathIndex.back(), interDepotDistance);
            double newPathDistance = getPathDistance(pathIndex);
            
            //printPath(pathIndex);
            //cerr << newPathDistance << endl;
            
            if (newPathDistance < bestPathDistance)
            {
                result = pathIndex;
                bestPathDistance = newPathDistance;
            }
            twoSwap(pathIndex, index[i], index[j]);
        }
    }
    
    return result;
}

// Chen
vector<unsigned> greedy(unsigned startDepot, unsigned size)
{
    vector<unsigned> pathIndex;
    vector<unsigned> pathIndexLeft;
    unsigned destIndex;
    unsigned current = startDepot;
    for(unsigned i = 0; i < size; i++)
        pathIndexLeft.push_back(i);
    
    destIndex = findClosestDest(current, pathIndexLeft, depotInterDistance);
    pathIndex.push_back(destIndex);
    pathIndexLeft.erase(find(pathIndexLeft.begin(), pathIndexLeft.end(), destIndex));
    current = destIndex;
    
    while(!pathIndexLeft.empty())
    {
        destIndex = findClosestDest(current, pathIndexLeft, interInterDistance);
        pathIndex.push_back(destIndex);
        pathIndexLeft.erase(find(pathIndexLeft.begin(), pathIndexLeft.end(), destIndex));
        current = destIndex;
    }
    
    return pathIndex;
}

// Chen
unsigned findClosestDest(unsigned currentIndex, std::vector<unsigned> &destIndexLeft, 
        std::vector<vector<double>> &distance)
{
    unsigned best = destIndexLeft[0];
    double bestDistance = distance[currentIndex][destIndexLeft[0]];
    
    for(unsigned i = 1; i < destIndexLeft.size(); i++)
    {
        unsigned dest = destIndexLeft[i];
        double newDistance = distance[currentIndex][dest];
        if(newDistance < bestDistance)
        {
            bestDistance = newDistance;
            best = destIndexLeft[i];
        }
    }
    
    return best;
}

// Chen
unsigned findClosestDest(unsigned currentIndex, std::vector<vector<double>> &distance)
{
    unsigned best = 0;
    double bestDistance = distance[currentIndex][0];
    
    for(unsigned i = 1; i < distance[currentIndex].size(); i++)
    {
        double newDistance = distance[currentIndex][i];
        if(newDistance < bestDistance)
        {
            bestDistance = newDistance;
            best = i;
        }
    }
    
    return best;
}

// Chen
void init(std::vector<unsigned> &intersections_to_traverse, std::vector<unsigned> &depot_locations)
{
    interInterDistance = findAllDistance(intersections_to_traverse, intersections_to_traverse);
    depotInterDistance = findAllDistance(depot_locations, intersections_to_traverse);
    interDepotDistance = findAllDistance(intersections_to_traverse, depot_locations);
    /*
    vector<unsigned> interDepot;
    unsigned interSize = intersections_to_traverse.size();
    unsigned depotSize = depot_locations.size();

    interDepot.reserve( interSize + depotSize );
    interDepot.insert( interDepot.end(), intersections_to_traverse.begin(), intersections_to_traverse.end() );
    interDepot.insert( interDepot.end(), depot_locations.begin(), depot_locations.end() );

    
    thread threadDI(findDepotInterDistance, intersections_to_traverse, interDepot);
    vector<vector<double>> interDistance = findAllPathLen(intersections_to_traverse, interDepot);
    threadDI.join ();
    
    for(unsigned i = 0; i < interDistance.size(); i++)
    {
        for(unsigned j = 0; j < interSize; j++)
            interInterDistance[i][j] = interDistance[i][j];
        for(unsigned j = interSize; j < interDistance[i].size(); j++)
            interDepotDistance[i][j-interSize] = interDistance[i][j];
    }*/
    cerr << "initialized" << endl;
}

void findDepotInterDistance(std::vector<unsigned> intersections_to_traverse, std::vector<unsigned> depot_locations)
{
    cerr << "threading" << endl;
    depotInterDistance = findAllPathLen(depot_locations, intersections_to_traverse);
}

// Chen
std::vector<std::vector<double>> findAllPathLen (std::vector<unsigned> a, std::vector<unsigned> b)
{
    unsigned aSize = a.size();
    unsigned bSize = b.size();
    vector<std::vector<double>> distanceBFS(aSize);
    
    for(unsigned i = 0; i < aSize; i++)
    {
        cerr << i << endl;
        distanceBFS[i] = getDistance(a[i], b);
        for(unsigned j = 0; j < bSize; j++)
        {
            cerr << distanceBFS[i][j] << " ";
        }
        cerr << endl;
    }
    //cerr << endl;
    return distanceBFS;
}

// Chen
std::vector<std::vector<double>> findAllDistance (std::vector<unsigned> &a, std::vector<unsigned> &b)
{
    unsigned aSize = a.size();
    unsigned bSize = b.size();
    vector<std::vector<double>> distance(aSize);
    
    for(unsigned i = 0; i < aSize; i++)
    {
        for(unsigned j = 0; j < bSize; j++)
        {
            distance[i].push_back(find_distance_between_two_points
                (getIntersectionPosition(a[i]), getIntersectionPosition(b[j])));
        }
    }
    return distance;
}

// Chen
void printPath(std::vector<unsigned> pathIndex)
{
    cerr << startDepot << "| ";
    for (unsigned i = 0; i < pathIndex.size(); i++) 
        cerr << pathIndex[i] << " ";
    cerr << "|" << endDepot << endl;
}

// Chen
std::vector<unsigned> getPath(std::vector<unsigned> pathIndex, std::vector<unsigned> &intersections_to_traverse, std::vector<unsigned> &depot_locations)
{
    vector<unsigned> wholePath;
    vector<unsigned> path;
    unsigned interSize = pathIndex.size();
    
    wholePath = find_path_between_intersections (depot_locations[startDepot], intersections_to_traverse[pathIndex[0]]);
    
    for (unsigned i = 0; i < interSize-1; i++) 
    {
        path = find_path_between_intersections (intersections_to_traverse[pathIndex[i]], intersections_to_traverse[pathIndex[i+1]]);
        wholePath.insert(wholePath.end(), path.begin(), path.end());
    }
    
    path = find_path_between_intersections (intersections_to_traverse[pathIndex[interSize-1]], depot_locations[endDepot]);
    wholePath.insert(wholePath.end(), path.begin(), path.end());
    
    return wholePath;
}

// Chen
double getPathDistance(std::vector<unsigned> &pathIndex)
{
    double sum;
    unsigned interSize = pathIndex.size();
    
    sum = depotInterDistance[startDepot][pathIndex[0]];
    for (unsigned i = 0; i < interSize-1; i++) 
        sum += interInterDistance[pathIndex[i]][pathIndex[i+1]];
    sum += interDepotDistance[pathIndex[interSize-1]][endDepot];
    
    return sum;
}



// Lin
std::vector<unsigned> two_opt(vector<unsigned> init_path)
{
    
    double init_time=getPathDistance(init_path);//cost
    double new_time;
    //for the return value of function check_path_valid
    vector<unsigned>  new_path=init_path;
    //final return path
    vector<unsigned>  final_path=init_path;
    //how many intersection in the init_path
    int size_of_path=init_path.size();
    
    for(int i=1; i< size_of_path-1; i++)
    {
        //switch 2 street segment
        unsigned temp=init_path[i];
        for(int j=i+1; j < size_of_path-1; j++){
            new_path[i]=new_path[j];
            new_path[j]=temp;
            new_time=getPathDistance(new_path);
            if(new_time<=init_time)
            {
                init_time=new_time;
                final_path=new_path;
                //cout<<"final: "<<final_path[0]<<" "<<final_path[1]<<" "<<final_path[2]<<endl;
            }
            new_path=init_path;
        }
    }   
    
    return final_path;
}


std::vector<unsigned> three_opt(vector<unsigned> &init_path)
{
    
    double init_time=getPathDistance(init_path);//cost
    double new_time;
    //for comparison in below loop
    vector<unsigned>  new_path=init_path;
    //final return path
    vector<unsigned>  final_path=init_path;
    //how many intersection in the init_path
    int size_of_path=init_path.size();
    //for(int b=0; b<=1; b++)
    //{
    /*for(int a=-1; a<size_of_path-1; a++)
    {
        if(a!=-1)
        {
            unsigned endpoint=new_path[size_of_path-1];
            new_path[size_of_path-1]=new_path[a];
            new_path[a]=endpoint;
        }*/
        for(int i=1; i< size_of_path-1; i++)
        {
            //switch 2 street segment
            unsigned temp=init_path[i];
            for(int j=i+1; j < size_of_path-1; j++){

                new_path[i]=new_path[j];
                new_path[j]=temp;
                int extra_couple=(j-i-1)/2;
                if(extra_couple>0)
                {
                    for(int k=1; k<=extra_couple; k++)
                    {
                        unsigned temp2=new_path[i+k];
                        new_path[i+k]=new_path[j-k];
                        new_path[j-k]=temp2;
                    }
                }
                new_time=getPathDistance(new_path);
                if(new_time<=init_time)
                {
                    init_time=new_time;
                    final_path=new_path;
                }
                new_path=init_path;
            }
        }   
    //}
    return final_path;
//}
}
// Lin
vector<unsigned> check_path_valid(vector<reach_node> path, int size_of_path)
{
    vector<unsigned> new_path;
    bool valid[size_of_path];
    
    //initialize valid, same size as path, used as flag to check if every node 
    //in the path is visited
    for(int j=0; j<size_of_path; j++) 
        valid[j]=0;  
    
    int i=0;
    valid[i]=1;
    new_path.push_back(path[0].index);
    while(path[i].index!=path[size_of_path-1].index)
    {
        if(valid[path[i].reach_to]==0)
        {
            i=path[i].reach_to;
            //create new path index
            new_path.push_back(path[i].index);
            valid[i]=1;
        }
        else
        {
            i=path[i].reach_from; 
            new_path.push_back(path[i].index);
            valid[i]=1;
        }
        i++;
    }
    //if there is one node is not visited, (global variable)found=false
    for(int j=0; j<size_of_path; j++)
    {
        if(valid[j]==0)
            found=false;
    }
    return new_path;  
}

// Lin
vector<reach_node> path_reaching_node(vector<unsigned> old_path, int size_of_path)
{
    vector<reach_node> path_reach_nodes; 
    for(int i=0; i<size_of_path; i++)
    {
        if(i==0)
        {
            reach_node nodes(-1,i+1,i);//(reach_from, reach_to, index)
            path_reach_nodes.push_back(nodes);
        }
        else if(i==size_of_path-1)
        {
            reach_node nodes(i-1,-1,i);
            path_reach_nodes.push_back(nodes);            
        }
        else
        {
            reach_node nodes(i-1,i+1,i);
            path_reach_nodes.push_back(nodes); 
        }
    }
    return path_reach_nodes;
}

// Lin
double total_traval_time(vector<unsigned> init_path)
{
    double total_time=0;
    for(unsigned i=0; i<init_path.size()-1; i++)
    {
        vector<unsigned> temp_inter=find_path_between_intersections(init_path[i], init_path[i+1]);
        total_time+=compute_path_travel_time(temp_inter);
    }
    return total_time;
}
