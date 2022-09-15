//
// Created by Wan Luan Lee on 9/4/22.
//

#ifndef FM_CELL_H
#define FM_CELL_H
#include <list>
#include <iostream>
#include <string>
#include "Net.h"
class Cell {
public:
    Cell(std::string name);
    void print();
    void set_prev(Cell* cell);
    void set_next(Cell* cell);
    Cell* get_prev();
    Cell* get_next();
    void set_gain(int gain);
    int get_gain();
    void set_iflocked(bool iflocked);
    bool get_iflocked();
    void insert_net(Net* new_net);
    std::string get_name() const;
    int net_count();
    void set_poisition(int pos);
    int get_position();
    std::list<Net*>& get_nets();
    void set_visited(bool if_visited);
    bool get_visited();
    int get_index();
    void set_f(int f);
    void set_t(int t);
    int get_f();
    int get_t();

private:
    std::string _name;
    std::list<Net*> _nets;
    int _index;
    int _num_net = 0;
    int _position = 0;
    bool _visited = false;
    bool _iflocked = false;
    int _gain = 0;
    Cell* _prev = NULL;
    Cell* _next = NULL;

    int _f= 0;
    int _t = 0;

};


#endif //FM_CELL_H
