
#include "population.h"
#include "random.h"
#include "chromosome.h"
#include "city.h"
#include "lib.h"

#include "mpi.h"


#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <vector>

using namespace std;

int main(int argc, char* argv[]){

    MPI_Init(&argc,&argv);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Random rnd;
    int seed[4];
    int p1, p2;
    ifstream Primes("Primes");
    if (Primes.is_open()){
      for(int i = 0; i <= rank; i++) {
            Primes >> p1 >> p2;
        }
    } else cerr << "PROBLEM: Unable to open Primes" << endl;
    Primes.close();

    ifstream input("seed.in");
    string property;
    if (input.is_open()){
      while ( !input.eof() ){
         input >> property;
         if( property == "RANDOMSEED" ){
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            rnd.SetRandom(seed,p1,p2);
         }
      }
      input.close();
    } else cerr << "PROBLEM: Unable to open seed.in" << endl;
      rnd.SaveSeed();
   
   
   int ncities = 34;
   int npaths = 100;
   int n_generations = 10000; 

   int nmigrations = 100;

   int selected = int(npaths/size);


    vector<city> circle_map(ncities);
    vector<city> square_map(ncities);

    if (rank == 0) {
    set_config_square(ncities, square_map, rnd);
    set_config_circ(ncities, circle_map, rnd);
    }

    vector<double> circle_map_coords(ncities * 2);
    if (rank == 0) {
        for(int i=0; i<ncities; i++){
            circle_map_coords[2*i] = circle_map[i].x;
            circle_map_coords[2*i+1] = circle_map[i].y; 
        }
    }
    
    MPI_Bcast(&circle_map_coords[0], ncities * 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    

    if (rank != 0) {
        for(int i=0; i<ncities; i++){
            circle_map[i].x = circle_map_coords[2*i];
            circle_map[i].y = circle_map_coords[2*i+1];
        }
    }


    population pop(rnd);
    pop.initialize(npaths, circle_map, ncities);

    string personal_filename = "circle_metrics_core_" + to_string(rank) + ".dat";

    ofstream clean_file(personal_filename); 
    clean_file.close();


    for(int i = 0; i < n_generations; i++) {
        pop.evolve(ncities);
        pop.sort(); 
        //pop.save_metrics(i, "metrics.dat");

        pop.save_metrics(i, personal_filename);

        if(i%nmigrations == 0 && i != 0)
        pop.migrate_best(rank, size, 1);

    }

    pop.final_migration(rank, size, selected);

    
    if (rank == 0) {
        cout << "Eden formato! Inizio il rush evolutivo finale..." << endl;
        
        ofstream clean_file("circle_metrics_eden.dat"); clean_file.close();

        int eden_generations = 200;
        for(int i = 0; i < eden_generations; i++) {
            pop.evolve(ncities);
            pop.sort();
            pop.save_metrics(i, "circle_metrics_eden.dat");
        }

        pop.save_best_path(circle_map, "circle_best_path.dat");
        cout << "L'evoluzione mondiale e' terminata. Percorso ottimale salvato!" << endl;
    }




    vector<double> square_map_coords(ncities * 2);
    if (rank == 0) {
        for(int i=0; i<ncities; i++){
            square_map_coords[2*i] = square_map[i].x;
            square_map_coords[2*i+1] = square_map[i].y; 
        }
    }
    

    MPI_Bcast(&square_map_coords[0], ncities * 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    

    if (rank != 0) {
        for(int i=0; i<ncities; i++){
            square_map[i].x = square_map_coords[2*i];
            square_map[i].y = square_map_coords[2*i+1];
        }
    }


    pop.initialize(npaths, square_map, ncities);

    personal_filename = "square_metrics_core_" + to_string(rank) + ".dat";

    ofstream square_file(personal_filename); 
    square_file.close();


    for(int i = 0; i < n_generations; i++) {
        pop.evolve(ncities);
        pop.sort(); 

        pop.save_metrics(i, personal_filename);
        
        if(i%nmigrations == 0 && i !=0)
        pop.migrate_best(rank, size, 1);
        
        
        if (i % 500 == 0) {
            cout << "Generazione " << i << " completata." << endl;
        }
    }
    
    pop.final_migration(rank, size, selected);

    
    if (rank == 0) {
        cout << "Eden formato! Inizio il rush evolutivo finale..." << endl;
        
        ofstream clean_file("square_metrics_eden.dat"); clean_file.close();

        int eden_generations = 200;
        for(int i = 0; i < eden_generations; i++) {
            pop.evolve(ncities);
            pop.sort();
            pop.save_metrics(i, "square_metrics_eden.dat");
        }

        pop.save_best_path(square_map, "square_best_path.dat");
        cout << "L'evoluzione mondiale e' terminata. Percorso ottimale salvato!" << endl;
    }

    MPI_Finalize();
    return 0;


}