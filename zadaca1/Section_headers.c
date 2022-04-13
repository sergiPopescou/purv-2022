#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
void read_section_header(const char* file_name)
{
    Elf64_Shdr section;
    Elf64_Ehdr header;
    FILE* file = fopen(file_name, "rb");
    if(file)
    {
        fread(&header, sizeof(header), 1, file);
        printf("There are %d section headers, starting at offset 0x%lx:\n", header.e_shnum, header.e_shoff);
        fseek(file, header.e_shoff, SEEK_SET);
        printf("%d\n", header.e_shstrndx);
        fread(&section, sizeof(section), 1, file);
        printf("\nSection Headers:\n");
        printf("  [Nr] Name              Type             Address           Offset\n");
		printf("       Size              EntSize          Flags  Link  Info  Align\n");
		

        // check the elf file (7f E F L)
        if (memcmp(header.e_ident, ELFMAG, SELFMAG) == 0)
        {
			for(int i = 0; i < header.e_shnum; i++, fread(&section, sizeof(section), 1, file), printf("\n")){
			   printf("  [%2d] %3d", i, section.sh_name);
			   switch(section.sh_type){
				   case SHT_NULL: 
						printf("%15s%-10s", " ", "NULL");
						break;
					case DT_VERNEED: 
						printf("%15s%-10s", " ", "VERNEED");
						break;
					case SHT_GNU_versym: 
						printf("%15s%-10s", " ", "VERSYM");
						break;
					case SHT_GNU_HASH: 
						printf("%15s%-10s", " ", "GNU_HASH");
						break;
					case SHT_PROGBITS: 
						printf("%15s%-10s", " ", "PROGBITS");
						break;
					case SHT_SYMTAB: 
						printf("%15s%-10s", " ", "SYMTAB");
						break;
					case SHT_STRTAB: 
						printf("%15s%-10s", " ", "STRTAB");
						break;
					case SHT_RELA: 
						printf("%15s%-10s", " ", "RELA");
						break;
					case SHT_HASH: 
						printf("%15s%-10s", " ", "HASH");
						break;
					case SHT_DYNAMIC: 
						printf("%15s%-10s", " ", "DYNAMIC");
						break;
					case SHT_NOTE: 
						printf("%15s%-10s", " ", "NOTE");
						break;
					case SHT_NOBITS: 
						printf("%15s%-10s", " ", "NOBITS");
						break;
					case SHT_REL: 
						printf("%15s%-10s", " ", "REL");
						break;
					case SHT_SHLIB: 
						printf("%15s%-10s", " ", "SHLIB");
						break;
					case SHT_DYNSYM: 
						printf("%15s%-10s", " ", "DYNSYM");
						break;
					case SHT_INIT_ARRAY: 
						printf("%15s%-10s", " ", "INIT_ARRAY");
						break;
					case SHT_FINI_ARRAY: 
						printf("%15s%-10s", " ", "FINI_ARRAY");
						break;
					case SHT_PREINIT_ARRAY: 
						printf("%15s%-10s", " ", "PREINIT_ARRAY");
						break;
					case SHT_GROUP: 
						printf("%15s%-10s", " ", "GROUP");
						break;
					case SHT_SYMTAB_SHNDX: 
						printf("%15s%-10s", " ", "SYMTAB_SHNDX");
						break;
					case SHT_NUM: 
						printf("%15s%-10s", " ", "NUM");
						break;
					default:
					printf("%15s%-10s", " ", " ");
						break;
				
			   }
			   printf("%7s%016lx"," ",section.sh_addr);
			   printf("  %08lx\n", section.sh_offset);
			   printf("       %016lx", section.sh_size);
			   printf("  %016lx", section.sh_entsize);
			   switch(section.sh_flags){
				   case SHF_WRITE:
						printf("W");
						break;
					case SHF_ALLOC:
						printf("%4s","A");
						break;
					case SHF_EXECINSTR:
						printf("E");
						break;
					case SHF_MERGE:
						printf("M");
						break;
					case SHF_STRINGS:
						printf("S");
						break;
					case SHF_INFO_LINK:
						printf("I");
						break;
					case SHF_LINK_ORDER:
						printf("L");
						break;
					case SHF_OS_NONCONFORMING:
						printf("O");
						break;
					case SHF_GROUP:
						printf("G");
						break;
					case SHF_TLS:
						printf("T");
						break;
					case SHF_MASKOS:
						printf("MA");
						break;
					case SHF_MASKPROC:
						printf("MS");
						break;
					case SHF_ORDERED:
						printf("O");
						break;
					case SHF_EXCLUDE:
						printf("EX");
						break;
					default:
						printf("%4s"," ");
						break;
			   }
			   printf("%8x", section.sh_link);
			   printf("%6x", section.sh_info);
			   printf("     %lx", section.sh_addralign);
			   
			 }
			 printf("Key to Flags:\n");
			 printf("  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),\n");
			 printf("  L (link order), O (extra OS processing required), G (group), T (TLS),\n");
			 printf("  C (compressed), x (unknown), o (OS specific), E (exclude),\n");
			 printf("  l (large), p (processor specific)\n");

        }   
        else
        {
           printf("File Is Not an ELF File!\n");
        }

        fclose(file);
    }
    else
    {
        perror("fopen");
    }
}
int main(int argc, char** args)
{
    if(argc <= 1)
    {
        printf("No File Was Provided\n");
        
        return EXIT_FAILURE;
    }

    read_section_header(args[1]);

    return EXIT_SUCCESS;
}
