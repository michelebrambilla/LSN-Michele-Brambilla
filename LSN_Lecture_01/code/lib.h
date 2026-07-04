#ifndef CHISQUARE_H
#define CHISQUARE_H

#include <vector>
#include <string>
#include <cmath>
#include <fstream>   // Necessario per ofstream
#include <iostream>  // Necessario per cerr e endl


class ChiSquare {
private:
    int L;
    int M;                     // Quanti step compongono un singolo blocco
    int current_steps;         // Contatore per sapere a che punto del blocco siamo
    std::vector<double> current_hits;
    std::vector<double> chisquare; // Memoria delle medie di ogni blocco completato
    double chivalue;
    int np;

    

public:
    // Costruttore
    ChiSquare(int block_steps, int divisions) {
    L = block_steps;
    M = divisions;
    np = block_steps/M;
    current_steps = 0;

    current_hits.assign(divisions, 0.0);

}


    // Metodi pubblici
    void newdata(double data){
    int value = int(data * M);
    current_hits[value]++;
    current_steps++;

    // Se il blocco è finito, salva la average e resetta i contatori
    if (current_steps == L) {

        chivalue = 0.0;

        for(int i = 0; i<M; i++)
            chivalue += std::pow((current_hits[i] - np), 2)/np;
        
        chisquare.push_back(chivalue);

        std::fill(current_hits.begin(), current_hits.end(), 0.0);
        current_steps = 0;
    }
}
void SaveResults(const std::string& nome_file) const {
    std::ofstream out(nome_file);
    if (!out.is_open()) {
        std::cerr << "Errore: impossibile aprire " << nome_file << std::endl;
        return;
    }

    for (size_t i = 0; i < chisquare.size(); i++) {

        out << chisquare[i] << "\n";
    }
    
    out.close();
}

};


#endif // CHISQUARE_H