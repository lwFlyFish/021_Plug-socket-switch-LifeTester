///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
#include "RealDatabase.h"

#include "SysConfig.h"


TEST_TYPE_E g_nTestType			   = TEST_TYPE_SOCKET;
uint8_t		g_nFlagMeanWhile_Again = 0;
uint8_t		g_nFlagTest_Stop	   = 0;


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
RTC_TIME_DEF g_stRtcTime;

uint8_t g_nMotor2Status_Step	  = 0;
uint8_t g_nMotor3Status_Step	  = 0;
uint8_t g_nGasInputDisableFlag	  = 0;
uint8_t g_nScaleReadEnd_bsa224s	  = 0;
uint8_t g_nScaleReadStart_bsa224s = 0;
uint8_t g_nScaleCount			  = 0;
uint8_t g_nEnableScaleZeroFlag	  = 0;
uint8_t g_nScaleCommFlag_bsa224s  = 0;

volatile u16 g_nSysLedRunCount		 = 10;
volatile u16 g_nSysLedBlinkCount	 = 100;
volatile u16 g_nAdcPeriodFilterCount = 10;
volatile u16 g_nRtcReadTimerCount	 = 10;

volatile u16 g_nStepMotorFsmCount = 10;

//系统处理
volatile BYTE g_nSysProcTimerCtrl = 10;

//系统时间片控制
volatile UINT16 g_nSysTimerCtrl = 0;
//系统显示时间片
volatile BYTE g_nSysDisplayTimeCtrl = 30;
// 1200ms反转定时
volatile UINT16 g_nSysTurnBackTimeCtrl = 0;
//
volatile BYTE g_nSysStaticFreshTimerCtrl = 10;

///////////////////////////////////////////////////////////////////////////

// LYY----------------------------------------------------------------------
// volatile u8 g_arrSysCommRecvBuf_SP1[SYSCOMMRECVBUFSIZE1];
// volatile u8 g_arrSysCommSendBuf_SP1[SYSCOMMSENDBUFSIZE1];
// volatile u8 g_arrSysCommRecvBuf_SP2[SYSCOMMRECVBUFSIZE2];
// volatile u8 g_arrSysCommSendBuf_SP2[SYSCOMMSENDBUFSIZE2];
// volatile u8 g_arrUart3RecvBuf[SYSCOMMRECVBUFSIZE3];
// volatile u8 g_arrSysCommSendBuf_SP3[SYSCOMMSENDBUFSIZE3];

//设备当前参数
// DEVCURPARAMINFO g_stDevCurParam;
//设备特别参数
// DEVSPECIALPARAMINFO g_stDevSpecialParam;
// 4档位标定参数 - 1x/2x/5x/10x 标定参数数组 - 372字节
// SYSBDPARAMINFO g_stSysBDParamInfo[4];
// //试验结果 - 220字节
// TESTRESULTINFO g_stTestResultInfo[1];  //试验结果结构数组
// //系统时间保护 - 10字节
// SYSTIMEPROTECTINFO g_stSysTimeProtectInfo;

volatile int32 g_nCurForceBaseValue = 0;
volatile int32 g_nCurForceValue		= 0;

//////////////////////////////////////////////////////////////////////////

// //系统状态标志
// SYSSTATUSTAGINFO g_nSysStatusTagInfo;
//修改时间标志
u8 g_nSetSysTimeFlag = 0;
//
// SYSSPECIALSTATUSINFO g_nSysSpecialStatusInfo;

//实时刷新标志
// WORK_PAGE_TAG_DEF g_nWorkPageShowTag;
//
// PID_PAGE_TAG_DEF g_stPIDPageShowTag;
//
// CONFIG_PAGE_TAG_DEF g_stConfigPageShowTag;

