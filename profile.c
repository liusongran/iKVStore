#include <profile.h>
#include <string.h>

int fputc(int _c, register FILE *_fp){
    //EUSCI_A_UART_transmitData(EUSCI_A0_BASE, (unsigned char) _c );
    uint8_t cTemp = (unsigned char) _c;
    bcUartSend(&cTemp, 1);
    return((unsigned char)_c);
}

int fputs(const char *_ptr, register FILE *_fp){
    unsigned int len;
    len = strlen(_ptr);
    bcUartSend((uint8_t *)_ptr, len);
    return len;
}
