//
// Created by 78472 on 2022/1/10.
//

#include "reqtest.h"
#include <iostream>
#include <regex>

using namespace std;


/*
 * 正则表达式：正则表达式程序库regex，它允许你使用通配符（wildcard）和pattern来查找和替换string中的字符
 *      Match：将整个输入拿来比对某个正则表达式
 *      Search：查找“与正则表达式吻合”的pattern
 *      Replace：将与正则表达式吻合之第一个（或后续所有）子序列替换掉
 *      Tokenize：根据“被指定为正则表达式”的切分器（separator）取得语汇单元（token）
 */

void out(bool b){
    cout << (b ? "found" : "not found") << endl;
}


/*
 * Match：将整个输入拿来比对某个正则表达式
 * 如果匹配成功
 *      1. 没有前缀和后缀
 *      2. "匹配合格之整体string"或"第n个匹配合格之substring"的内容、位置、长度
 */
void match(){
    string str1 = ":<tag>value</tag>:";
    smatch sm;
    //整体匹配，只关心结果
    auto found = regex_match(str1,regex(R"(<(.*).*</\1>)"));
    out(found);     //not match

    //整体匹配，分析匹配结果
    found = regex_match(str1, sm,regex(R"(.*<(.*)>(.*)</(\1)>.*)"));
    if(found){
        cout << "prefix: " << sm.prefix().str() << endl;
        cout << "suffix: " << sm.suffix().str() << endl;

        //完整匹配
        cout << "match: " << sm.str() << endl;
        cout << "length: " << sm.length() << endl;
        cout << "position: " << sm.position() << endl;

        //分组信息
        cout << "str(0): " << sm.str(0) << " length: " << sm.length(0) << " position: " << sm.position(0) << endl;
        cout << "str(1): " << sm.str(1) << " length: " << sm.length(1) << " position: " << sm.position(1) << endl;
        cout << "str(2): " << sm.str(2) << " length: " << sm.length(2) << " position: " << sm.position(2) << endl;
        cout << "str(3): " << sm.str(3) << " length: " << sm.length(3) << " position: " << sm.position(3) << endl;
    }
}



/*
 * typedef match_results<string::const_iterator>  smatch;
 * match_results对象作为一个整体来看，提供了
 *      成员函数str()：“匹配合格之整体string”或“第n个匹配合格之substring”
 *      成员函数length()：“匹配合格之整体string的长度”或“第n个匹配合格之substring的长度”
 *      成员函数position()：“匹配合格之整体string的位置”或“第n个匹配合格之substring的位置”
 *      成员函数begin()、cbegin()、end()和cend()可用来迭代sub_match对象，从m[0]到m[n]
 *
 * sub_match对象：派生自pair<>
 *      first成员是第一字符的位置，second成员是最末字符的下一个位置
 *      str()：以string形式取得字符
 *      length()：可取得字符数量
 *      operator << 将字符写至某个stream
 *
 */
void regex_search(){
    string data = "****:-<tag>value</tag>.";
    cout << "data: " << data << std::endl;

    smatch m;
    bool found = regex_search(data, m, regex("<(.*)>(.*)</(\\1)>"));

    //是否有匹配项，有几个匹配项
    cout << "m.empty(): " << boolalpha << m.empty() << endl;
    cout << "m.size(): " << m.size() << endl;

    if(found){
        cout << "m.prefix().str(): " << m.prefix().str() << endl;
        cout << "m.suffix().str(): " << m.suffix().str() << endl;

        cout << "m.str(): " << m.str() << endl;             //匹配合格之整体string
        cout << "m.length(): " << m.length() << endl;       //匹配合格之整体string的长度
        cout << "m.position(): " << m.position() << endl;   //匹配合格之整体string的位置
    }
    cout << endl << endl;

    //下标遍历方式
    for(int i = 0; i < m.size(); i++){
        cout << "m.str(" << i << "): "          << m.str(i) << endl;        //第n个匹配合格之substring
        cout << "m[" << i << "].length(): "     << m.length(i) << endl;     //第n个匹配合格之substring的长度
        cout << "m.position(" << i << "): "     << m.position(i) << endl;   //第n个匹配合格之substring的位置
    }
    cout << endl << endl;

    //迭代方式
    for(auto pos = m.begin(); pos != m.end(); ++pos){   //迭代sub_match对象，从m[0]到m[i]
        cout << *pos << std::endl;
        cout << "str: " << pos->str() << endl;
        cout << "length: " << pos->length() << endl;
        cout << "first: " << *pos->first << " second: " << *(pos->second - 1) << std::endl;
        cout << "-----------------------------" << std::endl;
    }
    cout << std::endl;

    //迭代方式
    for(auto & elem : m){
        cout << elem << std::endl;
        cout << "str: " << elem.str() << endl;
        cout << "length: " << elem.length() << endl;
        cout << "first: " << *(elem.first) << " second: " << *(elem.second - 1) << std::endl;
        cout << "*****************************" << std::endl;
    }
}


