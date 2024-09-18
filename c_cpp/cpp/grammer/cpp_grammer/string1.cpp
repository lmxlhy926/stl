

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <algorithm>

using namespace std;

void func1(void);
void func2(void);
void func3(void);
void func4(void);
void func5(void);
void func6(void);
void func7(void);



int main(void)
{
	
	func7();
	

	return 0;
}


void func1(void)
{
//字符串的长度，取指定的字符
	string str("1234");
		
	int size = str.size();
	int length = str.length();
	unsigned long maxlength = str.max_size();
	int capacity = str.capacity();
	
	cout << "str.size():" << str.size() << endl;
	cout << "str.length():" << str.length() << endl;
	cout << "str.max_size():" << str.max_size() << endl;
	cout << "str.capacity():" << str.capacity() << endl;
	
	char char_1 = str.at(0);
	char char_2 = str.at(1);
	char char_3 = str [2];
	char char_4 = str [3];
	putchar(char_1);
	putchar(char_2);
	putchar(char_3);
	putchar(char_4);

	return;
}

void func2(void)
{
	string str1 ("123456");
	string str;
	str.assign (str1); 
	cout << str << endl;	//123456
	
	str.assign (str1, 3, 3); 
	cout << str << endl;	//456
	
	str.assign (str1,2,str1.npos);
	cout << str << endl;	//3456
	
	str.assign (5,'X'); 
	cout << str << endl;	//XXXXX
	
	string::iterator itB;
	string::iterator itE;
	itB = str1.begin ();
	itE = str1.end();
	str.assign (itB, (--itE)); //从第 1 个至倒数第 2 个元素，赋值给字符串 str
	cout << str << endl;	//12345
	

	return;
}

void func3(void)
{
//A开始的位置，B开始的位置，B结束的位置

	string A="ello";
	string B = "H";
	B.insert (1 , 5 , 'C'); //HCCCCC
	cout << B << endl;

	A = "ello";
	B = "h";
	B.insert(1,A);
	cout << B << endl;	//hello
	
	A = "ello";
	B = "H";
	B.insert (1,"yanchy ",3);	//Hyan
	cout<< B <<endl;
	
	A = "ello";
	B = "H";
	B.insert (1,A,2,2);
	cout << B << endl;	//Hlo
	
	
	A = "ello";
	B = "h";
	string::iterator it = B.begin () +1;
	const string:: iterator itF = A.begin();
	const string:: iterator itG = A.end();
	B.insert(it,itF,itG);
	cout << B << endl;	//hello


	return;
}



void func4()
{
	string A = "ello";
	string B = "h";
	B.append(A);
	cout << B << endl;	//hello
	
	B = "hh";
	B.append("12345",2);
	cout << B << endl;	//hh12

	B = "h";
	B.append("12345",2,3);
	cout << B << endl;	//h345

	B = "H";
	B.append (10, 'a');
	cout << B << endl;	//Haaaaaaaaaa

	A = "ello";
	B = 'H';
	string::iterator itrs = A.begin();
	string::iterator ite = A.end();
	B.append(itrs, ite);
	cout << B << endl;	//Hello

	return;
}


void func5()
{
//字符串1的起始，结束。字符串2的起始，结束
	string var ("abcdefghijklmn");
	const string dest ("1234");
	string dest2 ("567891234");
	var.replace (3,3, dest);
	cout << "1: " << var << endl;	//1: abc1234ghijklmn
	
	var = "abcdefghijklmn";
	var.replace (3,1, dest.c_str(), 1, 3);
	cout << "2: " << var << endl;	//2: abc234efghijklmn

	var ="abcdefghijklmn";
	var.replace (3, 1, 5, 'x');
	cout << "3: " << var << endl;	//3: abcxxxxxefghijklmn

	string::iterator itA, itB;
	string::iterator itC, itD;
	itA = var.begin();
	itB = var.end();
	var = "abcdefghijklmn";
	var.replace (itA, itB, dest);
	cout << "4: " << var << endl;	//4: 1234
	
	
	var = "abodefghijklmn";
	itA = var.begin ();
	itB = var.end();
	itC = dest2.begin () +1;
	itD = dest2.end ();
	var.replace (itA, itB, itC, itD);
	cout << "5: " << var << endl;	//5: 67891234

	var = "abcdefghijklmn";
	var.replace (3, 1, dest2.c_str(), 4); //这种方式会限定字符串替换的最大长度
	cout <<"6: " << var << endl;	//6: abc5678efghijklmn


	return;
}


