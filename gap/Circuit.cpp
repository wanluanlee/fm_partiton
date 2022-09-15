//
// Created by Wan Luan Lee on 9/6/22.
//

#include "Circuit.h"

Circuit::Circuit(){

}
Circuit::~Circuit(){
    for(auto&& i : _all_cells) {
        delete(i.second);
    }
    for(auto&& i : _all_nets) {
        delete(i.second);
    }
//    for(auto i : _bucket_list_a) {
//        delete(i);
//    }
//    for(auto i : _bucket_list_b) {
//        delete(i);
//    }
}

void Circuit::create_graph(std::string file_name) {

    std::ifstream file("/Users/wanluanlee/CLionProjects/FM/test_cases/"+file_name);
    if(file.is_open()) {
        std::cout << "file open " << std::endl;
        std::string line;
        std::getline(file,line);
        _r = std::stof(line);
        std::string current_net_name = "";
        Net* current_net = NULL;
        while (std::getline(file,line)) {
            //std::cout << "line: " << line << std::endl;
            std::istringstream ss(line);
            std::string word;
            while (ss >> word) {
                if(word[0] == 'n') {
                    current_net_name = word;
                    current_net = new Net(word);
                    _all_nets.insert(std::pair<std::string, Net*>(current_net_name, current_net));
                }
                else if(word[0] == 'c') {
                        //check if cell exist already
                     Cell* new_cell = NULL;
                     if(_all_cells.find(word) == _all_cells.end()) {
                         new_cell = new Cell(word);
                         new_cell->insert_net(current_net);
                         current_net->insert_cell(new_cell);
                         _all_cells.insert(std::pair<std::string, Cell*>(word, new_cell));
                     }
                     else {
                         new_cell = _all_cells.at(word);
                         current_net->insert_cell(new_cell);
                         new_cell->insert_net(current_net);
                     }
                     new_cell->set_visited(false);
                     new_cell->set_poisition(-1);
                     int current_num_net = new_cell->net_count();
                     if(current_num_net > _max_gain) {
                         _max_gain = current_num_net;
                     }
                }
            }
        }  file.close();
    }

    _num_cells = _all_cells.size();
    _num_nets = _all_nets.size();
    std::cout << "total number of cells: " << _num_cells << std::endl;
    std::cout << "total number of _nets: " << _num_nets << std::endl;
}

void Circuit::init_partition() {
    size_t cell_size = _all_cells.size();

    //int cells_in_a = 0;
    for(auto&& i : _all_cells) {
        if(_cells_in_a < cell_size/2) {
            i.second->set_visited(true);
            i.second->set_poisition(0);
            _cells_in_a++;
           // _partition_a.push_back(i.second);
        }
        else {
            i.second->set_visited(true);
            i.second->set_poisition(1);
            _cells_in_b++;
            //_partition_b.push_back(i.second);
        }
    }
    //calculate_cutsize();
}

int Circuit::_compute_init_gain(Cell* c) {
    int pos = c -> get_position();
    int gain = 0;
    for(auto&& i : c -> get_nets()) {
        size_t num_f = 0;
        size_t num_t = 0;
        for(auto&& j : i -> get_cells()) {
            //calculate F(n)
            if(j -> get_position() == pos) {
                num_f++;
            }
            else {
                num_t--;
            }
        }
        if(num_f == 1) {gain++;}
        if(num_t == 0) {gain--;}
    }
    return gain;
    //c ->set_gain(gain);
}

void Circuit::_init_gain_bucket() {

    //init bucketlist for both partitions
    _bucket_list_a.clear();
    _bucket_list_b.clear();
    _gain_list.clear();
    _locked_cell.clear();
    for(int i = _max_gain * -1; i <= _max_gain; ++i) {
        Cell* new_cell = NULL;
        _bucket_list_a.push_back(new_cell);
        _bucket_list_b.push_back(new_cell);
    }

    for(auto&& i : _all_cells) {
        i.second ->set_iflocked(false);
        i.second ->set_prev(NULL);
        i.second ->set_next(NULL);
    }

    for(auto&& i : _all_cells) {
        int gain = _compute_init_gain(i.second);
        i.second->set_gain(gain);
        _add_to_bucket(i.second);
    }
}

