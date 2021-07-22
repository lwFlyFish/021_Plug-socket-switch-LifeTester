///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

#include "SysConfig.h"
#include "RealDatabase.h"
#include "SysGpioInit.h"
#include "SysIICProc.h"
#include "SysTick.h"

///////////////////////////////////////////////////////////////////////////
//Description : IIC��ʼ��
///////////////////////////////////////////////////////////////////////////
//����IIC��ʼ�ź� 
///////////////////////////////////////////////////////////////////////////
void IIC_Start(void)
{ 
	//sda�����
	SDA_OUT();
	SDA_1;
	SCL_1;
	delay_us(5);

	//START:when CLK is high,DATA change form high to low
	SDA_0;
	delay_us(5);

	//ǯסI2C����, ׼�����ͻ��������
	SCL_0;
}

//����IICֹͣ�ź�
void IIC_Stop(void)
{
	//sda�����
	SDA_OUT();
	SCL_0;

	//STOP:when CLK is high DATA change form low to high
	SDA_0;

	delay_us(5);
	SCL_1;

	//����I2C���߽����ź� 
	SDA_1; 
	delay_us(5);
}

//�ȴ�Ӧ���źŵ��� 
//����ֵ: 0, ����Ӧ��ʧ�� 
//        1, ����Ӧ��ɹ� 
u8 IIC_Wait_Ack(void) 
{ 
	u8 ucErrTime = 0;
	SDA_1;

	//SDA����Ϊ����   
	SDA_IN();      
	delay_us(5);
	SCL_1;
	delay_us(5);

	//while(GPIO_ReadInputPin(GPIOB,GPIO_PIN_4))
	while (READ_SDA)
	{
		ucErrTime++;
		if (ucErrTime > 250)
		{
			IIC_Stop();
			return 0;
		}
	}

	//ʱ�����0
	SCL_0;

	return 1;
}

//����ACKӦ�� 
void IIC_Ack(void) 
{ 
	SCL_0;
	SDA_OUT();
	SDA_0;
	
	delay_us(5);
	SCL_1;

	delay_us(5);
	SCL_0;
}

//������ACKӦ��	      
void IIC_NAck(void) 
{ 
	SCL_0;
	SDA_OUT();
	SDA_1;

	delay_us(5);
	SCL_1;

	delay_us(5);
	SCL_0;
}	  

void I2C_SendACK(u8 ack)
{    
	if (ack == 0)
	{
		SDA_0;
	}
	else
	{
		SDA_1;
	}

	//
	SCL_1;
	delay_us(8);

	SCL_0;
	delay_us(8);
}

//IIC����һ���ֽ� 
//���شӻ�����Ӧ�� 1, ��Ӧ�� 0, ��Ӧ��
u8 IIC_Write(u8 data)
{
	u8 i = 8;
	SDA_OUT();

	while (i--)
	{
		SCL_0;
		delay_us(5);

		if (data & 0x80)
			SDA_1;
		else
			SDA_0;

		data <<= 1;
		delay_us(5);
		SCL_1;
		delay_us(5);
	}

	SCL_0;

	if (IIC_Wait_Ack() == 1)
	{
		return 1;
	}
	else
	{
		IIC_Stop();
		return 0;
	}
} 	

//��1���ֽ�, ack=1ʱ, ����nACK, ack=0, ����ACK
u8 IIC_Read(u8 ack) 
{ 
	u8 i;
	u8 revDAT = 0;

	//uchar re_tmp = 0;
	//�ͷ�SDA
	SDA_1;

	// set SDA as input
	SDA_IN();

	for (i=0; i<8; i++)
	{
		//lyy 190910
		SCL_0;
		delay_us(8);
		revDAT <<= 1;
		SCL_1;
		delay_us(5);

		//if (GPIO_ReadInputPin(DS3231_SDA_GPIO_PORT, DS3231_SDA_GPIO_PINS))
		if (READ_SDA)
		{
			revDAT = (revDAT | 0x01);
		}
		else
		{
			revDAT = (revDAT & 0xfe);
		}

		delay_us(5);

	}

	// set SDA as output 
	SDA_OUT();

	//send ACK					 
	if (ack == 0)
	{
		SDA_0;
	}
	else
	{
		SDA_1;
	}

	SCL_1;
	delay_us(8);

	SCL_0;
	delay_us(8);

	return revDAT;
}

///////////////////////////////////////////////////////////////////////////
//����:IIC_ReadBytes()
//����:����ͨ��I2C���ߴӴӻ�����һ���ֽڵ�����
//����:
//	SlaveAddr:�ӻ���ַ(��7λ�Ǵӻ���ַ, ���λ�Ƕ���־1)
//	SubAddr:�ӻ����ӵ�ַ
//	Size:���ݵ��ֽ���
//	*dat:������յ�������
//����:
//	1:���ճɹ�
//	0:�ڽ��չ����г����쳣
///////////////////////////////////////////////////////////////////////////
u8 IIC_ReadBytes(unsigned char SlaveAddr, unsigned char SubAddr, u8 Size, u8* dat)
{
	u8 i = 0;

	//if ( Size == 0 ) return 0;
	//ȷ���ӻ���ַ���λ��0
	SlaveAddr &= 0xFE;	//ȷ�����λ��0
						//����I2C����
	IIC_Start();

	//���ʹӻ���ַ
	if (IIC_Write(SlaveAddr) == 0)
	{
		return 0;
	}

	//�����ӵ�ַ
	if (IIC_Write(SubAddr) == 0)
	{
		return 0;
	}

	//�����ظ���ʼ����
	IIC_Start();

	//���ʹӻ���ַ
	SlaveAddr |= 0x01;
	if (IIC_Write(SlaveAddr) == 0)
	{
		return 0;
	}

	//��������
	for (i = 0;i < Size - 1;i++)
	{
		*(dat + i) = IIC_Read(0);
	}

	*(dat + i) = IIC_Read(1);

	//������ϣ�ֹͣI2C���ߣ������ؽ��
	IIC_Stop();

	return 1;
}

///////////////////////////////////////////////////////////////////////////
//����:IIC_WriteOneByte()
//����:����ͨ��I2C������ӻ�д��1���ֽڵ�����
//����:
//	SlaveAddr:�ӻ���ַ(��7λ�Ǵӻ���ַ�����λ�Ƕ���־1)
//	SubAddr:�ӻ����ӵ�ַ
//	dat:д�������
//����:
//	1:���ͳɹ�
//	0:�ڷ��͹����г����쳣
///////////////////////////////////////////////////////////////////////////
u8 IIC_WriteOneByte(unsigned char SlaveAddr, unsigned char SubAddr, unsigned char dat)
{
	//ȷ���ӻ���ַ���λ��0
	SlaveAddr &= 0xFE;	//ȷ�����λ��0
						//����I2C����
	IIC_Start();

	//���ʹӻ���ַ
	if (IIC_Write(SlaveAddr) == 0)
	{
		return 0;
	}

	//�����ӵ�ַ
	if (IIC_Write(SubAddr) == 0)
	{
		return 0;
	}

	//д����
	if (IIC_Write(dat) == 0)
	{
		return 0;
	}

	//������ϣ�ֹͣI2C���ߣ������ؽ��
	IIC_Stop();

	return 1;
}

///////////////////////////////////////////////////////////////////////////
//IIC END
///////////////////////////////////////////////////////////////////////////





