#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "random.h"
#include "datablocker.h"

using namespace std;

double psi_trial(double x, double mu, double sigma2_inv){
    double psi = exp(-0.5*((x-mu)*(x-mu))*sigma2_inv) + exp(-0.5*((x+mu)*(x+mu))*sigma2_inv);
    return psi;
}

double potential(double x){
    return (x*x*x*x - 5./2.*x*x);
}

double kinetic(double mu, double sigma2_inv, double x){
    double exp_minus = exp(-0.5 * ((x-mu)*(x-mu))*sigma2_inv);
    double exp_plus  = exp(-0.5 * ((x+mu)*(x+mu))*sigma2_inv);
    
    double term_minus = exp_minus * (((x-mu)*(x-mu))*(sigma2_inv*sigma2_inv) - sigma2_inv);
    double term_plus  = exp_plus  * (((x+mu)*(x+mu))*(sigma2_inv*sigma2_inv) - sigma2_inv);
    
    return -0.5 * (term_minus + term_plus);
}

double energy(double x, double mu, double sigma2_inv){
    return (kinetic(mu, sigma2_inv, x)/psi_trial(x, mu, sigma2_inv) + potential(x));
}

bool metropolis_psi(double xnew, double x, double mu, double sigma2_inv, Random& rnd){

    bool decision = false;
    double psi_new = psi_trial(xnew, mu, sigma2_inv);
    double psi_old = psi_trial(x, mu, sigma2_inv);
    double prob = (psi_new * psi_new) / (psi_old * psi_old);

    if(prob >= 1){
        decision = true;
    } else{
        if (rnd.Rannyu() < prob)
            decision = true;
    }

    return decision;
}

double final_expected(double mu, double sigma2_inv, int L, int M, Random& rnd, double& err,double& x, DataBlocker& expvalue, double& acc_rate){

    expvalue.Reset();

    double delta = 2.5;
    double E = 0;
    int N = L*M;

    double accepted = 0;

    for(int i = 0; i < N; i++){
        double xnew = x + rnd.Rannyu(-1.0, 1.0)*delta;

        if(metropolis_psi(xnew, x, mu, sigma2_inv, rnd)){
            x = xnew;
            accepted++;
        }

        E = energy(x, mu, sigma2_inv);
        expvalue.newdata(E);
    }

    acc_rate = accepted / double(N);
    double final_E = expvalue.FinalResults(err);
    return final_E;
}


bool metropolis(double newE, double oldE, Random& rnd, double t){

    bool decision = false;

    double prob = exp(-(newE-oldE)/t);

    if(prob >= 1){
        decision = true;
    } else{
        if (rnd.Rannyu() < prob)
            decision = true;
    }
    return decision;

}

void print(const string& nome_file, double x, double y, double E, double err){
    ofstream out(nome_file, ios::app);
    if (!out.is_open()) {
        cerr << "Errore: impossibile aprire " << nome_file << endl;
        return;
    }

    out << x << "\t" << y << "\t" << E << "\t" << err << "\n";

    out.close();
}

