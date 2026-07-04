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




   int M = 1000000;
   int N = 100;
   int L = M / N;
   double l = 1;
   double d = 1.2;
   double z_center;


   DataBlocker pi_analizer(L);

   // Gli oggetti smistano in blocchi automaticamente.
   for (int i = 0; i < N; i++) {
   int n_hits = 0;

        for (int j = 0; j < L; j++){

            z_center = rnd.Rannyu(0.0, d / 2.0);
            double x, y, r_square;


            do{
               x = rnd.Rannyu(-1., 1.);
               y = rnd.Rannyu(0.,1.);
               r_square = x*x + y*y;
            } while(r_square > 1. || r_square == 0.);

        double sin_theta = y/sqrt(r_square);
    
            if (l/2.*sin_theta > z_center)
            n_hits ++;
        }

    double pi = 2.*l*double(L)/(double(n_hits)*d);
    pi_analizer.newvalue(pi);

        
   }

   // A simulazione finita, chiediamo agli oggetti di stampare i file
   pi_analizer.SaveResults("pi_values.txt");


   

   return 0;
}