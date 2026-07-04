#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "random.h"
#include "datablocker.h"
#include "lib.h"

using namespace std;


int main (int argc, char *argv[]){

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




   int M = 1000000;
   int N = 100;
   int L = M / N;


   // Creiamo due "oggetti analizzatori", uno per Z e uno per la Varianza
   // Passiamo L al costruttore in modo che sappiano quanto è grande un blocco
   DataBlocker r_analizer(L);
   DataBlocker r_squared_analizer(L);
   ChiSquare test(L, 100);

    // Un SOLO ciclo totale M volte! Gli oggetti smistano in blocchi automaticamente.
   for (int i = 0; i < M; i++) {
      double z = rnd.Rannyu();
      
      test.newdata(z);
      r_analizer.newdata(z);
      r_squared_analizer.newdata(pow(z - 0.5, 2));


   }

   // A simulazione finita, chiediamo agli oggetti di stampare i file
   test.SaveResults("Chisquare.txt");
   r_analizer.SaveResults("r_data.txt");
   r_squared_analizer.SaveResults("r_variance_data.txt");


   

   return 0;
}