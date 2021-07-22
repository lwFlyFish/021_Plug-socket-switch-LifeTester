///////////////////////////////////////////////////////////////////////////
//eeprom ����������ʱ//LED_ToggleHighLED(2, LED_BLINK); ��λled2��˸
///////////////////////////////////////////////////////////////////////////

#include "SysConfig.h"
#include "SysGpioInit.h"
#include "SysIICProc.h"
#include "24cxx.h" 
#include "SysTick.h"
#include "RealDatabase.h"
///////////////////////////////////////////////////////////////////////////

//��ʼ��IIC�ӿ�
//����0 ��ʼ������ 1ʧ��
u8 AT24CXX_Init(void)
{
	//IIC_Init();
	if(AT24CXX_Check() == 1)
	{
		return 1;
	}
	return 0;
}

//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{
	u8 temp = 0;
	u8 nErr = 1;
	IIC_Start();

	//
	if (EE_TYPE > AT24C16)
	{
		//����д����
		nErr &= IIC_Write(0XA0);

		//IIC_Wait_Ack();
		nErr &= IIC_Write(ReadAddr >> 8);//���͸ߵ�ַ

		//IIC_Wait_Ack();
	}
	else
	{
		//����������ַ0XA0,д����
		nErr &= IIC_Write(0XA0 + ((ReadAddr / 256) << 1));
	}

	//IIC_Wait_Ack();
	//���͵͵�ַ
	nErr &= IIC_Write(ReadAddr % 256);

	//IIC_Wait_Ack();
	IIC_Start();

	//�������ģʽ
	nErr &= IIC_Write(0xA1);

	//IIC_Wait_Ack();
	temp = IIC_Read(0);

	//����һ��ֹͣ����
	IIC_Stop();

	if (nErr != 1)
	{
		//����Щ����
		//LED_ToggleHighLED(2, LED_BLINK);
	}
	else
	{
		////LED_ToggleHighLED(2, LED_OFF);
	}

	return temp;
}

//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void AT24CXX_WriteOneByte(u16 WriteAddr, u8 DataToWrite)
{
	u8 nErr = 1;
	WC_DISABLE;
	delay_ms(1);

	IIC_Start();

	if (EE_TYPE > AT24C16)
	{
		//����д����
		nErr&=IIC_Write(0xA0);
		//IIC_Wait_Ack();

		//���͸ߵ�ַ
		nErr &= IIC_Write(WriteAddr >> 8);
	}
	else
	{
		//����������ַ0xA0,д���� 
		nErr &= IIC_Write(0XA0 + ((WriteAddr / 256) << 1));
	}

	//IIC_Wait_Ack();
	//���͵͵�ַ
	nErr &= IIC_Write(WriteAddr % 256);

	//IIC_Wait_Ack();
	//�����ֽ�
	nErr &= IIC_Write(DataToWrite);

	//IIC_Wait_Ack();
	//����һ��ֹͣ���� 
	IIC_Stop();

	delay_ms(5);

	if (nErr != 1)
	{
		//����Щ����
		//LED_ToggleHighLED(2, LED_BLINK);
	}
	else
	{
		////LED_ToggleHighLED(2, LED_OFF);
	}

	WC_ENABLE;
}

//��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ  
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void AT24CXX_WriteLenByte(u16 WriteAddr, u32 DataToWrite, u8 Len)
{
	u8 t;

	for (t=0; t<Len; t++)
	{
		AT24CXX_WriteOneByte(WriteAddr + t, (DataToWrite >> (8 * t)) & 0xFF);
	}
}

///////////////////////////////////////////////////////////////////////////
//��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ 
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
u32 AT24CXX_ReadLenByte(u16 ReadAddr, u8 Len)
{
	u8 t;

	u32 temp = 0;

	//
	for (t=0; t<Len; t++)
	{
		temp <<= 8;
		temp += AT24CXX_ReadOneByte(ReadAddr+Len-t-1);
	}

	return temp;
}

//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp = AT24CXX_ReadOneByte(EE_TYPE-1);//����ÿ�ο�����дAT24CXX			   

    if (temp == EE_VALUE_INIT)
	{
		return 0;
	}
	//�ų���һ�γ�ʼ�������
	else
	{
        AT24CXX_WriteOneByte(EE_TYPE-1, EE_VALUE_INIT);

		temp = AT24CXX_ReadOneByte(EE_TYPE-1);

        if (temp == EE_VALUE_INIT)
			return 0;
	}

	return 1;
}

//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Read(u16 ReadAddr, u8 *pBuffer, u16 NumToRead)
{
	while (NumToRead)
	{
		*pBuffer = AT24CXX_ReadOneByte(ReadAddr++);
		pBuffer++;
		NumToRead--;
	}
}

//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void AT24CXX_Write(u16 WriteAddr, u8* pBuffer, u16 NumToWrite)
{
	while (NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr, *pBuffer);
		WriteAddr++;

		pBuffer++;
	}
}







