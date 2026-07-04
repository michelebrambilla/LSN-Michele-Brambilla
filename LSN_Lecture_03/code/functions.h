#ifndef FUNCTIONS_H
#define FUNCTIONS_H


//File contenente le funzioni utilizzate nel main 


double max(double a, double b){
    if (a > b)
        return a;
    return b;
}


double BS_call (double S, double K, double T, double r){
    return exp(-r * T) * max(0, S - K);
}

double BS_put (double S, double K, double T, double r){
    return exp(-r * T) * max(0, K - S);
}

double final_S (double S0, double drift_step, int divisions, double sigma, double t, double z){
    return S0 * exp(drift_step*divisions + sigma*z);
}



#endif
