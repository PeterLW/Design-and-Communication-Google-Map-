#include <iostream>
#include <unittest++/UnitTest++.h>
#include <string>

#include "m2.h"
#include "m4.h"

using namespace std;

int main(int argc, char* argv[]) 
{
    cout << argv[1] << endl;
    string map_path = "/cad2/ece297s/public/maps/" + (string)argv[1] + ".bin";
    
    draw_map(map_path);
    
    return 0;
}