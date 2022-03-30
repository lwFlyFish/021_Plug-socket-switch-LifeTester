#include "SysTestProc.h"

#include "24cxx.h"
#include "RealDatabase.h"
#include "SaveParamProc.h"
#include "SysDIProc.h"
#include "SysDOInfoProc.h"
#include "SysLCD_PageInfo.h"
#include "SysLCD_PageParam.h"
#include "SysLCD_PageTest.h"

#define DO_TYPE_BEEP 5

// static uint8_t m_nMId=0; ///< 当前运行的电机ID
TEST_VALUE_T g_stTestValue;
TEST_VALUE_T g_stTestModeValue;
TEST_INFO_T	 g_stTestInfo[TEST_SELECT_NUMBER];

static void	   SysTestDoAllCtrlProc(uint8_t _flag);
static void	   SysTestClearFaultAllCtrlProc(TFT_STATUS_ICON_E _status);
static uint8_t SysCheckStopProc(void);
static void	   SysAutoStepOnProc(TEST_INFO_T *_info, TEST_SELECT_E _select);
static uint8_t SysAutoStepOnWaitProc(TEST_INFO_T *_info, TEST_SELECT_E _select, uint8_t _index);
static void	   SysAutoStepOffProc(TEST_INFO_T *_info, TEST_SELECT_E _select);
static uint8_t SysAutoStepOffWaitProc(TEST_INFO_T *_info, TEST_SELECT_E _select, uint8_t _index);
static void	   SysAutoStepCheckCountProc(TEST_INFO_T *_info, TEST_SELECT_E _select);
static uint8_t SysAutoStepSingleAllEndProc(uint8_t _step);
///////////////////////////////////////////////////////////////////////
// 10ms进一次
void SysTestCtrlProc(void)
{
    uint8_t flag = 0;

    //试验前后状态
    switch(g_stTestValue.nStep)
    {
    case TEST_STEP_IDLE: break;

    case TEST_STEP_START:	 //点了启动，跳转到这里，初始化时间变量,然后跳转到对应的状态
        SysTestDoAllCtrlProc(0);
        SysTestClearFaultAllCtrlProc(TFT_STATUS_ICON_DISABLE);

        flag = 0;
        for(uint8_t i = 0; i < TEST_SELECT_NUMBER; i++)
        {
            if(1 == g_stTftTestValue.arrAutomatic[i]) { flag = 1; }
        }
        if(0 == flag)
        {
            g_stTestValue.nStep = TEST_STEP_STOP;
            break;
        }	 ///< 没有选择工位，停机

        g_stTestValue.nStatus = TEST_STATUS_START;

        memset((TEST_INFO_T *)g_stTestInfo, 0, sizeof(TEST_INFO_T) * TEST_SELECT_NUMBER);
        for(uint8_t i = 0; i < TEST_SELECT_NUMBER; i++)
        {
            g_stTftTestValue.arrManual[i]		 = 0;
            g_stTestShowFlag.arrManual[i]		 = 1;
            g_stPageInfoValue[i].nFlagErrorClear = TFT_STATUS_ICON_DISABLE;
        }

        switch(g_stTestValue.nWorkMode)	   //判断工作模式
        {
        case TEST_MODE_MEAN_WHILE: g_stTestValue.nStep = TEST_STEP_START_MEAN_WHILE; break;

        case TEST_MODE_SEQUENCE: g_stTestValue.nStep = TEST_STEP_START_SEQUENCE; break;
        }

        g_stTestValue.nStepBkp = g_stTestValue.nStep;
        break;

    case TEST_STEP_STOP:	//停止状态
        SysTestDoAllCtrlProc(0);

        // SysTestClearFaultAllCtrlProc(TFT_STATUS_ICON_DISABLE);

        g_stTestShowFlag.nStart		= 1;
        g_stTestShowFlag.nWorkState = 1;

        g_stTestValue.nStatus = TEST_STATUS_STOP;
        g_stTestValue.nStep	  = TEST_STEP_IDLE;
        break;

    //同时工作
    case TEST_STEP_START_MEAN_WHILE: SysTestCtrlMeanWhile(); break;

    //轮流工作
    case TEST_STEP_START_SEQUENCE:
        SysTestCtrlSequence();
        break;

        //报警
    case TEST_STEP_START_WARN:
        flag = 1;
        for(uint8_t i = 0; i < TEST_SELECT_NUMBER; i++)
        {
            if((TFT_STATUS_ICON_DISABLE == g_stPageInfoValue[i].nFlagErrorClear) && (g_stTestInfo[i].nFaultCount >= g_stParamValue[g_nTestType][i].nFaultCount)) { flag = 0; }
        }

        if(1 == flag)
        {
            g_stTestValue.nStep = g_stTestValue.nStepBkp;
            SysDOCtrlProc(DO_TYPE_BEEP, 0);
        }
        else
        {
            SysDOCtrlProc(DO_TYPE_BEEP, 1);
        }
        break;

    //
    default: break;
    }
}

