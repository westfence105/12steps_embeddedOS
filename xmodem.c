#include "defines.h"
#include "serial.h"
#include "lib.h"
#include "xmodem.h"

#define XMODEM_SOH 0x01
#define XMODEM_STX 0x02
#define XMODEM_EOT 0x04
#define XMODEM_ACK 0x06
#define XMODEM_NAK 0x15
#define XMODEM_CAN 0x18
#define XMODEM_EOF 0x1a

#define XMODEM_BLOCK_SIZE 128

static int xmodem_wait();
static int xmodem_read_block( uint8 block_number, uint8* buf );

long xmodem_recv( uint8* buf ){
    uint8 c;
    uint8 block_number = 1;
    long size = 0;
    int r;
    int receiving = 0;

    while(1){
        if( !receiving ) xmodem_wait();

        c = serial_recv_byte( SERIAL_DEFAULT_DEVICE );

        if( c == XMODEM_EOT ){ /* 受信終了 */
            serial_send_byte( SERIAL_DEFAULT_DEVICE, XMODEM_ACK );
            break;
        }
        else if( c == XMODEM_CAN ){ /* 受信中断 */
            return -1;
        }
        else if( c == XMODEM_SOH ){ /* 受信開始 */
            receiving++;

            r = xmodem_read_block( block_number, buf );
            if( r < 0 ){ /* 受信エラー */
                serial_send_byte( SERIAL_DEFAULT_DEVICE, XMODEM_NAK );
            }
            else {
                block_number++;
                size += r;
                buf  += r;
                serial_send_byte( SERIAL_DEFAULT_DEVICE, XMODEM_ACK );
            }
        }
        else {
            if( receiving ) return -1;
        }
    }

    return size;
}

static int xmodem_wait(){
    long cnt = 0;

    while( !serial_is_recv_enable(SERIAL_DEFAULT_DEVICE) ){
        if( ++cnt >= 2000000 ){
            cnt = 0;
            serial_send_byte( SERIAL_DEFAULT_DEVICE, XMODEM_NAK );
        }
    }

    return 0;
}

static int xmodem_read_block( uint8 block_number, uint8* buf ){
    int i;

    uint8 block_num = serial_recv_byte( SERIAL_DEFAULT_DEVICE );
    if( block_num != block_number ){
        return -1;
    }

    block_num ^= serial_recv_byte( SERIAL_DEFAULT_DEVICE );
    if( block_num != 0xff ){
        return -1;
    }


    uint8 c;
    uint8 check_sum = 0;
    for( i = 0; i < XMODEM_BLOCK_SIZE; ++i ){
        c = serial_recv_byte( SERIAL_DEFAULT_DEVICE );
        *(buf++) = c;
        check_sum += c;
    }

    check_sum ^= serial_recv_byte( SERIAL_DEFAULT_DEVICE );
    if( check_sum != 0 ){
        return -1;
    }

    return i;
}
