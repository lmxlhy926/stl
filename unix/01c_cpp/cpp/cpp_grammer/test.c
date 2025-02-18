
#include <stdio.h>

struct s
{
	int a;
	int b;
};


struct s func1(void)
{
	struct s s1;
	s1.a = 10;
	s1.b = 20;
	return s1;
}
int main(void)
{
	struct s s2;
	s2 = func1();
	printf("%d %d\n", s2.a, s2.b);

	return 0;
}









































