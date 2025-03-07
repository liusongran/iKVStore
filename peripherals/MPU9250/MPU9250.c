#include "MPU9250.h"
#include "driverlib.h"
#include "iKVS_I2C.h"

int16_t magn[3];
int16_t accel[3];
int16_t gyro[3];
unsigned char BUF[10];
MPU9250_TypeDef MPU9250_Offset={0, 0, 0};
MPU9250_TypeDef_Off MPU9250_Magn_Offset={0, 0, 0};

void MPU9250_CalAvgValue(uint8_t *pIndex, int16_t *pAvgBuffer, int16_t InVal, int32_t *pOutVal){	
	uint8_t i;
	
	*(pAvgBuffer + ((*pIndex) ++)) = InVal;
  	*pIndex &= 0x07;
  	
  	*pOutVal = 0;
	for(i = 0; i < 8; i ++) {
    	*pOutVal += *(pAvgBuffer + i);
  	}
  	*pOutVal >>= 3;
}


void iKVS_PeriMPU9250_InitGyrOffset(void){
	uint8_t i;
	int32_t	TempGx = 0, TempGy = 0, TempGz = 0;
	
 	for(i = 0; i < 32; i ++){
		iKVS_PeriMPU9250_ReadGyro();
		
		TempGx += gyro[0];
		TempGy += gyro[1];
		TempGz += gyro[2];
		// TODO: Delay_us(100);
	}

	MPU9250_Offset.XX = TempGx >> 5;
	MPU9250_Offset.YY = TempGy >> 5;
    MPU9250_Offset.ZZ = TempGz >> 5;
}



void iKVS_PeriMPU9250_Init(void){
    // I2C_WriteOneByte(GYRO_ADDRESS, PWR_MGMT_1, 0x00);
	// I2C_WriteOneByte(GYRO_ADDRESS, SMPLRT_DIV, 0x07);
	// I2C_WriteOneByte(GYRO_ADDRESS, CONFIG, 0x06);
	// I2C_WriteOneByte(GYRO_ADDRESS, GYRO_CONFIG, 0x10);
	// I2C_WriteOneByte(GYRO_ADDRESS, ACCEL_CONFIG, 0x01);
    iKVS_I2CWriteByte2DevReg(GYRO_ADDRESS, PWR_MGMT_1, 0x00, 1);
    iKVS_I2CWriteByte2DevReg(GYRO_ADDRESS, SMPLRT_DIV, 0x07, 0);
    iKVS_I2CWriteByte2DevReg(GYRO_ADDRESS, CONFIG, 0x06, 0);
    iKVS_I2CWriteByte2DevReg(GYRO_ADDRESS, GYRO_CONFIG, 0x10, 0);
    iKVS_I2CWriteByte2DevReg(GYRO_ADDRESS, ACCEL_CONFIG, 0x01, 0);
	
	// TODO: Delay_ms(10);
	if(iKVS_PeriMPU9250_Check(GYRO_ADDRESS)){
		// TODO: GPIO_SetBits(GPIOB, GPIO_Pin_15);
	}else{
		// TODO: GPIO_ResetBits(GPIOB, GPIO_Pin_15);
	}
	
	iKVS_PeriMPU9250_InitGyrOffset();
}

void iKVS_PeriMPU9250_ReadAccel(void){ 
    uint8_t i;
    int16_t InBuffer[3] = {0}; 
	static int32_t OutBuffer[3] = {0};
	static MPU9250_AvgTypeDef MPU9250_Filter[3];

    // BUF[0] = I2C_ReadOneByte(ACCEL_ADDRESS, ACCEL_XOUT_L); 
    iKVS_I2CreadByte(ACCEL_ADDRESS, ACCEL_XOUT_L, &BUF[0]);
    // BUF[1] = I2C_ReadOneByte(ACCEL_ADDRESS, ACCEL_XOUT_H);
    iKVS_I2CreadByte(ACCEL_ADDRESS, ACCEL_XOUT_H, &BUF[1]);
    InBuffer[0] = (BUF[1]<<8) | BUF[0];

    // BUF[2] = I2C_ReadOneByte(ACCEL_ADDRESS, ACCEL_YOUT_L);
    iKVS_I2CreadByte(ACCEL_ADDRESS, ACCEL_YOUT_L, &BUF[2]);
    // BUF[3] = I2C_ReadOneByte(ACCEL_ADDRESS, ACCEL_YOUT_H);
    iKVS_I2CreadByte(ACCEL_ADDRESS, ACCEL_YOUT_H, &BUF[3]);
    InBuffer[1] = (BUF[3]<<8) | BUF[2];
					   
    // BUF[4] = I2C_ReadOneByte(ACCEL_ADDRESS, ACCEL_ZOUT_L);
    iKVS_I2CreadByte(ACCEL_ADDRESS, ACCEL_ZOUT_L, &BUF[4]);
    // BUF[5] = I2C_ReadOneByte(ACCEL_ADDRESS, ACCEL_ZOUT_H);
    iKVS_I2CreadByte(ACCEL_ADDRESS, ACCEL_ZOUT_H, &BUF[5]);
    InBuffer[2] = (BUF[5]<<8) | BUF[4];			       
   
    for(i = 0; i < 3; i ++)	{
      MPU9250_CalAvgValue(&MPU9250_Filter[i].Index, MPU9250_Filter[i].AvgBuffer, InBuffer[i], OutBuffer + i);
    }

    accel[0] = *(OutBuffer + 0);
    accel[1] = *(OutBuffer + 1);
    accel[2] = *(OutBuffer + 2); 
}