//
// STATICFILLTAG_WORKPAGEFIRST g_stSysStaticFillTag_WorkPageFirst;
// STATICFILLTAG_TESTPARAMPAGEFIRST g_stStaticFillTag_TestParamPageFirst;
// STATICFILLTAG_TESTPARAMPAGESECOND g_stStaticFillTag_TestParamPageSecond;
// STATICFILLTAG_CONFIGPARAMPAGEFIRST g_stStaticFillTag_ConfigParamParam;
// STATICFILLTAG_TESTRESULTPAGEFIRST g_stStaticFillTag_TestResultPageFirst;
// STATICFILLTAG_TESTRESULTPAGESECOND g_stStaticFillTag_TestResultPageSecond;
// STATICFILLTAG_RESULTSTATPAGEFIRST g_stStaticFillTag_ResultStatPageFirst;
// STATICFILLTAG_RESULTSTATPAGESECOND g_stStaticFillTag_ResultStatPageSecond;
// STATICFILLTAG_BDPAGEFIRST g_stStaticFillTag_BDPageParam = { 0 };

//////////////////////////////////////////////////////////////////////////

// LYY----------------------------------------------------------------------
volatile u8 g_nSysCommTimerCtrl = 10;

// UART1
volatile u16 g_nSysInitComm1Timer	   = 1000;
volatile u16 g_nSysCommTimeoutCtrl_SP1 = 0;
volatile u32 g_nSysCommDelayTime_SP1   = 0;
volatile s16 g_nSysCommStatusCheck_SP1 = 0;
volatile u8	 g_nSysCommStatusStep_SP1  = 0;
volatile u8	 g_nSysCommTimeoutTag_SP1  = 0;
volatile u8	 g_nDevCommStatus_SP1	   = DEVONLINE;
volatile u8	 g_nSysAddrError_SP1	   = 0;

//通信状态 0:非接收状态, 1:接收状态
volatile u8 g_bRecvStatus_SP1 = FALSE;
//通信状态
volatile CMDINFO g_stSysCmdInfo_SP1;
volatile s16	 g_nSP1RecvDelay = 0;

// UART2
volatile u16 g_nSysInitComm2Timer	   = 1000;
volatile u16 g_nSysCommTimeoutCtrl_SP2 = 0;
volatile u32 g_nSysCommDelayTime_SP2   = 0;
volatile s16 g_nSysCommStatusCheck_SP2 = 0;
volatile u8	 g_nSysCommStatusStep_SP2  = 0;
volatile u8	 g_nSysCommTimeoutTag_SP2  = 0;
volatile u8	 g_nDevCommStatus_SP2	   = DEVONLINE;
volatile u8	 g_nSysAddrError_SP2	   = 0;
//通信状态 0:非接收状态, 1:接收状态
volatile u8 g_bRecvStatus_SP2 = FALSE;
//通信状态
volatile CMDINFO g_stSysCmdInfo_SP2;
volatile s16	 g_nSP2RecvDelay				= 0;
volatile u16	 g_nSysRequestDataTimeoutTimer2 = 1000;

// UART3
volatile u16 g_nSysInitComm3Timer	   = 1000;
volatile u16 g_nSysCommTimeoutCtrl_SP3 = 0;
volatile u32 g_nSysCommDelayTime_SP3   = 0;
volatile s16 g_nSysCommStatusCheck_SP3 = 0;
volatile u8	 g_nSysCommStatusStep_SP3  = 0;
volatile u8	 g_nSysCommTimeoutTag_SP3  = 0;
volatile u8	 g_nDevCommStatus_SP3	   = DEVONLINE;
volatile u8	 g_nSysAddrError_SP3	   = 0;

//通信状态 0:非接收状态, 1:接收状态
volatile u8 g_bRecvStatus_SP3 = FALSE;
//通信状态
volatile CMDINFO g_stSysCmdInfo_SP3;
volatile s16	 g_nSP3RecvDelay				= 0;
volatile u16	 g_nSysRequestDataTimeoutTimer3 = 1000;