//check area constrain and return max _gain
Cell* Circuit::_find_max_gain_cell() {
    for(int i = 1; i <= _bucket_list_a.size(); ++i) {
        Cell* current_a_cell = _bucket_list_a[_bucket_list_a.size() - i];
        Cell* current_b_cell = _bucket_list_b[_bucket_list_b.size() - i];
        while( current_a_cell != NULL && current_b_cell != NULL){
            if(current_a_cell != NULL) {
                if(_check_balance(current_a_cell)) {return current_a_cell;}
                else {
                    current_a_cell->set_iflocked(true);
                    _remove_from_bucket(current_a_cell);
                }
            }
            if(current_b_cell != NULL) {
                if(_check_balance(current_b_cell)) {return current_b_cell;}
                else {
                    current_b_cell->set_iflocked(true);
                    _remove_from_bucket(current_b_cell);
                }
            }
            current_a_cell = _bucket_list_a[_bucket_list_a.size() - i];
            current_b_cell = _bucket_list_b[_bucket_list_b.size() - i];
        }
    }
    return NULL;
}

void Circuit::_remove_from_bucket(Cell* cell) {

    int pos = cell->get_position();

    Cell* head = NULL;
    //the first one and only one
    if(pos == 0) {
        head = _bucket_list_a[cell->get_gain() + _max_gain];
    }

    else {
        head = _bucket_list_b[cell->get_gain() + _max_gain];
    }

    if(head == NULL && cell == NULL) {
        return;
    }

    if(head != NULL && head->get_name() == cell->get_name()) {
        if(pos == 0) { _bucket_list_a[cell->get_gain() + _max_gain] = cell->get_next();}
        else{ _bucket_list_b[cell->get_gain() + _max_gain] = cell->get_next();}

    }
    if(cell->get_next() != NULL) {
        cell->get_next()->set_prev(cell->get_prev());
    }

    if(cell->get_prev() != NULL) {
        cell->get_prev()->set_next(cell->get_next());
    }
    cell->set_prev(NULL);
    cell->set_next(NULL);

    return;

}

void Circuit::_add_to_bucket(Cell* cell) {
    int idx = cell -> get_gain() + _max_gain;
    //check cell partition
    if(cell -> get_position() == 0) {
        Cell* current = _bucket_list_a[idx];
        cell->set_next(current);
        cell->set_prev(NULL);
        //add to the front
        if(current != NULL) {
            current->set_prev(cell);
            //current->set_prev(cell);
        }
        _bucket_list_a[idx] = cell;
    }
    if(cell -> get_position() == 1) {
        Cell* current = _bucket_list_b[idx];
        cell->set_next(current);
        cell->set_prev(NULL);
        //add to the front
        if(current != NULL) {
            current->set_prev(cell);
            //current->set_prev(cell);
        }
        _bucket_list_b[idx] = cell;
    }
}

void Circuit::_update_gain(Cell* c) {
    int pos = c -> get_position();

    for(auto&& i : c -> get_nets()) {
        size_t num_f = 1;
        size_t num_t = 0;
        for(auto&& j : i -> get_cells()) {
            if(j -> get_name() != c -> get_name()) {
                if(j -> get_position() == pos) {
                    num_f++;
                }
                else {
                    num_t++;
                }
            }

        }

        if(num_t == 0) {
            for(auto&& j : i -> get_cells()) {
                //increment gain by 1
                if(j -> get_iflocked() == false) {
                    _remove_from_bucket(j);
                    j ->set_gain(j->get_gain() + 1);
                    _add_to_bucket(j);
                }
            }
        }

        else if(num_t == 1) {
            for(auto&& j : i -> get_cells()) {
                //increment gain by 1
                if (j->get_iflocked() == false && j->get_position() != pos) {
                    _remove_from_bucket(j);
                    j->set_gain(j->get_gain() - 1);
                    _add_to_bucket(j);
                }
            }
        }

        //after move
        num_f--;
        num_t++;
        if(num_f == 0) {
            for(auto&& j : i -> get_cells()) {
                //increment gain by 1
                if(j -> get_iflocked() == false) {
                    _remove_from_bucket(j);
                    j ->set_gain(j->get_gain() - 1);
                    _add_to_bucket(j);
                }
            }
        }

        else if(num_f == 1) {
            for(auto&& j : i -> get_cells()) {
                //increment gain by 1
                if (j->get_iflocked() == false && j->get_position() == pos) {
                    _remove_from_bucket(j);
                    j->set_gain(j->get_gain() - 1);
                    _add_to_bucket(j);
                }
            }
        }

    }
}

