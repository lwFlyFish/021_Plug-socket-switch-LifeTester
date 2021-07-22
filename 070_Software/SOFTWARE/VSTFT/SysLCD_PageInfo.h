#ifndef __SYSLCD_PAGEINFO_H
#define __SYSLCD_PAGEINFO_H

#include "RealDatabase.h"

//��ؽ����ı���ID
typedef enum
{
	TFT_Info_Title = 0,
	TFT_Info_One_SetCount,
	TFT_Info_One_Countting,
	TFT_Info_One_ElecCount,
	TFT_Info_One_FaultCount,
	TFT_Info_Two_SetCount,
	TFT_Info_Two_Countting,
	TFT_Info_Two_ElecCount,
	TFT_Info_Two_FaultCount,
	TFT_Info_Thr_SetCount,
	TFT_Info_Thr_Countting,
	TFT_Info_Thr_ElecCount,
	TFT_Info_Thr_FaultCount,
	TFT_Info_One_RunState,
	TFT_Info_One_ElecDetection,
	TFT_Info_Two_RunState,
	TFT_Info_Two_ElecDetection,
	TFT_Info_Thr_RunState,
	TFT_Info_Thr_ElecDetection,
	TFT_Info_One_FaultCle,
	TFT_Info_Two_FaultCle,
	TFT_Info_Thr_FaultCle,
	TFT_Info_Text_Number
} TFT_Info_Show_Text_E;

typedef enum
{
	TFT_Info_Return_Button = 22,
} TFT_Info_Button_E;

typedef struct
{
    uint8_t nDevStatus;			   ///< �豸����״̬
                                   ///< ������⣬���ò���ҳ����ֵ
                                   ///< �趨���������ò���ҳ����ֵ
    uint32_t nRealCount;		   ///< ��ǰ����
    uint32_t nRealCurrentCount;	   ///< ��������
    uint32_t nRealErrorCount;	   ///< ���ϴ���
    uint8_t	 nFlagErrorClear;	   ///< ����������֮����1�����鿪ʼǰ��0
} TFT_INFO_VALUE_T;				   ///< ���ҳ��ı���

typedef struct
{
    uint8_t nDevStatus;			  ///< �豸����״̬
    uint8_t nCurrentCheck;		  ///< �������
    uint8_t nSetCount;			  ///< �趨���������ò���ҳ����ֵ
    uint8_t nRealCount;			  ///< ��ǰ����
    uint8_t nRealCurrentCount;	  ///< ��������
    uint8_t nRealErrorCount;	  ///< ���ϴ���
    uint8_t nFlagErrorClear;	  ///<�������
} TFT_INFO_SHOW_FLAG_T;			  ///< ���ҳ���ˢ�±�־


extern TFT_INFO_VALUE_T		g_stPageInfoValue[TEST_SELECT_NUMBER];
extern TFT_INFO_SHOW_FLAG_T g_stPageInfoShowFlag[TEST_SELECT_NUMBER];

void SysLCDTFT_PageInfoShowInfoProc(void);
// BYTE SysLCDTFT_PageInfoTextProc(BYTE nCount);
BYTE SysLCDTFT_PageInfoButtonProc(uint16_t _id);
void SysTFTLcd_PageInfoGotoProc(void);
BYTE SysLCDTFT_PageInfoIconProc(BYTE nCount, uint16_t _id);

#endif
