#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <cstdio>
#include "random.h"
#include "datablocker.h"
#include "lib.h"

using namespace std;


int main (int argc, char *argv[]){

    ofstream("parameters.txt", ios::trunc); 
    ofstream("Optimized_energy_evolution.txt", ios::trunc); 
    ofstream("Histo.txt", ios::trunc);

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

ofstream out_params("parameters.txt");
ofstream out_acc("acceptance.txt");

int L = 2000;
int M = 30;

DataBlocker expvalue(L);

double mu = 1.5;
double sigma = 1.5;
double sigma2_inv_old = 1./(sigma*sigma);
double t = 0.2;
double delta = 0.1;
double olderr;
double x = 0.75;
double acc = 0.0;
double oldE = final_expected(mu, sigma2_inv_old, L, M, rnd, olderr, x, expvalue, acc);
int iterations = 10;

double sigma2_inv = 0.0;

        out_params << mu << "\t" << sigma << "\t" << oldE << "\t" << olderr << "\n";

for (int j = 0; j < 40; j++){

    double accepted = 0;
    double trials = 0;

    double sum_vmc_acceptance = acc;

    for(int i = 0; i < iterations; i++){

        double munew = mu + rnd.Rannyu(-1.0, 1.0)*delta;
        double sigmanew = sigma + rnd.Rannyu(-1.0, 1.0)*delta;

        munew = fabs(munew);
        sigmanew = fabs(sigmanew);
        sigma2_inv = 1./(sigmanew*sigmanew);
        double newerr;
        double current_vmc_acc;
        double newE = final_expected(munew, sigma2_inv, L, M, rnd, newerr, x, expvalue, current_vmc_acc);
        
        sum_vmc_acceptance += current_vmc_acc;

        if(metropolis(newE, oldE, rnd, t)){
            mu = munew;
            sigma = sigmanew;
            oldE = newE;
            accepted++;
            olderr = newerr;
        }

        out_params << mu << "\t" << sigma << "\t" << oldE << "\t" << olderr << "\n";
        trials++; 
    }

    //if (j%5 == 0 && j != 0){
        M += 1;
        iterations += 2;
        delta *= 0.95;

    //}

    double acceptance = accepted/trials;
    double avg_vmc_acceptance = sum_vmc_acceptance/(trials + 1.);
    out_acc << acceptance << "\t" << avg_vmc_acceptance << endl;
    cout << "Completato blocco " << j+1 << " a temperatura " << t << " | Accettanza SA: \t" << acceptance << " | Accettanza VMC: \t" << avg_vmc_acceptance << endl;

    t *= 0.85;

}

out_params.close();
out_acc.close();

int N = L*M;
double delta_x = 2.5;

sigma2_inv = 1.0 / (sigma * sigma);

DataBlocker FinalEnergy(L);
ofstream out("Histo.txt");

for (int i = 0; i < N; i++){

    double xnew = x + rnd.Rannyu(-1.0, 1.0) * delta_x;
    
    if(metropolis_psi(xnew, x, mu, sigma2_inv, rnd)){
        x = xnew;
    }
    
    FinalEnergy.newdata(energy(x, mu, sigma2_inv));
    
    out << x << "\n";
}
out.close();


FinalEnergy.SaveResults("Optimized_energy_evolution.txt");

}



