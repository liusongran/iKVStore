#include "driverlib.h"

#define I2C_USCI_BASE       USCI_B0_BASE
#define I2C_USCI_VECTOR     USCI_B0_VECTOR
#define I2C_USCI_IV         UCB0IV

#define I2C_BUF_LENGTH      32
static char i2c_buf[I2C_BUF_LENGTH];
static uint8_t i2c_buf_len = 0;
static uint8_t i2c_buf_cur = 0;

static uint8_t *i2c_rx_buf = 0;
static uint8_t i2c_rx_buf_len = 0;

void iKVS_I2CGpioInit(void){
    /* Select I2C function for I2C_SDA & I2C_SCL */
    GPIO_setAsPeripheralModuleFunctionOutputPin(
            GPIO_PORT_P3,
            GPIO_PIN0 | GPIO_PIN1);
}


/***************************************************************************//**
 * @brief  Configures I2C
 * @param  none
 * @return none
 ******************************************************************************/
void iKVS_I2CInit(uint8_t slaveAddress){
    /* I2C Master Configuration Parameter */
    USCI_B_I2C_initMasterParam i2cConfig =
    {
        USCI_B_I2C_CLOCKSOURCE_SMCLK,
        UCS_getSMCLK(),
        USCI_B_I2C_SET_DATA_RATE_400KBPS
    };

    /* Initialize USCI_B0 and I2C Master to communicate with slave devices*/
    USCI_B_I2C_initMaster(I2C_USCI_BASE, &i2cConfig);

    /* Enable I2C Module to start operations */
    USCI_B_I2C_enable(I2C_USCI_BASE);

    // Specify slave address
    USCI_B_I2C_setSlaveAddress(I2C_USCI_BASE, slaveAddress);
    USCI_B_I2C_clearInterrupt(I2C_USCI_BASE, USCI_B_I2C_RECEIVE_INTERRUPT);
    USCI_B_I2C_enableInterrupt(I2C_USCI_BASE, USCI_B_I2C_RECEIVE_INTERRUPT);
    USCI_B_I2C_clearInterrupt(I2C_USCI_BASE, USCI_B_I2C_TRANSMIT_INTERRUPT);
    USCI_B_I2C_enableInterrupt(I2C_USCI_BASE, USCI_B_I2C_TRANSMIT_INTERRUPT);

    return;
}



/***************************************************************************//**
 * @brief  write a byte to specific register, cannot called in interrupt context
 * @param  none
 * @return none
 ******************************************************************************/
void iKVS_I2CwriteByte(uint8_t byte){
    while (USCI_B_I2C_isBusBusy(I2C_USCI_BASE));

    USCI_B_I2C_setMode(I2C_USCI_BASE, USCI_B_I2C_TRANSMIT_MODE);

    // Initiate start and send first character
    i2c_buf[0] = byte;
    i2c_buf_cur = 1;
    i2c_buf_len = 1;
    USCI_B_I2C_masterSendMultiByteStart(I2C_USCI_BASE, i2c_buf[0]);

    // wait for end
    __bis_SR_register(GIE + LPM0_bits);
    __no_operation();
}

void iKVS_I2CwriteWord(uint16_t word){
    while (USCI_B_I2C_isBusBusy(I2C_USCI_BASE));

    USCI_B_I2C_setMode(I2C_USCI_BASE, USCI_B_I2C_TRANSMIT_MODE);

    // Initiate start and send first character
    i2c_buf[0] = word >> 8;
    i2c_buf[1] = (uint8_t)word;
    i2c_buf_cur = 1;
    i2c_buf_len = 2;
    USCI_B_I2C_masterSendMultiByteStart(I2C_USCI_BASE, i2c_buf[0]);

    // wait for end
    __bis_SR_register(GIE + LPM0_bits);
    __no_operation();
}

