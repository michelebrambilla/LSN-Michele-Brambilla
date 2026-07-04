#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "random.h"
#include "datablocker.h"
#include "RandomWalker.h"

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




   int M = 10000;
   int N = 100;
   int D = 100;
   double s = 1;
   int L = int(M / N);
   int dimensions = 3;

   double pi = acos(-1.0);



   vector<DataBlocker> discrete_analizer(D, DataBlocker(L));
   vector<DataBlocker> continuum_analizer(D, DataBlocker(L));


ofstream cleanD("Discrete.txt");
cleanD.close();
ofstream cleanC("Continuum.txt");
cleanC.close();
ofstream cleanPD("Path_Discrete.txt");
cleanPD.close();
ofstream cleanPC("Path_Continuum.txt");
cleanPC.close();
ofstream final_pos_D("Final_Positions_D.txt");
ofstream final_pos_C("Final_Positions_C.txt");



// Voglio plottare andamento di un RW per tipo
ofstream path_discrete("Path_Discrete.txt", ios::app);
ofstream path_continuum("Path_Continuum.txt", ios::app);

RandomWalker discrete(D, s, dimensions);
RandomWalker continuum(D, s, dimensions);

   for (int i = 0; i < M; i++) {
      
      discrete.reset();
      continuum.reset();

      // Stampo origine
      if (i == 0) {
         discrete.print_position(path_discrete);
         continuum.print_position(path_continuum);
      }

      for (int step = 0; step < D; step++){

      // Random walk discreto
      int r = int(rnd.Rannyu(0, dimensions*2)); 
      // Volevo fare una sola estrazione per motivi computazionali. Quindi r/2, essendo divisione intera,
      // dà la direzione, e 1 - 2*(r%2) dà il segno dello spostamento.
      // Dopo la lezione 9 mi è venuto anche in mente che si potrebbe fare una tabella di spostamenti predefiniti
      // da cui pescare la mossa a seguito di estrazione.
      discrete.newstep(r / 2, 1 - 2 * (r % 2));

      double d = discrete.get_squared_distance();
      discrete_analizer[step].newdata(d);


      //Random walk continuo
      double phi = rnd.Rannyu(0, 2*pi);
      double cos_theta = rnd.Rannyu(-1, 1);
      double theta = acos(cos_theta);
      continuum.newcontinuumstep(phi, theta);
      double d_cont = continuum.get_squared_distance();
      continuum_analizer[step].newdata(d_cont);

      if (i == 0) {
         discrete.print_position(path_discrete);
         continuum.print_position(path_continuum);
      }

      }

      discrete.print_position(final_pos_D);
      continuum.print_position(final_pos_C);

   }

   final_pos_C.close();
   final_pos_D.close();
   path_discrete.close();
   path_continuum.close();

   // A simulazione finita, chiediamo agli oggetti di stampare i file
   for(int i = 0; i < D; i++){
   discrete_analizer[i].FinalResults("Discrete.txt");
   continuum_analizer[i].FinalResults("Continuum.txt");

   }

   

   return 0;
}