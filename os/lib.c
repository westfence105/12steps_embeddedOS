#include "defines.h"
#include "serial.h"
#include "lib.h"

void* memset( void* p, int c, long len ){
    for( char* q = p; 0 < len; --len ){
        *(q++) = c;
    }
    return p;
}

void* memcpy( void* dest, const void* src, long len ){
    char* d = dest;
    const char* s = src;
    while( 0 < len-- ){
        *(d++) = *(s++);
    }
    return d;
}

int memcmp( const void* p1, const void* p2, long len ){
    const char* b1 = p1;
    const char* b2 = p2;
    while( 0 < len-- ){
        if( *b1 != *b2 ){
            return ( *b2 - *b1 );
        }
        ++b1;
        ++b2;
    }
    return 0;
}

long strlen( const char* str ){
    long len = 0;
    while( *(str++) ) len++;
    return len;
}

char* strcpy( char* dest, const char* src ){
    char* d = dest;
    while( *src ){
        *(d++) = *(src++);
    }
    return d;
}

int strcmp( const char* s1, const char* s2 ){
    while( *s1 || *s2 ){
        if( *s1 != *s2 ) return *s2 - *s1;
        ++s1;
        ++s2;
    }
    return 0;
}

int strncmp( const char* s1, const char* s2, long len ){
    while( ( *s1 || *s2 ) && len-- ){
        if( *s1 != *s2 ) return *s2 - *s1;
        ++s1;
        ++s2;
    }
    return 0;
}

int putc( char c ){
    if( c == '\n' ){
        serial_send_byte( SERIAL_DEFAULT_DEVICE, '\r' );
    }

    return serial_send_byte( SERIAL_DEFAULT_DEVICE, *((unsigned char*)&c) );
}

long puts( char* str ){
    long i;
    for( i = 0; *str; i++ ) putc( *(str++) );
    return i;
}

uint8 getc(){
    uint8 c = serial_recv_byte( SERIAL_DEFAULT_DEVICE );
    serial_send_byte( SERIAL_DEFAULT_DEVICE, c ); //エコーバック
    return c;
}

long gets( uint8* buf ){
    long i = 0;
    uint8 c;
    do {
        c = getc();
        if( c == '\r' ) continue; //CRを無視
        if( c == '\n' ) c = 0;    //LFが来たら抜ける
        buf[i++] = c;
    } while(c);

    return i - 1;
}

int putxval( unsigned long value, int column ){
    char buf[9];
    char *p;
    p = buf + sizeof(buf) - 1;
    *(p--) = 0;

    if( !value && !column ) column++;

    while( value || column ){
        *(p--) = "0123456789abcdef"[value&0x0f];
        value >>= 4;
        if( column ) column--;
    }

    puts( p + 1 );

    return 0;
}
