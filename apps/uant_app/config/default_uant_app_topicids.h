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
 *   UANT_APP Application Topic IDs
 */

 // msgid는 앱에서 구독했을 때 dispatch에서 분기되어 처리되는 것들. 
#ifndef UANT_APP_TOPICIDS_H
#define UANT_APP_TOPICIDS_H

#define CFE_MISSION_UANT_APP_CMD_TOPICID       0x82
#define CFE_MISSION_UANT_APP_SEND_HK_TOPICID   0x83
#define CFE_MISSION_UANT_APP_HK_TLM_TOPICID    0x83
#define CFE_MISSION_UANT_APP_OP_TLM_TOPICID    0x84

#endif