/////////////////////////////////////同时试验工作过程//////////////////////////////////

void SysTestCtrlMeanWhile(void)
{
    uint8_t		 count = g_nTestType * TEST_SELECT_NUMBER;
    TEST_INFO_T *info  = NULL;

    if(1 == SysCheckStopProc())	   // 判断是否全部完成
    {
        g_stTestValue.nStep = TEST_STEP_STOP;
        return;
    }

    for(uint8_t i = 0; i < TEST_SELECT_NUMBER; i++)
    {
        if((g_stTftTestValue.arrAutomatic[i] == 1) && (0 == g_stTestInfo[i].nEndFlag))	  //选择工位
        {
            info = &g_stTestInfo[i];
            info->nRunTime += 10;	 //运行时间开始计时

            switch(info->nModeStep)
            {
            case 0:
                SysAutoStepOnProc(info, (TEST_SELECT_E)i);
                info->nModeStep = 1;
                break;

            case 1:
                if(1 == SysAutoStepOnWaitProc(info, (TEST_SELECT_E)i, count + i)) { info->nModeStep = 2; }
                break;

            case 2:
                SysAutoStepOffProc(info, (TEST_SELECT_E)i);
                info->nModeStep = 3;
                break;

            case 3:
                if(1 == SysAutoStepOffWaitProc(info, (TEST_SELECT_E)i, count + i)) { info->nModeStep = 4; }
                break;

            case 4:
                SysAutoStepCheckCountProc(info, (TEST_SELECT_E)i);
                info->nModeStep = 0;	//同时5
                break;

            case 5:
                if(1 == SysAutoStepSingleAllEndProc(5))
                {
                    for(uint8_t i = 0; i < TEST_SELECT_NUMBER; i++)
                    {
                        if(1 == g_stTftTestValue.arrAutomatic[i]) { g_stTestInfo[i].nModeStep = 0; }
                    }
                }
                break;

            default: break;
            }
        }
    }
}

/////////////////////////////////////轮流试验工作过程//////////////////////////////////
void SysTestCtrlSequence(void)
{
    uint8_t		  count	 = g_nTestType * TEST_SELECT_NUMBER;
    TEST_SELECT_E select = (TEST_SELECT_E)g_stTestValue.nSelect;
    TEST_INFO_T * info	 = &g_stTestInfo[TEST_SELECT_1];

    if(1 == SysCheckStopProc())	   // 判断是否全部完成
    {
        g_stTestValue.nStep = TEST_STEP_STOP;
        return;
    }

    if((g_stTftTestValue.arrAutomatic[select] == 1) && (0 == g_stTestInfo[select].nEndFlag))	//选择工位
    {
        info = &g_stTestInfo[select];
        info->nRunTime += 10;	 //运行时间开始计时
        switch(info->nModeStep)
        {
        case 0:	   // 工位打开
            SysAutoStepOnProc(info, select);
            info->nModeStep = 1;
            break;

        case 1:	   // 等待延时
            if(1 == SysAutoStepOnWaitProc(info, select, count + select)) { info->nModeStep = 2; }
            break;

        case 2:	   // 工位关闭
            SysAutoStepOffProc(info, select);
            info->nModeStep = 3;
            break;

        case 3:	   // 等待延时
            if(1 == SysAutoStepOffWaitProc(info, select, count + select))
            {
                g_stTestValue.nSelect++;
                info->nModeStep = 4;
            }
            break;

        case 4:	   // 判断次数
            SysAutoStepCheckCountProc(&g_stTestInfo[select], select);
            info->nModeStep = 0;
            break;

        default: break;
        }
    }
    else
    {
        g_stTestValue.nSelect++;
    }

    g_stTestValue.nSelect %= TEST_SELECT_NUMBER;
}

