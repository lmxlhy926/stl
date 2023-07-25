
#include <iostream>
#include <bitset>
#include <complex>

using namespace std;

void output(){
    std::cout << 7 << std::endl;
    std::cout << 4.5 << std::endl;
    std::cout << "hello world" << std::endl;
    std::bitset<10> flags(7);
    std::cout << flags << std::endl;
    std::complex<float> c(3.1, 7.4);
    std::cout << c << std::endl;
}

void input(){
    int i;
    float f;
    std::complex<double> c;
    std::cin >> i >> f >> c;
    std::cout << "read: " << i << " " << f << " " << c << std::endl;
}

void istream_input(){
    int read1 = std::cin.get();
    std::cout << "read1: " << read1 << std::endl;

    char read2;
    std::cin.get(read2);
    std::cout << "read2: " << read2 << std::endl;

    char buf[1024];
    std::cin.get(buf, 1024, '\n');
    std::cout << buf;

    std::cin.getline(buf, 1024, '\n');
    std::cout << buf;

    char readbuf[5];
    std::cin.read(readbuf, 5);
    std::cout << "readbuf: " << string(readbuf);

    std::cin.ignore(5, '\n').read(readbuf, 5);
    std::cout << "readbuf: " << string(readbuf);
}

void ostream_output(){
    std::cout.put('a') << std::endl;
    std::cout.write("hello", 5);
    std::cout.flush();
}



int main(int argc, char* argv[]){
    ostream_output();

    return 0;
}


