#ifndef H8_12STEPS_SERIAL_H
#define H8_12STEPS_SERIAL_H

int serial_init( int index );
int serial_is_send_enable( int index );
int serial_send_byte( int index, unsigned char c );
int serial_is_recv_enable( int index );
uint8 serial_recv_byte( int index );

#endif
