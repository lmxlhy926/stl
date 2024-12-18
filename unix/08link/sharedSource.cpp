
#include <cstdio>

// 动态库里定义的全局变量
int Global_a = 100;
int Global_b = 200;
int GlobalCommon = 100;

// 动态库里定义的重载函数，类对象
void showResult(int result){
    printf("-------------showResult(int) GlobalCommon: %d--------------\n", GlobalCommon);
    printf("the result is %d\n", result);
}

void showResult(int a, int b){
    printf("-------------showResult(int, int) GlobalCommon: %d--------------\n", GlobalCommon);
    printf("the result is %d %d\n", a, b);
}

class classType{
private:
    int a_, b_;
public:
    explicit classType(int a, int b): a_(a), b_(b){}

    void showResult(){
        printf("---in show: %d %d\n", a_, b_);
    }
};

// 共享库调用的外部函数
extern void sharedCallFunc();

/**
 * name mangling:
 *      在编译阶段将函数名进行替换，加入域和参数信息，通过这种方式实现重载
 *      目的是避免重复，通过找到一个编码方法，使得每个名称经过转换后是唯一的。
*/

// 声明用c的方式解析符号，不使用C++的name mangling
extern "C"  void show(int option, int result);
void show(int option, int result){
    switch(option){
        case 1:
            sharedCallFunc();
            showResult(result);
            break;
        case 2:
            sharedCallFunc();
            showResult(result, result);
            break;
        case 3:
        {
            classType a(result, result);
            a.showResult();
            break;
        }
        default:
            break;
    };
}



// 可以将源代码编译为.so格式的共享库，共享库是可以独立加载到内存中的，共享库有Program Headers来引导链接器加载。
