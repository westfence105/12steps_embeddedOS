#include "defines.h"
#include "elf.h"
#include "lib.h"

struct elf_header {
    struct {
        uint8 magic[4];
        uint8 class;
        uint8 format;
        uint8 version;
        uint8 abi;
        uint8 abi_version;
        uint8 reserve[7];
    } id;

    short type;
    short arch;
    long version;
    long entry_point;
    long program_header_offset;
    long section_header_offset;
    long flags;
    short header_size;
    short program_header_size;
    short program_header_count;
    short section_header_size;
    short section_header_count;
    short section_name_index;
};

struct elf_program_header {
    long type;
    long offset;
    long virtual_addr;
    long physical_addr;
    long file_size;
    long memory_size;
    long flags;
    long align;
};

static int elf_check( const struct elf_header* header ){
    if( memcmp( header->id.magic, "\x7f""ELF", 4 ) != 0 ) return -1;
    if( header->id.class   != 1 ) return -1; //ELF32
    if( header->id.format  != 2 ) return -1; //Big Endian
    if( header->id.version != 1 ) return -1; //version 1
    if( header->type       != 2 ) return -1; //Executable File
    if( header->version    != 1 ) return -1; //version 1
    if( header->arch != 46 && header->arch != 47 ) return -1; // H8/300 or H8/300H

    return 0;
}

static int elf_load_program( struct elf_header* header ){
    struct elf_program_header* program_header;

    void* header_addr = header;
    header_addr += header->program_header_offset;

    for( int i = 0; i < header->program_header_count; ++i ){
        program_header = (struct elf_program_header*)header_addr;

        if( program_header->type != 1 ) continue; //is loadable

    //    puts("Header "); putxval( i, 0 ); puts(":\n");
    //    puts(" offset:            0x"); putxval( program_header->offset,        6 ); putc('\n');
    //    puts(" virtual address:   0x"); putxval( program_header->virtual_addr,  8 ); putc('\n');
    //    puts(" physical address:  0x"); putxval( program_header->physical_addr, 8 ); putc('\n');
    //    puts(" file size:         0x"); putxval( program_header->file_size,     5 ); putc('\n');
    //    puts(" memory size:       0x"); putxval( program_header->memory_size,   5 ); putc('\n');
    //    puts(" flags:             0x"); putxval( program_header->flags,         2 ); putc('\n');
    //    puts(" align:             0x"); putxval( program_header->align,         2 ); putc('\n');
    //    putc('\n');

        memcpy( (uint8*)program_header->physical_addr, (uint8*)header + program_header->offset, program_header->file_size );
        memset( (uint8*)program_header->physical_addr + program_header->file_size, 0, program_header->memory_size - program_header->file_size );

        header_addr += header->program_header_size;
    }

    return 0;
}

unsigned long elf_load( uint8* buf ){
    struct elf_header* header = (struct elf_header*)buf;

    if( elf_check( header ) < 0 ) return NULL;
    if( elf_load_program( header ) < 0 ) return NULL;

    return header->entry_point;
}
