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
#include <cfe.h>
#include <HYVRID/hyvrid.h>
#include <uant/uant.h>
#include <uantapp/uant_app.h>
#include <string.h>

/*
** global data
*/
UANT_AppData_t UANT_AppData;

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* UANT_AppMain() -- Application entry point and main process loop          */
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
   HYVRID_SetRunStatusOn(HYVRID_RUNSTATUS_UANT);
    while (CFE_ES_RunLoop(&UANT_AppData.RunStatus) == true)
    {
        CFE_ES_PerfLogExit(UANT_PERF_ID);

        status = CFE_SB_RcvMsg(&MsgPtr, UANT_AppData.CommandPipe, CFE_SB_PEND_FOREVER);
        
        CFE_ES_PerfLogEntry(UANT_PERF_ID);

        if (status == CFE_SUCCESS) {
            UANT_ProcessCommandPacket(MsgPtr);
        }
        else {
            CFE_EVS_SendEvent(UANT_PIPE_ERR_EID, CFE_EVS_EventType_ERROR,
                              "UANT APP: SB Pipe Read Error, App Will Exit");
            UANT_AppData.RunStatus = CFE_ES_RunStatus_APP_ERROR;
        }

    }

    CFE_ES_PerfLogExit(UANT_PERF_ID);
    HYVRID_SetRunStatusOff(HYVRID_RUNSTATUS_UANT);
    CFE_ES_ExitApp(UANT_AppData.RunStatus);

    return;
}

int32 UANT_AppInit(void) {
    int32 status;

    /*
    ** Register the app with Executive services
    */
    CFE_ES_RegisterApp() ;

    /**
     * Initialize global AppData.
     */
    UANT_AppData.RunStatus = CFE_ES_RunStatus_APP_RUN;

    /**
     * Initialize event filters.
     */




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
                               HYVRID_PIPE_DEPTH,
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

void UANT_ProcessCommandPacket(CFE_SB_Msg_t* MsgPtr) {
    CFE_SB_MsgId_t MsgId = CFE_SB_GetMsgId(MsgPtr);
    UANT_AppData.ExecReportMsg.Report.MsgId = MsgId;

    switch (MsgId) {
        case UANT_CMD_MID:
            CFE_SB_InitMsg(&UANT_AppData.ExecReportMsg, TO_TLM_RETCODE_MID, sizeof(UANT_AppData.ExecReportMsg), true);
            UANT_ProcessGroundCommand(MsgPtr);
            break;

        case UANT_SNC_MID:
            CFE_SB_InitMsg(&UANT_AppData.ExecReportMsg, SN_TLM_RETCODE_MID, sizeof(UANT_AppData.ExecReportMsg), true);
            UANT_ProcessGroundCommand(MsgPtr);
            break;

        case UANT_OIF_MID:
            CFE_SB_InitMsg(&UANT_AppData.ExecReportMsg, TO_TLM_INFO_MID, sizeof(UANT_AppData.ExecReportMsg), true);
            UANT_ProcessRequestedTelemetry(MsgPtr);
            break;

        case UANT_SIF_MID:
            CFE_SB_InitMsg(&UANT_AppData.ExecReportMsg, SN_TLM_INFO_MID, sizeof(UANT_AppData.ExecReportMsg), true);
            UANT_ProcessRequestedTelemetry(MsgPtr);
            break;

        case UANT_SEND_HK_MID:
            CFE_SB_InitMsg(&UANT_AppData.ExecReportMsg, UANT_TLM_HK_MID, sizeof(UANT_AppData.ExecReportMsg), true);
            UANT_ReportHousekeeping();
            break;

        case UANT_SEND_BCN_MID:
            CFE_SB_InitMsg(&UANT_AppData.ExecReportMsg, UANT_TLM_BCN_MID, sizeof(UANT_AppData.ExecReportMsg), true);
            UANT_ReportBeacon();
            break;

        default:
            CFE_EVS_SendEvent(UANT_COMMAND_ERR_EID, CFE_EVS_EventType_ERROR,
			                  "UANT: invalid command packet,MID = 0x%x", MsgId);
            return;
    }

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&UANT_AppData.ExecReportMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*) &UANT_AppData.ExecReportMsg);

    return;
}

