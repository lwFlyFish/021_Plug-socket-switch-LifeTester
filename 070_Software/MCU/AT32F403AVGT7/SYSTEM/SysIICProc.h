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

//IO��������
#define SDA_IN()  SDA_PORT->CTRLL &=0XFF0FFFFF,SDA_PORT->CTRLL |= 0X00800000; //��������
#define SDA_OUT() SDA_PORT->CTRLL &=0XFF0FFFFF,SDA_PORT->CTRLL |= 0X00300000; //���

//��IO��������	 
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
	//����ACKӦ�� 
	extern void IIC_Ack(void);
	extern void IIC_NAck(void);
	extern void I2C_SendACK(u8 ack);
	//IIC����һ���ֽ� 
	//���شӻ�����Ӧ��   1����Ӧ�� 0����Ӧ��	    
	extern u8 IIC_Write(u8 data);
	//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK    
	extern u8 IIC_Read(u8 ack);

//private:




#endif




