#include<stdio.h>
#include<stdlib.h>
#include<elf.h>
#include<string.h>

char* machine[0x102]={"No specific instruction set",
                        "AT&T WE 32100",
                        "SPARC",
                        "x86",
                        "Motorola 68000 (M68k)",
                        "Motorola 88000 (M88k)",
                        "Intel MCU",
                        "Intel 80860",
                        "MIPS",
                        "IBM System/370",
                        "MIPS RS3000 Little-endian",
                        "Reserved for future use",
                        "Reserved for future use",
                        "Reserved for future use",
                        "Hewlett-Packard PA-RISC"
                        };

char* class[2]={"ELF32","ELF64"};
char* data[3]={"","little endian","big endian"};
char* version[1]={"1 (current)"};
char* os_abi[0x13]={"System V",
                    "HP-UX",
                    "NetBSD",
                    "Linux",
                    "GNU Hurd",
                    "Solaris",
                    "AIX",
                    "IRIX",
                    "FreeBSD",
                    "Tru64",
                    "Novel Modesto"};
char* type(int indx){
    switch (indx){
        case 0x00:
            return "ET_NONE (Unknown)";
        case 0x01:
            return "ET_REL (Relocatable file)";
        case 0x02:
            return "ET_EXEC (Executable file)";
        case  0x03:
            return "ET_DYN (Shared object)";
        default :
            return "";
    }
}
int main(int argc, char* argv[]){

    machine[0x3E]= (char*)malloc(sizeof("AMD x86-64")+1);
    
    strcpy(machine[0x3E],"AMD x86-64");
    Elf64_Ehdr f_header;
    if(argc<2){
        printf("Navedite naziv fajla kao argumente programa\n");
        return 1;
    }
    FILE* elf_file = fopen(argv[1],"r");
    if(!elf_file){
        printf("Fajl nije pronadjen\n");
        return 1;
    }
    fread(&f_header,1,sizeof(f_header),elf_file);


    printf("Elf Header:\n");
    /*--------------Magic----------------*/
    printf("  Magic:   ");
    for(int i=0;i<16;i++){
        printf("%02x ",f_header.e_ident[i]);
    }
    printf("\n");

    /*----------------------------------*/
    printf("  Class:                             %s\n",class[f_header.e_ident[5]]);
    printf("  Data:                              %s\n",data[f_header.e_ident[6]]);
    printf("  Version:                           %s\n",version[f_header.e_ident[7]]);
    printf("  OS/ABI:                            %s\n",os_abi[f_header.e_ident[8]]);
    printf("  ABI Version:                       %d\n",f_header.e_ident[9]);
    printf("  Type:                              %s\n",type(f_header.e_type));
    printf("  Machine:                           %s\n",machine[f_header.e_machine]);
    printf("  Version:                           0x%x\n",f_header.e_version);
    printf("  Entry point address:               0x%lx\n",f_header.e_entry);
    printf("  Start of program headers:          %ld (bytes into file)\n",f_header.e_phoff);
    printf("  Start of section headers:          %ld (bytes into file)\n",f_header.e_shoff);
    printf("  Flags:                             0x%d\n",f_header.e_flags);
    printf("  Size of this header:               %d (bytes)\n",f_header.e_ehsize);
    printf("  Size of program headers:           %d (bytes)\n",f_header.e_phentsize);
    printf("  Number of program headers:         %d\n",f_header.e_phnum);
    printf("  Size of section headers:           %d (bytes)\n",f_header.e_shentsize);
    printf("  Number of section headers:         %d\n",f_header.e_shnum);
    printf("  Section header string table index: %d\n",f_header.e_shstrndx);
    
    return 0;
}