/*******************************************************************************
**
**      GSC-18128-1, "Core Flight Executive Version 6.6"
**
**      Copyright (c) 2006-2019 United States Government as represented by
**      the Administrator of the National Aeronautics and Space Administration.
**      All Rights Reserved.
**
**      Licensed under the Apache License, Version 2.0 (the "License");
**      you may not use this file except in compliance with the License.
**      You may obtain a copy of the License at
**
**        http://www.apache.org/licenses/LICENSE-2.0
**
**      Unless required by applicable law or agreed to in writing, software
**      distributed under the License is distributed on an "AS IS" BASIS,
**      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**      See the License for the specific language governing permissions and
**      limitations under the License.
**
** File: uant_app.c
**
** Purpose:
**   This file contains the source code for the UANT App.
**
*******************************************************************************/

/*
**   Include Files:
*/
#include <cfe.h> //빌드하면 osconfig.h 생겨서 괜찮음음
#include <COSMIC/cosmic.h>
#include <uant/uant.h>
#include <uantapp/uant_app.h> 
#include <string.h>

#include "cfe_srl_api.h"
#include "cfe_srl_handle.h"
#include "cfe_sb.h"
#include "cfe_sb.h"            /* CFE_SB_MsgId_t, CFE_SB_TransmitMsg */
#include "cfe_msg.h"           /* CFE_MSG_GetMsgId, CFE_MSG_Init, CFE_MSG_SetMsgTimeStamp */
#include "cfe_time.h"          /* CFE_TIME_GetTime */
#include "cfe_evs.h"

