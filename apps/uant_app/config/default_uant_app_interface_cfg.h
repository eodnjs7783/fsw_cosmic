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
 *   UANT_APP Application Public Definitions
 *
 * This provides default values for configurable items that affect
 * the interface(s) of this module.  This includes the CMD/TLM message
 * interface, tables definitions, and any other data products that
 * serve to exchange information with other entities. 외부와도 공유되는 값
 *
 * @note This file may be overridden/superceded by mission-provided defintions
 * either by overriding this header or by generating definitions from a command/data
 * dictionary tool.
 */
#ifndef UANT_APP_INTERFACE_CFG_H
#define UANT_APP_INTERFACE_CFG_H

/**
 * \brief Length of string buffer in the Display Value command
 *
 * The Display Value command offers an example of how to use command
 * parameters of different types.  This macro controls the length
 * of the string parameter.
 */
#define UANT_APP_STRING_VAL_LEN 10

/*
#define MAX_TM_DATASIZE     170
#define MAX_RAW_DATASIZE    200
#define MAX_TC_DATASIZE     170
*/
#endif
