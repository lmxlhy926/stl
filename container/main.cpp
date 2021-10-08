
#include <iostream>
#include "mvector.h"
#include "mset.h"
#include "mmap.h"

using namespace std;

int main(int argc, char* argv[]){

    string option = "mul";
    container::mmap_functor(100, 200, option);
    cout << "----" << endl;

    return 0;

}








