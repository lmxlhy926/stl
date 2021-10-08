//
// Created by lhy on 2021/3/20.
//

#include "mmap.h"
#include <algorithm>
#include <functional>

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

        ssmap.erase("a");
        auto ite = ssmap.find("b");
        if (ite != ssmap.end())
            ssmap.erase(ite);

        ssmap.insert(std::make_pair("aa", "aa--"));
        ssmap.insert(std::pair<string, string>("bb", "bb--"));

        auto itebb = ssmap.find("bb");
        ssmap.erase(itebb, ssmap.end());

        for (auto &elem : ssmap) {
            std::cout << "elem_first: " << elem.first << " elem_second: " << elem.second << endl;
        }

    }


    using addFunction = std::function<int(int, int)>;

    void mmap_functor(int a, int b, string& option){

        std::map<string, addFunction> funMap;
        funMap.insert(std::pair<string, addFunction>("add", [](int a, int b) -> int{
            return a + b;
        }));

        funMap.insert(std::pair<string, addFunction>("sub", [](int a, int b) -> int{
            return a - b;
        }));

        funMap.insert(std::pair<string, addFunction>("mul", [](int a, int b) -> int{
            return a * b;
        }));

        auto pairRet = funMap.find(option);
        if(pairRet != funMap.end()){
            int ret = (*pairRet).second(a, b);
            cout << "ret: " << ret << endl;
        }
    }

}
