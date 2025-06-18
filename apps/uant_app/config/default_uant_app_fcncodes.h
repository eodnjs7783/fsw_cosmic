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
 *   Specification for the UANT_APP_APP command function codes
 *
 * @note
 *   This file should be strictly limited to the command/function code (CC)
 *   macro definitions.  Other definitions such as enums, typedefs, or other
 *   macros should be placed in the msgdefs.h or msg.h files.
 */
#ifndef UANT_APP_APP_FCNCODES_H
#define UANT_APP_APP_FCNCODES_H

/************************************************************************
 * Macro Definitions
 ************************************************************************/

/*
** Sample App command codes
*/
typedef enum {
    UANT_APP_NOOP_CC,
    UANT_APP_RESET_COUNTERS_CC,


    UANT_APP_RESET_CC,
    UANT_APP_GET_DEPLOYMENT_STATUS_CC,
    UANT_APP_ARM_ANTENNA_SYSTEMS_CC,
    UANT_APP_DISARM_CC,
    UANT_APP_AUTOMATED_DEPLOYMENT_CC,
    UANT_APP_CANCEL_DEPLOYMENT_ACTIVATION_CC,

    UANT_APP_DEPLOY_ANT1_CC,
    UANT_APP_DEPLOY_ANT2_CC,
    UANT_APP_DEPLOY_ANT3_CC,
    UANT_APP_DEPLOY_ANT4_CC,
    UANT_APP_DEPLOY_ANT1_OVERRIDE_CC,
    UANT_APP_DEPLOY_ANT2_OVERRIDE_CC,
    UANT_APP_DEPLOY_ANT3_OVERRIDE_CC,
    UANT_APP_DEPLOY_ANT4_OVERRIDE_CC,
    UANT_APP_REPORT_ANT_ACTIVATION_CNT_CC,
    UANT_APP_REPORT_ANT_ACTIVATION_TIME_CC,
    UANT_APP_MEASURE_SYSTEM_TEMPERATURE_CC,
}uant_app_cc_n;

#endif

// processgroundcommand 에서 경우를 나누기 위한 코드