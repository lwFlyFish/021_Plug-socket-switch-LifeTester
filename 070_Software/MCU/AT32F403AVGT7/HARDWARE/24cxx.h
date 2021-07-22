#ifndef __24CXX_H
#define __24CXX_H        1
//
///////////////////////////////////////////////////////////////////////////
#include "SysConfig.h"


///////////////////////////////////////////////////////////////////////////
//#define
///////////////////////////////////////////////////////////////////////////

//ʹ��д����
#define WC_ENABLE		EEPROM_CS_PORT->BSRE = EEPROM_CS_PINS
//�ر�д����
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

//������ʹ�õ���24c256, ���Զ���EE_TYPEΪAT24C256
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
	//ָ����ַ��ȡһ���ֽ�
	extern u8 AT24CXX_ReadOneByte(u16 ReadAddr);							
	//ָ����ַд��һ���ֽ�
	extern void AT24CXX_WriteOneByte(u16 WriteAddr, u8 DataToWrite);

	//ָ����ַ��ʼд��ָ�����ȵ�����
	extern void AT24CXX_WriteLenByte(u16 WriteAddr, u32 DataToWrite, u8 Len);
	//ָ����ַ��ʼ��ȡָ����������
	extern u32 AT24CXX_ReadLenByte(u16 ReadAddr, u8 Len);

	//��ָ����ַ��ʼд��ָ�����ȵ�����
	extern void AT24CXX_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite);	

	//��ָ����ַ��ʼ����ָ�����ȵ�����
	extern void AT24CXX_Read(u16 ReadAddr, u8 *pBuffer, u16 NumToRead);   	

	//�������
	extern u8 AT24CXX_Check(void);
	//��ʼ��IIC
	extern u8 AT24CXX_Init(void);

//private:





#endif










