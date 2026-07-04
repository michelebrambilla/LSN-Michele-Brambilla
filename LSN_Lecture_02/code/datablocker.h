#ifndef DATABLOCKER_H
#define DATABLOCKER_H

#include <vector>
#include <string>

class DataBlocker {
private:
    int L;                     // Quanti step compongono un singolo blocco
    int current_steps;         // Contatore per sapere a che punto del blocco siamo
    double current_sum;     // Accumulatore per il blocco in corso
    std::vector<double> block_average; // Memoria delle medie di ogni blocco completato

    // Metodo privato: serve solo all'interno della classe
    double error(double square_average, double average, int n) const;

public:
    // Costruttore
    DataBlocker(int block_steps);

    // Metodi pubblici
    void newdata(double data);
    void SaveResults(const std::string& file_name) const;
    void FinalResults(const std::string& file_name) const;
};

#endif // DATABLOCKER_H