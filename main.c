#include "defines.h"
#include "serial.h"
#include "lib.h"

int main(){
    serial_init(SERIAL_DEFAULT_DEVICE);
    puts("Hello World!\n");

    putxval( 0x10,   0 ); putc('\n');
    putxval( 0xffff, 0 ); putc('\n');

    while(1);
    return 0;
}
