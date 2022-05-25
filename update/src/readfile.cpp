
#include <iostream>
#include <fstream>



void readFromFile(){
    std::shared_ptr<ifstream> ifsptr(new ifstream(R"(D:\project\byjs\middlewareupdate\out\a.txt)", std::ios::in | std::ios::binary));
    char data[1024]{};
    int readCount;
    while((readCount = ifsptr->readsome(data, 1024 - 1)) != 0){
        data[readCount] = 0;
        std::cout << "read: " << data << endl;
    }
    std::cout << "====>read complete" << std::endl;
}


void readFromFileC(){

    std::shared_ptr<ifstream> ifsptr(new ifstream(R"(D:\project\byjs\middlewareupdate\out\picture.zip)", std::ios::in | std::ios::binary));
    ofstream ofs(R"(D:\project\byjs\middlewareupdate\out\download.zip)", std::ios::app | std::ios::binary);
    char c;
    while(ifsptr->get(c)){
        ofs.write(&c, 1);
    }
}







