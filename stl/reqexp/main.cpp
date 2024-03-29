#include <regex>
#include <iostream>
using namespace std;


/*
 * 正则表达式：正则表达式程序库regex，它允许你使用通配符（wildcard）和pattern来查找和替换string中的字符
 *      Match：将整个输入拿来比对某个正则表达式
 *      Search：查找“与正则表达式吻合”的pattern
 *      Replace：将与正则表达式吻合之第一个（或后续所有）子序列替换掉
 *      Tokenize：根据“被指定为正则表达式”的切分器（separator）取得语汇单元（token）     
 */


/**
 * sub_match对象：派生自pair<>
 *		str()：以string形式取得字符
 *		length()：可取得字符数量
 *		first成员是第一字符的迭代器，second成员是最末字符的下一个位置
 *		operator << ：将字符写至某个stream
 *
 * Class std::match_results<>是个template，必须依据其所处理的字符的iterator类型而实例化。
 * typedef match_results<string::const_iterator>  smatch;   针对string类型
 * match_results<>内含的sub_match对象：
 *      prefix()
 *		suffix()
 *		sub_match对象m[0]：表现"匹配合格之所有字符"
 *		sub_match对象m[n]：对于任何capture group，你可以访问一个对应的sub_match对象m[n]
 * 
 *
 * match_results对象作为一个整体来看，提供了如下方法**
 *		size()：sub_match对象的个数，但是不包括prefix、suffix。
 *		str()：“匹配合格之整体string”或“第n个匹配合格之substring”
 *		length()：“匹配合格之整体string的长度”或“第n个匹配合格之substring的长度”
 *		position()：“匹配合格之整体string的开始位置”或“第n个匹配合格之substring的开始位置”
 *		begin()、cbegin()、end()、cend()：可用来迭代sub_match对象，从m[0]到m[n]
 *
 */
void fullMatch(){
    string str1 = ":<tag中>value</tag中>:";
    if(regex_match(str1, regex(R"(<(.*)>.*</\1>)"))){     //字符串整体是否匹配某个指定的模式
        std::cout << "match" << std::endl;
    }

    //整体匹配，分析匹配结果
    smatch sm;
    bool matchResult = regex_match(str1, sm, regex(R"(.*<(.*)>(.*)</\1>.*)"));   //size = 3,
    if(matchResult){
        cout << "prefix: "      << sm.prefix()    << endl;    //sub_match对象
        cout << "suffix: "      << sm.suffix()    << endl;    //sub_match对象
        cout << "size: "        << sm.size()      << endl;    //包含的sub_match对象的个数
        std::cout << "------------sm.str(i)----------------" << std::endl;

        for(int i = 0; i < sm.size(); ++i){     //sm作为一个整体来看待, position()指示第几个字符
            std::cout << sm.str(i) << ", " << sm.length(i) << ", " << sm.position(i) << std::endl;
        }
        std::cout << "-------------sm[i].str()---------------" << std::endl;

        for(int i = 0; i < sm.size(); ++i){     //遍历每个sub_match对象
            std::cout << sm[i].str() << ", " << sm[i].length() << ", " << *sm[i].first << ", " << *(sm[i].second - 1) << endl;
        }
        std::cout << "--------------elem.str()--------------" << std::endl;

        for(auto& elem : sm){                    //遍历每个sub_match对象
            std::cout << elem.str() << ", " << elem.length() << ", " << *elem.first << ", " << *(elem.second - 1) << endl;
        }
        std::cout << "---------------pos->str()-------------" << std::endl;

        for(auto pos = sm.begin(); pos != sm.end(); pos++){     //遍历每个sub_match对象
            std::cout << pos->str() << ", " << pos->length() << ", " << *pos->first << ", " << *(pos->second - 1) << endl;
        }
        std::cout << "----------------------------" << std::endl;

        auto start = sm[0].first;
        auto end = sm[2].second;
        for(; start != end; ++start){
            cout << *start << std::endl;
        }
        std::cout << "----------------------------" << std::endl;

        auto group3Start = sm[2].second;
        auto group3End = sm[0].second;
        for(; group3Start != group3End; ++group3Start){
             cout << *group3Start << std::endl;
        }
    }
}


/*
 * regex_search(): 匹配一个子项
 */
void regex_search_single(){
    string data = ".<tag>value</tag>.";
    cout << "data: " << data << std::endl;

    smatch sm;
    bool found = regex_search(data, sm, regex(R"(<(.*)>(.*)</(\1)>)"));

    //是否有匹配项，有几个匹配项
    cout << "m.empty(): " << boolalpha << sm.empty() << endl;
    cout << "m.size(): " << sm.size() << endl;

    if(found){
        cout << "m.prefix(): "    << sm.prefix()     << endl;
        cout << "m.suffix(): "    << sm.suffix()     << endl;
        cout << "m.str(): "       << sm.str()        << endl;   //匹配合格之整体string
        cout << "m.length(): "    << sm.length()     << endl;   //匹配合格之整体string的长度
        cout << "m.position(): "  << sm.position()   << endl;   //匹配合格之整体string的位置
    }
}


/*
 *  regex函数试图在一种所谓greedy风格下操作，试图完成最长匹配
 */
