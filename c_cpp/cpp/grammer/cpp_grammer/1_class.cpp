#include<iostream>

using namespace std;

/*
类和对象：
	类：它定义了类的对象包括了什么，以及可以在这个对象上执行哪些操作。
		包含了数据的表示和用于处理数据的方法。(将变量和函数打包在一起)
		类中的变量和函数都称为类的成员。		
	对象：类的实体，依据类的描述创建的变量。
	访问：类的对象的公共数据成员可以使用直接成员访问运算符(.)来访问。

类成员函数：
	成员函数可以定义在类定义内部，或者单独使用范围解析运算符::来定义。
	在类定义中定义的成员函数把函数声明为内联的，即便没有使用inline标识符。
*/



/*
	数据封装是面向对象编程的一个重要特点，可以限制对类内部成员的访问。
	访问修饰符：public, private, protected
	每个标记区域在下一个标记区域开始之前或者在遇到类主体结束右括号之前都是有效的。
	public:在程序中类的外部是可以访问的。
	private:在类的外部是不可访问的，只有本类和友元函数可以访问私有成员。默认情况下，类的所有成员都是私有的。
	protected:和private很相似，可以被派生类所访问。

*/

/*
类的构造函数和析构函数：初始化/收尾处理
	构造函数：在每次创建类的新对象时执行 						**先创建对象再执行构造函数进行成员变量的初始化
			  构造函数的名称和类的名称是完全相同的，并且没有返回类型。
			 *主要的作用是为某些成员变量设置初始值，以及执行某些动作

	拷贝构造函数：特殊的构造函数，在创建对象时，使用同一类中之前创建的对象来初始化新创建的对象
				1.通过使用另一个同类型的对象来初始化新创建的对象
				2.复制对象把它作为参数传递给函数
				3.复制对象，并从函数返回这个对象
				如果在类中没有定义拷贝构造函数，编译器会自行定义一个。如果类带有指针变量，并有动态内存分配，
				则它必须有一个拷贝构造函数。
		
			  
	析构函数：在每次删除所创建的对象时执行
			  名称与类的名称完全相同，只是在名称前加个(~)作为前缀。不返回任何值，也不能带有任何参数。
			 *主要作用是跳出程序前释放资源
			 
*/

/*
友元函数：
		类的友元函数定义在类外部，但有权访问类的所有私有成员和保护成员。尽管友元函数的原型有在类定义中
	出现过，但是友元函数并不是成员函数。友元可以是一个函数，该函数被称为友元函数；友元也可以是一个类，该
	类被称为友元类。在这种情况下，整个类及其所有成员都是友元。如果要声明函数为一个类的友元，需要在类
	定义中该函数原型前使用关键字friend。
*/

/*
内联函数
	c++内联函数通常与类一起使用。如果一个函数是内联的，那么在编译时，编译器会把该函数的代码副本放置在每个调用该函数的地方。
	对内联函数进行任何修改，都要重新编译函数的所有客户端，因为编译器需要重新更换一次所有的代码，否则将会继续使用旧的函数。
	在函数名前面放置关键字inline，在调用函数之前需要对函数进行定义。
	在类定义中的定义的函数都是内联函数，即使没有使用inline说明符。


*/

/*
this指针
		在c++中，每个对象都能通过this指针来访问自己的地址。this指针是所有成员函数的隐含参数。因此在成员函数内部，它可以用来
	指向调用对象。友元函数没有this指针，因为友元不是类的成员。只有成员函数才有this指针。
*/


/*
指向类的指针
		一个指向C++类的指针与指向结构的指针类似，使用类的指针访问类的成员需要使用成员访问运算符->。
		必须在使用指针之前，对指针进行初始化。	

*/

/*
静态成员变量
		使用static关键字把类成员定义为静态的。当声明类的成员为静态时，这意味着无论创建多少个类的对象，静态成员都只有一个副本。
	静态成员在类的所有对象中是共享的。不能把静态成员的初始化放置在类的定义中，可以在类的外部通过使用范围解析运算符::来
	重新声明静态变量从而对它进行初始化。

静态成员函数
		如果把函数成员声明为静态的，就可以把函数和类的任何特定对象独立开来。静态成员函数即使在类对象不存在的情况下也能被调用，
	静态函数只要使用类名加范围解析运算符::就可以访问。静态成员函数只能访问静态成员数据、其它静态成员函数和类外部的其它函数。
	静态成员函数不能访问类的this指针。

静态成员函数和普通成员函数的区别：
	*静态成员函数没有this指针，只能访问静态成员(包括静态成员变量和静态成员函数)
	*普通成员函数有this指针，可以访问类中任意成员


*/

inline int Max(int x, int y)
{
	return ((x) > (y))?(x):(y);
}


class Box
{
public:
	double length;
	double breadth;
	double height;

	Box();		//不带参数的构造函数         	*没有返回值也没有void
	Box(double len, double bre);  //带参数的构造函数
	Box(double len, double bre, double hei);
	~Box();
	
	void setLength(double len);
	void setBreadth(double bre);
	void setHeight(double hei);
	double getVolume(void)
	{
		return length * breadth * height;
	}
};

void Box::setLength(double len)
{
	length = len;
}

void Box::setBreadth(double bre)
{
	breadth = bre;
}

