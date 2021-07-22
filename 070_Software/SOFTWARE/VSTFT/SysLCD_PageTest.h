#ifndef __SYSLCD_PAGETEST_H
#define __SYSLCD_PAGETEST_H

#include "RealDatabase.h"
#include "SysConfig.h"

//�������ؼ�ID
typedef enum
{
	TFT_Test_Text_Title = 0,
    TFT_Test_Text_WorkMethod,			  //������ʽ
    TFT_Test_Text_WorkState,			  //����״̬
    TFT_Test_Icon_Start,				  //����
    TFT_Test_Icon_Automatic_One,		  //��λһ�Զ�
    TFT_Test_Icon_Automatic_Two,		  //��λ���Զ�
    TFT_Test_Icon_Automatic_Thr,		  //��λ���Զ�
    TFT_Test_Icon_Manual_One,			  //��λһ�ֶ�
    TFT_Test_Icon_Manual_Two,			  //��λ���ֶ�
    TFT_Test_Icon_Manual_Thr,			  //��λ���ֶ�
    TFT_Test_Control_Show_Number = 10,	  //ˢ�¿ؼ�������

    TFT_Test_Button_Param = 10,	   //�������ý���
    TFT_Test_Button_First,		   //��ӭ����
    TFT_Test_Button_Info,		   //��ؽ���
    TFT_Test_Button_WorkMethod,	   //������ʽ

} TFT_Test_Control_E;

typedef struct
{
	uint8_t nWorkMode;
	uint8_t nWorkState;
	uint8_t nStart;
    uint8_t arrAutomatic[TEST_SELECT_NUMBER];
    uint8_t arrManual[TEST_SELECT_NUMBER];
} TFT_Test_Show_Flag_T;

typedef struct
{
    uint8_t nWorkMode;	   ///< ����ģʽ��ͬʱ������
    uint8_t nWorkState;	   ///< ����״̬
    uint8_t nStep;
	uint8_t arrAutomatic[TEST_SELECT_NUMBER];
	uint8_t arrManual[TEST_SELECT_NUMBER];
} TFT_Test_Value_T;


extern uint8_t g_nFlagMeanWhile;	//ͬʱ������ʼ�����־λ
extern uint8_t g_nFlagAgain;		//����������ʼ�����־λ

extern TFT_Test_Value_T		g_stTftTestValue;
extern TFT_Test_Show_Flag_T g_stTestShowFlag;

void SysLCDTFT_PageTestShowInfoProc(void);
BYTE SysLCDTFT_PageTestTextProc(BYTE nCount, uint16_t _id);
BYTE SysLCDTFT_PageTestButtonProc(uint16_t _id);
BYTE SysLCDTFT_PageTestIconProc(BYTE nCount, uint16_t _id);
void SysTFTLcd_PageTestGotoProc(void);


#endif
