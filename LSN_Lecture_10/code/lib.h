#include<iostream>
#include<vector>
#include <cmath>
#include "city.h"
#include "random.h"


void set_config_circ(int ncities, vector<city>& map, Random& rnd){
    for (int i = 0; i < ncities; i++){
        double theta = rnd.Rannyu(0, 2*M_PI); 
        map[i].x = cos(theta);                
        map[i].y = sin(theta);                
    }
}
void set_config_square(int ncities, vector<city>& map, Random& rnd){
    for (int i = 0; i<ncities; i++){
        map[i].x = rnd.Rannyu();
        map[i].y = rnd.Rannyu();            
    }
}