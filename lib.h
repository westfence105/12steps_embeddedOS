#ifndef H8_12STEPS_LIB_H
#define H8_12STEPS_LIB_H

void* memset( void* p, int c, long len );
void* memcpy( void* dest, const void* src, long len );
int memcmp( const void* p1, const void* p2, long len );
int strlen( const char* str );
char* strcpy( char* dest, const char* src );
int strcmp( const char* s1, const char* s2 );
int strncmp( const char* s1, const char* s2, long len );

int putc( char c );
int puts( char* str );
int putxval( unsigned long value, int column );

#endif
