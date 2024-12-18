
#include <iostream>
#include <stdio.h>

using namespace std;

/*
io对象是缓冲的，刷新缓冲的几种情况
	1. 程序正常结束，作为main函数结束的一部分，将清空所有缓冲区。
	2. 缓冲区满，则会刷新缓冲
	3. endl,flush也会刷缓冲


*/
void func1();
void func2();
void func3();
void func4();
void func5();
void func6();
void func7();


int main(void)
{
	
	func7();

	return 0;
}

void func1()
{
	cout << "alsjfl;lasjfa;lsjfal";
	while(1);
	return;
}

void func2()
{
	cout.put('c');
	cout.put('d');
	cout << endl;

	return;
}

void func3()
{
	int a;
	double b;
	char buf[1024];

	cin >> a >> b >> buf;
	cout << a << endl;
	cout << b << endl;
	cout << buf << endl;

	return;
}


void func4()
{
	char ch;
	while((ch = cin.get()) != EOF)
		cout.put(ch);
	
	return;
}

void func5()
{
	char ch;
	while(cin.get(ch))
		cout.put(ch);

	return;
}

void func6()
{
//istream& get(char *, int ,char )
	char str[80];
	cin.get(str,10,'a');
	cout << str << endl;

	return;
}


void func7()
{
//istream& getline(char *, int , char)
/*
	get和getline的区别是：get遇到界定符时停止执行，但是并不从流中提取界定符，再次调用
会遇到同一个界定符，函数立即返回，不会提取输入。getline则不同，他将从输入流中提取
界定符，但是仍然不会把它放到缓冲区中。

*/



	char buf[1024];

//	cin.get(buf, 1024, 'i');
//	cout << buf << endl;
//	cin.get(buf, 1024, 'i');
//	cout << buf << endl;

	cin.getline(buf, 1024, 'i');
	cout << buf << endl;
	cin.getline(buf, 1024, 'i');
	cout << buf << endl;


	return;
}

















































