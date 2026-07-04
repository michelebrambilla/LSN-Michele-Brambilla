#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "random.h"
#include "datablocker.h"

using namespace std;

double psi_trial(double x, double mu, double sigma){
    double psi = exp(-pow((x-mu),2)/(2*sigma*sigma)) + exp(-pow((x+mu),2)/(2*sigma*sigma));
    return psi;
}

double potential(double x){
    return (pow(x, 4) - 5./2.*x*x);
}

double kinetic(double mu, double sigma, double x){
    return -0.5*(exp(-pow((x-mu), 2)/(2*sigma*sigma))*((x-mu)*(x-mu)/pow(sigma, 4) - 1./(sigma*sigma)) + exp(-pow((x+mu), 2)/(2*sigma*sigma))*((x+mu)*(x+mu)/pow(sigma,4)-1./(sigma*sigma)));
}

double energy(double x, double mu, double sigma){
    return (kinetic(mu, sigma, x)/psi_trial(x, mu, sigma) + potential(x));
}

bool metropolis(double xnew, double x, double mu, double sigma, Random& rnd){

    bool decision = false;
    double prob = pow(psi_trial(xnew, mu, sigma), 2)/pow(psi_trial(x, mu, sigma), 2);

    if(prob >= 1){
        decision = true;
    } else{
        if (rnd.Rannyu() < prob)
            decision = true;
    }

    return decision;
}



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

int L = 5000;
int M = 50;
int N = L*M;


double mu = 0.0;
//double sigma = 0.55;
double x = 0.75;
double delta = 2.5;

double accepted = 0;
double trials = 0;

double E = 0;

for (int j = 0; j<80; j++){

    double sigma = 0.0;
    
    for (int s = 0; s<80; s++){


        DataBlocker expvalue(L);
        double err = 0.0;

        
        for(int i = 0; i < N; i++){
            double xnew = x + rnd.Rannyu(-1.0, 1.0)*delta;

            if(metropolis(xnew, x, mu, sigma, rnd)){
                x = xnew;
                accepted++;
            }

            E = energy(x, mu, sigma);
            expvalue.newdata(E);
            trials++;
        }

        cout << "Elemento " << j*80 + s << " su " << 80*80 << " completato" << endl;


        expvalue.FinalResults(mu, sigma, err, "values_grid.txt");
        sigma += 0.025;
    }

    mu += 0.025;
}

//expvalue.SaveResults("Hvalue.txt");

double acceptance = accepted/trials;
cout << "Accettanza: " << acceptance ;

}



