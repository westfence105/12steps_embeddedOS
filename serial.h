#ifndef H8_12STEPS_SERIAL_H
#define H8_12STEPS_SERIAL_H

int serial_init( int index );
int serial_is_send_enable( int index );
int serial_send_byte( int index, unsigned char c );

#endif
