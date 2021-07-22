///////////////////////////////////////////////////////////////////////////
//PCF8563 正常工作时//LED_ToggleHighLED(1, LED_ON); 高位led1闪烁
///////////////////////////////////////////////////////////////////////////
#include "RealDatabase.h"
//#include "SysConfig.h"
#include "SysIICProc.h"
#include "PCF8563.h"
#include "SysGpioInit.h"
//#include "SysLEDDisp.h"
#include "systick.h"
#include "SysIICProc.h"
///////////////////////////////////////////////////////////////////////////
// //时间结构体
// typedef struct
// {
// 	vu8 nHour;
// 	vu8 nMinute;
// 	vu8 nSecond;
// 
// 	//公历日月年周
// 	vu8  nMonth;
// 	vu8  nDay;
// 	vu8  nWeek;
// 	vu16 nYear;
// 
// } RTC_TIME_DEF;
// 日历结构体
// extern RTC_TIME_DEF g_stRtcTime;
 
//时钟结构体 
//RTC_TIME_DEF g_stRtcTime;
//读1个字节, ack=1时, 发送nACK, ack=0, 发送ACK
u8 IIC_Read_pcf8563(u8 ack) 
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

		revDAT <<= 1;
		SCL_1;
		delay_us(5);

//		if (GPIO_ReadInputPin(DS3231_SDA_GPIO_PORT, DS3231_SDA_GPIO_PINS))
		if (READ_SDA)
		{
			revDAT = (revDAT | 0x01);
		}
		else
		{
			revDAT = (revDAT & 0xfe);
		}

		delay_us(5);
				SCL_0;
		delay_us(8);
		
		
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
/**
*****************************************************************************
* @Name   : 将BIN转换为BCD
*
* @Brief  : none
*
* @Input  : BINValue: 输入BIN
*
* @Output : none
*
* @Return : BCD格式数值
*****************************************************************************
**/
static unsigned char RTC_BinToBcd2(unsigned char BINValue)
{
	unsigned char bcdhigh = 0;

	while (BINValue >= 10)
	{
		bcdhigh++;
		BINValue -= 10;
	}

	return ((unsigned char)(bcdhigh << 4) | BINValue);
}

/**
*****************************************************************************
* @Name   : 将BCD转换为BIN
*
* @Brief  : none
*
* @Input  : BCDValue: 输入BCD
*
* @Output : none
*
* @Return : BIN格式数值
*****************************************************************************
**/
static unsigned char RTC_Bcd2ToBin(unsigned char BCDValue)
{
	unsigned char tmp = 0;

	tmp = ((unsigned char)(BCDValue & (unsigned char)0xF0) >> (unsigned char)0x04) * 10;
	return (tmp + (BCDValue & (unsigned char)0x0F));
}

/**
*****************************************************************************
* @Name   : PCF8563某寄存器写入一个字节数据
*
* @Brief  : none
*
* @Input  : REG_ADD：要操作寄存器地址
*           dat：    要写入的数据
*
* @Output : none
*
* @Return : none
*****************************************************************************
**/
static void PCF8563_Write_Byte(unsigned char REG_ADD, unsigned char dat)
{
	IIC_Start();
	if (IIC_Write(PCF8563_Write)==1)  //发送写命令并检查应答位
	{
		IIC_Write(REG_ADD);
		IIC_Write(dat);  //发送数据
	}
	IIC_Stop();
}

/**
*****************************************************************************
* @Name   : PCF8563某寄存器读取一个字节数据
*
* @Brief  : none
*
* @Input  : REG_ADD：要操作寄存器地址
*
* @Output : none
*
* @Return : 读取得到的寄存器的值
*****************************************************************************
**/
static unsigned char PCF8563_Read_Byte(unsigned char REG_ADD)
{
	unsigned char ReData;

	IIC_Start();
	if (IIC_Write(PCF8563_Write)==1)  //发送写命令并检查应答位
	{
		IIC_Write(REG_ADD);  //确定要操作的寄存器
		IIC_Start();  //重启总线
		IIC_Write(PCF8563_Read);  //发送读取命令
		ReData = IIC_Read_pcf8563(1);  //读取数据
		//I2C_SendACK(1);  //发送非应答信号结束数据传送
	}
	IIC_Stop();
	return ReData;
}

