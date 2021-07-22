#ifndef __PCF8563_H_
#define __PCF8563_H_	  1

#include "RealDatabase.h"
//
///////////////////////////////////////////////////////////////////////////

#define PCF8563_Check_Data                       (unsigned char)0x55  //检测

#define PCF8563_Write                            (unsigned char)0xa2  //写命令
#define PCF8563_Read                             (unsigned char)0xa3  //读命令，或者用（PCF8563_Write + 1）

/******************************************************************************
参数寄存器地址宏定义
******************************************************************************/

#define PCF8563_Address_Control_Status_1         (unsigned char)0x00  //控制/状态寄存器1
#define PCF8563_Address_Control_Status_2         (unsigned char)0x01  //控制/状态寄存器2

#define PCF8563_Address_CLKOUT                   (unsigned char)0x0d  //CLKOUT频率寄存器
#define PCF8563_Address_Timer                    (unsigned char)0x0e  //定时器控制寄存器
#define PCF8563_Address_Timer_VAL                (unsigned char)0x0f  //定时器倒计数寄存器

#define PCF8563_Address_Years                    (unsigned char)0x08  //年
#define PCF8563_Address_Months                   (unsigned char)0x07  //月
#define PCF8563_Address_Days                     (unsigned char)0x05  //日
#define PCF8563_Address_WeekDays                 (unsigned char)0x06  //星期
#define PCF8563_Address_Hours                    (unsigned char)0x04  //小时
#define PCF8563_Address_Minutes                  (unsigned char)0x03  //分钟
#define PCF8563_Address_Seconds                  (unsigned char)0x02  //秒

#define PCF8563_Alarm_Minutes                    (unsigned char)0x09  //分钟报警
#define PCF8563_Alarm_Hours                      (unsigned char)0x0a  //小时报警
#define PCF8563_Alarm_Days                       (unsigned char)0x0b  //日报警
#define PCF8563_Alarm_WeekDays                   (unsigned char)0x0c  //星期报警

//
//频率输出寄存器 --> 0x0d
//
#define PCF_CLKOUT_Open                          (unsigned char)(1<<7)     //CLKOUT输出有效
#define PCF_CLKOUT_Close                         (unsigned char)(~(1<<7))  //CLKOUT输出被禁止并设成高阻抗

#define PCF_CLKOUT_F32768                        (unsigned char)0x00  //输出32.768KHz
#define PCF_CLKOUT_F1024                         (unsigned char)0x01  //输出1024Hz
#define PCF_CLKOUT_F32                           (unsigned char)0x02  //输出32Hz
#define PCF_CLKOUT_F1                            (unsigned char)0x03  //输出1Hz

/******************************************************************************
参数屏蔽宏定义
******************************************************************************/

#define PCF8563_Shield_Control_Status_1          (unsigned char)0xa8
#define PCF8563_Shield_Control_Status_2          (unsigned char)0x1f

#define PCF8563_Shield_Seconds                   (unsigned char)0x7f
#define PCF8563_Shield_Minutes                   (unsigned char)0x7f
#define PCF8563_Shield_Hours                     (unsigned char)0x3f

#define PCF8563_Shield_Days                      (unsigned char)0x3f
#define PCF8563_Shield_WeekDays                  (unsigned char)0x07
#define PCF8563_Shield_Months_Century            (unsigned char)0x1f
#define PCF8563_Shield_Years                     (unsigned char)0xff

#define PCF8563_Shield_Minute_Alarm              (unsigned char)0x7f
#define PCF8563_Shield_Hour_Alarm                (unsigned char)0x3f
#define PCF8563_Shield_Day_Alarm                 (unsigned char)0x3f
#define PCF8563_Shield_WeekDays_Alarm            (unsigned char)0x07

#define PCF8563_Shield_CLKOUT_Frequency          (unsigned char)0x03
#define PCF8563_Shield_Timer_Control             (unsigned char)0x03
#define PCF8563_Shield_Timer_Countdown_Value     (unsigned char)0xff

//
//定时器控制寄存器 --> 0x0e
//
#define PCF_Timer_Open                           (unsigned char)(1<<7)     //定时器有效
#define PCF_Timer_Close                          (unsigned char)(~(1<<7))  //定时器无效

#define PCF_Timer_F4096                          (unsigned char)0x00  //时钟频率4096Hz
#define PCF_Timer_F64                            (unsigned char)0x01  //时钟频率64Hz
#define PCF_Timer_F1                             (unsigned char)0x02  //时钟频率1Hz
#define PCF_Timer_F160                           (unsigned char)0x03  //时钟频率1/60Hz
///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
// Attribute
///////////////////////////////////////////////////////////////////////////
//public:
	//日历结构体
	//extern RTC_TIME_DEF g_stRtcTime;

	//月份日期数据表
	extern u8 const mon_table[12];

//private:





///////////////////////////////////////////////////////////////////////////
// Operation
///////////////////////////////////////////////////////////////////////////
//public:
	
	//初始化RTC
	u8 PCF8563_InitProc(void);
	void PCF8563_SetTime(u16 Year, u8 Mon, u8 Day, u8 Hour, u8 Min, u8 Sec, u8 Week);
	void PCF8563_ReadTime(RTC_TIME_DEF *stTime);

//private:

	static unsigned char RTC_BinToBcd2(unsigned char BINValue);
	static unsigned char RTC_Bcd2ToBin(unsigned char BCDValue);
	static void PCF8563_Write_Byte(unsigned char REG_ADD, unsigned char dat);
	static unsigned char PCF8563_Read_Byte(unsigned char REG_ADD);
	static void PCF8563_Write_nByte(unsigned char REG_ADD, unsigned char num, unsigned char *pBuff);
	static void PCF8563_Read_nByte(unsigned char REG_ADD, unsigned char num, unsigned char *pBuff);
	static void PCF8563_ClockOut(u8 nClockEn, u8 nClockFreq);

#endif

///////////////////////////////////////////////////////////////////////////