void UANT_ProcessGroundCommand(CFE_SB_Msg_t* MsgPtr) {
    int         status = CFE_STATUS_WRONG_MSG_LENGTH; /* Assume VerifyCmdLength error. */
    uint8       statusType = CMD_RETCODE_TYPE_BADCMD;
    uint16_t    CommandCode = CFE_SB_GetCmdCode(MsgPtr);
    UANT_AppData.ExecReportMsg.Report.CommandCode = CommandCode;
    UANT_Tlm_t* ReportMsg = &UANT_AppData.ExecReportMsg;

    switch (CommandCode) {
    case UANT_NOOP_CC:
        if (VerifyCmdLength(MsgPtr, sizeof(UANT_NoopCmd_t))) {
            CFE_EVS_SendEvent(UANT_COMMANDNOP_INF_EID, CFE_EVS_INFORMATION,
                                "UANT: NOOP command");
            status = CFE_SUCCESS;
            statusType = CMD_RETCODE_TYPE_APP;
        }
        break;

    case UANT_RESET_COUNTERS_CC:
        if (VerifyCmdLength(MsgPtr, sizeof(UANT_ResetCountersCmd_t))) {
            ResetCounters(&UANT_AppData.AppCnt);
            status = CFE_SUCCESS;
            statusType = CMD_RETCODE_TYPE_APP;
        }
        break;

    case UANT_RESET_APP_CMD_COUNTERS_CC:
        if (VerifyCmdLength(MsgPtr, sizeof(UANT_ResetAppCmdCountersCmd_t))) {
            ResetAppCmdCounters(&UANT_AppData.AppCnt);
            status = CFE_SUCCESS;
            statusType = CMD_RETCODE_TYPE_APP;
        }
        break;

    case UANT_RESET_DEVICE_CMD_COUNTERS_CC:
        if (VerifyCmdLength(MsgPtr, sizeof(UANT_ResetDeviceCmdCountersCmd_t))) {
            ResetDeviceCmdCounters(&UANT_AppData.AppCnt);
            status = CFE_SUCCESS;
            statusType = CMD_RETCODE_TYPE_APP;
        }
        break;

    case UANT_DEVICE_INIT_CC:
        if (VerifyCmdLength(MsgPtr, sizeof(UANT_DeviceInitCmd_t))) {
            status = isis_uant_i2c_init();
            status = CFE_SUCCESS;
            statusType = CMD_RETCODE_TYPE_APP;
        }
        break;

    case UANT_RESET_CC:
        COMMAND_CASE_HANDLING_NOARG(CMD_RETCODE_TYPE_DRIVER,
                                    UANT_ISIS_ResetCmd_t,
                                    ISIS_UANT_Reset);
        break;


    case UANT_ARM_ANTENNA_SYSTEMS_CC:
        COMMAND_CASE_HANDLING_NOARG(CMD_RETCODE_TYPE_DRIVER,
                                    UANT_ISIS_ArmAntennaSystemsCmd_t,
                                    ISIS_UANT_ArmAntennaSystems);
        break;

    case UANT_DISARM_CC: 
        COMMAND_CASE_HANDLING_NOARG(CMD_RETCODE_TYPE_DRIVER,
                                    UANT_ISIS_DisarmCmd_t,
                                    ISIS_UANT_Disarm);
        break;

    case UANT_AUTOMATED_DEPLOYMENT_CC:
        COMMAND_CASE_HANDLING_ARG(CMD_RETCODE_TYPE_DRIVER,
                                    UANT_ISIS_AutomatedDeploymentCmd_t,
                                    ISIS_UANT_AutomatedSequentialDeployment,
                                    arg);
        break;

    case UANT_CANCEL_DEPLOYMENT_ACTIVATION_CC:
        COMMAND_CASE_HANDLING_NOARG(CMD_RETCODE_TYPE_DRIVER,
                                    UANT_ISIS_CancelDeploymentActivationCmd_t,
                                    ISIS_UANT_CancelDeploymentSystemActivation);
        break;

    case UANT_DEPLOY_ANT1_CC: 
        COMMAND_CASE_HANDLING_ARG(CMD_RETCODE_TYPE_DRIVER,
                                    UANT_ISIS_DeployAnt1Cmd_t,
                                    ISIS_UANT_DeployAntenna1,
                                    arg);
        break;

    case UANT_DEPLOY_ANT2_CC:
        COMMAND_CASE_HANDLING_ARG(CMD_RETCODE_TYPE_DRIVER,
                                    UANT_ISIS_DeployAnt2Cmd_t,
                                    ISIS_UANT_DeployAntenna2,
                                    arg);
        break;

    case UANT_DEPLOY_ANT3_CC:
        COMMAND_CASE_HANDLING_ARG(CMD_RETCODE_TYPE_DRIVER,
                                    UANT_ISIS_DeployAnt3Cmd_t,
                                    ISIS_UANT_DeployAntenna3,
                                    arg);
        break;

    case UANT_DEPLOY_ANT4_CC:
        COMMAND_CASE_HANDLING_ARG(CMD_RETCODE_TYPE_DRIVER,
                                    UANT_ISIS_DeployAnt4Cmd_t,
                                    ISIS_UANT_DeployAntenna4,
                                    arg);
        break;

    case UANT_DEPLOY_ANT1_OVERRIDE_CC:
        COMMAND_CASE_HANDLING_ARG(CMD_RETCODE_TYPE_DRIVER,
                                    UANT_ISIS_DeployAnt1OverrideCmd_t,
                                    ISIS_UANT_DeployAntenna1WithOverride,
                                    arg);
        break;

    case UANT_DEPLOY_ANT2_OVERRIDE_CC:
        COMMAND_CASE_HANDLING_ARG(CMD_RETCODE_TYPE_DRIVER,
                                    UANT_ISIS_DeployAnt2OverrideCmd_t,
                                    ISIS_UANT_DeployAntenna2WithOverride,
                                    arg);
        break;

    case UANT_DEPLOY_ANT3_OVERRIDE_CC:
        COMMAND_CASE_HANDLING_ARG(CMD_RETCODE_TYPE_DRIVER,
                                    UANT_ISIS_DeployAnt3OverrideCmd_t,
                                    ISIS_UANT_DeployAntenna3WithOverride,
                                    arg);
        break;

    case UANT_DEPLOY_ANT4_OVERRIDE_CC:
        COMMAND_CASE_HANDLING_ARG(CMD_RETCODE_TYPE_DRIVER,
                                    UANT_ISIS_DeployAnt4OverrideCmd_t,
                                    ISIS_UANT_DeployAntenna4WithOverride,
                                    arg);
        break;

    default: {
        /**
         * Invalid CC.
         */
        status = CFE_STATUS_BAD_COMMAND_CODE;
        statusType = CMD_RETCODE_TYPE_BADCMD;
        break;
    }
    }

    ReportMsg->Report.DataSize = 0;
    ReportMsg->Report.RetCode = status;
    ReportMsg->Report.RetCodeType = statusType;

    CmdErrCounter(&UANT_AppData.AppCnt, &ReportMsg->Report);

    if (status != CFE_SUCCESS || status != DEVICE_SUCCESS) {
        CFE_EVS_SendEvent(UANT_CMD_ERR_EID, CFE_EVS_EventType_ERROR,
                          "UANT App: cc %d err: status %X:%d",
                          CommandCode, statusType, status);
    }

    return;
}

