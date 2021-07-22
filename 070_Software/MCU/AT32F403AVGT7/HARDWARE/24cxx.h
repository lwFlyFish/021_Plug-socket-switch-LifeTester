#ifndef __24CXX_H
#define __24CXX_H        1
//
///////////////////////////////////////////////////////////////////////////
#include "SysConfig.h"


///////////////////////////////////////////////////////////////////////////
//#define
///////////////////////////////////////////////////////////////////////////

//使能写保护
#define WC_ENABLE		EEPROM_CS_PORT->BSRE = EEPROM_CS_PINS
//关闭写保护
#define WC_DISABLE		EEPROM_CS_PORT->BRE	 = EEPROM_CS_PINS

#define EEPROM_WRITE_ADDR   0xA0
#define EEPROM_READ_ADDR    0xA1

#define AT24C01				127
#define AT24C02				255
#define AT24C04				511
#define AT24C08				1023
#define AT24C16				2047
#define AT24C32				4095
#define AT24C64				8191
#define AT24C128			16383
#define AT24C256			32767

//开发板使用的是24c256, 所以定义EE_TYPE为AT24C256
#define EE_TYPE				AT24C256

///////////////////////////////////////////////////////////////////////////
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
	//指定地址读取一个字节
	extern u8 AT24CXX_ReadOneByte(u16 ReadAddr);							
	//指定地址写入一个字节
	extern void AT24CXX_WriteOneByte(u16 WriteAddr, u8 DataToWrite);

	//指定地址开始写入指定长度的数据
	extern void AT24CXX_WriteLenByte(u16 WriteAddr, u32 DataToWrite, u8 Len);
	//指定地址开始读取指定长度数据
	extern u32 AT24CXX_ReadLenByte(u16 ReadAddr, u8 Len);

	//从指定地址开始写入指定长度的数据
	extern void AT24CXX_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite);	

	//从指定地址开始读出指定长度的数据
	extern void AT24CXX_Read(u16 ReadAddr, u8 *pBuffer, u16 NumToRead);   	

	//检查器件
	extern u8 AT24CXX_Check(void);
	//初始化IIC
	extern u8 AT24CXX_Init(void);

//private:





#endif










