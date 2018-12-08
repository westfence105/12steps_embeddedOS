#include "defines.h"
#include "serial.h"
#include "lib.h"
#include "xmodem.h"
#include "elf.h"

static int init(){
    extern int erodata, data_start, edata, bss_start, ebss;
    memcpy( &data_start, &erodata, (long)&edata - (long)&data_start );
    memset( &bss_start, 0, (long)&ebss - (long)&bss_start );

    serial_init(SERIAL_DEFAULT_DEVICE);

    return 0;
}

static int dump( uint8* buf, long size ){
    long i;

    if( size < 0 ){
        puts("no data.\n");
        return -1;
    }

    for( i = 0; i < size; ++i ){
        putxval( buf[i], 2 );

        if( ( i & 0xf ) == 0xf ){
            putc('\n');
        }
        else {
            if( ( i & 0xf ) == 7 ) putc(' ');
            putc(' ');
        }
    }
    putc('\n');

    return 0;
}

static void wait(){
    volatile long i;
    for( i = 0; i < 300000; ++i );
}

int main(){
    static char buf[16];
    static long size = -1;
    static uint8* loadbuf = NULL;
    extern int buffer_start;

    long command_length;

    init();

    puts("bootloader started.\n");

    while(1){
        puts("bootloader> ");
        command_length = gets( (uint8*)buf );

        if( strcmp( buf, "load" ) == 0 ){
            puts("Waiting XMODEM Transfer...\n");
            loadbuf = (uint8*)&buffer_start;
            size = xmodem_recv(loadbuf);
            wait();
            if( size < 0 ){
                puts("\nXMODEM Receive Error!\n");
            }
            else {
                puts("\nXMODEM Receive Succeeded.\n");
            }
        }
        else if( strcmp( buf, "dump" ) == 0 ){
            puts("size: "); putxval( size, 0 ); putc('\n');
            dump( loadbuf, size );
        }
        else if( strcmp( buf, "run" ) == 0 ){
            elf_load( loadbuf );
        }
        else if( strcmp( buf, "shutdown" ) == 0 ){
            break;
        }
        else if( command_length > sizeof(buf) ){
            puts("Warning: Too long input.");
        }
        else {
            puts("unknown command.\n");
        }
    }

    puts("Bye.\n\n");
    return 0;
}
