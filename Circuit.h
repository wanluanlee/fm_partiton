//
// Created by Wan Luan Lee on 9/6/22.
//

#ifndef FM_CIRCUIT_H
#define FM_CIRCUIT_H
#include <string>
#include "Cell.h"
#include "Net.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <chrono>

typedef struct gain_bucket {
    int id;
    Cell* cell;
    struct gain_bucket *next, *previous;
}gain_bucket;

class Circuit {

public:
    Circuit();
    ~Circuit();
    void create_graph(std::string file_name);
    void init_partition();
    size_t compute_cutsize();
    //void calculate_cutsize();
    void fm_pass();
    size_t min_cutsize(int num_it);
    void retrive_best();
    size_t get_partition_a_size();
    size_t get_partition_b_size();
    std::list<Cell*> get_partition_a_list();
    std::list<Cell*> get_partition_b_list();

private:
    float _r = 0;
    int _max_gain = 0;
    size_t _num_cells = 0;
    size_t _num_nets = 0;
    std::map<std::string, Cell*> _all_cells;
    std::map<std::string, Net*> _all_nets;
    size_t _cells_in_a = 0;
    size_t _cells_in_b = 0;
   // void _fill_bucket_list();

    std::vector<Cell*> _bucket_list_a;
    std::vector<Cell*> _bucket_list_b;
    std::vector<Cell*> _locked_cell;
    std::vector<int> _gain_list;

    int _compute_init_gain(Cell* c);
    void _update_gain(Cell* c);

    void _init_gain_bucket();
    Cell* _find_max_gain_cell();
    bool _check_balance(Cell* cell_to_be_move);
    void _remove_from_bucket(Cell* cell);
    void _add_to_bucket(Cell* cell);
    void _move_cell(Cell* cell_to_be_move);
    size_t _find_max_index(std::vector<int> int_list);
};


#endif //FM_CIRCUIT_H
