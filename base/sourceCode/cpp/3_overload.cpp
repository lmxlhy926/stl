#include<iostream>
using namespace std;

/*
重载函数和重载运算符：
		在同一个作用域中的某个函数和运算符指定多个定义，分别称为函数重载和运算符重载。
	重载声明是指一个与之前已经在该作用域内声明过的函数或方法具有相同名称的声明，但是
	他们的参数列表和定义(实现)不相同。
		当调用一个重载函数或重载运算符时，编译器通过把您所使用的参数类型与定义中的参数
	类型进行比较，决定选用最合适的定义。选择最合适的重载函数或重载运算符的过程，称为重载决策。

总结：
		函数或运算符之前就定义过，现在重新定义，现在的定义和之前的定义的名称相同但是参数列表
	不同，当然实现也不同。当使用重载的函数或者运算符时，编译器根据实参的参数列表和形参
	列表进行比对，从而选择最合适的定义。
*/

/*
函数重载：
		在同一个作用域内，可以声明几个功能类似的同名函数。但是这些同名函数的形式参数(参数个数，
	类型或者顺序)必须不同。不能仅通过返回值类型的不同来重载函数。

总结：
	*同一个作用域
	*名称相同，功能类似
	*形式参数不同(参数个数，参数类型，参数顺序)
	*和返回值类型无关
*/

/*
运算符重载：
		可以重定义或者重载大部分c++内置的运算符。这样就可以使用自定义类型的运算符。
	重载的运算符是带有特殊名称的函数，函数名是由关键字operator和其后要重载的运算符
	符号构成。与其它函数一样，重载运算符有一个返回类型和一个参数列表。

*/

class printData
{
public:
	void print(int i)
	{
		cout << "整数为： " << i << endl;
	}

	void print(double f)
	{
		cout << "浮点数为： " << f << endl;
	}

	void print(char c[])
	{
		cout << "字符串为： " << c << endl;
	}
	
};


class Distance
{
private:
	int feet;
	int inches;
	int arr[3] = {1,2,3};
public:
	Distance()
	{
		cout << "调用构造器Distance(): \n";
		feet = 0;
		inches = 0;
	}
	Distance(int f)
	{
		cout << "调用Distance构造器(int f)\n";
		feet = f;
		inches = f;
	}
	Distance(int f, int i)
	{	
		cout << "调用Distance构造器(int f, int i): \n";
		feet = f;
		inches = i;
	}
	Distance(const Distance& d)
	{
		cout << "调用拷贝构造函数" << endl;
		this->feet = d.feet;
		this->inches = d.inches;
	}
	void displayDistance()
	{
		cout << "F: " <<feet << " I: " << inches << endl;
	}
	

//一元运算符通常出现在它们所操作的对象的左边，但有时它们也可以作为后缀。比如obj++、obj--
	Distance operator -()										
	{       
		cout << "调用operator -" << endl;
		feet = -feet;
		inches = -inches;
		return Distance(feet, inches);
	}
//一元运算符，默认是前缀形式
	Distance operator ++()		
	{	
		cout << "前缀operator ++" << endl;
		++this->feet;
		++this->inches;		//先自增
		return Distance(this->feet ,this->inches);	//返回自增后的值
	}
//()里的int表明是一种后缀形式，而不是表示整数
	Distance operator ++(int)		
	{
		cout << "后缀operator ++" << endl;
		Distance d(this->feet,this->inches);
		this->feet++;
		this-inches++;
		return d;	//返回自增前的值
	}
//二元运算符	
	Distance operator +(const Distance& d)
	{
		cout << "调用operator +" << endl;
		return Distance(this->feet+d.feet, this->inches+d.inches);
	}
//赋值运算符，用于创建一个对象，比如拷贝构造函数
	Distance operator =(const Distance& d)
	{
		cout << "operator =" << endl;
		this->feet = d.feet;
		this->inches = d.inches;
		return Distance(d.feet,d.inches);
	}
//关系运算符，用于比较类的对象
	bool operator <(const Distance& d)
	{
		if(this->feet < d.feet)
			return true;
		if(this->feet = d.feet && this->inches < d.inches)
			return true;
		return false;
	}
//函数调用运算符()可以被重载用于类的对象。创建了一个可以传递任意数目参数的运算符函数。	
	Distance operator()(int a, int b)
	{
		cout << "调用operator(int a, int b)" << endl;
		this->feet = a;
		this->inches = b;
		return Distance(this->feet/*,this->inches*/);
	}
	Distance operator()(int a, int b, int c)
	{
		cout << "调用operator(int a, int b, int c)" << endl;
		this->feet = a+c;
		this->inches = b+c;
		return Distance(this->feet,this->inches);
	}
//数组访问运算符。重载该运算符用于增强操作c++数组的功能。
	int& operator[](int i)
	{
		if(i > 2)
		{
			cout << "索引超过最大值" << endl;
			return arr[0];
		}
		return arr[i];
	}
/*
	c++能够使用流提取运算符>>和流插入运算符<<来输入和输出内置的数据类型。可以重载流提取运算符和流
插入运算符来操作对象等用户自定义数据类型。
	运算符重载函数声明为类的友元函数，这样就能不创建对象而直接调用函数。
*/
	friend ostream &operator<<(ostream &output,const Distance &d)
	{
		output << "F: " << d.feet << " I: " << d.inches <<endl;
		return output;
	}
	friend istream &operator>>(istream &input,Distance &d)
	{
		input >> d.feet >> d.inches;
		return input;
	}


	
};

int main(void)
{

	printData pd;
	pd.print(5);	//函数名称相同，但是参数类型不同。根据参数类型选择相应的函数定义。
	pd.print(200.2);
	char c[] = "hello c++";
	pd.print(c);
	cout << "----------------------------------------------------\n";


	Distance D1(3,4);
	-D1;
	D1.displayDistance();
	cout << "----------------------------------------------------\n";

	Distance D2(1,2);
	Distance D3(1,2);
	Distance D4;	//无需参数传递时的格式
	D4 = D2 + D3;
	D4.displayDistance();
	cout << "----------------------------------------------------\n";

	Distance D5(1,2);
	Distance D6;
	if(D5 < D6)
		cout << "D5 < D6\n";
	else 
		cout << "D5 !< D6\n";
	cout << "----------------------------------------------------\n";

	Distance D7,D8;
	D7 = D8++;
	D7.displayDistance();
	D7 = ++D8;	
	D7.displayDistance();
	cout << "----------------------------------------------------\n";

	Distance D9(2,3);
	Distance D10 = D9;	//调用拷贝构造函数
	D9(1,2);
	D9(1,2,3);
	cout << "----------------------------------------------------\n";

	Distance D11;
	cout << "D11[2]: " << D11[2] << endl;
	cout << "D11[3]: " << D11[3] << endl;
	cout << "----------------------------------------------------\n";

	Distance D12(4,5);
	cin >>D12;
	cout << D12;

}
































































