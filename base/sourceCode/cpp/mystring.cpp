

#include <iostream>
#include <string.h>
#include <stdio.h>

class Mystring
{
public:
	Mystring(const char *str = NULL);	//带参数默认值的函数声明
	Mystring(const Mystring & other);	//利用对象产生一个新的对象时，用拷贝构造
	Mystring & operator=(const Mystring & another);	//用对象给已有的对象赋值。
	
	Mystring & operator+(const Mystring & other);
	void dis();
	~Mystring();
	
private:
	char *_str;
};


//实现
Mystring::Mystring(const char *str)
{
	printf("constructor\n");
	
	if(str == NULL)
	{
		this->_str = new char[1];
		*this->_str = '\0';
	}else
	{
		int len = strlen(str);
		this->_str = new char[len + 1];
		strcpy(this->_str, str);
	}
}

Mystring::Mystring(const Mystring & other)
{
	printf("copy constructor\n");
	int len = strlen(other._str);
	this->_str = new char[len + 1];
	strcpy(this->_str, other._str);

	
}

Mystring & Mystring::operator=(const Mystring & another)
{
	printf("operator=\n");
	if(this == &another){
		return *this;
	}else{
		delete []this->_str;
		int len = strlen(another._str);
		strcpy(this->_str, another._str);
		return *this;
	}
}

void Mystring::dis()
{
	printf("%s\n", this->_str);
}

Mystring::~Mystring()
{
	delete []  this->_str;
}

int main(void)
{
/*
	Mystring s = "china";	//构造器
	Mystring s1("china");	//构造器
	s.dis();
	s1.dis();
*/


/*
	Mystring s("china");	//构造器
	Mystring s1 = s;	//拷贝构造
	Mystring s2(s);		//拷贝构造
*/


/*
	Mystring s("china");	//构造
	Mystring s1("japan");	//构造
	s = s1;	//赋值
*/

	Mystring s;	//构造
	s = "america";	//无名对象的构造，赋值

	return 0;
}











































