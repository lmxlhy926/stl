//
// Created by 78472 on 2022/1/10.
//

#ifndef STL_REGEX_H
#define STL_REGEX_H


//完整匹配
void fullMatch();

//用整体字符串作为参数搜索
void regex_search();

//用字符串迭代器标示的位置进行搜索
void regex_search_iterator();

//匹配时，尝试匹配最长的匹配项
void regex_search_maxlong();

//迭代器访问一个字符串中所有匹配的match_results
void regex_iterator_test();

//用正则表达式切分字符串
void regex_token_iterator_test();

void regex_replace_test();

void ecmaTest();

#endif //STL_REGEX_H