// UART4
volatile u16 g_nSysInitComm4Timer	   = 1000;
volatile u16 g_nSysCommTimeoutCtrl_SP4 = 0;
volatile u32 g_nSysCommDelayTime_SP4   = 0;
volatile s16 g_nSysCommStatusCheck_SP4 = 0;
volatile u8	 g_nSysCommStatusStep_SP4  = 0;
volatile u8	 g_nSysCommTimeoutTag_SP4  = 0;
volatile u8	 g_nDevCommStatus_SP4	   = DEVONLINE;
volatile u8	 g_nSysAddrError_SP4	   = 0;

//通信状态 0:非接收状态, 1:接收状态
volatile u8 g_bRecvStatus_SP4 = FALSE;
//通信状态
volatile CMDINFO g_stSysCmdInfo_SP4;
volatile s16	 g_nSP4RecvDelay				= 0;
volatile u16	 g_nSysRequestDataTimeoutTimer4 = 1000;

// UART5
volatile u16 g_nSysInitComm5Timer	   = 1000;
volatile u16 g_nSysCommTimeoutCtrl_SP5 = 0;
volatile u32 g_nSysCommDelayTime_SP5   = 0;
volatile s16 g_nSysCommStatusCheck_SP5 = 0;
volatile u8	 g_nSysCommStatusStep_SP5  = 0;
volatile u8	 g_nSysCommTimeoutTag_SP5  = 0;
volatile u8	 g_nDevCommStatus_SP5	   = DEVONLINE;
volatile u8	 g_nSysAddrError_SP5	   = 0;

//通信状态 0:非接收状态, 1:接收状态
volatile u8 g_bRecvStatus_SP5 = FALSE;
//通信状态
volatile CMDINFO g_stSysCmdInfo_SP5;
volatile s16	 g_nSP5RecvDelay				= 0;
volatile u16	 g_nSysRequestDataTimeoutTimer5 = 1000;

// UART6
volatile u16 g_nSysInitComm6Timer	   = 1000;
volatile u16 g_nSysCommTimeoutCtrl_SP6 = 0;
volatile u32 g_nSysCommDelayTime_SP6   = 0;
volatile s16 g_nSysCommStatusCheck_SP6 = 0;
volatile u8	 g_nSysCommStatusStep_SP6  = 0;
volatile u8	 g_nSysCommTimeoutTag_SP6  = 0;
volatile u8	 g_nDevCommStatus_SP6	   = DEVONLINE;
volatile u8	 g_nSysAddrError_SP6	   = 0;

//通信状态 0:非接收状态, 1:接收状态
volatile u8 g_bRecvStatus_SP6 = FALSE;
//通信状态
volatile CMDINFO g_stSysCmdInfo_SP6;
volatile s16	 g_nSP6RecvDelay				= 0;
volatile u16	 g_nSysRequestDataTimeoutTimer6 = 1000;

//////////////////////////////////////////////////////////////////////////
volatile u8 g_nUSART1_Tx_Char;
volatile u8 g_nUARTSendOver_Flag;

// 485用 UART3用作485
volatile u8 g_nSysUsart3RecvOver_Flag	   = 0;
volatile u8 g_nSysUsart3SendOver_Flag	   = 0;
volatile u8 g_n485CommStatusShiftDelay_SP3 = 0;
//模拟串口驱动485
volatile u8 g_nSysUsart6RecvOver_Flag	   = 0;
volatile u8 g_nSysUsart6SendOver_Flag	   = 0;
volatile u8 g_n485CommStatusShiftDelay_SP6 = 0;

volatile u8 g_nSysUsart4RecvOver_Flag	   = 0;
volatile u8 g_nSysUsart4SendOver_Flag	   = 0;
volatile u8 g_n485CommStatusShiftDelay_SP4 = 0;

//读写指针
volatile BYTE g_nWrPos = 0x00;
volatile BYTE g_nRdPos = 0x00;


//系统状态标志
// SYSSTATUSTAGINFO g_nSysStatusTagInfo;
// SYSPASSWORDINFO g_stSyInPassWordInfo;