void regex_search1(){
    string data = "<person>\n"
                  "<first>Nico</first>\n"
                  "<last>Josuttis</last>\n"
                  "</person>\n";
    cout << "data: " << data << endl;

    regex reg("<(.*)>(.*)</(\\1)>");
    smatch m;

    //字符串迭代器
    string::const_iterator pos = data.cbegin();
    string::const_iterator end = data.cend();

    //搜索时，变化起始位置，起始位置更新为后缀位置；
    for(; regex_search(pos, end, m, reg); pos = m.suffix().first){
        cout << "match: " << m.str() << endl;
        cout << "tag: " << m.str(1) << endl;
        cout << "value: " << m.str(2) << endl;
    }
}


/*
 *  regex函数试图在一种所谓greedy风格下操作，试图完成最长匹配
 */
void regex_search_maxlong(){
    string data = "<person>"
                  "<first>Nico</first>"
                  "<last>Josuttis</last>"
                  "</person>";
    cout << "data: " << data << endl;

    regex reg("<(.*)>(.*)</(\\1)>");
    smatch m;

    string::const_iterator pos = data.cbegin();
    string::const_iterator end = data.cend();
    bool flag = regex_search(pos, end, m, reg);
    if(flag){
        cout << "match: " << m.str() << endl;
        cout << "tag: " << m.str(1) << endl;
        cout << "value: " << m.str(2) << endl;
    }


    for(; regex_search(pos, end, m, reg); pos = m.suffix().first){
        cout << "match: " << m.str() << endl;
        cout << "tag: " << m.str(1) << endl;
        cout << "value: " << m.str(2) << endl;
    }


    if(end == m.suffix().first){
        cout << "true---true" << std::endl;
    }
}


/*
 * 为了逐一迭代正则查找的所有匹配成果，可以使用regex_iterator, 类型为regex_iterator<>
 * typedef regex_iterator<string::const_iterator>  sregex_iterator;
 * 其default构造函数定义出一个指向“末尾之下一位置的iterator”
 * bidirectional iterator
 */
void regex_iterator(){
    string data = "<person>\n"
                  "<first>Nico</first>\n"
                  "<last>Josuttis</last>\n"
                  "</person>\n";
    cout << "data: " << data << endl;

    regex reg("<(.*)>(.*)</(\\1)>");

    sregex_iterator pos(data.cbegin(), data.cend(), reg);
    sregex_iterator end;
    for(; pos != end; ++pos){
        cout << "match: " << pos->str() << endl;
        cout << "tag: " << pos->str(1) << endl;
        cout << "value: " << pos->str(2) << endl;
    }
    cout << endl << endl;

    sregex_iterator beg(data.cbegin(), data.cend(), reg);
    for_each(beg, end, [](const smatch& m){
        cout << "match: " << m.str() << endl;
        cout << "tag: " << m.str(1) << endl;
        cout << "value: " << m.str(2) << endl;
    });
}


/*
 * 将string拆分为一个个语汇单元，或以某个东西分割string， 分隔符可以被指定为一个正则表达式
 * typedef regex_token_iterator<string::const_iterator>  sregex_token_iterator;
 * 初始化sregex_token_iterator：字符序列的起点和终点，一个正则表达式，一列整数值
 *      -1： 匹配之正则表达式之间的子序列
 *      0：  匹配之正则表达式
 *      n：  第n个匹配次表达式
 */
void regex_token(){
    string data = "<person>\n"
                  "<first>Nico</first>\n"
                  "<last>Josuttis</last>\n"
                  "</person>\n";
    cout << "data: " << data << endl;

    regex reg("<(.*)>(.*)</(\\1)>");
    sregex_token_iterator pos(data.cbegin(), data.cend(), reg, {0, 1, 2});
    sregex_token_iterator end;
    for(; pos != end; ++pos){
        cout << "match: " << pos->str() << " length: " << pos->length() << endl;
    }
    cout << endl;

    string names = "nico"
                   ", jim , helmut  , paul; tim ; john pual. rita";
    regex sep("[ \t\n]*[,;.][ \t\n]*");  //(,;.)以及前后环绕的任意空白字符
    sregex_token_iterator pos1(names.cbegin(), names.cend(), sep, -1);
    for(; pos1 != end; ++pos1){
        cout << "name: " << *pos1 << endl;
    }
}

/*
 * 将“匹配某个正则表达式的字符序列”替换为“另一个字符序列”
 */
void test7(){
    string data = "<person>\n"
                  "<first>Nico</first>\n"
                  "<last>Josuttis</last>\n"
                  "</person>\n";
    cout << "data: " << data << endl;

    regex reg("<(.*)>(.*)</(\\1)>");
    cout << regex_replace(data, reg, R"($1 == "$2")", regex_constants::format_first_only) << endl;
    cout << regex_replace(data, reg, R"($&)", regex_constants::format_first_only) << endl;
}




























