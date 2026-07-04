#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "random.h"

using namespace std;


double dice_average(int n, Random& rnd){

   double sum = 0;
   for (int i = 0; i < n; i++){
      int z = int(rnd.Rannyu() * 6) + 1;
      sum += z;
   }

   return sum/n;

}


double exp_average(int n, Random& rnd, double lambda){

   double sum = 0;
   for (int i = 0; i < n; i++){
      double z = rnd.Exponential(lambda);
      sum += z;
   }

   return sum/n;

}

double CauchyLorentz_average(int n, Random& rnd, double gamma, double mu){

   double sum = 0;
   for (int i = 0; i < n; i++){
      double z = rnd.CauchyLorentz(gamma, mu);
      sum += z;
   }

   return sum/n;

}

void print(const string& file_name, vector <double> v, int M) {

   ofstream out(file_name);
    if (!out.is_open()) {
        cerr << "Errore: impossibile aprire " << file_name << endl;
        return;
    }

    for (int i = 0; i < M; i++)
      out << v[i] << "\n";

    out.close();

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


   vector<int> N = {1, 2, 50, 100};
   int M = 1e4;
   double lambda = 1;
   double gamma = 3;
   double mu = 1;
   vector<double> dice_averages;
   vector<double> exp_averages;
   vector<double> lorentz_averages;


   
   for ( size_t j = 0; j < N.size(); j++){
      for (int i = 0; i < M; i++) {

         double dice = dice_average(N[j], rnd);
         double exp = exp_average(N[j], rnd, lambda);
         double Lorentz = CauchyLorentz_average(N[j], rnd, gamma, mu);

         dice_averages.push_back(dice);
         exp_averages.push_back(exp);
         lorentz_averages.push_back(Lorentz);

      }
   }




   // A simulazione finita, chiediamo agli oggetti di stampare i file
   print("dice_averages.txt", dice_averages, M*N.size());
   print("exponential_averages.txt", exp_averages, M*N.size());
   print("lorentz_averages.txt", lorentz_averages, M*N.size());
   

   return 0;
}