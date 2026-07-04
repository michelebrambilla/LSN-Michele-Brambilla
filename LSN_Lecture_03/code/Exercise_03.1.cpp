#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "random.h"
#include "datablocker.h"
#include "functions.h"

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


   double S0 = 100;
   double T = 1;
   double K = 100;
   double r = 0.1;
   double sigma = 0.25;
   int divisions = 100;
   double x = T/divisions;

   double drift_step = (r - 0.5 * sigma * sigma) * x;
   double vol_step = sigma * sqrt(x);


   DataBlocker call_analizer(L);
   DataBlocker put_analizer(L);
   DataBlocker path_call_analizer(L);
   DataBlocker path_put_analizer(L);


   vector<vector<double>> paths(L, vector<double>(divisions + 1));





   for (int i = 0; i < M; i++) {

      double S_partial = S0;

      if (i < L) {
        paths[i][0] = S0;
    }

      for (int j = 0; j < divisions; j++){
        double w = rnd.Gauss(0,1);
        S_partial = S_partial * exp(drift_step + vol_step*w);
        if(i < L){
         paths[i][j+1] = S_partial;
        }
      }


      double z = rnd.Gauss(0,1);
      double S = final_S(S0, drift_step, divisions, sigma, T, z);
      double call = BS_call(S, K, T, r);
      double put = BS_put(S, K, T, r);

      double path_call = BS_call(S_partial, K, T, r);
      double path_put = BS_put(S_partial, K, T, r);
        
      call_analizer.newdata(call);
      put_analizer.newdata(put);
      path_call_analizer.newdata(path_call);
      path_put_analizer.newdata(path_put);
      
   }


   call_analizer.SaveResults("call_data.txt");
   put_analizer.SaveResults("put_data.txt");
   path_call_analizer.SaveResults("path_call_data.txt");
   path_put_analizer.SaveResults("path_put_data.txt");

   ofstream out_paths("paths.txt");
    
   for (int j = 0; j <= divisions; j++) {
        
        double current_t = j * x;
        out_paths << current_t << "\t";

        for (int i = 0; i < 100; i++) {
            out_paths << paths[i][j] << "\t";
        }
        out_paths << "\n";
    }
    out_paths.close();

    return 0;
}





