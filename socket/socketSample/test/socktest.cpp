//
// Created by 78472 on 2022/9/12.
//


#include <vector>
#include <iostream>

using namespace std;
int main(int argc, char* argv[]){

    std::vector<int> vec = {1, 2, 3, 4};
    for(auto pos = vec.begin(); pos != vec.end();){
        std::cout << *pos << std::endl;
        pos = vec.erase(pos);
    }
    std::cout << "vec.size(): " << vec.size() << std::endl;


    return 0;
}





