bool Circuit::_check_balance(Cell* cell_to_be_move) {

    size_t new_a_cells = _cells_in_a;
    size_t new_b_cells = _cells_in_b;

    if(cell_to_be_move->get_position() == 0) {
        new_a_cells--;
        new_b_cells++;
    }

    else if(cell_to_be_move->get_position() == 1) {
        new_a_cells++;
        new_b_cells--;
    }
    float left_constrain = static_cast<float>(_num_cells) * (1.0 - static_cast<float>(_r)) / 2;
    float right_constrain = static_cast<float>(_num_cells) * (1.0 + static_cast<float>(_r)) / 2;
    //going to move to partition B
    if(cell_to_be_move -> get_position() == 0) {
        if(new_a_cells > left_constrain && new_b_cells < right_constrain) {
            return true;
        }
    }
    //going to move to partiton A
    else {
        if(new_b_cells > left_constrain && new_a_cells < right_constrain) {
            return true;
        }
    }

    return false;
}

void Circuit::_move_cell(Cell* cell_to_be_move) {
    //find from the partition and move it and lock it
    int pos = cell_to_be_move->get_position();
    if(pos == 0) {
        cell_to_be_move->set_poisition(1);
        _cells_in_b++;
        _cells_in_a--;
    }

    else if(pos == 1) {
        cell_to_be_move->set_poisition(0);
        _cells_in_b--;
        _cells_in_a++;

    }
}

size_t Circuit::compute_cutsize() const {

    int num_cutnet = 0;

    for(auto&& i : _all_nets) {
        int num_a = 0;
        int num_b = 0;
        for(auto&& j : i.second -> get_cells()) {
            if(j -> get_position() == 0) {num_a++;}
            else {num_b++;}
            if(num_a != 0 && num_b != 0) {
                num_cutnet++;
                break;
            }
        }
    }
    return num_cutnet;
}

size_t Circuit::get_partition_a_size() const {
    return _cells_in_a;
}

size_t Circuit::get_partition_b_size() const {
    return _cells_in_b;
}

std::list<Cell*> Circuit::get_partition_a_list() const {
    std::list<Cell*> cells_in_a;
    int in_a = 0;
    for(auto&& i : _all_cells) {
        if(i.second -> get_position() == 0) {
            cells_in_a.push_back(i.second);
            in_a++;
        }
    }
    return cells_in_a;
}

std::list<Cell*> Circuit::get_partition_b_list() const {
    std::list<Cell*> cells_in_b;
    for(auto&& i : _all_cells) {
        if(i.second -> get_position() == 1) {
            cells_in_b.push_back(i.second);
        }
    }
    return cells_in_b;
}

size_t Circuit::_find_max_index(std::vector<int> int_list) {
    size_t index = -1;
    int max = -1;
    for(size_t i = 0; i < int_list.size(); ++i) {
        if(int_list[i] >= max) {
            max = int_list[i];
            index = i;
        }
    }

    return index;
}

void Circuit::retrive_best() {
    size_t index = _find_max_index(_gain_list);
    //undo moving after _index
    //get node
    std::vector<Cell*> undo_cell = {_locked_cell.begin() + index + 1, _locked_cell.end()};
    for(size_t i = 0; i < undo_cell.size(); ++i) {
        _move_cell(undo_cell[i]);
    }
}

void Circuit::fm_pass() {

    _init_gain_bucket();

    std::cout << "finish init _gain bucket" << std::endl;
    int accum_gain = 0;
    //find max _gain at each side
    while(_locked_cell.size() < _num_cells) {
        Cell* max_cell = _find_max_gain_cell();

        if(max_cell != NULL) {
//            if(max_cell->get_name() == "c122066") {
//                std::cout << "large " << std::endl;
//            }
            //std::cout << "lock cell " << max_cell -> get_name() << std::endl;
            //std::cout << "lock cell " << _locked_cell.size() << std::endl;
            _remove_from_bucket(max_cell);
            max_cell->set_iflocked(true);
            accum_gain += max_cell->get_gain();
            _update_gain(max_cell);
            _move_cell(max_cell);
            _locked_cell.push_back(max_cell);
            _gain_list.push_back(accum_gain);
        }
        else {
            break;
        }
    }
}

size_t Circuit::min_cutsize(int num_it) {
//    fm_pass();
//    size_t cutsize = compute_cutsize();
//    size_t min_cutsize = 0;
//    int i = 0;
//    while(i < num_it) {
//        fm_pass();
//        size_t new_cutsize = compute_cutsize();
//        std::cout << "cutsize " << new_cutsize << std::endl;
//        if(new_cutsize < cutsize) {
//            if_cutsize_change = false;
//        }
//
//        else {
//            cutsize = new_cutsize;
//        }
//
//        i++;
//    }
//
//    return cutsize;
}


