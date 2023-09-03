#include<iostream>
using namespace std;

/*
多态：
	1.一个基类，多个基类的派生类
	2.派生类覆写了基类中的成员函数(意味着基类中的某些函数和派生类中的某些函数名称相同但是实现不同)
	3.**指向基类的指针指向了派生类的对象
	4.使用基类指针只能调用基类中存在的函数，如果派生类中存在相应的覆写函数，则实际上调用的是派生类的覆写函数

*/


/*
虚函数：
	虚函数是在基类中使用关键字virtual声明的函数。在派生类中重新定义基类中定义的虚函数时，会告诉
编译器不要静态链接到该函数。而是根据所调用的对象类型来选择调用的函数，这种操作被称为动态链接。

纯虚函数：
	你可能想要在基类中定义虚函数，以便在派生类中重新定义该函数从而更好的适用于对象。但是你在基类
中又不能对虚函数给出有意义的实现，这个时候就会用到纯虚函数。
	例如：virtual int area() = 0; 	=0告诉编译器，函数没有主体，该虚函数是纯虚函数。

*/


/*
抽象：
	抽象把代码分离为接口和实现。所以在设计组件时，必须保持接口独立于实现，这样，如果改变底层实现，
接口也将保持不变。在这种情况下，不管任何程序使用接口，接口都不会受影响，只需要将最新的实现重新编译即可。

封装：
	数据封装是面向对象编程中的把数据和操作数据的函数绑定在一起的一个概念，
	数据封装引出了数据隐藏的概念，数据隐藏通过(public,protected,private)来实现。
	
封装vs抽象：
	数据封装是把数据和操作数据的函数绑定在一起，数据抽象是仅向用户暴露接口而把具体的实现细节隐藏起来。
	
*/


/*
接口：
	接口描述了类的行为和功能，而不需要完成类的特定实现，接口是通过抽象类来实现的。
	
	抽象类：(包含纯虚函数)
		在类中至少有一个函数被声明为纯虚函数，这个类则为抽象类。设计抽象类的目的是为了给其它类提供一个
	可以继承的适当的基类。抽象类不能被用于实例化对象，它只能作为接口使用。**如果一个抽象类的子类需要被实例化，
	则必须实现每个虚函数。

	具体类：
		不包含纯虚函数的类，可以用于实例化对象。

	**面向对象的系统可能会使用一个抽象基类为所有的外部应用程序提供一个适当的、通用的、标准化的接口。然后，
	派生类通过继承抽象基类，就把所有类似的操作都继承下来。外部应用程序提供的功能(即公有函数)在抽象基类中是以
	纯虚函数的形式存在的。**这些纯虚函数在相应的派生类中被实现。

*/

class Shape
{
public:
	int width;
	int height;
public:
	Shape(int a, int b)
	{
		this->width = a;
		this->height = b;
	}

/*
	在基类中将成员函数声明为虚函数，当使用指针访问对象的成员函数时，编译器将会去看指针的实际内容
而不是它的类型。由于指针存储的是派生类对象的地址，所以会调用派生类的函数。
*/
	virtual int area()		
	{
		cout << "parent class area :" << endl;
		return 0;
	}

};
	
class Rectangle:public Shape
{
public:
	int height;
public:
	Rectangle(int a = 0, int b = 0):Shape(a,b)
	{
		height = 100;
	}
	int area()
	{
		cout << "Rectangle class area :" << endl;
		return(width*height);
	}
	int area1()
	{
		cout << "Rectangle class area1 :" << endl;
		return(width*height);
	}
};




class Shape1
{
protected:
	int a;
	int b;
public:
	Shape1(int a, int b)
	{
		this->a = a;
		this->b = b;
	}
	virtual int sum() = 0;
};

class ShapeSon1:Shape1
{
public:
	ShapeSon1(int a, int b):Shape1(a,b)
	{
	
	}
	int sum()
	{
		return a+b;
	}

};

class ShapeSon2:Shape1
{
public:
	ShapeSon2(int a, int b):Shape1(a,b)
	{

	}
	virtual int sum()
	{
		return a*b;
	}

};



int main(void)
{

	Shape *shape = NULL;	//指向基类的指针
	Rectangle R1(2,5);		//派生类对象
	shape = &R1;			//基类指针指向派生类对象
	shape->area();			//该形式只能调用基类中存在的函数，如果派生类中覆写了该函数，则实际调用的是派生类中的函数。
	cout << "------------------------------------------\n";

	ShapeSon1 ss1(1,2);
	cout << ss1.sum() << endl;
	ShapeSon2 ss2(1,2);
	cout << ss2.sum() << endl;

}

























































