/*
* ETF Banja Luka (PuRV - 2022)
* Aleksandar Beslic 1198/17
* remake of: readelf --file-header  (64-bit)
**/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<elf.h>

void read_file_header(const char* file_name)
{
    Elf64_Ehdr header;
    FILE* file = fopen(file_name, "rb");

    if(file)
    {
        // read the header
        fread(&header, sizeof(header), 1, file);

        // check the elf file (7f E F L)
        if (memcmp(header.e_ident, ELFMAG, SELFMAG) == 0) 
        {
            
            printf("ELF header:\n");

              /********************************************/
             /*              Magic number                */
            /********************************************/

            printf("  %-36s ", "Magic:");
            for (size_t i = 0; i < EI_NIDENT; i++)
            {
                printf("%02x ", header.e_ident[i]);
            }
            printf("\n");

              /********************************************/
             /*              Class                       */
            /********************************************/
            
            printf("  %-36s ", "Class:");
            switch (header.e_ident[EI_CLASS])
            {
            case ELFCLASSNONE:
                printf("Invalid class\n");
                break;

            case ELFCLASS32:
                printf("ELF32\n");
                break;

            case ELFCLASS64:
                printf("ELF64\n");
                break;
            default:
                break;
            }

              /********************************************/
             /*              Data                        */
            /********************************************/

            printf("  %-36s ", "Data:");
            switch (header.e_ident[EI_DATA])
            {
            case ELFDATANONE:
                printf("Invalid data encoding\n");
                break;

            case ELFDATA2LSB:
                printf("2's complement, little-endian\n");
                break;

            case ELFDATA2MSB:
                printf("2's complement, big-endian\n");
                break;

            default:
                break;
            }
            
              /********************************************/
             /*              Version                     */
            /********************************************/

            printf("  %-36s ", "Version:");
            if (header.e_ident[EI_VERSION] == EV_CURRENT)
            {
                printf("%0#2x (Current)\n", EV_CURRENT);
            }
            else
            {
                printf("Invalid version\n");
            }

              /********************************************/
             /*              OS/ABI                      */
            /********************************************/

            printf("  %-36s ", "OS/ABI:");
            switch (header.e_ident[EI_OSABI])
            {
            case ELFOSABI_SYSV:
                printf("UNIX System V ABI\n");
                break;
            
            case ELFOSABI_HPUX:
                printf("HP-UX ABI\n");
                break;
            
            case ELFOSABI_NETBSD:
                printf("NetBSD ABI\n");
                break;
            
            case ELFOSABI_LINUX:
                printf("Linux ABI\n");
                break;
            
            case ELFOSABI_SOLARIS:
                printf("Solaris ABI\n");
                break;
            
            case ELFOSABI_IRIX:
                printf("IRIX ABI\n");
                break;
            
            case ELFOSABI_FREEBSD:
                printf("FreeBSD ABI\n");
                break;
            
            case ELFOSABI_TRU64:
                printf("TRU64 UNIX ABI\n");
                break;
            
            case ELFOSABI_ARM:
                printf("ARM architecture ABI\n");
                break;
            
            case ELFOSABI_STANDALONE:
                printf("Stand-alone (embedded) ABI\n");
                break;
            
            default:
                break;
            }

              /********************************************/
             /*              ABI Version                 */
            /********************************************/

            printf("  %-36s %d\n", "ABI Version:", header.e_ident[EI_ABIVERSION]);

              /********************************************/
             /*              Type                        */
            /********************************************/

            printf("  %-36s ", "Type:");
            switch (header.e_type)
            {
            case ET_NONE:
                printf("No file type\n");
                break;
            
            case ET_REL:
                printf("Relocatable file\n");
                break;
            
            case ET_EXEC:
                printf("Executable file\n");
                break;
            
            case ET_DYN:
                printf("Shared object file\n");
                break;
            
            case ET_CORE:
                printf("Core file\n");
                break;
            
            case ET_LOPROC:
                printf("LOPROC (Processor-specific)\n");
                break;
            
            case ET_HIPROC:
                printf("HIPROC (Processor-specific)\n");
                break;
            
            default:
                break;
            }

              /********************************************/
             /*             Valid Machine types          */
            /********************************************/

            printf("  %-36s ", "Machine:");
            switch (header.e_machine)
            {
            case EM_NONE:
                printf("An unknown machine\n");
                break;
            
            case EM_M32:
                printf("AT&T WE 32100\n");
                break;
            
            case EM_SPARC:
                printf("Sun Microsystems SPARC\n");
                break;
            
            case EM_386:
                printf("Intel 80386\n");
                break;
            
            case EM_68K:
                printf("Motorola 68000\n");
                break;
            
            case EM_88K:
                printf("Motorola 88000\n");
                break;
            
            case EM_860:
                printf("Intel 80860\n");
                break;
            
            case EM_MIPS:
                printf("MIPS RS3000 (big-endian only)\n");
                break;
            
            case EM_PARISC:
                printf("HP/PA\n");
                break;
            
            case EM_SPARC32PLUS:
                printf("SPARC with enhanced instruction set\n");
                break;
            
            case EM_PPC:
                printf("PowerPC\n");
                break;
            
            case EM_PPC64:
                printf("PowerPC 64-bit\n");
                break;
            
            case EM_S390:
                printf("IBM S/390\n");
                break;
            
            case EM_ARM:
                printf("Advanced RISC Machines\n");
                break;
            
            case EM_SH:
                printf("Renesas SuperH\n");
                break;
            
            case EM_SPARCV9:
                printf("SPARC v9 64-bit\n");
                break;
            
            case EM_IA_64:
                printf("Intel Itanium\n");
                break;
            
            case EM_X86_64:
                printf("AMD x86-64\n");
                break;
            
            case EM_VAX:
                printf("DEC Vax\n");
                break;
            
            case EM_RISCV:
                printf("RISC-V\n");
                break;
            
            default:
                break;
            }

              /********************************************/
             /*             Version                      */
            /********************************************/

            printf("  %-36s ", "Version:");
            if (header.e_version == EV_CURRENT)
            {
                printf("%0#2x\n", header.e_version);
            }
            else
            {
                printf("Invalid version\n");
            }

              /********************************************/
             /*              Other stuff                 */
            /********************************************/

            printf("  %-36s 0x%lx\n", "Entry point address:", header.e_entry);
            printf("  %-36s %lu (bytes into file)\n", "Start of program headers:", header.e_phoff);
            printf("  %-36s %lu (bytes into file)\n", "Start of section headers:", header.e_shoff);
            printf("  %-36s 0x%x\n", "Flags:", header.e_flags);
            printf("  %-36s %d (bytes)\n", "Size of this header:", header.e_ehsize);
            printf("  %-36s %d (bytes)\n", "Size of program headers:", header.e_phentsize);
            printf("  %-36s %d\n", "Number of program headers:", header.e_phnum);
            printf("  %-36s %d (bytes)\n", "Size of section headers:", header.e_shentsize);
            printf("  %-36s %d\n", "Number of section headers:", header.e_shnum);
            printf("  %-36s %d\n", "Section header string table index:", header.e_shstrndx);
            
        }   
        else
        {
            printf("File That Was Providet Is Not an ELF File!\n");
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

    read_file_header(args[1]);

    return EXIT_SUCCESS;
}