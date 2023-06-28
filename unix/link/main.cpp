
// 声明外部函数
int add(int a, int b);
int sub(int a, int b);
void showResult(int result);

// 定义本地函数
int mul(int a, int b){
    return a * b;
}

// 声明外部全局变量
extern int Global_a, Global_b;

// 全局变量包含函数地址
typedef int(*addFuncType)(int, int);
addFuncType addFuncPtr = add;


int main(int argc, char* argv[]){
    int local_a = 1, local_b = 2;
    showResult(mul(local_a, local_b));
    showResult(local_a + local_b);
    int result = add(local_a, local_b);
    result = sub(result, 4);
    showResult(result);
    return 0;
}



