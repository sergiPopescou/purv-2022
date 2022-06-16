#include <stdio.h>
#include <elf.h>
#include <inttypes.h>
#include <stdlib.h>


int main(int argc, char** argv)
{
    FILE* elf_file = fopen(argv[1], "rb");
   
    if(elf_file)
    {
        Elf64_Ehdr elf_header;
        fread(&elf_header, sizeof(elf_header), 1, elf_file);
       
        if(elf_header.e_ident[0] == 0x7F && elf_header.e_ident[1] == 'E' && elf_header.e_ident[2]== 'L' && elf_header.e_ident[3]== 'F')
        {
            Elf64_Phdr p_header;
            Elf64_Phdr array[elf_header.e_phnum];
            rewind(elf_file);

            printf("Elf file type is ");
            switch (elf_header.e_type)
            {
            case ET_NONE:
                printf("NONE (An Unkown type)\n");
                break;
            case ET_REL:
                printf("REL (A Relocatable file)\n");
                break;
            case ET_CORE:
                printf("CORE (A Core file)\n");
                break;
            case ET_DYN:
                printf("DYN (A Shared object)\n");
                break;
            case ET_EXEC:
                printf("EXEC (An Executable file)\n");
                break;     
            default:
                break;
            }

            printf("Entery point is 0x%"PRIx64"\n", elf_header.e_entry);
            printf("There are %d program headers starting at offset %"PRIu64"\n", elf_header.e_phnum, elf_header.e_phoff);
            printf("\nProgram Headers:\nType   Offset   VirtAddr   PhysAddr   FileSiz   MemSiz   Flags   Align\n");

            for(int i = 0; i < elf_header.e_phnum; ++i)
            { 
                fseek(elf_file, elf_header.e_phoff + i * sizeof(p_header), SEEK_SET);
                fread(&p_header, sizeof(p_header), 1, elf_file);
                array[i] = p_header;
                switch (p_header.p_type)
                {
                case PT_NULL:
                    printf("\nNULL ");
                    break;
                case PT_LOAD:
                    printf("\nLOAD ");
                    break;
                case PT_DYNAMIC:
                    printf("\nDYNAMIC ");
                    break;
                case PT_INTERP:
                    printf("\nINTERP ");
                    break;
                case PT_NOTE:
                    printf("\nNOTE ");
                    break;
                case PT_SHLIB:
                    printf("\nSHLIB ");
                    break;
                case PT_PHDR:
                    printf("\nPHDR ");
                    break;
                case PT_GNU_EH_FRAME:
                    printf("\nGNU_EH_FRAME ");
                    break;
                case PT_GNU_STACK:
                    printf("\nGNU_STACK ");
                    break;
                case PT_GNU_RELRO:
                    printf("\nGNU_RELRO ");
                    break;
                default:
                    printf("\nSPECIFIC ");
                    break;
                }

                printf("0x%"PRIx64" ", p_header.p_offset);
                printf("0x%"PRIx64" ", p_header.p_vaddr);
                printf("0x%"PRIx64" ", p_header.p_paddr);
                printf("0x%"PRIx64" ", p_header.p_filesz);
                printf("0x%"PRIx64" ", p_header.p_memsz);
                switch (p_header.p_flags)
                {
                case PF_X:
                    printf("X ");
                    break;
                case PF_R:
                    printf("R ");
                    break;
                case PF_W:
                    printf("W ");
                    break;
                default:
                    break;
                }
                printf("0x%"PRIx64"\n", p_header.p_align);
            }
            
            
            
            printf("\nSegment to section mapping:\n");
            printf("Segment sections...\n");
            for(int i = 0; i < elf_header.e_phnum; ++i)
            {
                printf("%d ", i);
                char* sect_names = NULL;
                Elf64_Shdr section;

                fseek(elf_file, elf_header.e_shoff + elf_header.e_shstrndx * sizeof(section), SEEK_SET);
                fread(&section, sizeof(section), 1, elf_file);

                sect_names = (char*)malloc(section.sh_size);
                fseek(elf_file, section.sh_offset, SEEK_SET);
                fread(sect_names, section.sh_size, 1, elf_file);

                for (int j = 0; j < elf_header.e_shnum; j++)
                {
                    const char* name = "";

                    fseek(elf_file, elf_header.e_shoff + j * sizeof(section), SEEK_SET);
                    fread(&section, sizeof(section),1, elf_file);

                    if (section.sh_offset >= array[i].p_offset && section.sh_offset < array[i].p_offset + array[i].p_filesz)
                    {
                        name = sect_names + section.sh_name;
                        printf("%s ", name);
                    }
                }
                printf("\n");
                free(sect_names);
            }
        } 
        else
        {
            printf("This is not a ELF file!");
        }
    }
    else
    {
        printf("Error, file didn't open correctly!");
    }
    return 0;
}