void UANT_ProcessRequestedTelemetry(CFE_SB_Msg_t* MsgPtr){
    int         status = CFE_STATUS_WRONG_MSG_LENGTH; /* Assume VerifyCmdLength error. */
    uint8       statusType = CMD_RETCODE_TYPE_BADCMD;
    uint16_t    CommandCode = CFE_SB_GetCmdCode(MsgPtr);
    UANT_AppData.ExecReportMsg.Report.CommandCode = CommandCode;
    UANT_Tlm_t* ReportMsg = &UANT_AppData.ExecReportMsg;

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
#ifdef HYVRID_LOG_ON
    CFE_ES_WriteToSysLog("UANT App: Housekeeping Collection");
#endif
#ifdef HYVRID_PLATFORM_CPU1
    memset(&UANT_AppData.ExecReportMsg.Data.hk, HYVRID_RUNSTATUS_UANT, sizeof(UANT_HK_t));
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
#ifdef HYVRID_LOG_ON
    CFE_ES_WriteToSysLog("UANT App: Beacon Collection");
#endif
#ifdef HYVRID_PLATFORM_CPU1
    memset(&UANT_AppData.ExecReportMsg.Data.bcn, HYVRID_RUNSTATUS_UANT, sizeof(UANT_BCN_t));
    return;
#endif
    int32 status;
    UANT_AppData.ExecReportMsg.Report.DataSize = sizeof(UANT_HK_t);
    UANT_BCN_t* bcn = &UANT_AppData.ExecReportMsg.Data.bcn;
    status |= ISIS_UANT_ReportDeploymentStatus(&bcn->DeployState);

    return;
}