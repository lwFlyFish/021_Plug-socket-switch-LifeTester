///////////////////////////////////////////////////////////////////////////
// file
// author
// version  1.0
// date
//
///////////////////////////////////////////////////////////////////////////
#include "SaveParamProc.h"

#include "24cxx.h"
#include "RealDatabase.h"
#include "SysConfig.h"
#include "SysLCD_PageParam.h"
#include "SysTick.h"

///////////////////////////////////////////////////////////////////////////

static u8 m_nFirstConfigFlag = 0;

static uint8_t	m_nStep	 = 0;
static uint32_t m_nTime	 = 0;
static uint8_t	m_nCount = 0;

/*
上电读取系统存储参数
*/
void EE_ReadSysConfigInfo(void)
{
    m_nTime += 10;

    switch(m_nStep)
    {
    case 0:	   // 5s
        if(m_nTime > 2000) { m_nStep = 1; }
        break;

    case 1:
        m_nFirstConfigFlag = AT24CXX_ReadOneByte(FIRST_RUN_ADDR);
        if(m_nFirstConfigFlag != EE_VALUE_INIT)
        {
            if(++m_nCount < 5)
            {
                m_nStep	 = 0;
                m_nCount = 0;
            }
            else
            {
                EE_InitConfigParam();
                m_nStep = 2;
            }
        }
        else
        {
            m_nStep = 2;
        }
        break;

    case 2:
        EE_ReadAllParam();
        m_nStep = 3;
        break;

    default: break;
    }
}


static void EE_InitConfigParam(void)
{
    //保存标志
    AT24CXX_WriteOneByte(FIRST_RUN_ADDR, EE_VALUE_INIT);

    for(uint8_t i = 0; i < TEST_TYPE_NUMBER; i++)
    {
        for(uint8_t j = 0; j < TEST_SELECT_NUMBER; j++)
        {
            g_stParamValue[i][j].nTestCount		   = 10;
            g_stParamValue[i][j].nTurnTime		   = 50;
            g_stParamValue[i][j].nLeaveTime		   = 50;
            g_stParamValue[i][j].nShortTime		   = 60;
            g_stParamValue[i][j].nBrokenTime	   = 60;
            g_stParamValue[i][j].nFaultCount	   = 2;
            g_stParamValue[i][j].nCurrentDetection = 0;
        }
    }

    SysParamSaveAllProc();
}

static void EE_ReadAllParam(void) { SysParamReadAllProc(); }


// 保存一个变量
void SysParamSaveProc(TEST_TYPE_E _type, TEST_SELECT_E _select, TFT_PARAM_SAVE_INDEX_E _index)
{
    uint16_t		   nAddr = EE_ADDR_TFT_PARAM + (_type * TEST_SELECT_NUMBER + _select) * EE_SIZE_PAGE;
    TFT_Param_Value_T *st	 = &g_stParamValue[_type][_select];

    switch(_index)
    {
    case TFT_PARAM_SAVE_INDEX_TEST_COUNT: AT24CXX_Write(nAddr + OffsetAddr(TFT_Param_Value_T, nTestCount), (uint8_t *)&st->nTestCount, sizeof(uint32_t)); break;
    case TFT_PARAM_SAVE_INDEX_TURN_TIME: AT24CXX_Write(nAddr + OffsetAddr(TFT_Param_Value_T, nTurnTime), (uint8_t *)&st->nTurnTime, sizeof(uint32_t)); break;
    case TFT_PARAM_SAVE_INDEX_LEAVE_TIME: AT24CXX_Write(nAddr + OffsetAddr(TFT_Param_Value_T, nLeaveTime), (uint8_t *)&st->nLeaveTime, sizeof(uint32_t)); break;
    case TFT_PARAM_SAVE_INDEX_SHORT_TIME: AT24CXX_Write(nAddr + OffsetAddr(TFT_Param_Value_T, nShortTime), (uint8_t *)&st->nShortTime, sizeof(uint32_t)); break;
    case TFT_PARAM_SAVE_INDEX_BROKEN_TIME: AT24CXX_Write(nAddr + OffsetAddr(TFT_Param_Value_T, nBrokenTime), (uint8_t *)&st->nBrokenTime, sizeof(uint32_t)); break;
    case TFT_PARAM_SAVE_INDEX_FAULT_COUNT: AT24CXX_Write(nAddr + OffsetAddr(TFT_Param_Value_T, nFaultCount), (uint8_t *)&st->nFaultCount, sizeof(uint32_t)); break;
    case TFT_PARAM_SAVE_INDEX_CURRENT_DETECT: AT24CXX_Write(nAddr + OffsetAddr(TFT_Param_Value_T, nCurrentDetection), (uint8_t *)&st->nCurrentDetection, sizeof(uint8_t)); break;

    default: break;
    }
}

