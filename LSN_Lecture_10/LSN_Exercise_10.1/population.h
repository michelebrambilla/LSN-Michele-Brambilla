#ifndef __population__
#define __population__

#include "random.h"
#include "chromosome.h"
#include "city.h"
#include "mpi.h"

#include <vector>
#include <cmath>
#include <algorithm>
#include<fstream>

using namespace std;


class population{

private:

    vector<path> _paths;
    vector<vector<double>> _distance_matrix;
    Random& _rnd;

    void fill_distances(vector<city>& map){
        int n = map.size();
        _distance_matrix.assign(n, std::vector<double>(n, 0.0));
        for (int i = 0; i < n; i++){
            for(int j = i+1; j < n; j++){
                double dx = map[i].x - map[j].x;
                double dy = map[i].y - map[j].y;

                double distance = sqrt(dx*dx + dy*dy);
                _distance_matrix[i][j] = distance;
                _distance_matrix[j][i] = distance;
            }
        }
    }


public:

    population(Random& rnd) : _rnd(rnd) {}

    void initialize(int npaths, vector<city>& map, int ncities){

        fill_distances(map);

        _paths.reserve(npaths);
        path base_path;
        base_path._sequence.reserve(ncities);
    
        for (int i = 0; i<ncities; i++)
            base_path._sequence.push_back(i);

        for (int i = 0; i < npaths; ++i) {
        path new_path = base_path; 

        for(int j = ncities - 1; j > 1; --j) {
            int k = int(_rnd.Rannyu(1, j + 1)); 
            swap(new_path._sequence[j], new_path._sequence[k]);
        }
        
        _paths.push_back(new_path);
        }
    }

    void assign_length(path& p){
        p._length = 0.0;
        int n = p._sequence.size();
        for (int i = 0; i < n-1; i++){
            p._length += _distance_matrix[p._sequence[i]][p._sequence[i+1]];
        }
        p._length += _distance_matrix[p._sequence[n-1]][p._sequence[0]];
    }

    void sort(){
        int n = _paths.size();
        for (int i = 0; i<n; i++){
            assign_length(_paths[i]);
        }

        std::sort(_paths.begin(), _paths.end(), [](const path& a, const path& b) {
            return a._length < b._length;
        });
    }

    int select_parent(){
        int n = _paths.size();
        double p = 2;
        double r = _rnd.Rannyu();
        int j = int(n*pow(r, p));
        return j;
    }

    bool check(const path& p, int ncities) {
        if (p._sequence.size() != ncities) return false; 
        if (p._sequence[0] != 0) return false;           

        
        vector<int> counts(ncities, 0);
        int n = p._sequence.size();
        for (int i = 0; i<n; i++) {
            if (p._sequence[i] < 0 || p._sequence[i] >= n) return false; 
            counts[i]++;
            if (counts[i] > 1) return false;            
        }
        return true;
    }

    void mutation_swap(path& p, int ncities){
        int i = (int)_rnd.Rannyu(1, ncities);
        int j = (int)_rnd.Rannyu(1, ncities);
        swap(p._sequence[i], p._sequence[j]);
    }

    void mutation_global_shift(path& p, int ncities) {
        int shift = int(_rnd.Rannyu(1, ncities - 1));
        vector<int> copy = p._sequence;

        for(int i = 1; i < ncities; i++) {
            
            int new_index = i + shift;
            
            //PBC! Mutuate da NSL, riconducono la posizione ncities alla 1, lasciando intatta la 0.
            if (new_index >= ncities) {
                new_index = 1 + (new_index - 1) % (ncities - 1);
            }

            p._sequence[new_index] = copy[i];
        }
    }


    void mutation_swap_block(path& p, int ncities){
        int max = (int)(ncities-1)/2;
        int m = (int)_rnd.Rannyu(1,  max + 1);

        int start = (int)_rnd.Rannyu(1, ncities -2 * m + 1 );
        int start2 = start + m;

        for (int i = 0; i < m; i++)
            swap(p._sequence[start+i], p._sequence[start2+i]);
    }

