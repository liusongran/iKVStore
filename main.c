#include "driverlib.h"
#include "mcu.h"
#include "profile.h"
//******************************************************************************
//!
//!   MSP430F5529 Flash memory: 0x4400 ~ 0xFFFF
//!   cpu freq.: 16MHz
//!   baud rate: 115200bps, data length:8, parity:None, stopbit:1
//******************************************************************************
void main (void){
    _init_mcu();
    printf("------Let's GO.------\r\n");
    while(1);
    // uint16_t record[3];
    // record = __dummy_sensor_read(); //TODO: in kernel.c
    // ts = __get_timestamp();         //TODO: in kernel.c
    // __ikv_put(ts, record);        //TODO: in kernel.c
}
