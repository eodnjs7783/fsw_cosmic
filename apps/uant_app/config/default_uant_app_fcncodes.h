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
 * @file
 *   Specification for the UANT_APP command function codes
 *
 * @note
 *   This file should be strictly limited to the command/function code (CC)
 *   macro definitions.  Other definitions such as enums, typedefs, or other
 *   macros should be placed in the msgdefs.h or msg.h files.
 */
#ifndef UANT_APP_FCNCODES_H
#define UANT_APP_FCNCODES_H

/************************************************************************
 * Macro Definitions
 ************************************************************************/

/*
** Sample App command codes
*/
#define UANT_APP_NOOP_CC           0
#define UANT_APP_RESET_COUNTERS_CC 1
#define UANT_APP_PROCESS_CC        2
#define UANT_APP_DISPLAY_PARAM_CC  3
#define UANT_APP_RESET_CC                         4
#define UANT_APP_ARM_CC                           5
#define UANT_APP_DISARM_CC                        6
#define UANT_APP_AUTODEPLOY_CC                    7
#define UANT_APP_DEPLOY_ANT_CC                    8
#define UANT_APP_DEPLOY_ANT1_OVERRIDE_CC          9
#define UANT_APP_DEPLOY_ANT2_OVERRIDE_CC         10
#define UANT_APP_DEPLOY_ANT3_OVERRIDE_CC         11
#define UANT_APP_DEPLOY_ANT4_OVERRIDE_CC         12
#define UANT_APP_CANCEL_DEPLOY_ACT_CC            13
#define UANT_APP_GET_STATUS_CC                   14
#define UANT_APP_GET_TEMP_CC                     15
#define UANT_APP_REPORT_ANT1_ACTIVATION_CNT_CC   16
#define UANT_APP_REPORT_ANT2_ACTIVATION_CNT_CC   17
#define UANT_APP_REPORT_ANT3_ACTIVATION_CNT_CC   18
#define UANT_APP_REPORT_ANT4_ACTIVATION_CNT_CC   19
#define UANT_APP_REPORT_ANT1_ACTIVATION_TIME_CC  20
#define UANT_APP_REPORT_ANT2_ACTIVATION_TIME_CC  21
#define UANT_APP_REPORT_ANT3_ACTIVATION_TIME_CC  22
#define UANT_APP_REPORT_ANT4_ACTIVATION_TIME_CC  23
#endif
