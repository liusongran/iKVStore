#ifndef MSP430F5529_MCU_H_
#define MSP430F5529_MCU_H_
#include <msp430.h>
#include <driverlib.h>
#include <mcu.h>

#define UCA1_OS     1   // 1 = oversampling mode, 0 = low-freq mode
#define UCA1_BR0    8   // Value of UCA1BR0 register
#define UCA1_BR1    0   // Value of UCA1BR1 register
#define UCA1_BRS    0   // Value of UCBRS field in UCA1MCTL register
#define UCA1_BRF    11  // Value of UCBRF field in UCA1MCTL register

void bcUartSend(uint8_t * buf, uint8_t len);
void _init_mcu(void);
void _blink_led_sec(uint8_t sec);
#endif /* MSP430F5529_MCU_H_ */