//配置参数
CONFIG_PARAM_DEF	 g_stConfigParam = {0};
CONFIG_PARAM_TAG_DEF g_stConfigTag	 = {0};
TEST_PARAM_DEF		 g_stRealParam;
// TEST_PARAM_BUF_DEF g_arrRealParamBuf[REAL_BUF_NUM] = { 0 };
// RESULT_PARAM_DEF g_stResultParam[RESULT_NUM] = { 0 };

// //PID参数
PID_AI_PARAM_DEF   g_stPIDAIParam  = {0};
PID_AI_SET_TAG_DEF g_stPIDAISetTag = {0};

// //标定信息
// BD_PARAM_DEF g_stBDParam = { 0 };
// u8 g_nBDCurPointCount = 0;

// AI通信
u8 g_nCurCommAddr	 = 0;
u8 g_nAICmdAckOkFlag = 1;
//设置温控器标志
u8 g_nAISetBusy = 0;

u8 g_nFillStaticInfoFlag = 1;
//结果页面计数
u8 g_nResultItemCount = 1;
//记录保存的结果数量
u8 g_nResultItemTotal = RESULT_NUM;
//最后结果保存的位置
u8 g_nResultItemPoint = 1;


// PID参数自整定结束
u8 g_nPIDAutoEndFlag = 0;

DELAY_INFO_DEF g_stLampInfo = {1800, 0}, g_stColdWaterInfo = {1, 0}, g_stAI7048DelayInfo = {2, 0};

//
char g_stWarningBuf[10] = "正常";

//绝对值编码器结果
u16 g_nAbsEncoderValue	 = 0;
u16 g_nReadAbsEncoderTag = 0;

///////////////////////////////////////////////////////////////////////////
//组装多字节到长整形 nFirstValue 在低位
int32 ConfigMulitBytesToValueProc(BYTE nFirstValue, BYTE nSeocndValue, BYTE nThirdValue, BYTE nFourthValue)
{
	int32 nTempValue = 0;
    int32 nResult	 = 0;

	nTempValue = 0;
    nResult	   = 0;

	nTempValue = nFirstValue;
	nResult |= nTempValue;

	nTempValue = 0;
	nTempValue = nSeocndValue;
	nResult |= (nTempValue << 8);

	nTempValue = 0;
	nTempValue = nThirdValue;
	nResult |= (nTempValue << 16);

	nTempValue = 0;
	nTempValue = nFourthValue;
	nResult |= (nTempValue << 24);

	return nResult;
}

///////////////////////////////////////////////////////////////////////////
u8 SysDecToBCDProc(u8 nNum)
{
	u8 bcd;

    bcd = ((nNum / 10 % 10) << 4) + nNum % 10;

	return (bcd);
}

u8 SysBCDToDecProc(u8 bcd)	  //十进制转BCD
{
	return (0xff & (bcd >> 4)) * 10 + (0xf & bcd);
}

void SysDWordToBCDProc(u8* pBCDStr, u32 nNum, u8 nLen)
{
    u16 tmp;
    u16 i = 0;

    while(nNum && nLen)
    {
        tmp			 = (u8)(nNum % 100);
        nNum		 = nNum / 100;
		pBCDStr[i++] = SysDecToBCDProc(tmp);
        nLen--;
    }

	//
    while(nLen--) { pBCDStr[i++] = 0x00; }
}

u16 SysWordToBCDProc(u16 nNum)
{
    u8	nLowValue;
    u8	nHihValue;
	u16 nValue;

	nLowValue = (u8)(nNum % 100);
	nLowValue = SysDecToBCDProc(nLowValue);

	nNum = nNum / 100;
    if(nNum >= 100) { nHihValue = (u8)(nNum % 100); }
	else
	{
		nHihValue = (u8)nNum;
	}
	nHihValue = SysDecToBCDProc(nHihValue);

	//
	nValue = nHihValue;
	nValue = (nValue << 8) | nLowValue;

	return nValue;
}

u16 SysBCDToWordProc(u16 nBCD)
{
	u16 nHex = 0;

	//
    nHex = (nBCD >> 12 & 0x000F) * 1000 + (nBCD >> 8 & 0x000F) * 100 + (nBCD >> 4 & 0x000F) * 10 + (nBCD & 0x000F);

	return nHex;
}

