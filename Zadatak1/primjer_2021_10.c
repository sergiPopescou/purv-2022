#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* funkcija()
{
    char c = 'x';
//printf("In function %c",c);
    return &c;
}

int main(int argc, char **argv)
{
    char * pok = funkcija();
    char niz[9];
	
    memset(niz, 'y', sizeof(niz));
    //printf("size is %ld",sizeof(niz));

    niz[0] = *pok;
    printf("\n%c ",niz[0]);
    return 0;
}

