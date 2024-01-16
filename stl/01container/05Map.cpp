//
// Created by lhy on 2021/3/20.
//

#include <map>
#include <string>
#include <iostream>

#include <algorithm>
#include <functional>

using namespace std;

/*
 *  1. 将key/value pair当做元素进行管理，它们可根据key的排序准则自动为元素排序
 *  2. 不可以直接改变元素的key,因为这会破坏正确次序
 */

namespace container {

    void mmap_insert() {

        std::map<string, string> mapConstainer;
        //插入单个元素,4种元素插入方式
        mapConstainer.insert(std::map<string, string>::value_type("a", "a"));
        mapConstainer.insert(decltype(mapConstainer)::value_type("b", "b"));
        mapConstainer.insert(std::pair<const string, string>("c", "c"));
        mapConstainer.insert(std::make_pair("d", "d"));

        //插入初值列, 插入单个，插入多个
        mapConstainer.insert({"e", "3"});
        mapConstainer.insert({
                                     {"f", "f"},
                                     {"g", "g"},
                                     {"h", "h"}
                             });


        //使用for循环，range_based for循环，for_each + lamda打印map元素
        std::map<string, string>::iterator pos;
        for (pos = mapConstainer.begin(); pos != mapConstainer.end(); pos++) {
            cout << "key: " << pos->first << " value: " << pos->second << endl;
        }
        cout << "-----------------------" << endl;

        for (auto &elem : mapConstainer) {
            cout << "key: " << elem.first << " value: " << elem.second << endl;
        }
        cout << "-----------------------" << endl;

        for_each(mapConstainer.begin(), mapConstainer.end(),
                 [](std::pair<const string, string> &elem) {  //明确声明元素类型
                     cout << "key: " << elem.first << " value: " << elem.second << endl;
                 });
    }


    void mmap_erase() {

        std::map<string, string> ssmap;
        ssmap = {{"a", "first"},
                 {"b", "second"},
                 {"c", "third"}};

        ssmap.erase("a");                //删除key等于特定值的元素
        auto ite = ssmap.find("b");
        if (ite != ssmap.end())
            ssmap.erase(ite);               //删除特定位置的元素

        ssmap.insert(std::make_pair("aa", "aa--"));
        ssmap.insert(std::pair<string, string>("bb", "bb--"));

        auto itebb = ssmap.find("bb");
        ssmap.erase(itebb, ssmap.end());    //删除一个区间的元素

        for (auto &elem : ssmap) {
            std::cout << "elem_first: " << elem.first << " elem_second: " << elem.second << endl;
        }

    }


    using mathFunction = std::function<int(int, int)>;

    int sub(int a, int b){
        return a - b;
    }

    class mul{
    public:
        int operator() (int a, int b){
            return a * b;
        }
    };

    void mmap_functor(int a, int b, string& option){

        std::map<string, mathFunction> funMap;
        funMap.insert(std::pair<const string, mathFunction>("add", [](int a, int b) -> int{     //lamda对象
            return a + b;
        }));

        funMap.insert(std::pair<const string, mathFunction>("sub", sub));          //普通函数

        funMap.insert(std::pair<const string, mathFunction>("mul", mul()));     //函数对象

        auto pairRet = funMap.find(option);
        if(pairRet != funMap.end()){
            cout << "ret: " << (*pairRet).second(a, b) << endl;    //dereference迭代器，pair对象元素的访问
        }
    }

}



int main(int argc, char* argv[]){

    std::map<int, string> ismap;
    ismap.insert(std::make_pair(1, "hello"));
    ismap.insert(std::make_pair(2, "world"));
    ismap.insert(std::make_pair(3, "python"));

    int intMax = ismap.begin()->first;
    for(auto& elem : ismap){
        if(elem.first > intMax){
            intMax = elem.first;
        }
    }
    string s = ismap[intMax];
    std::cout << s << std::endl;


    return 0;
}