///////////////////////////////////////////////////////////////////////////
// pStr1存储长度要大于最后拼接的长度
// Description:字符串连接函数
///////////////////////////////////////////////////////////////////////////
void StringCat(char* pStr1, char* pStr2)
{
	int i, j;

    for(i = 0; pStr1[i] != '\0'; i++) { pStr1[i] = pStr1[i]; }

    for(j = 0; pStr2[j] != '\0'; j++) { pStr1[i + j] = pStr2[j]; }

	pStr1[i + j] = '\0';
}


void StringCopy(char* pStr1, char* pStr2, unsigned char nLen)
{
    int i;	  //, j;

    for(i = 0; pStr2[i] != '\0' && i < nLen; i++) { pStr1[i] = pStr2[i]; }

	pStr1[i] = '\0';
}

u8 StringLen(char* pStr)
{
	u8 nLen = 0;

    while(*pStr++ != '\0') { nLen++; }

	return nLen;
}

void MemSet(char* pStr, char nData, u8 nLen)
{
	u8 i = 0;

    for(i = 0; i < nLen; i++) { *(pStr + i) = nData; }
}

/*
// C prototype : void StrToHex(BYTE *pbDest, BYTE *pbSrc, int nLen)
// parameter(s): [OUT] pbDest - 输出缓冲区
//	[IN] pbSrc - 字符串
//	[IN] nLen - 16进制数的字节数(字符串的长度/2)
// return value:
// remarks : 将字符串转化为16进制数
*/
// void StrToHex(BYTE *pbDest, BYTE *pbSrc, int nLen)
// {
// 	char h1, h2;
// 	BYTE s1, s2;
// 	int i;
//
// 	for (i = 0; i < nLen; i++)
// 	{
// 		h1 = pbSrc[2 * i];
// 		h2 = pbSrc[2 * i + 1];
//
// 		s1 = toupper(h1) - 0x30;
// 		if (s1 > 9)
// 			s1 -= 7;
//
// 		s2 = toupper(h2) - 0x30;
// 		if (s2 > 9)
// 			s2 -= 7;
//
// 		pbDest[i] = s1 * 16 + s2;
// 	}
// }

/*
// C prototype : void HexToStr(BYTE *pbDest, BYTE *pbSrc, int nLen)
// parameter(s): [OUT] pbDest - 存放目标字符串
//	[IN] pbSrc - 输入16进制数的起始地址
//	[IN] nLen - 16进制数的字节数
// return value:
// remarks : 将16进制数转化为字符串
*/
void HexToStr(BYTE* pbDest, BYTE* pbSrc, int nLen)
{
    char ddl, ddh;
    u8	 i, j;


    for(i = 0; i < nLen; i++)
	{
		ddh = 48 + pbSrc[i] / 16;
		ddl = 48 + pbSrc[i] % 16;
        if(ddh > 57) ddh = ddh + 7;
        if(ddl > 57) ddl = ddl + 7;
        pbDest[(nLen - i - 1) * 2]	   = ddh;
		pbDest[(nLen - i - 1) * 2 + 1] = ddl;
	}

	//去掉0
    for(i = 0; i < nLen * 2; i++)
	{
        if(*(pbDest + i) != '0') { break; }
	}
    for(j = 0; i < nLen * 2; i++, j++) { *(pbDest + j) = *(pbDest + i); }

    // pbDest[nLen * 2] = '\0';
	*(pbDest + j) = '\0';
}

//循环右移
unsigned int Cror(unsigned int temp, unsigned char num)
{
	unsigned char i;
    for(i = 0; i < num; i++)
	{
        if(temp & 0x01)
        {
            temp >>= 1;
            temp |= 0x80;
        }
        else
            temp >>= 1;
	}
	return temp;
}
//循环左移
unsigned int Crol(unsigned int temp, unsigned char num)
{
	unsigned char i;
    for(i = 0; i < num; i++)
	{
        if(temp & 0x80)
        {
            temp <<= 1;
            temp |= 0x01;
        }
        else
            temp <<= 1;
	}
	return temp;
}