void iKVS_I2CWriteByte2DevReg(uint8_t dev, uint8_t reg, uint8_t byte, uint8_t flag){
    // Skip the rest setup after the very first call
    if(flag){
        USCI_B_I2C_setSlaveAddress(I2C_USCI_BASE, dev);
    }
    
    while (USCI_B_I2C_isBusBusy(I2C_USCI_BASE));
    USCI_B_I2C_setMode(I2C_USCI_BASE, USCI_B_I2C_TRANSMIT_MODE);

    // Initiate start and send first character
    i2c_buf[0] = reg;
    i2c_buf[1] = byte;
    i2c_buf_cur = 1;
    i2c_buf_len = 2;
    USCI_B_I2C_masterSendMultiByteStart(I2C_USCI_BASE, i2c_buf[0]);

    // wait for end
    __bis_SR_register(GIE + LPM0_bits);
    __no_operation();
}



/***************************************************************************//**
 * @brief  read some byte from specific register, cannot called in interrupt context
 * @param  none
 * @return none
 ******************************************************************************/
void iKVS_I2CreadByte(uint8_t dev, uint8_t RegAddr, uint8_t* b){
    USCI_B_I2C_setSlaveAddress(I2C_USCI_BASE, dev);

	while (USCI_B_I2C_isBusBusy(I2C_USCI_BASE));

    // send address
    USCI_B_I2C_setMode(I2C_USCI_BASE, USCI_B_I2C_TRANSMIT_MODE);
    i2c_buf_cur = 1;
    i2c_buf_len = 1;
    USCI_B_I2C_masterSendSingleByte(I2C_USCI_BASE, RegAddr);

    // receive
    USCI_B_I2C_setMode(I2C_USCI_BASE, USCI_B_I2C_RECEIVE_MODE);
    i2c_rx_buf = b;
    i2c_rx_buf_len = 1;
    USCI_B_I2C_masterReceiveSingleStart(I2C_USCI_BASE);

    // wait for end
    __bis_SR_register(GIE + LPM0_bits);
    __no_operation();
}

void iKVS_I2CreadBytes(uint8_t RegAddr, uint8_t length, uint8_t* data){
	while (USCI_B_I2C_isBusBusy(I2C_USCI_BASE));
    // send address
    USCI_B_I2C_setMode(I2C_USCI_BASE, USCI_B_I2C_TRANSMIT_MODE);
    i2c_buf_cur = 1;
    i2c_buf_len = 1;
    USCI_B_I2C_masterSendSingleByte(I2C_USCI_BASE, RegAddr);

    // receive
    USCI_B_I2C_setMode(I2C_USCI_BASE, USCI_B_I2C_RECEIVE_MODE);
    i2c_rx_buf = data;
    i2c_rx_buf_len = length;
    USCI_B_I2C_masterReceiveMultiByteStart(I2C_USCI_BASE);

    // wait for end
    __bis_SR_register(GIE + LPM0_bits);
    __no_operation();
}




#pragma vector = I2C_USCI_VECTOR
__interrupt void USCI_B0_ISR(void){
    switch (__even_in_range(I2C_USCI_IV, 12)){
        case USCI_I2C_UCTXIFG:
            if (i2c_buf_cur < i2c_buf_len){
                USCI_B_I2C_masterSendMultiByteNext( I2C_USCI_BASE, i2c_buf[i2c_buf_cur]);
                i2c_buf_cur++;
            }else{
                USCI_B_I2C_masterSendMultiByteStop(I2C_USCI_BASE);
                //Clear master interrupt status
                USCI_B_I2C_clearInterrupt(I2C_USCI_BASE,
                                          USCI_B_I2C_TRANSMIT_INTERRUPT);
                __bic_SR_register_on_exit(LPM0_bits);
            }
            break;
        case USCI_I2C_UCRXIFG:
            i2c_rx_buf_len--;
            if(i2c_rx_buf_len){
                if(i2c_rx_buf_len== 1){
                    //Initiate end of reception -> Receive byte with NAK
                    *i2c_rx_buf++ = USCI_B_I2C_masterReceiveMultiByteFinish( I2C_USCI_BASE);
                }else{
                    //Keep receiving one byte at a time
                    *i2c_rx_buf++ = USCI_B_I2C_masterReceiveMultiByteNext( I2C_USCI_BASE);
                }
            }else{
                //Receive last byte
                *i2c_rx_buf= USCI_B_I2C_masterReceiveMultiByteNext(I2C_USCI_BASE);
                __bic_SR_register_on_exit(LPM0_bits);
            }
            break;
    }
}
