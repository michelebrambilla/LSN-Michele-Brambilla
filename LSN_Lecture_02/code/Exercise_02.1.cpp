#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "random.h"
#include "datablocker.h"

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




   int M = 100000;
   int N = 100;
   int L = M / N;

   DataBlocker cosine_analizer(L);
   DataBlocker importance_sampling_analizer(L);

   for (int i = 0; i < M; i++) {

      double z = rnd.Rannyu();  
      cosine_analizer.newdata(M_PI/2*cos(M_PI*z/2));

      double m = rnd.lineare();
      importance_sampling_analizer.newdata(M_PI/2*cos(M_PI*m/2)/(2-2*m));
      
   }

   // A simulazione finita, chiediamo agli oggetti di stampare i file
   cosine_analizer.SaveResults("cosine_data.txt");
   importance_sampling_analizer.SaveResults("importance.txt");





   return 0;

}