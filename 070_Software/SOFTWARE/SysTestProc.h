#ifndef __SYS_TEST_PROC_H
#define __SYS_TEST_PROC_H

#include "RealDatabase.h"


typedef enum	///< ʵ����̵Ĳ���
{
    TEST_STEP_IDLE = 0,			   ///< ����״̬
    TEST_STEP_STOP,				   ///< ֹͣ״̬
    TEST_STEP_START,			   ///< ����������״̬��ת�����Ȼ���ʼ��ʱ���������
    TEST_STEP_START_MEAN_WHILE,	   ///< ͬʱ����ģʽ
    TEST_STEP_START_SEQUENCE,	   ///< ���ι���ģʽ
    TEST_STEP_START_WARN,		   ///< ����
} TEST_STEP_E;

typedef struct
{
    uint8_t nWorkMode;	  ///ѡ����ģʽ
    uint8_t nStatus;	  ///< ʵ��״̬
    uint8_t nStep;		  ///< ��������
    uint8_t nStepBkp;	  ///< ��������
    uint8_t nSelect;	  ///< ��������ģʽ�µĵ�ǰ��λ
} TEST_VALUE_T;

typedef struct
{
    uint8_t	 nModeStep;		 ///< ͬʱ����ģʽ�ڵĹ�������
    uint8_t	 nValidFlag;	 ///< �������ģʽ�£��Ƿ�������
    uint8_t	 nEndFlag;		 ///< ���ж�����ɱ�־
    uint32_t nRunTime;		 ///< ����ʱ�䣬��λ��MS
    uint32_t nTestCount;	 ///< ��ǰ�������
    uint32_t nValidTime;	 ///< �������ģʽ�£���⵽�л��޵�ʱ��
    uint32_t nFaultCount;	 ///< �������
} TEST_INFO_T;


extern TEST_VALUE_T g_stTestValue;	  ///< ֻ��һ�����У���������
extern TEST_VALUE_T g_stTestModeValue;
extern TEST_INFO_T	g_stTestInfo[TEST_SELECT_NUMBER];

extern void SysTestCtrlProc(void);
extern void SysTestCtrlMeanWhile(void);	   //ͬʱ����������
extern void SysTestCtrlSequence(void);	   //��������������
extern void SysTestCtrlManual(void);	   //�ֶ�����������
extern void SysTestManualIconProc(TEST_SELECT_E _select, uint8_t _flag);
extern void SysTestAutoIconProc(TEST_SELECT_E _select, uint8_t _flag);

#endif