// 图标动作时，对应的DO动作。flag是图标返回的数值
void SysTestManualIconProc(TEST_SELECT_E _select, uint8_t _flag)
{
    g_stTestShowFlag.arrManual[_select] = 1;

    if(TEST_STATUS_STOP != g_stTestValue.nStatus) { return; }

    switch(_flag)
    {
    case TFT_STATUS_ICON_DISABLE: g_stTftTestValue.arrManual[_select] = TFT_STATUS_ICON_ENABLE; break;

    case TFT_STATUS_ICON_ENABLE: g_stTftTestValue.arrManual[_select] = TFT_STATUS_ICON_DISABLE; break;

    default: g_stTftTestValue.arrManual[_select] = TFT_STATUS_ICON_DISABLE; break;
    }

    SysDOCtrlProc(_select, g_stTftTestValue.arrManual[_select]);
}

// 图标动作时，刷新数据。flag是图标返回的数值
void SysTestAutoIconProc(TEST_SELECT_E _select, uint8_t _flag)
{
    g_stTestShowFlag.arrAutomatic[_select] = 1;

    if(TEST_STATUS_STOP != g_stTestValue.nStatus) { return; }

    switch(_flag)
    {
    case TFT_STATUS_ICON_DISABLE: g_stTftTestValue.arrAutomatic[_select] = TFT_STATUS_ICON_ENABLE; break;

    case TFT_STATUS_ICON_ENABLE: g_stTftTestValue.arrAutomatic[_select] = TFT_STATUS_ICON_DISABLE; break;

    default: g_stTftTestValue.arrAutomatic[_select] = TFT_STATUS_ICON_DISABLE; break;
    }
}

//控制所有DO统一动作
static void SysTestDoAllCtrlProc(uint8_t _flag)
{
    for(uint8_t i = 0; i < TEST_SELECT_NUMBER; i++) { SysDOCtrlProc(i, _flag); }
    SysDOCtrlProc(DO_TYPE_BEEP, _flag);
}

//控制所有DO统一动作
static void SysTestClearFaultAllCtrlProc(TFT_STATUS_ICON_E _status)
{
    for(uint8_t i = 0; i < TEST_SELECT_NUMBER; i++) { g_stPageInfoValue[i].nFlagErrorClear = _status; }
}

static uint8_t SysCheckStopProc(void)
{
    uint8_t flag = 1;

    for(uint8_t i = 0; i < TEST_SELECT_NUMBER; i++)
    {
        if(0 == g_stParamValue[g_nTestType][i].nTestCount) { g_stTestInfo[i].nEndFlag = 1; }

        if(1 == g_stTftTestValue.arrAutomatic[i])
        {
            if(0 == g_stTestInfo[i].nEndFlag) { flag = 0; }
        }
    }

    return flag;
}

// 打开开关，计时时间和运行时间清零
static void SysAutoStepOnProc(TEST_INFO_T *_info, TEST_SELECT_E _select)
{
    SysDOCtrlProc(_select, 1);
    _info->nValidTime = 0;
    _info->nRunTime	  = 0;
}

