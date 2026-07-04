#include "datablocker.h"
#include <fstream>
#include <cmath>
#include <iostream>

using namespace std;

// Costruttore: inizializza le variabili a zero
DataBlocker::DataBlocker(int block_steps) {
    L = block_steps;
    current_steps = 0;
    current_sum = 0.0;
}

double DataBlocker::error(double square_average, double average, int n) const {
    if (n == 0) return 0;
    return sqrt((square_average - average * average) / n);
}

// Quando chiami questa funzione, l'oggetto decide cosa fare
void DataBlocker::newdata(double data) {
    current_sum += data;
    current_steps++;

    // Se il blocco è finito, salva la average e resetta i contatori
    if (current_steps == L) {
        block_average.push_back(current_sum / L);
        current_sum = 0.0;
        current_steps = 0;
    }
}


// Fa il calcolo cumulativo (O(N)) e stampa su file
void DataBlocker::SaveResults(const string& nome_file) const {
    ofstream out(nome_file);
    if (!out.is_open()) {
        cerr << "Errore: impossibile aprire " << nome_file << endl;
        return;
    }

    int N = block_average.size();
    double cumulative_sum = 0.0;
    double square_cumulative_sum = 0.0;

    for (int i = 0; i < N; i++) {
        cumulative_sum += block_average[i];
        square_cumulative_sum += (block_average[i]*block_average[i]);

        double progressive_average = cumulative_sum / (i + 1);
        double square_progressive_average = square_cumulative_sum / (i + 1);
        double err_prog = error(square_progressive_average, progressive_average, i);

        out << progressive_average << "\t" << err_prog << "\n";
    }
    
    out.close();
}

void DataBlocker::FinalResults(const string& nome_file) const {
    ofstream out(nome_file, ios::app);
    if (!out.is_open()) {
        cerr << "Errore: impossibile aprire " << nome_file << endl;
        return;
    }

    int N = block_average.size();
    double cumulative_sum = 0.0;
    double square_cumulative_sum = 0.0;

    for (int i = 0; i < N; i++) {
        cumulative_sum += block_average[i];
        square_cumulative_sum += pow(block_average[i], 2);
    }

    double average = cumulative_sum / N;
    double square_average = square_cumulative_sum / N;
    double err = error(square_average, average, (N-1));

    double rms = 0.0;
    double err_rms = 0.0;

    if(average > 0){
        rms = sqrt(average);
        err_rms = err/(2.0*sqrt(rms));
    }

    
    out << rms << "\t" << err_rms << "\n";
    
    
    out.close();
}