/**
*****************************************************************************
* @Name   : PCF8563写入多组数据
*
* @Brief  : none
*
* @Input  : REG_ADD：要操作寄存器起始地址
*           num：    写入数据数量
*           *WBuff： 写入数据缓存
*
* @Output : none
*
* @Return : none
*****************************************************************************
**/
static void PCF8563_Write_nByte(unsigned char REG_ADD, unsigned char num, unsigned char *pBuff)
{
	unsigned char i = 0;

	IIC_Start();
	if ((IIC_Write(PCF8563_Write)==1))  //发送写命令并检查应答位
	{
		IIC_Write(REG_ADD);  //定位起始寄存器地址
		for (i = 0;i < num;i++)
		{
			IIC_Write(*pBuff);  //写入数据
			pBuff++;
		}
	}
	IIC_Stop();
}

/**
*****************************************************************************
* @Name   : PCF8563读取多组数据
*
* @Brief  : none
*
* @Input  : REG_ADD：要操作寄存器起始地址
*           num：    读取数据数量
*
* @Output : *WBuff： 读取数据缓存
*
* @Return : none
*****************************************************************************
**/
static void PCF8563_Read_nByte(unsigned char REG_ADD, unsigned char num, unsigned char *pBuff)
{
	unsigned char i = 0;

	IIC_Start();
	if ((IIC_Write(PCF8563_Write)==1))  //发送写命令并检查应答位
	{
		//指示RTC正常工作
		//LED_ToggleHighLED(1, LED_ON);

		IIC_Write(REG_ADD);  //定位起始寄存器地址
		IIC_Start();  //重启总线
		IIC_Write(PCF8563_Read);  //发送读取命令
		//接收数据
		for (i = 0;i < num - 1;i++)
		{
			//发送应答
			*(pBuff + i) = IIC_Read_pcf8563(0);
		}

		*(pBuff + i) = IIC_Read_pcf8563(1);//最后一位 非应答结束传递
	}
	IIC_Stop();
}

/**
*****************************************************************************
* @Name   : PCF8563 时钟输出
*
* @Brief  : none
*
* @Input  : nClockEn 1使能   0关闭
*			nClockFreq 输出频率
*
* @Output : none
*
* @Return : none
*****************************************************************************
**/
static void PCF8563_ClockOut(u8 nClockEn,  u8 nClockFreq)
{
	if (nClockEn == 1)
	{
		//时钟输出-32.768
		PCF8563_Write_Byte(PCF8563_Address_CLKOUT, PCF_CLKOUT_Open | nClockFreq);
	}
	else
	{
		PCF8563_Write_Byte(PCF8563_Address_CLKOUT, PCF_CLKOUT_Close);
	}
}

/**
  *****************************************************************************
  * @Name   : PCF8563检测是否存在
  *
  * @Brief  : 向定时器倒计时寄存器写入一个数值再读取出来做对比，相同正确，不同则错误
  *
  * @Input  : none
  *
  * @Output : none
  *
  * @Return : 0: 正常
  *           1: PCF8563错误或者损坏
  *****************************************************************************
**/
unsigned char PCF8563_Check(void)
{
	unsigned char test_value = 0;
	unsigned char Time_Count = 0;  //定时器倒计时数据缓存

	if(PCF8563_Read_Byte(PCF8563_Address_Timer) & 0x80)  //如果打开了定时器，则先关闭
	{
		PCF8563_Write_Byte(PCF8563_Address_Timer, PCF_Timer_Close);  //先关闭定时器
		Time_Count = PCF8563_Read_Byte(PCF8563_Address_Timer_VAL);  //先保存计数值
	}

	PCF8563_Write_Byte(PCF8563_Address_Timer_VAL, PCF8563_Check_Data);  //写入检测值
	for(test_value = 0;test_value < 50;test_value++)  {}  //延时一定时间再读取
	test_value = PCF8563_Read_Byte(PCF8563_Address_Timer_VAL);  //再读取回来

	if(Time_Count != 0)  //启动了定时器功能，则恢复
	{
		PCF8563_Write_Byte(PCF8563_Address_Timer_VAL, Time_Count);  //恢复现场
		PCF8563_Write_Byte(PCF8563_Address_Timer, PCF_Timer_Open);  //启动定时器
	}

	if(test_value != PCF8563_Check_Data)  return 1;  //器件错误或者损坏
	
	return 0;  //正常
}

/**
*****************************************************************************
* @Name   : PCF8563初始化
*
* @Brief  : none
*
* @Input  : none
*
* @Output : none
*
* @Return : 0:初始化正常   1：初始化错误
*****************************************************************************
**/

