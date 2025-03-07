#ifndef MSP430F5529_I2C_H_
#define MSP430F5529_I2C_H_

void iKVS_I2CGpioInit(void);
void iKVS_I2CInit(uint8_t slaveAddress);
void iKVS_I2CwriteByte(uint8_t byte);
void iKVS_I2CwriteWord(uint16_t word);
void iKVS_I2CWriteByte2DevReg(uint8_t dev, uint8_t reg, uint8_t byte, uint8_t flag);
void iKVS_I2CreadByte(uint8_t dev, uint8_t RegAddr, uint8_t* b);
void iKVS_I2CreadBytes(uint8_t RegAddr, uint8_t length, uint8_t* data);

#endif /* MSP430F5529_I2C_H_ */
