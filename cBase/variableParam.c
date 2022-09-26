#include <stdarg.h>
#include <stdio.h>


/*
 *  变长参数：如何处理一个甚至连名字都没有的参数表
 *      *va_list类型用于声明一个变量，该变量将依次引用各参数。将该变量成为ap,意思是"参数指针"。
 *
 *      *宏va_start将ap初始化为指向第一个无名参数的指针，在使用ap之前，该宏必须被调用一次。
 *      参数表必须至少包括一个有名参数，va_start将最后一个有名参数作为起点。
 *
 *      每次调用va_arg，该函数都将返回一个参数，并将ap指向下一个参数。
 *      va_arg使用一个类型名来决定返回的对象类型、指针移动的步长。
 *
 *      必须在函数返回之前调用va_end，以完成一些必要的清理工作

以下摘自《C陷阱与缺陷》
    这里有一个陷阱需要避免：
    va_arg宏的第2个参数不能被指定为char、short或者float类型。
    因为char和short类型的参数会被转换为int类型，而float类型的参数会被转换为double类型 ……
    例如，这样写肯定是不对的：
    c = va_arg(ap,char);
    因为我们无法传递一个char类型参数，如果传递了，它将会被自动转化为int类型。上面的式子应该写成：
    c = va_arg(ap,int);

C标准对默认实际参数提升规则有明确规定。也就是说带有可变长参数列表的函数， 绝对不会接受到char类型的实际参数。
C标准对va_arg是否自动对齐没有任何说明。也就是说自动对齐工作，编译器可做可不做。
在所有C实现上，能保证第①点，不能保证第②点，所以尽管编译器实现了自动对齐，也要按标准来。

 */

void minprintf(char *fmt, ...)
{
    va_list ap;             //创建参数指针变量
    va_start(ap, fmt);      //指向第一个未命名参数
    printf("address: %p\n", ap);

    char *p, *sval;
    int ival;
    double dval;

    for (p = fmt; *p; p++) {
        if (*p != '%') {
            putchar(*p);
            continue;
        }
        switch (*++p) {
            case 'd':
                ival = va_arg(ap, int);    //type<int>: 指明返回对象的类型，指针移动的步长；ap指向下一个参数；
                printf("==>%d\n", ival);
                printf("address: %p\n", ap);
                break;
            case 'f':
                dval = va_arg(ap, double);  //type<double>
                printf("==>%f\n", dval);
                printf("address: %p\n", ap);
                break;
            case 's':
                for (sval = va_arg(ap, char *); *sval; sval++)  //type<char*>
                    putchar(*sval);
                printf("address: %p\n", ap);
                break;
            default:
                putchar(*p);
                break;
        }
    }
    va_end(ap); /* clean up when done */
}

int main(int argc, char* argv[]){



    return 0;
}





















