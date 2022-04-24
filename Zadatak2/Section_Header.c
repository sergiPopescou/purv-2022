#include <sys/stat.h>
#include <sys/mman.h>
#include <elf.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

static char  Arr[]={'I','S','M','X','A','W'};//Uzimaju se u obzir samo Flegovi koji su se pojavljivali.


char * func_Flags(int n)//Funkcija za odredjivanje Flegova nekog segmenta 
{

    char  Arr1[]={'0','0','0','0','0','0'};//Niz korespondira nizu Arr tako sto svaka '1'/'0' predstavlja Fleg na istoj poziciji u nizu koji neki Segment ima/nema.

    char *res=(char  *)malloc(sizeof(char)*7);  
    int po=64;
    int k=n;
    
    for(int i1=0;i1<6;i1++)
    {
    	if(k>=po)
    	{
    		k-=po;
    		Arr1[i1]= '1';	
    	}
    	
    	if(po==16)
    	po=4;
    	else 
    	po/=2;
    }
    
    int pos=0;
    
    for(int i=5;i>=0;i--)
    {
    	if(Arr1[i]=='1')
    	res[pos++]=Arr[i];
    }
    
    res[pos]='\0';
    return  res;

}

char* type_of_Segment(int value)//Funkcija za dobijanje Tipa Segmenta(koji kategorise sadrzaj i semantiku nekog segmenta)
{  
    switch (value)
    {
     	case 0:
            return "NULL";
        case 1:
            return "PROGBITS";
        case 2:
            return "SYMTAB";
        case 3:
            return "STRTAB";
        case 4:
            return "RELA";
        case 5:
            return "HASH";
        case 6:
            return "DYNAMIC";
        case 7:
            return "NOTE";
        case 8:
            return "NOBITS";
        case 9:
            return "REL";
        case 10:
            return "SHLIB";
        case 11:
            return "DYNSYM";
        case 14:
            return "INIT_ARRAY";
        case 15:
            return "FINI_ARRAY";
            
        case 1879048190:
            return "VERNEED";
        case 1879048191:
            return "VERSYM";
        case 1879048182:
            return "GNU_HASH";   
            
            
        case 0x70000000:
            return "LOPROC";
        case 0x7fffffff:
            return "HIPROC";
        case 0x80000000:
            return "LOUSER";
        case 0xffffffff:
            return "HIUSER";
       
 
        default :
            return "";
    }
}

int main(int argc, char *argv[])
{
    struct stat st;
    const char *fname = "";

    if (argc < 2) 
    return 0;
 
    fname = argv[1];

    if (stat(fname, &st) != 0) 
    {
    perror("stat");
    return 1;
    }

   int fd = open(fname, O_RDONLY);
   char *p = mmap(0,  st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

   Elf64_Ehdr *ehdr = (Elf64_Ehdr*)p;//Potreban nam je i Elf Header zbog vrijednosti : e_shnum(broj segmenata u Section Header-u),e_shoff(govori gdje se nalazi Section Header),e_shstrndx(indeks do Section Header-ove String tabele pomocu koje odredjujemo ime nekog segmenta)
   
   Elf64_Shdr *shdr = (Elf64_Shdr *)(p + ehdr->e_shoff);//Preskacemo do Section Header-a  pomocu vrijednosti e_shoff 
   int shnum = ehdr->e_shnum;

   Elf64_Shdr *sh_strtab = &shdr[ehdr->e_shstrndx]; //String Tabela 
   const char *const sh_strtab_p = p + sh_strtab->sh_offset;
   
    printf("There are %d section headers, starting at offset 0x%04x:\n\n",shnum,ehdr->e_shoff);
    printf("Section Headers:\n");
    printf("  [Nr] Name              Type             Address           Offset\n");
    printf("       Size              EntSize          Flags  Link  Info  Align\n");
    
   for (int i = 0; i < shnum; ++i) 
   {
    char *pok=func_Flags(shdr[i].sh_flags);
    printf("  [%d] %-17s %-16s %.16x  %.8x\n",i,sh_strtab_p + shdr[i].sh_name,type_of_Segment(shdr[i].sh_type),shdr[i].sh_addr,shdr[i].sh_offset);
    printf("       %.16x  %.16x  %2s      %2d    %2d     %ld\n",shdr[i].sh_size,shdr[i].sh_entsize,pok,shdr[i].sh_link,shdr[i].sh_info,shdr[i].sh_addralign);      
    free(pok);
    //Kod odredjenih segmenata(na pocetku) postoji pomjeranje za 1 poziciju pri formatiranju na izlaz neznam zasto.
   } 

    printf("Key to Flags:\n");
    printf("  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),\n");
    printf("  L (link order), O (extra OS processing required), G (group), T (TLS),\n");
    printf("  C (compressed), x (unknown), o (OS specific), E (exclude),\n");
    printf("  l (large), p (processor specific)\n");
    
    return 0;
}


