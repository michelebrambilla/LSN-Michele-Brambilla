
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
   
   
   int ncities = 110;
   int npaths = 100;
   int n_generations = 10000; 

   int nmigrations = 100;

   int selected = int(npaths/size);

   vector<city> map(ncities);


    if (rank == 0) {
    ifstream input_file("cap_prov_ita.dat");
    if (!input_file.is_open()) {
        cerr << "ERRORE: Impossibile aprire cap_prov_ita.dat" << endl;
    }
    for (int i = 0; i < ncities; i++) {
        input_file >> map[i].x >> map[i].y;
    }
    input_file.close();
    cout << "Mappa caricata: 110 capoluoghi italiani." << endl;
    }

    vector<double>map_coords(ncities * 2);
    if (rank == 0) {
        for(int i=0; i<ncities; i++){
           map_coords[2*i] = map[i].x;
           map_coords[2*i+1] = map[i].y; 
        }
    }
    
    MPI_Bcast(&map_coords[0], ncities * 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    

    if (rank != 0) {
        for(int i=0; i<ncities; i++){
           map[i].x =map_coords[2*i];
           map[i].y =map_coords[2*i+1];
        }
    }


    population pop(rnd);
    pop.initialize(npaths, map, ncities);

    string personal_filename = "migrations_type2_metrics_core_" + to_string(rank) + ".dat";

    ofstream clean_file(personal_filename); 
    clean_file.close();

    //ofstream clean_file("metrics.dat"); clean_file.close();


    for(int i = 0; i < n_generations; i++) {
        pop.evolve(ncities);
        pop.sort(); 
        //pop.save_metrics(i, "metrics.dat");

        pop.save_metrics(i, personal_filename);


        if(i%nmigrations == 0 && i != 0){
            pop.migrate_best(rank, size, 1);
        if(rank == 0)
            cout << "Migrazione compiuta alla generazione numero " << i << endl;
        }

        if(rank == 0 && i%1000 == 0)
        cout << "Generazione " << i << " completata." << endl;


    }

    pop.final_migration(rank, size, selected);

    
    if (rank == 0) {
        cout << "Eden formato! Inizio il rush evolutivo finale..." << endl;
        
        ofstream clean_file("migrations_type2_metrics_eden.dat"); clean_file.close();

        int eden_generations = 200;
        for(int i = 0; i < eden_generations; i++) {
            pop.evolve(ncities);
            pop.sort();
            pop.save_metrics(i, "migrations_type2_metrics_eden.dat");
        }

        pop.save_best_path(map, "migration_type2_best_path.dat");
        cout << "L'evoluzione mondiale e' terminata. Percorso ottimale salvato!" << endl;
    }

    MPI_Finalize();
    return 0;


}