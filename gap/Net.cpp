//
// Created by Wan Luan Lee on 9/4/22.
//

#include "Net.h"
#include <iostream>
Net::Net(const std::string& name) {
    _name = name;
}

void Net::insert_cell(Cell* cell_name) {
    _cells.push_back(cell_name);
    this->_num_cells++;
}

void Net::print() {
    std::cout << "print net: " << _name << std::endl;
}

//void Net::print_cells() {
//    std::cout << "net: " << this->_name << "connects cells: ";
//    for(auto& cel : cells) {
//        cel->print();
//        std::cout << ", ";
//    }
//    std::cout << std::endl;
//}
//


std::list<Cell*>& Net::get_cells() {
    return _cells;
}
