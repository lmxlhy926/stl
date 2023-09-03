

#include <iostream>
#include <stdio.h>

using namespace std;
int main(void)
{

	string str = "firstfirst";
	string str2;
	str2 = "second";
	
//赋值运算
	string str3 = str;

//求字符串长度
	cout << "str.size: " << str.size() << endl;
	cout << "str2.size: " << str2.size() << endl;

//转换为c字符串
	const char *str4 = str3.c_str();
	printf("str4: %s\n", str4);

	return 0;
}


