void func6()
{
//string::size_type类型是无符号整数类型
//待寻找字符或者字符串，开始寻找的起始位置

	string str_ch (" for");
	string str (" Hi, Peter, I'm sick. Please bought some drugs for me.");
	string::size_type m= str.find ('P', 5);
	string::size_type rm= str.rfind('P', 5);
	cout << "Example - find() : The position (forward) of 'P' is: " << (int) m << endl;	//5
	cout << "Example - rfind(): The position (reverse) of 'P' is: " << (int) rm << endl;	//5
	
	string::size_type n = str.find (" some", 0);
	string::size_type rn = str.rfind (" some", 0);
	cout << "Example - find () : The position (forward) of 'some' is: " << (int) n << endl;	//35
	cout << "Example - rfind () : The position (reverse) of 'some' is: " << (int) rn << endl;	//-1
	
	string::size_type mo = str.find (" drugs", 0, 5);
	string::size_type rmo = str.rfind (" drugs", 0, 5);
	cout << "Example - find(): The position (forward) of 'drugs' is: " << (int) mo << endl;	//40
	cout << "Example - rfind(): The position (reverse) of 'drugs' is: " << (int) rmo << endl;	//-1
	
	string::size_type no = str.find (str_ch, 0);
	string::size_type rno = str.rfind(str_ch, 0);
	cout << "Example - find (): The position of 'for' is: " << (int) no << endl;	//46
	cout << "Example - rfind(): The position of 'for' is: " << (int) rno << endl;	//-1




	return;
}


void func7()
{
//模板使得算法独立于存储的数据类型，迭代器使得算法独立于使用的容器类型。

	string s ("The zip code of Hondelage in Germany is 38108.");
	cout << "Original: " << s << endl;
	string sd(s.begin(),s.end ()); //构造函数中使用迭代器
	cout << "Destination: " << sd << endl;

	string sd1;
	sd1.append (sd.begin(),(sd.end() -7)); //append()函数中使用迭代器
	cout << "Destination sd1: " << sd1 << endl;

	string sd2;
	string::reverse_iterator iterA;
	string temp = "0";
	for (iterA = sd.rbegin (); iterA != sd.rend (); iterA++) //reverse_iterator
	{
		temp=* iterA;
		//cout << temp << endl;
		sd2.append (temp);
	}
	cout << "Destination sd2: " << sd2 << endl;

	sd2.erase (0, 15); //erase()函数中使用迭代器
	cout << "Destination sd2 (Erased 15 chars) : " << sd2 << endl;

	string::iterator iterB = sd2.begin ();
	string sd3 = string ("12345678");
	sd2.insert (sd2.begin(), sd3.begin(), sd3.end()); //insert()函数中使用迭代器
	cout << "Destination sd2 (Insert 8 chars) : " << sd2 << endl;

	sd2.replace (sd2.begin (), sd2.end(), "This is an Exarrple of Replace"); //Replace
	cout <<"Destination sd2 (Replace All): " << sd2 << endl; // replace ()函数中使用迭代器
	
//	Original: The zip code of Hondelage in Germany is 38108.
//	Destination: The zip code of Hondelage in Germany is 38108.
//	Destination sd1: The zip code of Hondelage in Germany is
//	Destination sd2: .80183 si ynamreG ni egalednoH fo edoc piz ehT
//	Destination sd2 (Erased 15 chars) : eG ni egalednoH fo edoc piz ehT
//	Destination sd2 (Insert 8 chars) : 12345678eG ni egalednoH fo edoc piz ehT
//	Destination sd2 (Replace All): This is an Exarrple of Replace

	return;
}




























