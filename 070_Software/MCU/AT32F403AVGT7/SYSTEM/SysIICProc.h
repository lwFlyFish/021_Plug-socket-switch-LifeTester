#ifndef __iic_H
#define __iic_H 1
//
///////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////
//define
///////////////////////////////////////////////////////////////////////////

//   SCL  PE1
//   SDA  PE0
#define SDA_1	SDA_PORT->BSRE = SDA_PINS
#define SDA_0	SDA_PORT->BRE  = SDA_PINS
#define SCL_1	SCL_PORT->BSRE = SCL_PINS
#define SCL_0	SCL_PORT->BRE  = SCL_PINS

//IO方向设置
#define SDA_IN()  SDA_PORT->CTRLL &=0XFF0FFFFF,SDA_PORT->CTRLL |= 0X00800000; //上拉输入
#define SDA_OUT() SDA_PORT->CTRLL &=0XFF0FFFFF,SDA_PORT->CTRLL |= 0X00300000; //输出

//读IO操作函数	 
//GPIO_ReadInputDataBit(SDA_PORT,SDA_PINS)
#define READ_SDA ((SDA_PORT->IPTDT & SDA_PINS) == SDA_PINS ? 1:0)

//
///////////////////////////////////////////////////////////////////////////
// Attribute
///////////////////////////////////////////////////////////////////////////
//public:



//private:



///////////////////////////////////////////////////////////////////////////
// Operation
///////////////////////////////////////////////////////////////////////////
//public:
	extern u8 IIC_ReadBytes(unsigned char SlaveAddr, unsigned char SubAddr, u8 Size, u8 *dat);
	extern u8 IIC_WriteOneByte(unsigned char SlaveAddr, unsigned char SubAddr, unsigned char dat);

	extern void IIC_Start(void);
	extern void IIC_Stop(void);
	extern u8 IIC_Wait_Ack(void);
	//产生ACK应答 
	extern void IIC_Ack(void);
	extern void IIC_NAck(void);
	extern void I2C_SendACK(u8 ack);
	//IIC发送一个字节 
	//返回从机有无应答   1，有应答 0，无应答	    
	extern u8 IIC_Write(u8 data);
	//读1个字节，ack=1时，发送ACK，ack=0，发送nACK    
	extern u8 IIC_Read(u8 ack);

//private:




#endif




