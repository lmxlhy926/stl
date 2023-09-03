

#include <iostream>

/*
	使用的类库之间可能存在同名的函数或者全局变量而产生冲突。
	NameSpace是对全局区域的再次划分

namespace NAMESPACE
{
	全局变量		int a;
	数据类型		struct Stu{};
	函数		void func();
	其它命名空间	namespace
}

	

3种使用方法：
	1.	using namespace space;
	2.	using space::a;	a = 200;
	3.	space::a = 200;

*/


/*
namespace xx
{
	class A
	{
	public:
		A();
		~A();
	};
}

using namespace xx
{
	A::A(){}
	A::~A(){}
}



*/




namespace myspace
{
	int x = 2;
}

namespace otherspace
{
	int x = 3;
}

using namespace std;
int main(void)
{
	
//	{
//		using myspace::x;
//		cout << x << endl;
//
//	}
//
//	{
//		using otherspace::x;
//		cout << x << endl;
//	}

{
	using namespace myspace;
	cout << x << endl;

}

{
	using namespace otherspace;
	cout << x << endl;
}


	return 0;

}













