void Box::setHeight(double hei)
{
	height = hei;
}
Box::Box(void)		//()里有void
{
	cout << "object is being created:Box()\n";
}
Box::Box(double len, double bre)
{
	cout << "object is being created:Box(double len, double bre)\n";
	length = len;
	breadth = bre;
}
Box::Box(double len, double bre, double hei):length(len), breadth(bre), height(hei)
{
	cout << "object is being created:Box(double len, double bre, double hei)\n";
}
Box::~Box(void)		//()里有void
{
	cout << "the object is being deleted\n";
}
//---------------------------------------------------------------------------------------------


class Line
{
public:	
	Line(int len); 	//简单的构造函数
	Line(const Line &obj);	//拷贝构造函数
	~Line();	//析构函数
	int getLength(void);
	
private:
	int *ptr;
};


Line::Line(int len)
{
	cout << "调用构造函数" << endl;
	ptr = new int;	//为指针分配内存
	*ptr = len;
}

Line::Line(const Line &obj)
{
	cout << "调用拷贝构造函数并为指针ptr分配内存" << endl;
	ptr = new int;
	*ptr = *obj.ptr;	//赋值值
}

Line::~Line(void)
{
	cout << "释放内存" << endl;
	delete ptr;
}

int Line::getLength(void)
{
	return *ptr;
}

void display(Line obj)
{
	cout << "line的大小：" << obj.getLength() << endl;
}
//---------------------------------------------------------------------------------------------



//该类的友元函数可以访问该类的私有成员和保护成员
class Box2
{
private:
	double width;
public:
	friend void printWidth(Box2 box);  //友元函数在类中声明但并不是类的成员，可以访问类的私有和保护成员
	void setWidth(double wid);
};

void Box2::setWidth(double wid)
{
	width = wid;
}

void printWidth(Box2 box) 
{
	cout << "width of box:" << box.width <<endl;  //在友元函数中可以直接访问该类的私有成员和保护成员
}
//---------------------------------------------------------------------------------------------


class Box3
{
private:
	double length;
	double breadth;
	double height;
	
public:
	Box3(double l = 2.0, double b = 2.0, double h = 2.0)
	{
		cout << "调用Box3构造器" << endl;
		length = l;
		breadth = b;
		height = h;
	}
	Box3(Box3 &box);  	//引用本质上传递的还是指针，所以会改变原对象的值。所以应该是Box3(const Box3 &box)
	~Box3();
	
	double volume()
	{
		return length * breadth *height;
	}
	double getHeight()
	{
		return this->height;
	}
	int compare(Box3 box)
	{
		return this->volume() > box.volume();
	}

};
Box3::Box3(Box3 &box)
{
	cout << "调用Box3的拷贝构造函数" << endl;
	this->length = box.length;
	this->breadth = box.breadth;
	this->height = box.height;
	box.height = 100;
}
Box3::~Box3()
{
	cout << "Box3结束" << endl;
}
//---------------------------------------------------------------------------------------------



class Box4
{
private:
	double length;
	double breadth;
	double height;
public:
	static int objectCount;
	
	Box4(double l, double b, double h)
	{
		cout << "调用Box4构造器" << endl;
		this->length = l;
		this->breadth = b;
		this->height = h;
		objectCount++;
	}
	~Box4()
	{
		cout << "Box4析构函数" << endl;
	}
	static int getCount()
	{
		return objectCount;
	}
};
int Box4::objectCount = 2;  //在类外部重新声明静态变量从而对它进行初始化。





int main(void)
{

	Box box(1,2,3);
	cout << "box.getVolume == " << box.getVolume() << endl;
	cout <<"----------------------------------------------------\n";


	Line line1(10);	//调用普通构造函数
	Line line2 = line1;	//调用拷贝构造函数  		通过另一个同类型的对象来初始化新创建的对象
	display(line1);	//调用拷贝构造函数				复制对象将其作为参数传递给函数
	display(line2);	//调用拷贝构造函数				复制对象将其作为参数传递给函数
	cout <<"----------------------------------------------------\n";


	Box2 box2;
	box2.setWidth(10.0);
	printWidth(box2);
	cout <<"----------------------------------------------------\n";


	cout << "Max(20,10):" << Max(20,10) << endl;	
	cout <<"----------------------------------------------------\n";


	Box3 box31(1,2,3);
	Box3 box32(4,5,6);
	cout << box31.volume() << endl;
	cout << box32.volume() << endl;
	if(box31.compare(box32))
	{	
		cout << "box31 > box32" << endl;
		cout << "box32.height = :" << box32.getHeight() << endl;
	}else
	{		
		cout << "box31 < box32" << endl;		
		cout << "box32.height = :" << box32.getHeight() << endl;
	}
	cout <<"----------------------------------------------------\n";
		
	Box3 box33(1,2,3);
	Box3 *ptrbox33 = NULL;	//定义指向类的指针
	ptrbox33 = &box33;	
	cout << "box33.height == :" << ptrbox33->getHeight() << endl;  //使用指针访问对象的成员
	cout <<"----------------------------------------------------\n";


	cout << "objectCount == " <<Box4::getCount() << endl;
	Box4 box41(1,2,3);
	cout << "objectCount == " << Box4::objectCount << endl;


	
	return 0;
}

































