/*
** global data
*/
UANT_AppData_t UANT_AppData;
CFE_ES_MAIN(UANT_AppMain); //app register, produce task ...
/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* UANT_AppMain() -- Application entry point and main process Ploop          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void UANT_AppMain( void )
{
    int32  status;
    CFE_SB_Msg_t * MsgPtr;

    CFE_ES_PerfLogEntry(UANT_PERF_ID);

    status = UANT_AppInit();
    if (status != CFE_SUCCESS) {
        UANT_AppData.RunStatus = CFE_ES_RunStatus_APP_ERROR;
    }

    status = isis_uant_i2c_init();
    if (status != CFE_SUCCESS) {
        UANT_AppData.RunStatus = CFE_ES_RunStatus_APP_ERROR;
    }

    /*
    ** UANT Runloop
    */
   COSMIC_SetRunStatusOn(COSMIC_RUNSTATUS_UANT);
    while (CFE_ES_RunLoop(&UANT_AppData.RunStatus) == true)
    {
        CFE_ES_PerfLogExit(UANT_PERF_ID);
        CFE_SB_Buffer_t *BufPtr = NULL;
        status = CFE_SB_ReceiveBuffer(&BufPtr, UANT_AppData.CommandPipe, CFE_SB_PEND_FOREVER);
        
        CFE_ES_PerfLogEntry(UANT_PERF_ID);

        if (status == CFE_SUCCESS && &BufPtr != NULL) {
            UANT_ProcessCommandPacket((Bufptr);

            CFE_SB_ReleaseMessageBuffer(BufPtr);
        }
        else 
        {
            CFE_EVS_SendEvent(UANT_PIPE_ERR_EID, CFE_EVS_EventType_ERROR,
                              "UANT APP: SB Pipe Read Error, App Will Exit");
            UANT_AppData.RunStatus = CFE_ES_RunStatus_APP_ERROR;
        }

    }

    CFE_ES_PerfLogExit(UANT_PERF_ID);
    COSMIC_SetRunStatusOff(COSMIC_RUNSTATUS_UANT);
    CFE_ES_ExitApp(UANT_AppData.RunStatus);

    return;
}

int32 UANT_AppInit(void) {
    int32 status = CFE_SUCCESS;
    
    /**
     * Initialize global AppData.
     */
    UANT_AppData.RunStatus = CFE_ES_RunStatus_APP_RUN;


    /*
    ** Register the events
    */ 
    status = CFE_EVS_Register(NULL, 0, CFE_EVS_NO_FILTER);
    if (status != CFE_SUCCESS) {
        CFE_EVS_SendEvent(UANT_PIPE_ERR_EID, CFE_EVS_EventType_ERROR,
                            "UANT App: Error Registering Events, RC = 0x%08lX\n", (unsigned long)status);
        return status;
    }

    /*
    ** Create the Software Bus command pipe and subscribe to housekeeping
    **  messages
    */
    status = CFE_SB_CreatePipe(&UANT_AppData.CommandPipe,
                               COSMIC_PIPE_DEPTH,
                               "UANT_CMD_PIPE");
    if (status != CFE_SUCCESS) {
        CFE_EVS_SendEvent(UANT_PIPE_ERR_EID, CFE_EVS_EventType_ERROR,
                            "UANT App: Error creating pipe, RC = 0x%08lX\n", (unsigned long)status);
        return status;
    }

    status = CFE_SB_Subscribe(UANT_CMD_MID, UANT_AppData.CommandPipe);
    if (status != CFE_SUCCESS) {
        CFE_ES_WriteToSysLog("UANT App: Error Subscribing to Command, RC = 0x%08lX\n", (unsigned long)status);
        return status;
    }
    status = CFE_SB_Subscribe(UANT_SNC_MID, UANT_AppData.CommandPipe);
    if (status != CFE_SUCCESS) {
        CFE_ES_WriteToSysLog("UANT App: Error Subscribing to Command, RC = 0x%08lX\n", (unsigned long)status);
        return status;
    }
    status = CFE_SB_Subscribe(UANT_OIF_MID, UANT_AppData.CommandPipe);
    if (status != CFE_SUCCESS) {
        CFE_ES_WriteToSysLog("UANT App: Error Subscribing to Command, RC = 0x%08lX\n", (unsigned long)status);
        return status;
    }
    status = CFE_SB_Subscribe(UANT_SIF_MID, UANT_AppData.CommandPipe);
    if (status != CFE_SUCCESS) {
        CFE_ES_WriteToSysLog("UANT App: Error Subscribing to Command, RC = 0x%08lX\n", (unsigned long)status);
        return status;
    }

    status = CFE_SB_Subscribe(UANT_SEND_HK_MID, UANT_AppData.CommandPipe);
    if (status != CFE_SUCCESS) {
        CFE_ES_WriteToSysLog("UANT App: Error Subscribing to HK request, RC = 0x%08lX\n", (unsigned long)status);
        return status;
    }

    status = CFE_SB_Subscribe(UANT_SEND_BCN_MID, UANT_AppData.CommandPipe);
    if (status != CFE_SUCCESS) {
        CFE_EVS_SendEvent(UANT_PIPE_ERR_EID, CFE_EVS_EventType_ERROR,
                            "UANT App: Error Subscribing to Beacon Data Request, RC = 0x%08lX", (unsigned long)status);
        return status;
    }


    ResetCounters(&UANT_AppData.AppCnt);

    CFE_EVS_SendEvent(UANT_STARTUP_INF_EID, CFE_EVS_EventType_INFORMATION,
                      "UANT App Initialized."); //TODO: ADD VERSION LOG

	return CFE_SUCCESS;
}

void UANT_ProcessCommandPacket(CFE_SB_Buffer_t *BufPtr)
{
    CFE_Status_t         status;
    CFE_SB_MsgId_t       MsgId;
    CFE_MSG_Message_t   *InMsg  = &BufPtr->Msg;                        /* 수신된 Raw SB 메시지 헤더 */
    CFE_MSG_Message_t   *OutMsg = &UANT_AppData.ExecReportMsg.Msg;     /* 응답용 ExecReportMsg.Msg */

    /* ----------------------------------------------------
     * 1) 수신된 메시지에서 MsgId만 추출
     * ---------------------------------------------------- */
    MsgId = CFE_MSG_GetMsgId(InMsg);
    UANT_AppData.ExecReportMsg.Report.MsgId = MsgId;

    /* ----------------------------------------------------
     * 2) MsgId별 분기 처리
     * ---------------------------------------------------- */
    switch (MsgId)
    {
        case UANT_CMD_MID:
            /* ▶ 응답 메시지 헤더 초기화 (이전 CFE_SB_InitMsg 대체) */
            status = CFE_MSG_Init(
                         OutMsg,
                         TO_TLM_RETCODE_MID,
                         sizeof(UANT_AppData.ExecReportMsg),
                         true /* 모두 0으로 초기화 */
                     );
            if (status != CFE_SUCCESS)
            {
                CFE_EVS_SendEvent(
                    UANT_INIT_ERR_EID,
                    CFE_EVS_EventType_ERROR,
                    "UANT: CFE_MSG_Init(CMD_RET) failed 0x%08lX",
                    (unsigned long)status
                );
                return;
            }
            /* ▶ 원래 SB 메시지를 파싱해 실제 커맨드를 처리하던 함수 호출 */
            UANT_ProcessGroundCommand(InMsg);
            break;

        case UANT_SNC_MID:
            status = CFE_MSG_Init(
                         OutMsg,
                         SN_TLM_RETCODE_MID,
                         sizeof(UANT_AppData.ExecReportMsg),
                         true
                     );
            if (status != CFE_SUCCESS)
            {
                CFE_EVS_SendEvent(
                    UANT_INIT_ERR_EID,
                    CFE_EVS_EventType_ERROR,
                    "UANT: CFE_MSG_Init(SNC_RET) failed 0x%08lX",
                    (unsigned long)status
                );
                return;
            }
            UANT_ProcessGroundCommand(InMsg);
            break;

        case UANT_OIF_MID:
            status = CFE_MSG_Init(
                         OutMsg,
                         TO_TLM_INFO_MID,
                         sizeof(UANT_AppData.ExecReportMsg),
                         true
                     );
            if (status != CFE_SUCCESS)
            {
                CFE_EVS_SendEvent(
                    UANT_INIT_ERR_EID,
                    CFE_EVS_EventType_ERROR,
                    "UANT: CFE_MSG_Init(OIF_INFO) failed 0x%08lX",
                    (unsigned long)status
                );
                return;
            }
            UANT_ProcessRequestedTelemetry(InMsg);
            break;

        case UANT_SIF_MID:
            status = CFE_MSG_Init(
                         OutMsg,
                         SN_TLM_INFO_MID,
                         sizeof(UANT_AppData.ExecReportMsg),
                         true
                     );
            if (status != CFE_SUCCESS)
            {
                CFE_EVS_SendEvent(
                    UANT_INIT_ERR_EID,
                    CFE_EVS_EventType_ERROR,
                    "UANT: CFE_MSG_Init(SIF_INFO) failed 0x%08lX",
                    (unsigned long)status
                );
                return;
            }
            UANT_ProcessRequestedTelemetry(InMsg);
            break;

        case UANT_SEND_HK_MID:
            status = CFE_MSG_Init(
                         OutMsg,
                         UANT_TLM_HK_MID,
                         sizeof(UANT_AppData.ExecReportMsg),
                         true
                     );
            if (status != CFE_SUCCESS)
            {
                CFE_EVS_SendEvent(
                    UANT_INIT_ERR_EID,
                    CFE_EVS_EventType_ERROR,
                    "UANT: CFE_MSG_Init(HK_TLM) failed 0x%08lX",
                    (unsigned long)status
                );
                return;
            }
            UANT_ReportHousekeeping();
            break;

        case UANT_SEND_BCN_MID:
            status = CFE_MSG_Init(
                         OutMsg,
                         UANT_TLM_BCN_MID,
                         sizeof(UANT_AppData.ExecReportMsg),
                         true
                     );
            if (status != CFE_SUCCESS)
            {
                CFE_EVS_SendEvent(
                    UANT_INIT_ERR_EID,
                    CFE_EVS_EventType_ERROR,
                    "UANT: CFE_MSG_Init(BCN_TLM) failed 0x%08lX",
                    (unsigned long)status
                );
                return;
            }
            UANT_ReportBeacon();
            break;

        default:
            /* 잘못된 MsgId인 경우 이벤트만 발생시키고 함수 종료 */
            CFE_EVS_SendEvent(
                UANT_COMMAND_ERR_EID,
                CFE_EVS_EventType_ERROR,
                "UANT: Invalid command MsgId = 0x%08X",
                (unsigned int)MsgId
            );
            return;
    }

    /* ----------------------------------------------------
     * 3) 타임스탬프 설정 및 전송
     *    - 과거 CFE_SB_TimeStampMsg → 최신 CFE_MSG_SetMsgTimeStamp
     *    - 과거 CFE_SB_SendMsg     → 최신 CFE_SB_TransmitMsg
     * ---------------------------------------------------- */
    CFE_MSG_SetMsgTimeStamp(OutMsg, CFE_TIME_GetTime());

    status = CFE_SB_TransmitMsg(OutMsg, true /* originating */);
    if (status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(
            UANT_SEND_ERR_EID,
            CFE_EVS_EventType_ERROR,
            "UANT: CFE_SB_TransmitMsg failed 0x%08lX",
            (unsigned long)status
        );
    }
}


void UANT_ProcessGroundCommand(CFE_MSG_Message_t *MsgPtr)
{
    int32        status      = CFE_STATUS_WRONG_MSG_LENGTH; /* 기본값: 길이 오류 */
    uint8        statusType  = CMD_RETCODE_TYPE_BADCMD;      /* 기본값: 잘못된 CC */
    CFE_MSG_FcnCode_t CommandCode = 0;
    UANT_Tlm_t  *ReportMsg   = &UANT_AppData.ExecReportMsg;

    /* 1) 함수 코드(Function Code) 추출 */
    if (CFE_MSG_GetFcnCode(MsgPtr, &CommandCode) != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(UANT_CMD_ERR_EID, CFE_EVS_EventType_ERROR,
                          "UANT: CFE_MSG_GetFcnCode failed");
        return;
    }
    ReportMsg->Report.CommandCode = (uint16_t)CommandCode;

    /* 2) CC별 처리 */
    switch (CommandCode)
    {
        case UANT_NOOP_CC:
            if (VerifyCmdLength(MsgPtr, sizeof(UANT_NoopCmd_t)))
            {
                CFE_EVS_SendEvent(UANT_COMMANDNOP_INF_EID, CFE_EVS_EventType_INFORMATION,
                                  "UANT: NOOP command");
                status     = CFE_SUCCESS;
                statusType = CMD_RETCODE_TYPE_APP;
            }
            break;

        case UANT_RESET_COUNTERS_CC:
            if (VerifyCmdLength(MsgPtr, sizeof(UANT_ResetCountersCmd_t)))
            {
                ResetCounters(&UANT_AppData.AppCnt);
                status     = CFE_SUCCESS;
                statusType = CMD_RETCODE_TYPE_APP;
            }
            break;

        case UANT_RESET_APP_CMD_COUNTERS_CC:
            if (VerifyCmdLength(MsgPtr, sizeof(UANT_ResetAppCmdCountersCmd_t)))
            {
                ResetAppCmdCounters(&UANT_AppData.AppCnt);
                status     = CFE_SUCCESS;
                statusType = CMD_RETCODE_TYPE_APP;
            }
            break;

        case UANT_RESET_DEVICE_CMD_COUNTERS_CC:
            if (VerifyCmdLength(MsgPtr, sizeof(UANT_ResetDeviceCmdCountersCmd_t)))
            {
                ResetDeviceCmdCounters(&UANT_AppData.AppCnt);
                status     = CFE_SUCCESS;
                statusType = CMD_RETCODE_TYPE_APP;
            }
            break;

        case UANT_DEVICE_INIT_CC:
            if (VerifyCmdLength(MsgPtr, sizeof(UANT_DeviceInitCmd_t)))
            {
                status       = isis_uant_i2c_init();
                status       = CFE_SUCCESS; /* 초기화 성공 가정 */
                statusType   = CMD_RETCODE_TYPE_APP;
            }
            break;

        case UANT_RESET_CC:
            COMMAND_CASE_HANDLING_NOARG(
                CMD_RETCODE_TYPE_DRIVER,
                UANT_ISIS_ResetCmd_t,
                ISIS_UANT_Reset
            );
            break;

        case UANT_ARM_ANTENNA_SYSTEMS_CC:
            COMMAND_CASE_HANDLING_NOARG(
                CMD_RETCODE_TYPE_DRIVER,
                UANT_ISIS_ArmAntennaSystemsCmd_t,
                ISIS_UANT_ArmAntennaSystems
            );
            break;

        case UANT_DISARM_CC:
            COMMAND_CASE_HANDLING_NOARG(
                CMD_RETCODE_TYPE_DRIVER,
                UANT_ISIS_DisarmCmd_t,
                ISIS_UANT_Disarm
            );
            break;

        case UANT_AUTOMATED_DEPLOYMENT_CC:
            COMMAND_CASE_HANDLING_ARG(
                CMD_RETCODE_TYPE_DRIVER,
                UANT_ISIS_AutomatedDeploymentCmd_t,
                ISIS_UANT_AutomatedSequentialDeployment,
                arg
            );
            break;

        case UANT_CANCEL_DEPLOYMENT_ACTIVATION_CC:
            COMMAND_CASE_HANDLING_NOARG(
                CMD_RETCODE_TYPE_DRIVER,
                UANT_ISIS_CancelDeploymentActivationCmd_t,
                ISIS_UANT_CancelDeploymentSystemActivation
            );
            break;

        case UANT_DEPLOY_ANT1_CC:
            COMMAND_CASE_HANDLING_ARG(
                CMD_RETCODE_TYPE_DRIVER,
                UANT_ISIS_DeployAnt1Cmd_t,
                ISIS_UANT_DeployAntenna1,
                arg
            );
            break;

        case UANT_DEPLOY_ANT2_CC:
            COMMAND_CASE_HANDLING_ARG(
                CMD_RETCODE_TYPE_DRIVER,
                UANT_ISIS_DeployAnt2Cmd_t,
                ISIS_UANT_DeployAntenna2,
                arg
            );
            break;

        case UANT_DEPLOY_ANT3_CC:
            COMMAND_CASE_HANDLING_ARG(
                CMD_RETCODE_TYPE_DRIVER,
                UANT_ISIS_DeployAnt3Cmd_t,
                ISIS_UANT_DeployAntenna3,
                arg
            );
            break;

        case UANT_DEPLOY_ANT4_CC:
            COMMAND_CASE_HANDLING_ARG(
                CMD_RETCODE_TYPE_DRIVER,
                UANT_ISIS_DeployAnt4Cmd_t,
                ISIS_UANT_DeployAntenna4,
                arg
            );
            break;

        case UANT_DEPLOY_ANT1_OVERRIDE_CC:
            COMMAND_CASE_HANDLING_ARG(
                CMD_RETCODE_TYPE_DRIVER,
                UANT_ISIS_DeployAnt1OverrideCmd_t,
                ISIS_UANT_DeployAntenna1WithOverride,
                arg
            );
            break;

        case UANT_DEPLOY_ANT2_OVERRIDE_CC:
            COMMAND_CASE_HANDLING_ARG(
                CMD_RETCODE_TYPE_DRIVER,
                UANT_ISIS_DeployAnt2OverrideCmd_t,
                ISIS_UANT_DeployAntenna2WithOverride,
                arg
            );
            break;

        case UANT_DEPLOY_ANT3_OVERRIDE_CC:
            COMMAND_CASE_HANDLING_ARG(
                CMD_RETCODE_TYPE_DRIVER,
                UANT_ISIS_DeployAnt3OverrideCmd_t,
                ISIS_UANT_DeployAntenna3WithOverride,
                arg
            );
            break;

        case UANT_DEPLOY_ANT4_OVERRIDE_CC:
            COMMAND_CASE_HANDLING_ARG(
                CMD_RETCODE_TYPE_DRIVER,
                UANT_ISIS_DeployAnt4OverrideCmd_t,
                ISIS_UANT_DeployAntenna4WithOverride,
                arg
            );
            break;

        default:
            /* 유효하지 않은 CC */
            status     = CFE_STATUS_BAD_COMMAND_CODE;
            statusType = CMD_RETCODE_TYPE_BADCMD;
            break;
    }

    /* 3) ExecReportMsg에 결과 기록 */
    ReportMsg->Report.DataSize    = 0;
    ReportMsg->Report.RetCode     = status;
    ReportMsg->Report.RetCodeType = statusType;

    /* 명령/장치 에러 카운터 업데이트 */
    CmdErrCounter(&UANT_AppData.AppCnt, &ReportMsg->Report);

    /* 오류 발생 시 이벤트 전송 */
    if (status != CFE_SUCCESS && status != DEVICE_SUCCESS)
    {
        CFE_EVS_SendEvent(UANT_CMD_ERR_EID, CFE_EVS_EventType_ERROR,
                          "UANT App: CC %u ERR (Type=%u, Code=%ld)",
                          (unsigned)CommandCode,
                          (unsigned)statusType,
                          (long)status);
    }
}


void UANT_ProcessRequestedTelemetry(CFE_MSG_Message_t *MsgPtr){
    int32        status      = CFE_STATUS_WRONG_MSG_LENGTH; /* 기본값: 길이 오류 */
    uint8        statusType  = CMD_RETCODE_TYPE_BADCMD;      /* 기본값: 잘못된 CC */
    CFE_MSG_FcnCode_t CommandCode = 0;
    UANT_Tlm_t  *ReportMsg   = &UANT_AppData.ExecReportMsg;

    /* 1) 함수 코드(Function Code) 추출 */
    if (CFE_MSG_GetFcnCode(MsgPtr, &CommandCode) != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(UANT_CMD_ERR_EID, CFE_EVS_EventType_ERROR,
                          "UANT: CFE_MSG_GetFcnCode failed");
        return;
    }
    ReportMsg->Report.CommandCode = (uint16_t)CommandCode; // 텔레메트리 보고서에 기록용용

    switch (CommandCode) {
    case UANT_GET_DEPLOYMENT_STATUS_CC:
        TELEMETRY_CASE_HANDLING_NOARG(CMD_RETCODE_TYPE_DRIVER,
                                UANT_ISIS_ReportDeploymentStatusCmd_t,
                                ISIS_UANT_ReportDeploymentStatus,
                                uint16);
        break;

    case UANT_REPORT_ANT_ACTIVATION_CNT_CC:
        TELEMETRY_CASE_HANDLING_ARG(CMD_RETCODE_TYPE_DRIVER,
                                    UANT_ISIS_ReportAntActivationTimeCmd_t,
                                    ISIS_UANT_ReportAntennaActivationCount,
                                    arg,
                                    uint8);
        break;

    case UANT_REPORT_ANT_ACTIVATION_TIME_CC:
        TELEMETRY_CASE_HANDLING_ARG(CMD_RETCODE_TYPE_DRIVER,
                                    UANT_ISIS_ReportAntActivationTimeCmd_t,
                                    ISIS_UANT_ReportAntennaActivationTime,
                                    arg,
                                    uint16);
        break;

    case UANT_MEASURE_SYSTEM_TEMPERATURE_CC:
        TELEMETRY_CASE_HANDLING_NOARG(CMD_RETCODE_TYPE_DRIVER,
                                UANT_ISIS_MeasureSystemTemperatureCmd_t,
                                ISIS_UANT_MeasureAntennaSystemTemperature,
                                uint16);
        break;


    default:
        status = CFE_STATUS_BAD_COMMAND_CODE;
        statusType = CMD_RETCODE_TYPE_BADCMD;
        break;  
    }

    ReportMsg->Report.RetCode = status;
    ReportMsg->Report.RetCodeType = statusType;

    CmdErrCounter(&UANT_AppData.AppCnt, &ReportMsg->Report);
    if (status != DEVICE_SUCCESS || status != CFE_SUCCESS) {
        CFE_EVS_SendEvent(UANT_CMD_ERR_EID, CFE_EVS_EventType_ERROR,
                          "UANT App: cc %d err: status %X:%d",
                          CommandCode, statusType, status);
    }
    return;
}

// TODO: implement default housekeeping or delete this.
void UANT_ReportHousekeeping(void)
{
#ifdef COSMIC_LOG_ON
    CFE_ES_WriteToSysLog("UANT App: Housekeeping Collection");
#endif
#ifdef COSMIC_PLATFORM_CPU1
    memset(&UANT_AppData.ExecReportMsg.Data.hk, COSMIC_RUNSTATUS_UANT, sizeof(UANT_HK_t));
    return;
#endif
    int32 status;
    UANT_AppData.ExecReportMsg.Report.DataSize = sizeof(UANT_HK_t);
    UANT_HK_t* hk = &UANT_AppData.ExecReportMsg.Data.hk;
    status |= ISIS_UANT_ReportDeploymentStatus(&hk->DeployState);


    return;
}

void UANT_ReportBeacon(void)
{
#ifdef COSMIC_LOG_ON
    CFE_ES_WriteToSysLog("UANT App: Beacon Collection");
#endif
#ifdef COSMIC_PLATFORM_CPU1
    memset(&UANT_AppData.ExecReportMsg.Data.bcn, COSMIC_RUNSTATUS_UANT, sizeof(UANT_BCN_t));
    return;
#endif
    int32 status;
    UANT_AppData.ExecReportMsg.Report.DataSize = sizeof(UANT_BCN_t);
    UANT_BCN_t* bcn = &UANT_AppData.ExecReportMsg.Data.bcn;
    status |= ISIS_UANT_ReportDeploymentStatus(&bcn->DeployState);

    return;
}