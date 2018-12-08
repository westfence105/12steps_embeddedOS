#include "defines.h"
#include "serial.h"

#pragma bit_order left

#define SERIAL_SCI_NUM 3

#define H8_3069F_SCI0 ((volatile struct h8_3069f_sci *)0xffffb0)
#define H8_3069F_SCI1 ((volatile struct h8_3069f_sci *)0xffffb8)
#define H8_3069F_SCI2 ((volatile struct h8_3069f_sci *)0xffffc0)

struct h8_3069f_sci {
    union {
        volatile uint8 BYTE;
        struct {
            uint8 CA   :1;   //Communication mode
            uint8 CHR  :1;   //Character length
            uint8 PE   :1;   //Parity Enable
            uint8 PM   :1;   //Parity Mode (Odd/Even)
            uint8 STOP :1;   //Stop Bit Length
            uint8 MP   :1;   //Multi Processor mode
            uint8 CKS  :2;   //Clock Select
        } BIT;
    } SMR;

    volatile uint8 BRR;  //Bitrate

    union {
        volatile uint8 BYTE;
        struct {
            uint8 TIE  :1;  //Transmit Interrupt Enable
            uint8 RIE  :1;  //Receive Interrupt Enable
            uint8 TE   :1;  //Transmit Enable
            uint8 RE   :1;  //Receive Enable
            uint8 MPIE :1;  //Multi-Processor Interrupt Enable
            uint8 TEIE :1;  //Transmit-End Interrupt Enable
            uint8 CKE  :2;  //Clock Enable
        } BIT;
    } SCR;

    volatile uint8 TDR;  //Transmit-Data

    union {
        volatile uint8 BYTE;
        struct {
            uint8 TDRE :1;  //TDR(Transmit Data Register) Empty(Ready to catch next)
            uint8 RDRF :1;  //RDR(Receive Data Register) Full
            uint8 ORER :1;  //Over-run Error
            uint8 FER  :1;  //Framing Error / Error-Signal Status
            uint8 PER  :1;  //Parity Error
            uint8 TEND :1;  //Transmit-End
            uint8 MPB  :1;  //Multi-Processor Bit
            uint8 MPBT :1;  //Multi-Processor Bit Transfer
        } BIT;
    } SSR;

    volatile uint8 RDR;  //Receive-Data;

    union {
        volatile uint8 BYTE;
        struct {
            uint8      :4;
            uint8 SDIR :1;  //Smart-card data transfer Direction
            uint8 SINV :1;  //Smart-card data Invert
            uint8      :1;
            uint8 SMIF :1;  //Smart-card Interface mode
        } BIT;
    } SCMR; //Smart-Card Mode
};

static struct {
    volatile struct h8_3069f_sci* sci;
} regs[SERIAL_SCI_NUM] = {
    { H8_3069F_SCI0 },
    { H8_3069F_SCI1 },
    { H8_3069F_SCI2 },
};

int serial_init(int index){
    if( index < 0 || SERIAL_SCI_NUM <= index ) return -1;

    volatile struct h8_3069f_sci* sci = regs[index].sci;

    sci->SCR.BYTE = 0;
    sci->SMR.BYTE = 0;
    sci->BRR = 64; //20MHz -> 9600bps
    sci->SCR.BIT.RE = 1;
    sci->SCR.BIT.TE = 1;
    sci->SSR.BYTE = 0;

    return 0;
}

int serial_is_send_enable( int index ){
    if( index < 0 || SERIAL_SCI_NUM <= index ) return 0;

    return ( regs[index].sci->SCR.BIT.TE && regs[index].sci->SSR.BIT.TDRE );
}

int serial_send_byte( int index, unsigned char c ){
    if( index < 0 || SERIAL_SCI_NUM <= index ) return -1;

    volatile struct h8_3069f_sci* sci = regs[index].sci;

    while( !serial_is_send_enable(index) );

    sci->TDR = (uint8)c;
    sci->SSR.BIT.TDRE &= 0;

    return 0;
}

int serial_is_recv_enable( int index ){
    if( index < 0 || SERIAL_SCI_NUM <= index ) return 0;

    return ( regs[index].sci->SCR.BIT.RE && regs[index].sci->SSR.BIT.RDRF );
}

uint8 serial_recv_byte( int index ){
    if( index < 0 || SERIAL_SCI_NUM <= index ) return -1;

    volatile struct h8_3069f_sci* sci = regs[index].sci;

    while( !serial_is_recv_enable(index) );

    uint8 c = sci->RDR;
    sci->SSR.BIT.RDRF = 0;

    return c;
}
