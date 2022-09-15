#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <chrono>
#include "Cell.h"
#include "Net.h"
#include "Circuit.h"
#include "vector"
#include <random>
//bucklist struct


int main(int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();
    Circuit* circult = new Circuit();
    circult->create_graph(argv[1]);
    std::cout << "finish graph " << std::endl;
    circult->init_partition();
    std::cout << "finish init " << std::endl;
    std::cout << "init Cutsize= " << circult->compute_cutsize() << std::endl;
    circult->fm_pass();
    std::cout << "finish fm " << std::endl;
    circult->retrive_best();
    std::ofstream out_file;
    out_file.open(argv[2]);
    out_file << "Cutsize = " << circult->compute_cutsize() << "\n";
    out_file << "G1 " << circult->get_partition_a_size() << "\n";
    std::list<Cell*> partition_a_list = circult->get_partition_a_list();
    for(auto&& i : partition_a_list) {
        out_file << i->get_name() << " ";
    }
    out_file << ";\n";
    out_file << "G2 " << circult->get_partition_b_size() << "\n";
    std::list<Cell*> partition_b_list = circult->get_partition_b_list();
    for(auto&& i : partition_b_list) {
        out_file << i->get_name() << " ";
    }
    out_file << ";\n";
    out_file.close();

    delete(circult);
    //auto stop = std::chrono::high_resolution_clock::now();
   //auto duration = duration_cast<std::chrono::microseconds>(stop - start);
    //std::cout << duration.count() << std::endl;
    return 0;
}
