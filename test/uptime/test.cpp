
#include <sys/sysinfo.h>
#include <stdio.h>
#include <map>
#include <set>
#include <string>
#include <iostream>

using namespace std;

void uptime(){
    struct sysinfo si;
    sysinfo(&si);
    long uptime = si.uptime;
    printf("系统已经运行: %ld秒\n", uptime);
}

int main(int argc, char* argv[]){
    std::map<string, std::set<string>> mapSet{
        {"hello", {"1", "2", "3"}}
    };

    for(auto elem : mapSet){
        int size = elem.second.size();
        std::cout << elem.first << std::endl;
        for(auto interElem : elem.second){
            std::cout << "--: " << interElem << std::endl;
        }

    }




    return 0;
}


















