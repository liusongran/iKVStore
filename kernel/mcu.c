#include <msp430.h>
#include <driverlib.h>
#include <mcu.h>
#include <stdint.h>


void __init_cs(uint32_t mclkFreq){
    UCS_initClockSignal(UCS_FLLREF, UCS_REFOCLK_SELECT, UCS_CLOCK_DIVIDER_1);
    UCS_initClockSignal(UCS_ACLK, UCS_REFOCLK_SELECT, UCS_CLOCK_DIVIDER_1);
    UCS_initFLLSettle(mclkFreq/1000, mclkFreq/32768);
        UCSCTL3 = (UCSCTL3 & ~(SELREF_7)) | (SELREF__REFOCLK);
        UCSCTL4 = (UCSCTL4 & ~(SELA_7)) | (SELA__REFOCLK);
}

void __init_gpio(void){
    P1OUT = 0x00;
    P1DIR = 0xFF;
    P2OUT = 0x00;
    P2DIR = 0xFF;
    P3OUT = 0x00;
    P3DIR = 0xFF;
    P4OUT = 0x00;
    P4DIR = 0xFF;
    P5OUT = 0x00;
    P5DIR = 0xFF;
    P6OUT = 0x00;
    P6DIR = 0xFF;
    P7OUT = 0x00;
    P7DIR = 0xFF;
    P8OUT = 0x00;
    P8DIR = 0xFF;
}


// Initializes the USCI_A1 module as a UART, using baudrate settings in bcUart.h.  The baudrate is dependent on SMCLK speed.
void __init_uartA(void){
    // Always use the step-by-step init procedure listed in the USCI chapter of
    // the F5xx Family User's Guide
    UCA1CTL1 |= UCSWRST;        // Put the USCI state machine in reset
    UCA1CTL1 |= UCSSEL__SMCLK;  // Use SMCLK as the bit clock

    // Set the baudrate
    UCA1BR0 = UCA1_BR0;
    UCA1BR1 = UCA1_BR1;
    UCA1MCTL = (UCA1_BRF << 4) | (UCA1_BRS << 1) | (UCA1_OS);

    P4SEL |= BIT4+BIT5;         // Configure these pins as TXD/RXD

    UCA1CTL1 &= ~UCSWRST;       // Take the USCI out of reset
    //UCA1IE |= UCRXIE;           // Enable the RX interrupt.  Now, when bytes are rcv'ed, the USCI_A1 vector will be generated.
}


// Sends 'len' bytes, starting at 'buf'
void bcUartSend(uint8_t * buf, uint8_t len){
    uint8_t i = 0;

    // Write each byte in buf to USCI TX buffer, which sends it out
    while (i < len){
        UCA1TXBUF = *(buf+(i++));
        // Wait until each bit has been clocked out...
        while(!(UCTXIFG==(UCTXIFG & UCA1IFG))&&((UCA1STAT & UCBUSY)==UCBUSY));
    }
}

void _blink_led_sec(uint8_t sec){
    uint8_t i;
    for(i=0; i<sec; i++){
        __delay_cycles(4000000);
        P1OUT |= 0x01;
        __delay_cycles(4000000);
        P1OUT &= 0b11111110;
    }
}

//cpu freq.:16MHz
//baud rate: 115200bps, data length:8, parity:None, stopbit:1
void _init_mcu(void){
    WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer
    __init_cs(16000000);
    __init_gpio();
    __init_uartA();
    _blink_led_sec(5);
}
