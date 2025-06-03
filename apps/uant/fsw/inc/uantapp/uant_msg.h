/*******************************************************************************
** File:
**   uant_app_msg.h 
**
** Purpose: 
**  Define UANT App  Messages and info
**
** Notes:
**
**
*******************************************************************************/
#ifndef _UANT_MSG_H_
#define _UANT_MSG_H_

#include <cfe.h>
#include <HYVRID/hyvrid.h>

typedef HYVRID_NoArgsCmd_t    UANT_NoopCmd_t;
typedef HYVRID_NoArgsCmd_t    UANT_ResetCountersCmd_t;
typedef HYVRID_NoArgsCmd_t    UANT_ResetAppCmdCountersCmd_t;
typedef HYVRID_NoArgsCmd_t    UANT_ResetDeviceCmdCountersCmd_t;
typedef HYVRID_NoArgsCmd_t    UANT_DeviceInitCmd_t;

typedef HYVRID_NoArgsCmd_t    UANT_ISIS_ResetCmd_t;
typedef HYVRID_NoArgsCmd_t    UANT_ISIS_ReportDeploymentStatusCmd_t;
typedef HYVRID_NoArgsCmd_t    UANT_ISIS_ArmAntennaSystemsCmd_t;
typedef HYVRID_NoArgsCmd_t    UANT_ISIS_DisarmCmd_t;
typedef HYVRID_U8ArgsCmd_t    UANT_ISIS_AutomatedDeploymentCmd_t;
typedef HYVRID_NoArgsCmd_t    UANT_ISIS_CancelDeploymentActivationCmd_t;

typedef HYVRID_U8ArgsCmd_t    UANT_ISIS_DeployAnt1Cmd_t;
typedef HYVRID_U8ArgsCmd_t    UANT_ISIS_DeployAnt2Cmd_t;
typedef HYVRID_U8ArgsCmd_t    UANT_ISIS_DeployAnt3Cmd_t;
typedef HYVRID_U8ArgsCmd_t    UANT_ISIS_DeployAnt4Cmd_t;
typedef HYVRID_U8ArgsCmd_t    UANT_ISIS_DeployAnt1OverrideCmd_t;
typedef HYVRID_U8ArgsCmd_t    UANT_ISIS_DeployAnt2OverrideCmd_t;
typedef HYVRID_U8ArgsCmd_t    UANT_ISIS_DeployAnt3OverrideCmd_t;
typedef HYVRID_U8ArgsCmd_t    UANT_ISIS_DeployAnt4OverrideCmd_t;
typedef HYVRID_U8ArgsCmd_t    UANT_ISIS_ReportAntActivationCntCmd_t;
typedef HYVRID_U8ArgsCmd_t    UANT_ISIS_ReportAntActivationTimeCmd_t;
typedef HYVRID_NoArgsCmd_t    UANT_ISIS_MeasureSystemTemperatureCmd_t;



#endif