/*
用数学运算来计算输入的整数有几位
*/
u8 GetDecNumber(int num)
{
	u8 i = 0;

	do
	{
		num = num / 10;
		i++;
    } while(num != 0);

	return i;
}


/*
计算十进制的每一位的权重
nWeight ：不能为0,从1-n
*/
u32 DecimalWeight(u8 nWeight)
{
    u8	nCount	= 0;
	u32 nReturn = 1;
    if(nWeight == 0) { return 1; }
    if(nWeight == 1) { return 1; }
    for(nCount = 2; nCount <= nWeight; nCount++) { nReturn *= 10; }
	return nReturn;
}

/*
u16 nSource:整形数据
u8 nPoints：小数位数
*/
float IntToFloatProc(s32 nSource, u8 nPoints)
{
	float fTemp = 0.0;
    s32	  nTemp = 0;
    switch(nPoints)
	{
    case 0: fTemp = (float)(nSource / 1.0); break;

	case 1:
		fTemp = (float)(nSource / 10.0);
		nTemp = (s32)(fTemp * 10);
		fTemp = nTemp / 10.0;
		break;
	case 2:
		fTemp = (float)(nSource / 100.0);
		nTemp = (s32)(fTemp * 100);
		fTemp = nTemp / 100.0;
		break;
	case 3:
		fTemp = (float)(nSource / 1000.0);
		nTemp = (s32)(fTemp * 1000);
		fTemp = nTemp / 1000.0;
		break;
	case 4:
		fTemp = (float)(nSource / 10000.0);
		nTemp = (s32)(fTemp * 10000);
		fTemp = nTemp / 10000.0;
		break;
	case 5:
		fTemp = (float)(nSource / 100000.0);
		nTemp = (s32)(fTemp * 100000);
		fTemp = nTemp / 100000.0;
		break;

	case 6:
		fTemp = (float)(nSource / 1000000.0);
		nTemp = (s32)(fTemp * 1000000);
		fTemp = nTemp / 1000000.0;
		break;

    default: fTemp = (float)(nSource / 1.0); break;
	}
	return fTemp;
}

/*
说明：程序自动计算小数位数，nLimitPointsMax用来限制想要得到的小数位数
float nSource,小数
pSourcePoints传入的小数位数
u8 nLimitPointsMax，限制的小数位数
*/
char strFloat[20];
s32	 FloatToIntProc(float nSource, u8* pSourcePoints, u8 nLimitPointsMax)
{
    u8	  nPoints = 0;
    u8	  nCount  = 0;
    u8	  i		  = 0;
    char* ptr;

    for(nCount = 0; nCount < 20; nCount++) { strFloat[nCount] = 0; }

	sprintf(strFloat, "%f", nSource);

	nPoints = strlen(strFloat) - 1;
	//去掉无效位
    for(i = 0; i < nPoints; i++)
	{
        if(strFloat[nPoints - i] == '0') { strFloat[nPoints - i] = 0; }
		else
		{
			break;
		}
	}

	ptr = strstr(strFloat, ".");
    if(ptr != NULL)
	{
		nPoints = strlen(ptr) - 1;
        if(nPoints >= nLimitPointsMax) { nPoints = nLimitPointsMax; }
		*pSourcePoints = nPoints;
        return (s32)(nSource * DecimalWeight(nPoints + 1));
	}
    else	//没有小数
	{
		*pSourcePoints = 0;
		return (s32)nSource;
	}
}

#include "..\MALLOC\malloc.h"

#include <stdio.h>
#include <stdlib.h>
//
// 因为涉及到内存操作，需要 #include <stdlib.h>
char* myStrncpy(char* dest, const char* src, int n)
{
    int	  size = sizeof(char) * (n + 1);
    char* tmp  = (char*)mymalloc(size);	   // 开辟大小为n+1的临时内存tmp

    if(tmp)
	{
        memset(tmp, '\0', size);	   // 将内存初始化为0
        memcpy(tmp, src, size - 1);	   // 将src的前n个字节拷贝到tmp
        memcpy(dest, tmp, size);	   // 将临时空间tmp的内容拷贝到dest

        myfree(tmp);	// 释放内存
		return dest;
	}
    else
	{
		return NULL;
	}
}


