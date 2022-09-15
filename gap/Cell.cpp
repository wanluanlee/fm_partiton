//
// Created by Wan Luan Lee on 9/4/22.
//

#include "Cell.h"

Cell::Cell(std::string name) {
    _name = name;
}

void Cell::print() {
    std::cout << "print cell: " << this->_name << std::endl;
}

void Cell::insert_net(Net* new_net) {
    _nets.push_back(new_net);
    _num_net++;
}

int Cell::net_count() {
    return _num_net;
}

std::string Cell::get_name() const{
    return _name;
}

void Cell::set_poisition(int pos) {
    _position = pos;
}

int Cell::get_position() {
    return _position;
}

std::list<Net*>& Cell::get_nets() {
    return _nets;
}

void Cell::set_visited(bool if_visited) {
    _visited = if_visited;
}

bool Cell::get_visited() {
    return _visited;
}

int Cell::get_index() {
    return _index;
}

void Cell::set_next(Cell *cell) {
    _next = cell;
}

void Cell::set_prev(Cell *cell) {
    _prev = cell;
}

Cell* Cell::get_next() {
    return _next;
}

Cell* Cell::get_prev() {
    return this->_prev;
}

void Cell::set_iflocked(bool iflocked) {
    this->_iflocked = iflocked;
}

bool Cell::get_iflocked() {
    return this->_iflocked;
}

void Cell::set_gain(int gain) {
    this->_gain = gain;
}

int Cell::get_gain() {
    return this->_gain;
}

void Cell::set_f(int f) {
    _f = f;
    //_gain = _fs - _te;
}

void Cell::set_t(int t) {
    _t = t;
    //_gain = _fs - _te;
}

int Cell::get_f() {
    return _f;
}

int Cell::get_t() {
    return _t;
}




