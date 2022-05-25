//
// Created by lhy on 2020/11/19.
//

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]){

    std::ofstream file(argv[1]);
    file << "#define DOUBLEADD" << endl;

    return 0;
}













