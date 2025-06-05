#include <cfe.h>
#include <COSMIC/cosmic.h>


bool VerifyCmdLength(const CFE_MSG_Message_t *MsgPtr, CFE_MSG_Size_t ExpectedLength)
{   

    CFE_MSG_Size_t ActualSize;
     if (CFE_MSG_GetSize(MsgPtr, &ActualSize) != CFE_SUCCESS) {
        return false;
    }
    return (ActualSize == ExpectedLength);
}


void ResetAppCmdCounters(COSMIC_AppCount_t * AppCnt) {
    AppCnt->AppCmdCounter = 0;
    AppCnt->AppErrCounter = 0;
    return;
}

void ResetDeviceCmdCounters(COSMIC_AppCount_t * AppCnt) {
    AppCnt->DeviceCmdCounter = 0;
    AppCnt->DeviceErrCounter = 0;
    return;
}

void ResetCounters(COSMIC_AppCount_t * AppCnt) {
    ResetAppCmdCounters(AppCnt);
    ResetDeviceCmdCounters(AppCnt);
    return;
}

void CmdErrCounter(COSMIC_AppCount_t * AppCnt, COSMIC_CmdExecutionReportMsg_t * ExecMsg) {
    if(!AppCnt)
        return;
    if (ExecMsg->RetCodeType == CFE_STATUS_BAD_COMMAND_CODE)
        return;
    /**
     * Increment the command counters.
     */
    if (ExecMsg->RetCodeType != CMD_RETCODE_TYPE_DRIVER) {
        /**
         * This was an Application-level command...
         */
        AppCnt->AppCmdCounter++;
        if (ExecMsg->RetCode != CFE_SUCCESS) {
            AppCnt->AppErrCounter++;
        }
    }
    else {
        /**
         * or else one for the hardware.
         */
        AppCnt->DeviceCmdCounter++;
        if (ExecMsg->RetCode != DEVICE_SUCCESS) {
            AppCnt->DeviceErrCounter++;
        }
    }
    return;
}