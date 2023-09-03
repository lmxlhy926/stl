
//形式参数
void func1(const model m)	//形参是一个对象，需要调用拷贝构造函数创建一个对象。
{}
void func2(const model& m)	//形参是一个引用，相当于一个指针变量，指向一个对象，并不需要创建新的对象。
{}

void testfunc1();
void testfunc2();