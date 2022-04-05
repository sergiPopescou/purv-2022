#include <stdio.h>

int add_numbers(int n1, int n2)
{
     int sum=n1+n2;
     return sum;
}

int suma;

int main()
{
	 static char *poruka = "Suma brojeva 1 i 2 je";
     int n1;
     int n2;
     scanf("%d", n1);
     scanf("%d", n2);
     suma=add_numbers(n1,n2);
     printf("%s %d\n", poruka, suma);

     return 0;
}
