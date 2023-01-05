#include<stdio.h>
#include<unistd.h>
#include<sys/syscall.h>

int main(){
	printf("add syscall test, input 2 integers\n");
	int a, b;
	scanf("%d %d", &a, &b);
	long sum = syscall(335, a, b);
	printf("%d + %d = %ld\n", a, b, sum);
	printf("max syscall test, input 3 integers\n");
	int x, y, z;
	scanf("%d %d %d", &x, &y, &z);
	long max = syscall(336, x, y, z);
	printf("max number of the inputed 3 integers is %ld\n", max);
	return 0;
}
