
int add(int a, int b);
int sub(int a, int b);
void showResult(int result);
extern int a, b;
extern char* ptr;

int main(int argc, char* argv[]){
    if(ptr != nullptr){
        showResult(0);
    }
    int c = 0, d = 0;
    int result = add(a, b);
    result = sub(result, 4);
    showResult(result);
    return 0;
}



