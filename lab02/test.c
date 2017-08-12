#include <stdio.h>


typedef unsigned char byte;

int main() {
	int n = 300;
	byte* p = (byte*) &n; 
	printf("%d\n",n);
	printf("%d\t%d\t%d\t%d\n",p[0],p[1],p[2],p[3]);
	printf("%p\t \t \t %p\n",p,p+3);
	return 0;
}