void regex_search_tryMaxlong(){
    string data = "<person>"
                  "<first>Nico</first>"
                  "<last>Josuttis</last>"
                  "</person>";
    cout << "data: " << data << endl;

    smatch sm;
    bool flag = regex_search(data.cbegin(), data.cend(), sm, regex(R"(<(.*)>(.*)</(\1)>)"));
    if(flag){
        cout << "match: " << sm.str() << endl;
        cout << "tag: " << sm.str(1) << endl;
        cout << "value: " << sm.str(2) << endl;
    }

    //如果prefix, suffix为空
    //sm.prefix().first == sm.prefix().second == data.cbegin()
    //sm.suffix().first == sm.suffix().second == data.cend()
    if(sm.prefix().first == sm.prefix().second && sm.prefix().second == data.cbegin()){
        std::cout << "first == second == cbegin" << std::endl;
    }
    if(sm.suffix().first == sm.suffix().second && sm.suffix().second == data.cend()){
        std::cout << "first == second == cend" << std::endl;
    }
}

/*
 * regex_search():
 *      查找字符串的表示：
 *              1. 一个完整的字符串
 *              2. 字符串的起始位置，终止位置
 */
void regex_search_multiple(){
    string data = "<person>\n"
                  "<first>Nico</first>\n"
                  "<last>Josuttis</last>\n"
                  "</person>\n";
    cout << "data: " << data << endl;

    regex reg(R"(<(.*)>(.*)</(\1)>)");
    smatch sm;

    //字符串迭代器
    string::const_iterator beg = data.cbegin();
    //搜索时，变化起始位置，起始位置更新为后缀位置；
    for(; regex_search(beg, data.cend(), sm, reg); beg = sm.suffix().first){
        cout << "prefix: " << sm.prefix() << endl;
        cout << "match: " << sm.str() << endl;
        cout << "tag: " << sm.str(1) << endl;
        cout << "value: " << sm.str(2) << endl;
        cout << "suffix: " << sm.suffix() << endl;
        std::cout << "--------------" << std::endl;
    }
}


/*
 * 为了逐一迭代正则查找的所有匹配成果，可以使用regex_iterator, 类型为regex_iterator<>
 * typedef regex_iterator<string::const_iterator>  sregex_iterator;
 * 其default构造函数定义出一个指向“末尾之下一位置的iterator”
 * 匹配效果：
 *      1. 第一个匹配：匹配整个给定的字符串
 *      2. 后续匹配：从上一个匹配结果的suffix的起点到给定字符串的末尾
 */
void regex_search_iterator(){
    string data = "<person>\n"
                  "<first>Nico</first>\n"
                  "<last>Josuttis</last>\n"
                  "</person>\n";
    cout << "data: " << data << endl;

    regex reg(R"(<(.*)>(.*)</(\1)>)");

    //构造match_result迭代器
    //可以像使用其它bidirectional iterator一样地使用这个iterator。
    sregex_iterator pos(data.cbegin(), data.cend(), reg);
    sregex_iterator end;
    for(; pos != end; ++pos){
        cout << "prefix: " << pos->prefix() << endl;
        cout << "suffix: " << pos->suffix() << endl;
        cout << "match: " << pos->str() << endl;
        cout << "tag: " << pos->str(1) << endl;
        cout << "value: " << pos->str(2) << endl;
        std::cout << "---------------" << std::endl;
    }
    cout << endl << endl;

    //可以把这样的iterator放进STL算法中使用。
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
void regex_search_token_iterator(){
    string data = "<person>\n"
                  "<first>Nico</first>\n"
                  "<last>Josuttis</last>\n"
                  "</person>\n";
    cout << "data: " << data << endl;

    regex reg("<(.*)>(.*)</(\\1)>");

    //迭代submatch对象
    sregex_token_iterator pos(data.cbegin(), data.cend(), reg, {3, 2, 1, 0, -1, 0});
    sregex_token_iterator end;
    for(; pos != end; ++pos){
        cout << "match: " << pos->str() << " length: " << pos->length() << endl;
    }
    std::cout << "---------------------" << std::endl;

    string names = "nico, jim , helmut  , paul; tim ; john pual. rita";
    regex sep("[ \t\n]*[,;.][ \t\n]*");  //(,;.)以及前后环绕的任意空白字符
    sregex_token_iterator pos2(names.cbegin(), names.cend(), sep, {-1});
    for(; pos2 != end; ++pos2){
        cout << "name: " << pos2->str() << ", length: " << pos2->length() 
             << ", " << *(pos2->first) << "-" << *(pos2->second -1) << std::endl;
    }
}


/*
 * 将“匹配某个正则表达式的字符序列”替换为“另一个字符序列”
 * 必须指明一个替代动作，并在其中以字符$表示“匹配之次表达式”
 *      $& ： 整个匹配表达式
 *      $n ： 第n个matched capture group
 *      $
 *
 *  替换执行原理：
 *      1. 执行搜索匹配，找到所有的匹配项（match_results）
 *      2. 按照replacement，替换指定的匹配项。replacement中可以指定前缀、后缀、整体匹配，捕获组
 */
void regex_replace_test(){
    string data = "<person>\n"
                  "<first>Nico</first>\n"
                  "<last>Josuttis</last>\n"
                  "</person>\n";
    cout << "data: " << data << endl;

    regex reg("<(.*)>(.*)</(\\1)>");
    
    cout << regex_replace(data, reg, R"($1 == "$2")", regex_constants::format_first_only) << endl;
    cout << "------------------" << endl;
    cout << regex_replace(data, reg, R"($1 == "$2")") << endl;
    cout << "------------------" << endl;
    cout << regex_replace(data, reg, "hello--world") << endl;
    cout << "------------------" << endl;
    cout << regex_replace(data, reg, R"($&)", regex_constants::format_first_only) << endl;
}


int main(int argc, char *argv[]){
    regex_replace_test();


    return 0;
}




























