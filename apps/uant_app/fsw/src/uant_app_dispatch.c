/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
 * \file
 *   This file contains the source code for the Sample App.
 */

/*
** Include Files:
*/
#include "uant.h" 
#include "uant_app.h"
#include "uant_app_dispatch.h"
#include "uant_app_cmds.h"
#include "uant_app_eventids.h"
#include "uant_app_msgids.h"
#include "uant_app_msg.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* Verify command packet length                                               */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool UANT_APP_VerifyCmdLength(const CFE_MSG_Message_t *MsgPtr, size_t ExpectedLength)
{
    bool              result       = true;
    size_t            ActualLength = 0;
    CFE_SB_MsgId_t    MsgId        = CFE_SB_INVALID_MSG_ID;
    CFE_MSG_FcnCode_t FcnCode      = 0;

    CFE_MSG_GetSize(MsgPtr, &ActualLength);

    /*
    ** Verify the command packet length.
    */
    if (ExpectedLength != ActualLength)
    {
        CFE_MSG_GetMsgId(MsgPtr, &MsgId);
        CFE_MSG_GetFcnCode(MsgPtr, &FcnCode);

        CFE_EVS_SendEvent(UANT_APP_CMD_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u",
                          (unsigned int)CFE_SB_MsgIdToValue(MsgId), (unsigned int)FcnCode, (unsigned int)ActualLength,
                          (unsigned int)ExpectedLength);

        result = false;

        UANT_APP_Data.ErrCounter++;
    }

    return result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* UANT ground commands                                                     */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void UANT_APP_ProcessGroundCommand(const CFE_SB_Buffer_t *SBBufPtr)
{
    CFE_MSG_FcnCode_t CommandCode = 0;

    CFE_MSG_GetFcnCode(&SBBufPtr->Msg, &CommandCode);

    /*
    ** Process UANT app ground commands
    */
    switch (CommandCode)
    {
        case UANT_APP_NOOP_CC:
            if (UANT_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(UANT_APP_NoopCmd_t)))
            {
                UANT_APP_NoopCmd((const UANT_APP_NoopCmd_t *)SBBufPtr);
            }
            break;

        case UANT_APP_RESET_COUNTERS_CC:
            if (UANT_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(UANT_APP_ResetCountersCmd_t)))
            {
                UANT_APP_ResetCountersCmd((const UANT_APP_ResetCountersCmd_t *)SBBufPtr);
            }
            break;
        /* ───────── 리셋 / ARM / DISARM ───────── */
        case UANT_APP_RESET_CC:
            if (UANT_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(CFE_MSG_CommandHeader_t)))
                ISIS_UANT_Reset();
            break;

        case UANT_APP_ARM_CC:
            if (UANT_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(CFE_MSG_CommandHeader_t)))
                ISIS_UANT_ArmAntennaSystems();
            break;

        case UANT_APP_DISARM_CC:
            if (UANT_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(CFE_MSG_CommandHeader_t)))
                ISIS_UANT_Disarm();
            break;

        /* ───────── 자동 전개 ───────── */
        case UANT_APP_AUTODEPLOY_CC:
            if (UANT_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(UANT_APP_AutoDeployCmd_t)))
            {
                const UANT_APP_AutoDeployCmd_t *cmd = (const UANT_APP_AutoDeployCmd_t *)SBBufPtr;
                ISIS_UANT_AutomatedSequentialDeployment(cmd->BurnTime);
            }
            break;

        /* ───────── 단일 안테나 전개 ───────── */
        case UANT_APP_DEPLOY_ANT_CC:
            if (UANT_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(UANT_APP_DeployAntCmd_t)))
            {
                const UANT_APP_DeployAntCmd_t *cmd = (const UANT_APP_DeployAntCmd_t *)SBBufPtr;
                switch (cmd->AntNum)
                {
                    case 1: ISIS_UANT_DeployAntenna1(cmd->BurnTime); break;
                    case 2: ISIS_UANT_DeployAntenna2(cmd->BurnTime); break;
                    case 3: ISIS_UANT_DeployAntenna3(cmd->BurnTime); break;
                    case 4: ISIS_UANT_DeployAntenna4(cmd->BurnTime); break;
                    default:
                        CFE_EVS_SendEvent(UANT_APP_CC_ERR_EID, CFE_EVS_EventType_ERROR,
                                        "Deploy ANT: invalid antenna num %d", cmd->AntNum);
                        break;
                }
            }
            break;
        /* Override 전개 */
        case UANT_APP_DEPLOY_ANT1_OVERRIDE_CC:
            if (UANT_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(UANT_APP_DeployAntCmd_t)))
            {
                const UANT_APP_DeployAntCmd_t *cmd = (const UANT_APP_DeployAntCmd_t *)SBBufPtr;
                ISIS_UANT_DeployAntenna1WithOverride(cmd->BurnTime);
            }
            break;

        case UANT_APP_DEPLOY_ANT2_OVERRIDE_CC:
            if (UANT_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(UANT_APP_DeployAntCmd_t)))
            {
                const UANT_APP_DeployAntCmd_t *cmd = (const UANT_APP_DeployAntCmd_t *)SBBufPtr;
                ISIS_UANT_DeployAntenna2WithOverride(cmd->BurnTime);
            }
            break;

        case UANT_APP_DEPLOY_ANT3_OVERRIDE_CC:
            if (UANT_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(UANT_APP_DeployAntCmd_t)))
            {
                const UANT_APP_DeployAntCmd_t *cmd = (const UANT_APP_DeployAntCmd_t *)SBBufPtr;
                ISIS_UANT_DeployAntenna3WithOverride(cmd->BurnTime);
            }
            break;

        case UANT_APP_DEPLOY_ANT4_OVERRIDE_CC:
            if (UANT_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(UANT_APP_DeployAntCmd_t)))
            {
                const UANT_APP_DeployAntCmd_t *cmd = (const UANT_APP_DeployAntCmd_t *)SBBufPtr;
                ISIS_UANT_DeployAntenna4WithOverride(cmd->BurnTime);
            }
            break;

        /* ───────── 전개 취소 ───────── */
        case UANT_APP_CANCEL_DEPLOY_ACT_CC:
            if (UANT_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(CFE_MSG_CommandHeader_t)))
                ISIS_UANT_CancelDeploymentSystemActivation();
            break;

        /* ───────── 상태/센서 조회 ───────── */
        case UANT_APP_GET_STATUS_CC:
            if (UANT_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(CFE_MSG_CommandHeader_t)))
            {
                uint16 status;
                ISIS_UANT_ReportDeploymentStatus(&status);
                CFE_EVS_SendEvent(UANT_APP_DRV_INF_EID, CFE_EVS_EventType_INFORMATION,
                                "ANT-6F deploy status = 0x%04X", status);
            }
            break;

        case UANT_APP_GET_TEMP_CC:
            if (UANT_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(CFE_MSG_CommandHeader_t)))
            {
                uint16 raw;
                ISIS_UANT_MeasureAntennaSystemTemperature(&raw);
                /* 센서 스케일 변환(예: °C) 후 HK 버퍼에 저장하거나 바로 이벤트로 출력 */
                CFE_EVS_SendEvent(UANT_APP_DRV_INF_EID, CFE_EVS_EventType_INFORMATION,
                                "ANT-6F temp raw = %u", raw);
            }
            break;

        /* 카운트 보고 */
        case UANT_APP_REPORT_ANT1_ACTIVATION_CNT_CC:
        case UANT_APP_REPORT_ANT2_ACTIVATION_CNT_CC:
        case UANT_APP_REPORT_ANT3_ACTIVATION_CNT_CC:
        case UANT_APP_REPORT_ANT4_ACTIVATION_CNT_CC:
        {
            if (UANT_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(CFE_MSG_CommandHeader_t)))
            {
                uint8 count;
                uint8 ant = CommandCode - UANT_APP_REPORT_ANT1_ACTIVATION_CNT_CC + 1;
                ISIS_UANT_ReportAntennaActivationCount(ant, &count);
                CFE_EVS_SendEvent(UANT_APP_DRV_INF_EID, CFE_EVS_EventType_INFORMATION,
                                 "ANT-%d activation count = %u", ant, count);
            }
        }
            break;

        /* 활성화 시간 보고 */
        case UANT_APP_REPORT_ANT1_ACTIVATION_TIME_CC:
        case UANT_APP_REPORT_ANT2_ACTIVATION_TIME_CC:
        case UANT_APP_REPORT_ANT3_ACTIVATION_TIME_CC:
        case UANT_APP_REPORT_ANT4_ACTIVATION_TIME_CC:
        {
            if (UANT_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(CFE_MSG_CommandHeader_t)))
            {
                uint16 time;
                uint8 ant = CommandCode - UANT_APP_REPORT_ANT1_ACTIVATION_TIME_CC + 1;
                ISIS_UANT_ReportAntennaActivationTime(ant, &time);
                CFE_EVS_SendEvent(UANT_APP_DRV_INF_EID, CFE_EVS_EventType_INFORMATION,
                                 "ANT-%d activation time = %u", ant, time);
            }
        }
            break;
        /*
        case UANT_APP_PROCESS_CC:
            if (UANT_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(UANT_APP_ProcessCmd_t)))
            {
                UANT_APP_ProcessCmd((const UANT_APP_ProcessCmd_t *)SBBufPtr);
            }
            break;

        case UANT_APP_DISPLAY_PARAM_CC:
            if (UANT_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(UANT_APP_DisplayParamCmd_t)))
            {
                UANT_APP_DisplayParamCmd((const UANT_APP_DisplayParamCmd_t *)SBBufPtr);
            }
            break;
        */
        /* default case already found during FC vs length test */
        default:
            CFE_EVS_SendEvent(UANT_APP_CC_ERR_EID, CFE_EVS_EventType_ERROR, "Invalid ground command code: CC = %d",
                              CommandCode);
            break;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/*  Purpose:                                                                  */
/*     This routine will process any packet that is received on the UANT    */
/*     command pipe.                                                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void UANT_APP_TaskPipe(const CFE_SB_Buffer_t *SBBufPtr)
{
    CFE_SB_MsgId_t MsgId = CFE_SB_INVALID_MSG_ID;

    CFE_MSG_GetMsgId(&SBBufPtr->Msg, &MsgId);

    switch (CFE_SB_MsgIdToValue(MsgId))
    {
        case UANT_APP_CMD_MID:
            UANT_APP_ProcessGroundCommand(SBBufPtr);
            break;

        case UANT_APP_SEND_HK_MID:
            UANT_APP_SendHkCmd((const UANT_APP_SendHkCmd_t *)SBBufPtr);
            break;

        default:
            CFE_EVS_SendEvent(UANT_APP_MID_ERR_EID, CFE_EVS_EventType_ERROR,
                              "UANT: invalid command packet,MID = 0x%x", (unsigned int)CFE_SB_MsgIdToValue(MsgId));
            break;
    }
}
