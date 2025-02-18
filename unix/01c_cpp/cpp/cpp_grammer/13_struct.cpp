#include<iostream>
#include<cstring>

using namespace std;

/*
	数组存放一组相同类型的变量，结构体可以存放一组不同类型的变量。
	结构体是一种用户自定义的可用数据类型
	指向该结构体的指针使用->访问结构体的成员
*/

//用typedef给结构体变量类型起别名。
typedef struct books  //struct books book1;	mybook book2;这两者是等效的
{
	char author[20];
	char name[20];
	int id;
}mybook;   

//用typedef给结构体变量指针类型起别名
typedef mybook *mybook_t;

void func1(mybook book);
void func2(mybook_t book);

int main(void)
{
//结构体变量使用.来访问成员变量
	mybook book1;
	strcpy(book1.author,"尤瓦尔");
	strcpy(book1.name,"人类简史");
	book1.id = 1234;
	func1(book1);
	func2(&book1);  //隐形变量：&book1是mybook_t类型的
	cout << "boo1.author == " << book1.author << endl;
	cout << "book1.name == " << book1.name << endl;
	cout << "book1.id == " << book1.id << endl;


	





	return 0;
}


void func1(mybook book)
{
	strcpy(book.author,"this is");
	return;
}

void func2(mybook_t book)	//将实参的值赋值给形参，值的类型必须是相同的。
{
	strcpy(book->author,"we are ");
	
	return;
}




