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
** File: uant_app.h
**
** Purpose:
**   This file is main hdr file for the UANT application.
**
**
*******************************************************************************/

#ifndef _UANT_APP_H_
#define _UANT_APP_H_

/*
** Required header files.
*/
#include <cfe.h>

#include "uant_cc.h"
#include "uant_events.h"
#include "uant_msg.h"
#include "uant_tlm.h"

/************************************************************************
** Type Definitions
*************************************************************************/

typedef struct {
    uint32 RunStatus;
    CFE_SB_PipeId_t CommandPipe;

    HYVRID_AppCount_t AppCnt;
    UANT_Tlm_t ExecReportMsg;

    uint32 EOTaskID;
}UANT_AppData_t;

/****************************************************************************/
/*
** Local function prototypes.
**
** Note: Except for the entry point (UANT_AppMain), these
**       functions are not called from any other source module.
*/
void UANT_AppMain(void);
int32 UANT_AppInit(void);
void UANT_ProcessCommandPacket(CFE_SB_Msg_t* MsgPtr);

void UANT_ProcessGroundCommand(CFE_SB_Msg_t* MsgPtr);
void UANT_ProcessRequestedTelemetry(CFE_SB_Msg_t* MsgPtr);

void UANT_ReportHousekeeping(void);
void UANT_ReportBeacon(void);

#endif /* _uant_app_h_ */
