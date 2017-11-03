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
  x.a=0;
  x.b=0;
  x.c=17;
  int c;
  union _as_union un;
  un.b = x;
  int n;
  printf("as an unsigned int: %u\n", un.u);
  char *p = (char *) &un;
  printf("p = %p\n", p);
  for (int i=0; i<4; i++) {
    printf("&p[%d] =  %p : %d\n",i, p+i, p[i]);
  }
  printf("&c = %p\n&un = %p\n&n = %p\n", &c, &un, &n);
  printf("%p %d\n%p %d\n",p, (int) p[0],p+3,(int) p[3]);




  return 0;
}