u8 PCF8563_InitProc(void)
{
	RTC_TIME_DEF stTime;

	if(PCF8563_Check())
	{
		return 1 ;
	}
		
	//检测掉电
	if((PCF8563_Read_Byte(PCF8563_Address_Seconds)&0x80)==0x80)
	{
		//清除VL
		PCF8563_Write_Byte(PCF8563_Address_Seconds, 0x00);
	
		//清除控制寄存器
		PCF8563_Write_Byte(PCF8563_Address_Control_Status_1, 0x00);
		PCF8563_Write_Byte(PCF8563_Address_Control_Status_2, 0x00);

		stTime = GetSoftWareBuildTargetTime();

		PCF8563_SetTime(stTime.nYear, stTime.nMonth, stTime.nDay, stTime.nHour, stTime.nMinute, stTime.nSecond,1);

	}

	//清除控制寄存器
	PCF8563_Write_Byte(PCF8563_Address_Control_Status_1, 0x00);
	PCF8563_Write_Byte(PCF8563_Address_Control_Status_2, 0x00);

	//时钟输出
	PCF8563_ClockOut(0, PCF_CLKOUT_F32768);

	return 0;
	
}		 				    

/**
*****************************************************************************
* @Name   : PCF8563设置时间
*
* @Brief  : none
*
* @Input  : u16 Year 1900-2099  用于判断世纪标志
*			u8 Mon			输入10进制
*			u8 Day
*			u8 Hour
*			u8 Min
*			u8 Sec
*			u8 Week	星期
* @Output : none
*
* @Return : none
*****************************************************************************
**/
void PCF8563_SetTime(u16 Year,u8 Mon,u8 Day,u8 Hour,u8 Min,u8 Sec,u8 Week)
{
	//秒 分 时 日 星期 月/世纪 年
	u8 arrBuf[7];

	//数据校验
	if (Year > 2099)Year = 2000;  //恢复00年
	if (Mon > 12)Mon = 1;  //恢复1月
	if (Day > 31)Day = 1;  //恢复1日
	if (Week > 6)Week = 1;  //恢复星期一
	if (Hour > 23)Hour = 0;  //恢复0小时
	if (Min > 59)Min = 0;  //恢复0分钟

	//进制转化
	arrBuf[0] = RTC_BinToBcd2(Sec& PCF8563_Shield_Seconds);//秒
	arrBuf[1] = RTC_BinToBcd2(Min& PCF8563_Shield_Minutes);//分
	arrBuf[2] = RTC_BinToBcd2(Hour& PCF8563_Shield_Hours);//时
	arrBuf[3] = RTC_BinToBcd2(Day& PCF8563_Shield_Days);//日
	arrBuf[4] = RTC_BinToBcd2(Week& PCF8563_Shield_WeekDays);//星期
	if (Year >= 2000)
	{
		arrBuf[5] = RTC_BinToBcd2(Mon & PCF8563_Shield_Months_Century);//月/世纪
		arrBuf[6] = RTC_BinToBcd2((Year-2000)& PCF8563_Shield_Years);//年
	}
	else
	{
		arrBuf[5] = RTC_BinToBcd2(Mon  & PCF8563_Shield_Months_Century)|0x80;//月/世纪
		arrBuf[6] = RTC_BinToBcd2((Year-1900)& PCF8563_Shield_Years);//年
	}
	
	//写入
	PCF8563_Write_nByte(PCF8563_Address_Seconds, 7, arrBuf);
	
}

/**
*****************************************************************************
* @Name   : PCF8563读时间
*
* @Brief  : none
*
* @Input  : RTC_TIME_DEF *stTime 时间结构体
*
* @Output : *stTime 时间
*
* @Return : none
*****************************************************************************
**/
void PCF8563_ReadTime(RTC_TIME_DEF *stTime)
{
	//秒 分 时 日 星期 月/世纪 年
	u8 arrBuf[7];

	//读数据
	PCF8563_Read_nByte(PCF8563_Address_Seconds, 7, arrBuf);

	//转化
	stTime->nSecond = RTC_Bcd2ToBin(arrBuf[0] & PCF8563_Shield_Seconds);
	stTime->nMinute = RTC_Bcd2ToBin(arrBuf[1] & PCF8563_Shield_Minutes);
	stTime->nHour = RTC_Bcd2ToBin(arrBuf[2] & PCF8563_Shield_Hours);
	stTime->nDay = RTC_Bcd2ToBin(arrBuf[3] & PCF8563_Shield_Days);
	stTime->nWeek = RTC_Bcd2ToBin(arrBuf[4] & PCF8563_Shield_WeekDays);
	stTime->nMonth = RTC_Bcd2ToBin(arrBuf[5] & PCF8563_Shield_Months_Century);
	if ((arrBuf[5] & 0x80) == 0x80)
	{
		stTime->nYear = RTC_Bcd2ToBin(arrBuf[6] & PCF8563_Shield_Years)+1900;
	}
	else
	{
		stTime->nYear = RTC_Bcd2ToBin(arrBuf[6] & PCF8563_Shield_Years) + 2000;
	}
	
}	