    void mutation_reverse(path& p, int ncities){
        int start = (int)_rnd.Rannyu(1, ncities);
        int end = (int)_rnd.Rannyu(start, ncities);
        int diff = end - start;
        vector<int> copy = p._sequence;
        for (int i = 0; i < diff; i++)
            p._sequence[start + i] = copy[end-i-1];
    }


    void crossover(path& mom, path& dad, path& son1, path& son2, int ncities){
        son1 = mom;
        son2 = dad;

        int cut = int(_rnd.Rannyu(1, ncities));
        int fill_index = cut;

        for (int i = 1; i < ncities; i++){
            int counts = 0;
            for (int j = 1; j < cut; j++){
                if (dad._sequence[i] != son1._sequence[j])
                counts++;
            }
            if(counts == cut-1){
            son1._sequence[fill_index] = dad._sequence[i];
            fill_index++;
            }
        }

        fill_index = cut;

        for (int i = 1; i < ncities; i++){
            int counts = 0;
            for (int j = 1; j < cut; j++){
                if (mom._sequence[i] != son2._sequence[j])
                counts++;
            }
            if(counts == cut-1){
            son2._sequence[fill_index] = mom._sequence[i];
            fill_index++;
            }
        }

    }


    void evolve(int ncities) {
        
        sort(); 
        
        vector<path> next_generation;
        next_generation.reserve(_paths.size());
        
        while (next_generation.size() < _paths.size()) {
            
            int mom_idx = select_parent();
            int dad_idx = select_parent();
            
            path son1 = _paths[mom_idx];
            path son2 = _paths[dad_idx];
            
            if (_rnd.Rannyu() < 0.70) {
                crossover(_paths[mom_idx], _paths[dad_idx], son1, son2, ncities);
            }

            if (_rnd.Rannyu() < 0.08) mutation_swap(son1, ncities);
            if (_rnd.Rannyu() < 0.08) mutation_global_shift(son1, ncities); 
            if (_rnd.Rannyu() < 0.08) mutation_swap_block(son1, ncities);
            if (_rnd.Rannyu() < 0.08) mutation_reverse(son1, ncities);

            
            if (_rnd.Rannyu() < 0.08) mutation_swap(son2, ncities);
            if (_rnd.Rannyu() < 0.08) mutation_global_shift(son2, ncities); 
            if (_rnd.Rannyu() < 0.08) mutation_swap_block(son2, ncities);
            if (_rnd.Rannyu() < 0.08) mutation_reverse(son2, ncities);

            
            if (next_generation.size() < _paths.size()) {
                if (check(son1, ncities)) next_generation.push_back(son1);
                else cout << "ATTENZIONE: Son1 fallito!" << endl;
            }
            
            if (next_generation.size() < _paths.size()) {
                if (check(son2, ncities)) next_generation.push_back(son2);
                else cout << "ATTENZIONE: Son2 fallito!" << endl;
            }
        }

        
        _paths = next_generation;
    }


    
    void save_metrics(int generation, const string& filename) {
        ofstream out(filename, ios::app);
        
        
        double best_length = _paths[0]._length;
        
        
        double sum = 0.0;
        int half = _paths.size() / 2;
        for(int i = 0; i < half; i++) {
            sum += _paths[i]._length;
        }
        double mean_length = sum / half;
        
        out << generation << "\t" << best_length << "\t" << mean_length << "\n";
        out.close();
    }


    void save_best_path(const vector<city>& map, const string& filename) {
        ofstream out(filename);
        path best = _paths[0];
        
        for(int i = 0; i < best._sequence.size(); i++) {
            int city_idx = best._sequence[i];
            out << map[city_idx].x << "\t" << map[city_idx].y << "\n";
        }
        
        int first_city = best._sequence[0];
        out << map[first_city].x << "\t" << map[first_city].y << "\n";
        
        out.close();
    }


