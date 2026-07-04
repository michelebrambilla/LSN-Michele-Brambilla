#ifndef RANDOMWALKER_H
#define RANDOMWALKER_H

#include <vector>
#include <string>
#include <cmath>
#include <fstream>   
#include <iostream>  


class RandomWalker {
private:
    int L;                     // Quanti step compongono un singolo random walk
    int current_steps;         // Contatore per sapere a che punto del random walk siamo
    std::vector<double> position; // Posizione a ogni step
    double s;
    int dimensions;

public:
    // Costruttore
    RandomWalker(int walk_length, double step_length, int d){

        L = walk_length;
        current_steps = 0;
        s = step_length;
        dimensions = d;
        position.assign(dimensions, 0.0);

    }

    // Metodi pubblici

    void reset() {
        std::fill(position.begin(), position.end(), 0.0);
        current_steps = 0;
    }

    void newstep(int i, int sign){
    
        position[i] += sign*s;
    }

    void newcontinuumstep(double phi, double theta){
        position[0] += cos(phi)*sin(theta)*s;
        position[1] += sin(phi)*sin(theta)*s;
        position[2] += cos(theta)*s;

    }

    double get_squared_distance(){
        double dist2 = 0.0;
        for(int i = 0; i < dimensions; i++) {
            dist2 += position[i] * position[i];
        }
        return dist2;
    }

    void print_position(std::ofstream& out) const {
        for(int i = 0; i < dimensions; i++) {
            out << position[i];
            if (i < dimensions - 1) out << "\t";
        }
        out << "\n";
    }
    
};

#endif // RANDOMWALKER_H