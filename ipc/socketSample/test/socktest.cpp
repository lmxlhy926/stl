//
// Created by 78472 on 2022/9/12.
//


#include <vector>
#include <iostream>
#include <cstdarg>

using namespace std;

void minprintf(char *fmt, ...)
{
    va_list ap; /* points to each unnamed arg in turn */
    char *p, *sval;
    int ival;
    double dval;
    va_start(ap, fmt); /* make ap point to 1st unnamed arg */
    printf("address: %p\n", ap);
    for (p = fmt; *p; p++) {
        if (*p != '%') {
            putchar(*p);
            continue;
        }
        switch (*++p) {
            case 'd':
                ival = va_arg(ap, int);
                printf("==>%d\n", ival);
                printf("address: %p\n", ap);
                break;
            case 'f':
                dval = va_arg(ap, double);
                printf("==>%f\n", dval);
                printf("address: %p\n", ap);
                break;
            case 's':
                for (sval = va_arg(ap, char *); *sval; sval++)
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

void variable_arg(int num, ...){
    va_list vaList;
    va_start(vaList, num);
    for(int i = 0; i < num; ++i){
        std::cout << va_arg(vaList, int) << std::endl;
    }
    va_end(vaList);
}

int main(int argc, char* argv[]){
    minprintf("%d%d%f%s", 6, 6, 2.5, "hesssssssssssssssssssssss");

    return 0;
}





