RTC_TIME_DEF GetSoftWareBuildTargetTime(void)
{
    char		 arrDate[20];	 // Jul 03 2015
    char		 arrTime[20];	 // 06:17:05
    char		 pDest[20];
	RTC_TIME_DEF stTime;

    sprintf(arrDate, "%s", __DATE__);	 // Jul 03 2015
    sprintf(arrTime, "%s", __TIME__);	 // 06:17:05

    // char *strncpy(char *dest, const char *src, int n)
	//(char*)(&(pDest[0])) = myStrncpy(pDest, arrDate, 3);
	sprintf(pDest, "%s", myStrncpy(pDest, arrDate, 3));

    if(strcmp(pDest, "Jan") == 0)
        stTime.nMonth = 1;
    else if(strcmp(pDest, "Feb") == 0)
        stTime.nMonth = 2;
    else if(strcmp(pDest, "Mar") == 0)
        stTime.nMonth = 3;
    else if(strcmp(pDest, "Apr") == 0)
        stTime.nMonth = 4;
    else if(strcmp(pDest, "May") == 0)
        stTime.nMonth = 5;
    else if(strcmp(pDest, "Jun") == 0)
        stTime.nMonth = 6;
    else if(strcmp(pDest, "Jul") == 0)
        stTime.nMonth = 7;
    else if(strcmp(pDest, "Aug") == 0)
        stTime.nMonth = 8;
    else if(strcmp(pDest, "Sep") == 0)
        stTime.nMonth = 9;
    else if(strcmp(pDest, "Oct") == 0)
        stTime.nMonth = 10;
    else if(strcmp(pDest, "Nov") == 0)
        stTime.nMonth = 11;
    else if(strcmp(pDest, "Dec") == 0)
        stTime.nMonth = 12;
    else
        stTime.nMonth = 1;

    sprintf(pDest, "%s", myStrncpy(pDest, arrDate + 4, 2));
    // int atoi(const char *nptr);
	stTime.nDay = atoi(pDest);

	sprintf(pDest, "%s", myStrncpy(pDest, arrDate + 4 + 3, 4));
    // int atoi(const char *nptr);
	stTime.nYear = atoi(pDest);

    // time
	sprintf(pDest, "%s", myStrncpy(pDest, arrTime, 2));
	stTime.nHour = atoi(pDest);
    sprintf(pDest, "%s", myStrncpy(pDest, arrTime + 3, 2));
	stTime.nMinute = atoi(pDest);
	sprintf(pDest, "%s", myStrncpy(pDest, arrTime + 3 + 3, 2));
	stTime.nSecond = atoi(pDest);

	return stTime;
}

///////////////////////////////////////////////////////////////////////////
const u8 m_arrCRCHih[] = {0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
                          0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
                          0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
                          0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
                          0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00,
                          0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
                          0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80,
                          0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
                          0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40};

//
const u8 m_arrCRCLow[] = {0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9,
                          0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12,
                          0x13, 0xD3, 0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E,
                          0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5,
                          0x27, 0xE7, 0xE6, 0x26, 0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C,
                          0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD,
                          0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57,
                          0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A,
                          0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 0x40};

// RTU CRC
u16 GetSysCRCValueProc(u8* pMsg, u16 nDataLen)
{
	//
	u8 nCRCHih = 0xFF;
	u8 nCRCLow = 0xFF;
	u8 nIndex;

	//
    while(nDataLen--)
	{
		//
        nIndex = nCRCHih ^ *pMsg++;

		//
		nCRCHih = nCRCLow ^ m_arrCRCHih[nIndex];
		nCRCLow = m_arrCRCLow[nIndex];
    }

    //
    return (nCRCHih << 8 | nCRCLow);
}
