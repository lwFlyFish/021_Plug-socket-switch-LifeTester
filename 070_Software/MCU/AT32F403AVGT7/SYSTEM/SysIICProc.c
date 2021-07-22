///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

#include "SysConfig.h"
#include "RealDatabase.h"
#include "SysGpioInit.h"
#include "SysIICProc.h"
#include "SysTick.h"

///////////////////////////////////////////////////////////////////////////
//Description : IIC初始化
///////////////////////////////////////////////////////////////////////////
//产生IIC起始信号 
///////////////////////////////////////////////////////////////////////////
void IIC_Start(void)
{ 
	//sda线输出
	SDA_OUT();
	SDA_1;
	SCL_1;
	delay_us(5);

	//START:when CLK is high,DATA change form high to low
	SDA_0;
	delay_us(5);

	//钳住I2C总线, 准备发送或接收数据
	SCL_0;
}

//产生IIC停止信号
void IIC_Stop(void)
{
	//sda线输出
	SDA_OUT();
	SCL_0;

	//STOP:when CLK is high DATA change form low to high
	SDA_0;

	delay_us(5);
	SCL_1;

	//发送I2C总线结束信号 
	SDA_1; 
	delay_us(5);
}

//等待应答信号到来 
//返回值: 0, 接收应答失败 
//        1, 接收应答成功 
u8 IIC_Wait_Ack(void) 
{ 
	u8 ucErrTime = 0;
	SDA_1;

	//SDA设置为输入   
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

	//时钟输出0
	SCL_0;

	return 1;
}

//产生ACK应答 
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

//不产生ACK应答	      
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

//IIC发送一个字节 
//返回从机有无应答 1, 有应答 0, 无应答
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

//读1个字节, ack=1时, 发送nACK, ack=0, 发送ACK
u8 IIC_Read(u8 ack) 
{ 
	u8 i;
	u8 revDAT = 0;

	//uchar re_tmp = 0;
	//释放SDA
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
//函数:IIC_ReadBytes()
//功能:主机通过I2C总线从从机接收一个字节的数据
//参数:
//	SlaveAddr:从机地址(高7位是从机地址, 最低位是读标志1)
//	SubAddr:从机的子地址
//	Size:数据的字节数
//	*dat:保存接收到的数据
//返回:
//	1:接收成功
//	0:在接收过程中出现异常
///////////////////////////////////////////////////////////////////////////
u8 IIC_ReadBytes(unsigned char SlaveAddr, unsigned char SubAddr, u8 Size, u8* dat)
{
	u8 i = 0;

	//if ( Size == 0 ) return 0;
	//确保从机地址最低位是0
	SlaveAddr &= 0xFE;	//确保最低位是0
						//启动I2C总线
	IIC_Start();

	//发送从机地址
	if (IIC_Write(SlaveAddr) == 0)
	{
		return 0;
	}

	//发送子地址
	if (IIC_Write(SubAddr) == 0)
	{
		return 0;
	}

	//发送重复起始条件
	IIC_Start();

	//发送从机地址
	SlaveAddr |= 0x01;
	if (IIC_Write(SlaveAddr) == 0)
	{
		return 0;
	}

	//接收数据
	for (i = 0;i < Size - 1;i++)
	{
		*(dat + i) = IIC_Read(0);
	}

	*(dat + i) = IIC_Read(1);

	//接收完毕，停止I2C总线，并返回结果
	IIC_Stop();

	return 1;
}

///////////////////////////////////////////////////////////////////////////
//函数:IIC_WriteOneByte()
//功能:主机通过I2C总线向从机写入1个字节的数据
//参数:
//	SlaveAddr:从机地址(高7位是从机地址，最低位是读标志1)
//	SubAddr:从机的子地址
//	dat:写入的数据
//返回:
//	1:发送成功
//	0:在发送过程中出现异常
///////////////////////////////////////////////////////////////////////////
u8 IIC_WriteOneByte(unsigned char SlaveAddr, unsigned char SubAddr, unsigned char dat)
{
	//确保从机地址最低位是0
	SlaveAddr &= 0xFE;	//确保最低位是0
						//启动I2C总线
	IIC_Start();

	//发送从机地址
	if (IIC_Write(SlaveAddr) == 0)
	{
		return 0;
	}

	//发送子地址
	if (IIC_Write(SubAddr) == 0)
	{
		return 0;
	}

	//写数据
	if (IIC_Write(dat) == 0)
	{
		return 0;
	}

	//接收完毕，停止I2C总线，并返回结果
	IIC_Stop();

	return 1;
}

///////////////////////////////////////////////////////////////////////////
//IIC END
///////////////////////////////////////////////////////////////////////////