    void migrate(int rank, int cores, int itag){

        int ncities = _paths[0]._sequence.size();
        vector<int> sequence(cores);

        if (rank == 0){
            for(int i = 0; i<cores; i++)
            sequence[i] = i; 
            for(int j = cores-1; j > 1; --j) {
                    int k = int(_rnd.Rannyu(0, j + 1)); 
                    swap(sequence[j], sequence[k]);
            }  
        }

        MPI_Bcast(&sequence[0], cores, MPI_INT, 0, MPI_COMM_WORLD);

        int dest = sequence[rank];
        int source = 0;
        
        for(int i = 0; i < cores; i++) {
            if (sequence[i] == rank) {
                source = i;
                break;
            }
        }

        if (dest != rank) {
            vector<int> send_buffer = _paths[0]._sequence;
            vector<int> recv_buffer(ncities);

            MPI_Request req;
            MPI_Status stat;

            MPI_Isend(&send_buffer[0], ncities, MPI_INT, dest, itag, MPI_COMM_WORLD, &req);
            MPI_Recv(&recv_buffer[0], ncities, MPI_INT, source, itag, MPI_COMM_WORLD, &stat);
            MPI_Wait(&req, &stat);

            int dim = _paths.size();
            int extinct = (int)_rnd.Rannyu(1, dim);

            for(int k = 0; k<ncities; k++){
                _paths[extinct]._sequence[k] = recv_buffer[k];
            }

            assign_length(_paths[extinct]);
            sort();
        }

    }


    void migrate_best(int rank, int cores, int itag){

        int ncities = _paths[0]._sequence.size();
        vector<int> sequence(cores);

        // Il rank 0 decide l'ordine di migrazione casuale
        if (rank == 0){
            for(int i = 0; i<cores; i++)
                sequence[i] = i; 
            for(int j = cores-1; j > 1; --j) {
                int k = int(_rnd.Rannyu(0, j + 1)); 
                swap(sequence[j], sequence[k]);
            }  
        }

        MPI_Bcast(&sequence[0], cores, MPI_INT, 0, MPI_COMM_WORLD);

        int dest = sequence[rank];
        int source = 0;
        
        for(int i = 0; i < cores; i++) {
            if (sequence[i] == rank) {
                source = i;
                break;
            }
        }

        if (dest != rank) {
    
            vector<int> send_buffer = _paths[0]._sequence;
            vector<int> recv_buffer(ncities);

            MPI_Request req;
            MPI_Status stat;


            MPI_Isend(&send_buffer[0], ncities, MPI_INT, dest, itag, MPI_COMM_WORLD, &req);
            MPI_Recv(&recv_buffer[0], ncities, MPI_INT, source, itag, MPI_COMM_WORLD, &stat);
            MPI_Wait(&req, &stat);

            for(int k = 0; k<ncities; k++){
                _paths[0]._sequence[k] = recv_buffer[k];
            }

            assign_length(_paths[0]);
            
            sort();
        }
    }

    void final_migration(int rank, int cores, int selected){

        //Block distribution: voglio una nuova popolazione della stessa dimensione. 
        int r = _paths.size() % cores;
        int ncities = _paths[0]._sequence.size();
        
        vector<int> send_buffer((selected+1) * ncities );
        for(int i = 0; i < (selected+1); i++) {
            for(int k = 0; k < ncities; k++) {
                send_buffer[i * ncities + k] = _paths[i]._sequence[k];
            }
        }

        vector<int> recv_buffer;
        if (rank == 0) recv_buffer.resize(cores * (selected +1) * ncities + 1);


        MPI_Gather(&send_buffer[0], (selected + 1) * ncities, MPI_INT, 
                   &recv_buffer[0], (selected + 1) * ncities, MPI_INT, 0, MPI_COMM_WORLD);

        if(rank == 0){

            int index = 0;
        
            for(int i = 0; i<cores; i++){
                int q = (i < r) ? (selected + 1) : selected;
            
                for(int j = 0; j<q; j++){
                    for(int k = 0; k<ncities; k++){
                    _paths[index]._sequence[k] = recv_buffer[(i *(selected+1) + j) * ncities + k];
                    }
                    assign_length(_paths[index]);
                    index++;
                }
            }
            sort();
        }
    }


};

#endif