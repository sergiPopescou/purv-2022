#include <stdio.h> 

int add_numbers(int n1, int n2)
{
     int sum=n1+n2;
     return sum;
}

int suma;

int main()
{
	 char *poruka = "Suma brojeva 1 i 2 je";
     int *p; 
     int n1=1;
     int n2=2;
     
     suma=add_numbers(n1,n2);
     printf("%s %d %d\n", poruka, suma, p);

     return 0;
}