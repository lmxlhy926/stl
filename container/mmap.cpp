//
// Created by lhy on 2021/3/20.
//

#include "mmap.h"
#include <algorithm>
#include <functional>

namespace container{

    void mmapTest(){

        std::map<string, string> mapConstainer;
    //插入单个元素
        mapConstainer.insert(std::map<string, string>::value_type("a", "a"));
        mapConstainer.insert(decltype(mapConstainer)::value_type("b", "b"));
        mapConstainer.insert(std::pair<const string, string>("c", "c"));
        mapConstainer.insert(std::make_pair("d", "d"));
    //插入初值列
        mapConstainer.insert({"e", "3"});
        mapConstainer.insert({
                                     {"f", "f"},
                                     {"g", "g"},
                                     {"h", "h"}
                                 });

        cout << "is empty: " << mapConstainer.empty() << endl;
        cout << "size: " << mapConstainer.size() << endl;



    //使用for循环，range_based for循环，for_each + lamda打印map元素
        std::map<string, string>::iterator pos;
        for(pos = mapConstainer.begin(); pos != mapConstainer.end(); pos++){
            cout << "key: " << pos->first << " value: " << pos->second << endl;
        }
        cout << "-----------------------" << endl;


        for(auto &elem : mapConstainer){
            cout << "key: " << elem.first << " value: " << elem.second << endl;
        }
        cout << "-----------------------" << endl;


        for_each(mapConstainer.begin(), mapConstainer.end(),
                [](std::pair<const string, string>& elem){  //明确声明元素类型
            cout << "key: " << elem.first << " value: " << elem.second << endl;
        });
    }

    using functionHandler = std::function<void()>;

    void mmapTest1(){
        std::map<string, functionHandler> fmap;
        fmap.insert(std::pair<const string, functionHandler>("fuc", [](){
            std::cout << "==>fuc\n" << endl;
        }));

//        fmap.find("func").
    }





}