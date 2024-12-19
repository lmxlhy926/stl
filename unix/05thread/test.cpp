
#include <mutex>
#include <condition_variable>
#include <future>
#include <vector>

using namespace std;


std::mutex mutex_;
std::condition_variable cond;
std::vector<int> vec;


void provider(){


}


void consumer(){
    

}


int main(int argc, char* argv[]){
    std::async(std::launch::async, provider);
    std::async(std::launch::async, consumer);

    return 0;
}






