#include <regex>
#include <iostream>
#include "regex.h"


using namespace std;

void test(){
    /*
    * 完整匹配：返回值标识是否匹配成功
    * 查询匹配结果
    */

    smatch sm;
    string str = "prefix<tag>hello<tag>suffix";
    bool ret = regex_search(str, sm, regex(R"(<(.*)>.*<(.*)>)"));
    if(ret){
        std::cout << "匹配成功" << std::endl;
    }

    //打印查询结果信息
    /*
     * 前缀、后缀：
     */
    std::cout << "prefix: " << sm.prefix().str() << "  length: " << sm.prefix().length() << std::endl;
    std::cout << *sm.prefix().first << " " << *(sm.prefix().second - 1) << std::endl;
    std::cout << "suffix: " << sm.suffix().str() << "  length: " << sm.suffix().length() << std::endl;
    std::cout << *sm.suffix().first << "  " << *(sm.suffix().second - 1) << std::endl;

    std::cout << "size: " << sm.size() << std::endl;
    std::cout << "length: " << sm.length() << std::endl;

    //遍历
    for(auto beg = sm.begin(); beg != sm.end(); ++beg){
//        std::cout << beg->str() << " " << beg->length() << " " << *(beg->first) << " " << *(beg->second -1) << std::endl;
        std::cout << *beg << std::endl;
    }
}


int main(int argc, char *argv[]){
    ecmaTest();

    return 0;
}




























