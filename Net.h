//
// Created by Wan Luan Lee on 9/4/22.
//

#ifndef FM_NET_H
#define FM_NET_H
#include <list>
#include <string>

class Cell;
class Net {
public:
    Net(std::string name);
    void insert_cell(Cell* cell_name);
    void print();
    std::list<Cell*>& get_cells();
    //void print_cells();
private:
    std::string _name;
    std::list<Cell*> _cells;
    int _num_cells = 0;
};


#endif //FM_NET_H