// 保存所有变量
void SysParamSaveAllProc(void)
{
    uint16_t nAddr = 0;

    for(uint8_t i = 0; i < TEST_TYPE_NUMBER; i++)
    {
        for(uint8_t j = 0; j < TEST_SELECT_NUMBER; j++)
        {
            nAddr = EE_ADDR_TFT_PARAM + (i * TEST_SELECT_NUMBER + j) * EE_SIZE_PAGE;
            AT24CXX_Write(nAddr, (uint8_t *)&g_stParamValue[i][j], sizeof(TFT_Param_Value_T));
        }
    }
}

// 读取一个变量
void SysParamReadProc(TEST_TYPE_E _type, TEST_SELECT_E _select, TFT_PARAM_SAVE_INDEX_E _index)
{
    uint16_t		   nAddr = EE_ADDR_TFT_PARAM + (_type * TEST_SELECT_NUMBER + _select) * EE_SIZE_PAGE;
    TFT_Param_Value_T *st	 = &g_stParamValue[_type][_select];

    switch(_index)
    {
    case TFT_PARAM_SAVE_INDEX_TEST_COUNT: AT24CXX_Read(nAddr + OffsetAddr(TFT_Param_Value_T, nTestCount), (uint8_t *)&st->nTestCount, sizeof(uint32_t)); break;
    case TFT_PARAM_SAVE_INDEX_TURN_TIME: AT24CXX_Read(nAddr + OffsetAddr(TFT_Param_Value_T, nTurnTime), (uint8_t *)&st->nTurnTime, sizeof(uint32_t)); break;
    case TFT_PARAM_SAVE_INDEX_LEAVE_TIME: AT24CXX_Read(nAddr + OffsetAddr(TFT_Param_Value_T, nLeaveTime), (uint8_t *)&st->nLeaveTime, sizeof(uint32_t)); break;
    case TFT_PARAM_SAVE_INDEX_SHORT_TIME: AT24CXX_Read(nAddr + OffsetAddr(TFT_Param_Value_T, nShortTime), (uint8_t *)&st->nShortTime, sizeof(uint32_t)); break;
    case TFT_PARAM_SAVE_INDEX_BROKEN_TIME: AT24CXX_Read(nAddr + OffsetAddr(TFT_Param_Value_T, nBrokenTime), (uint8_t *)&st->nBrokenTime, sizeof(uint32_t)); break;
    case TFT_PARAM_SAVE_INDEX_FAULT_COUNT: AT24CXX_Read(nAddr + OffsetAddr(TFT_Param_Value_T, nFaultCount), (uint8_t *)&st->nFaultCount, sizeof(uint32_t)); break;
    case TFT_PARAM_SAVE_INDEX_CURRENT_DETECT: AT24CXX_Read(nAddr + OffsetAddr(TFT_Param_Value_T, nCurrentDetection), (uint8_t *)&st->nCurrentDetection, sizeof(uint8_t)); break;

    default: break;
    }
}

// 读取所有变量
void SysParamReadAllProc(void)
{
    uint16_t nAddr = 0;

    for(uint8_t i = 0; i < TEST_TYPE_NUMBER; i++)
    {
        for(uint8_t j = 0; j < TEST_SELECT_NUMBER; j++)
        {
            nAddr = EE_ADDR_TFT_PARAM + (i * TEST_SELECT_NUMBER + j) * EE_SIZE_PAGE;
            AT24CXX_Read(nAddr, (uint8_t *)&g_stParamValue[i][j], sizeof(TFT_Param_Value_T));
        }
    }
}
