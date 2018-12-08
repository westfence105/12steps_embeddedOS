#include "defines.h"
#include "serial.h"
#include "lib.h"

int main(){
    serial_init(SERIAL_DEFAULT_DEVICE);
    puts("Hello World!\n");

    char buf[32];
    long len;
    while(1){
        puts("> ");
        len = gets( (uint8*)buf );

        if( len > 31 ){
            puts("Warning: Too long input.");
        }

        if( strncmp( buf, "echo ", 5 ) == 0 ){
            puts( buf + 5 );
            putc('\n');
        }
        else if( strcmp( buf, "exit" ) == 0 ){
            putc('\n');
            break;
        }
        else {
            puts("unknown command.\n");
        }
    }

    puts("exit");
    return 0;
}
