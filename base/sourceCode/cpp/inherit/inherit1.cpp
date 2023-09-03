

#include <iostream>
using namespace std;

/*
派生类的构造器默认会自动调用父类的空参构造器，可以显示的指定调用的重载构造器。
注意调用父类构造器时的位置。

*/

/*
继承的类中每个成员的全称，有类名限定。

*/


class A
{
public:
	A(int m, int n):a(m),b(n){
		cout << "in A constructor" << endl;
	}
	A(const A& another)
	{
		this->a = another.a;
		this->b = another.b;
	}
	void dis()
	{
		cout << this->a << endl;
		cout << this->b << endl;
	}
	
private:
	int a;
	int b;
};


class B:public A
{
public:
	B(int m, int n, int p):A(m,n),c(p){
		cout << "in B constructor" << endl;
	}
	B(const B& another):A(another)
	{
		this->c = another.c;
	}

	void dis()
	{
		A::dis();
		cout << this->c << endl;;
	}
	
private:
	int c;

};



//赋值运算符
class student
{
public:
	student(int m):a(m){}
	student &operator=(const student & another)
	{
		this->a = another.a;
		return *this;
	}
	void dis()
	{
		cout << this->a << endl;
	}
	
private:
	int a;
};

class graduate:student
{
public:
	graduate(int m, int n):student(m),b(n){}
	graduate &operator=(const graduate & another)
	{
		if(this == &another)
			return *this;
		
		student::operator=(another);
		this->b = another.b;
		return *this;
	}
	void dis()
	{
		student::dis();
		cout << this->b << endl;
	}
	
private:
	int b;
};




int main(void)
{
	B b(1,2,3);
	b.dis();
/*
in A constructor
in B constructor
1
2
3
*/

	// graduate g1(1,2);
	// g1.dis();
	// graduate g2(4,5);
	// g2.dis();
	// cout << "--------" << endl;
	// g2 = g1;
	// g2.dis();

	return 0;
};

























