// 测量电流开关开的时间，到达短路时间后检查是否电流故障
static uint8_t SysAutoStepOnWaitProc(TEST_INFO_T *_info, TEST_SELECT_E _select, uint8_t _index)
{
    uint8_t flag	= 0;
    uint8_t flag_di = DI_Flag_Set;

    if(flag_di == DI_Flag[_index])	  ///< 检测到电流的时间累加
    { _info->nValidTime += 10; }

    if(_info->nRunTime >= g_stParamValue[g_nTestType][_select].nShortTime * 100)
    {
        _info->nValidFlag = 0;
        if(_info->nValidTime >= g_stParamValue[g_nTestType][_select].nTurnTime * 100)	 ///< 电流开关时间大于接通时间，电流正常
        { _info->nValidFlag = 1; }

        flag = 1;
    }

    return flag;
}

// 关闭开关，计时时间和运行时间清零
static void SysAutoStepOffProc(TEST_INFO_T *_info, TEST_SELECT_E _select)
{
    SysDOCtrlProc(_select, 0);
    _info->nValidTime = 0;
    _info->nRunTime	  = 0;
}

// 测量电流开关关的时间，到达短路时间后检查是否电流故障
static uint8_t SysAutoStepOffWaitProc(TEST_INFO_T *_info, TEST_SELECT_E _select, uint8_t _index)
{
    uint8_t flag = 0;

    uint8_t flag_di = DI_Flag_Reset;

    if(flag_di == DI_Flag[_index]) { _info->nValidTime += 10; }

    if(_info->nRunTime >= g_stParamValue[g_nTestType][_select].nBrokenTime * 100)
    {
        if(1 == _info->nValidFlag)	  ///< 接通状态正常
        {
            _info->nValidFlag = 0;
            if(_info->nValidTime >= g_stParamValue[g_nTestType][_select].nLeaveTime * 100) { _info->nValidFlag = 1; }	 ///< 电流开关时间大于断开时间，电流正常
        }

        if(TFT_STATUS_ICON_ENABLE == g_stParamValue[g_nTestType][_select].nCurrentDetection)	///< 使能电流检测
        {
            if(0 == _info->nValidFlag)	  ///< 电流故障
            {
                _info->nFaultCount++;
                g_stPageInfoShowFlag[_select].nRealErrorCount = 1;
            }
        }

        g_stTestInfo[_select].nTestCount++;						///< 试验次数累加
        g_stPageInfoShowFlag[_select].nRealCount		= 1;	///< 刷新数据
        g_stPageInfoShowFlag[_select].nRealCurrentCount = 1;

        flag = 1;
    }
    return flag;
}

// 检查试验次数是否达到试验次数
static void SysAutoStepCheckCountProc(TEST_INFO_T *_info, TEST_SELECT_E _select)
{
    if(_info->nFaultCount >= g_stParamValue[g_nTestType][_select].nFaultCount)	  ///< 故障次数大于设置值
    {
        if(TFT_STATUS_ICON_DISABLE == g_stPageInfoValue[_select].nFlagErrorClear)	 ///< 没有点击故障清除
        {
            // SysTFTLcd_PageInfoGotoProc();	 ///< 弹出报警页面对话框，而且蜂鸣器响

            // g_stTestValue.nStep = TEST_STEP_START_WARN;
            _info->nEndFlag							 = 1;
            g_stPageInfoShowFlag[_select].nDevStatus = 1;
        }
    }

    if(_info->nTestCount >= g_stParamValue[g_nTestType][_select].nTestCount)	///< 达到设置次数
    {
        _info->nEndFlag							 = 1;
        g_stPageInfoShowFlag[_select].nDevStatus = 1;
    }
}

// 检测所有的达到停止条件
static uint8_t SysAutoStepSingleAllEndProc(uint8_t _step)
{
    uint8_t flag = 1;

    for(uint8_t i = 0; i < TEST_SELECT_NUMBER; i++)
    {
        if((1 == g_stTftTestValue.arrAutomatic[i]) && (0 == g_stTestInfo[i].nEndFlag))
        {
            if(_step != g_stTestInfo[i].nModeStep)
            {
                flag = 0;
                break;
            }
        }
    }
    return flag;
}
