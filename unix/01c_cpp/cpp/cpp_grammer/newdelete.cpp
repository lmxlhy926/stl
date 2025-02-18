

#include <iostream>

//申请的时候会调用构造器完成初始化，释放的时候，会调用析构器完成内存的清理

using namespace std;

int main(void)
{
	float a = 3.5;
	int b = static_cast<float>(a);

	cout << b << endl;

	
	return 0;
}


void func1(void)
{
	int *a = new int(3);
	cout << *a << endl;
	delete a;

	string *ps = new string("china");
	cout << *ps << endl;
	delete ps;

	return;
}
