void iKVS_PeriMPU9250_ReadGyro(void){ 
    uint8_t i;
	int16_t InBuffer[3] = {0}; 
	static int32_t OutBuffer[3] = {0};
	static MPU9250_AvgTypeDef MPU9250_Filter[3];

    // BUF[0] = I2C_ReadOneByte(GYRO_ADDRESS, GYRO_XOUT_L); 
    iKVS_I2CreadByte(GYRO_ADDRESS, GYRO_XOUT_L, &BUF[0]);
    // BUF[1] = I2C_ReadOneByte(GYRO_ADDRESS, GYRO_XOUT_H);
    iKVS_I2CreadByte(GYRO_ADDRESS, GYRO_XOUT_H, &BUF[1]);
    InBuffer[0] = (BUF[1]<<8) | BUF[0];
   
    // BUF[2] = I2C_ReadOneByte(GYRO_ADDRESS, GYRO_YOUT_L);
    iKVS_I2CreadByte(GYRO_ADDRESS, GYRO_YOUT_L, &BUF[2]);
    // BUF[3] = I2C_ReadOneByte(GYRO_ADDRESS, GYRO_YOUT_H);
    iKVS_I2CreadByte(GYRO_ADDRESS, GYRO_YOUT_H, &BUF[3]);
    InBuffer[1] = (BUF[3]<<8) | BUF[2];
    
    // BUF[4] = I2C_ReadOneByte(GYRO_ADDRESS, GYRO_ZOUT_L);
    iKVS_I2CreadByte(GYRO_ADDRESS, GYRO_ZOUT_L, &BUF[4]);
    // BUF[5] = I2C_ReadOneByte(GYRO_ADDRESS, GYRO_ZOUT_H);
    iKVS_I2CreadByte(GYRO_ADDRESS, GYRO_ZOUT_H, &BUF[5]);
    InBuffer[2] = (BUF[5]<<8) | BUF[4];	

    for(i = 0; i < 3; i ++)	{
      MPU9250_CalAvgValue(&MPU9250_Filter[i].Index, MPU9250_Filter[i].AvgBuffer, InBuffer[i], OutBuffer + i);
    }

    gyro[0] = *(OutBuffer + 0) - MPU9250_Offset.XX;
    gyro[1] = *(OutBuffer + 1) - MPU9250_Offset.YY;
    gyro[2] = *(OutBuffer + 2) - MPU9250_Offset.ZZ;
}


void iKVS_PeriMPU9250_ReadMag(void){ 
    uint8_t i;
    int16_t InBuffer[3] = {0}; 
    static int32_t OutBuffer[3] = {0};
    static MPU9250_AvgTypeDef MPU9250_Filter[3];

    // I2C_WriteOneByte(GYRO_ADDRESS, 0x37, 0x02);//turn on Bypass Mode 
    iKVS_I2CWriteByte2DevReg(GYRO_ADDRESS, 0x37, 0x02, 1);
    // TODO: Delay_ms(10);
    // I2C_WriteOneByte(MAG_ADDRESS, 0x0A, 0x01);	
    iKVS_I2CWriteByte2DevReg(MAG_ADDRESS, 0x0A, 0x01, 1);
    // TODO: Delay_ms(10);

    // BUF[0] = I2C_ReadOneByte(MAG_ADDRESS, MAG_XOUT_L);
    iKVS_I2CreadByte(MAG_ADDRESS, MAG_XOUT_L, &BUF[0]);
    // BUF[1] = I2C_ReadOneByte(MAG_ADDRESS, MAG_XOUT_H);
    iKVS_I2CreadByte(MAG_ADDRESS, MAG_XOUT_H, &BUF[1]);
    InBuffer[1] = (BUF[1]<<8) | BUF[0];

    // BUF[2] = I2C_ReadOneByte(MAG_ADDRESS, MAG_YOUT_L);
    iKVS_I2CreadByte(MAG_ADDRESS, MAG_YOUT_L, &BUF[2]);
    // BUF[3] = I2C_ReadOneByte(MAG_ADDRESS, MAG_YOUT_H);
    iKVS_I2CreadByte(MAG_ADDRESS, MAG_YOUT_H, &BUF[3]);
    InBuffer[0] = (BUF[3]<<8) | BUF[2];
    
    // BUF[4] = I2C_ReadOneByte(MAG_ADDRESS, MAG_ZOUT_L);
    iKVS_I2CreadByte(MAG_ADDRESS, MAG_ZOUT_L, &BUF[4]);
    // BUF[5] = I2C_ReadOneByte(MAG_ADDRESS, MAG_ZOUT_H);
    iKVS_I2CreadByte(MAG_ADDRESS, MAG_ZOUT_H, &BUF[5]);
    InBuffer[2] = (BUF[5]<<8) | BUF[4];	
    InBuffer[2] = -InBuffer[2];
	 
    for(i = 0; i < 3; i ++)	{
      MPU9250_CalAvgValue(&MPU9250_Filter[i].Index, MPU9250_Filter[i].AvgBuffer, InBuffer[i], OutBuffer + i);
    } 

	magn[0] = *(OutBuffer + 0)-MPU9250_Magn_Offset.X_Off_Err;
	magn[1] = *(OutBuffer + 1)-MPU9250_Magn_Offset.Y_Off_Err;
	magn[2] = *(OutBuffer + 2)-MPU9250_Magn_Offset.Z_Off_Err;
}

bool iKVS_PeriMPU9250_Check(uint8_t dev){
    uint8_t recvByte = 0;
    iKVS_I2CreadByte(dev, WHO_AM_I, &recvByte);
   	if(WHO_AM_I_VAL == recvByte)  {
   		return true;
   	}else {
   		return false;
   	}	
}
