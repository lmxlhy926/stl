//
// Created by lhy on 2021/1/23.
//

#include "overload.h"
#include <cstring>

namespace fundamental{

#if 0
   MyString::MyString(const char *str) {
       if(str == nullptr){
           str_ = new char[1];
           *str_ = '\0';
       }else{
           int len = strlen(str);
           str_ = new char[len + 1];
           strcpy(str_, str);
       }
   }

   MyString::~MyString(){
       delete(str_);
   }

    MyString::MyString(const MyString &other) {
        int len = strlen(other.str_);
        this->str_ = new char[len + 1];
        strcpy(this->str_, other.str_);
    }

    MyString &MyString::operator=(const MyString &other) {
        if(this == &other){
            return *this;
        }else{
            delete(str_);
            int len = strlen(other.str_);
            str_ = new char[len + 1];
            strcpy(str_, other.str_);
            return *this;
        }
    }

    MyString MyString::operator+(const MyString &other) {
       int len = strlen(this->str_) + strlen(other.str_);

       MyString str;
       delete str.str_;

       str.str_ = new char[len + 1];
       memset(str.str_, 0, len + 1);

       strcat(str.str_, this->str_);
       strcat(str.str_, other.str_);

       return str;
    }

    bool MyString::operator==(const MyString& other){
        return strcmp(this->str_, other.str_) == 0;
    }

    char& MyString::operator[](int index){
        return str_[index];
    }
#endif

}