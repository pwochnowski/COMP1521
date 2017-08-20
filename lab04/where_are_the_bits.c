// where_are_the_bits.c ... determine bit-field order
// COMP1521 Lab 03 Exercise
// Written by ...

#include <stdio.h>
#include <stdlib.h>

struct _bit_fields {
	unsigned int a : 4,
							 b : 8,
							 c : 20;
};

union _as_union {
	unsigned int u;
	struct _bit_fields b;
};

int main(void)
{
	struct _bit_fields x;
	x.a=1;
	x.b=0;
	x.c=1;
	//x.c=1;
	union _as_union un;
	un.b = x;
	printf("as an unsigned: %u\n", un.u);
	char *p = (char *) &un;
	for (int i=0; i<4; i++) {
		printf("%p : %d\n", p+i, p[i]);
	}

	int n = 1;
	p = (char*) &n;
	printf("%p %d\n%p %d\n",p, (int) p[0],p+3,(int) p[3]);




	return 0;
}
