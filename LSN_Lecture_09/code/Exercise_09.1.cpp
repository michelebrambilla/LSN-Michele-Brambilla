
#include "population.h"
#include "random.h"
#include "chromosome.h"
#include "city.h"
#include "lib.h"


#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <vector>

using namespace std;

int main(){

   Random rnd;
   int seed[4];
   int p1, p2;
   ifstream Primes("Primes");
   if (Primes.is_open()){
      Primes >> p1 >> p2 ;
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
   int n_generations = 90000; 

    //Scommentare e sostituire il nome della mappa in uno dei due TSP analyzers seguenti per avere confronto con computazione parallela.
    //Modificare n_generations e/o _npaths!

    vector<city> map(ncities);


    ifstream input_file("cap_prov_ita.dat");
    if (!input_file.is_open()) {
        cerr << "ERRORE: Impossibile aprire cap_prov_ita.dat" << endl;
    }
    for (int i = 0; i < ncities; i++) {
        input_file >> map[i].x >> map[i].y;
    }
    input_file.close();
    cout << "Mappa caricata: 110 capoluoghi italiani." << endl;



   //vector<city> circle_map(ncities);
   vector<city> square_map(ncities);

   set_config_square(ncities, square_map, rnd);
   //set_config_circ(ncities, circle_map, rnd);


    population pop(rnd);
    pop.initialize(npaths, map, ncities);

    // Puliamo i file di output prima di iniziare
    ofstream clean_file("non_parallel2_metrics.dat"); clean_file.close();


    for(int i = 0; i < n_generations; i++) {
        pop.evolve(ncities);
        pop.sort(); // Assicuriamoci che il migliore sia in posizione 0
        
        // Salviamo la lunghezza migliore e la media
        pop.save_metrics(i, "non_parallel2_metrics.dat");
        
        if (i % 500 == 0) {
            cout << "Generazione " << i << " completata." << endl;
        }
    }

    // Alla fine di tutte le generazioni, salviamo il percorso finale
    pop.save_best_path(map, "non_parallel2_best_path.dat");
    cout << "Evoluzione terminata! Dati salvati." << endl;



    pop.initialize(npaths, square_map, ncities);

    //Puliamo i file di output prima di iniziare
    ofstream clean_file_square("square_metrics.dat"); clean_file_square.close();


    for(int i = 0; i < n_generations; i++) {
        pop.evolve(ncities);
        pop.sort(); // Assicuriamoci che il migliore sia in posizione 0
        
        // Salviamo la lunghezza migliore e la media
        pop.save_metrics(i, "square_metrics.dat");
        
        if (i % 500 == 0) {
            cout << "Generazione " << i << " completata." << endl;
        }
    }

    // Alla fine di tutte le generazioni, salviamo il percorso finale
    pop.save_best_path(square_map, "square_best_path.dat");
    cout << "Evoluzione terminata! Dati salvati." << endl;


}