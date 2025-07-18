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

/*
**   File:
**    cfe_es_objtab.c
**
**   Purpose:
**     This file contains the OS_object_table for system initialization/startup.
**
**  References:
**     Flight Software Branch C Coding Standard Version 1.0a
**     cFE Flight Software Application Developers Guide
**
**  Notes:
**
*/

/*
** Include files
*/
#include "cfe_es_module_all.h"

/* Init functions from other modules are in separate headers */
#include "cfe_evs_core_internal.h"
#include "cfe_fs_core_internal.h"
#include "cfe_sb_core_internal.h"
#include "cfe_tbl_core_internal.h"
#include "cfe_time_core_internal.h"
#include "cfe_config_core_internal.h"

#include "cfe_srl_core_internal.h"

/*
**
** ES_object_table
** Note: The name field in this table should be no more than OS_MAX_API_NAME -1 characters.
**
*/
CFE_ES_ObjectTable_t CFE_ES_ObjectTable[CFE_PLATFORM_ES_OBJECT_TABLE_SIZE] = {
    /*
    ** Spare entries -- The spares should be distributed evenly through this table
    */
    {.ObjectType = CFE_ES_NULL_ENTRY},
    {.ObjectType = CFE_ES_NULL_ENTRY},
    {.ObjectType = CFE_ES_NULL_ENTRY},

    /* Initialize the configuration registry early, so it can be used by core apps */
    {.ObjectType = CFE_ES_FUNCTION_CALL, .ObjectName = "CFE_Config_Init", .FuncPtrUnion.FunctionPtr = CFE_Config_Init},

    /*
    ** cFE core early initialization calls. These must be done before the tasks start
    */
    {.ObjectType               = CFE_ES_FUNCTION_CALL,
     .ObjectName               = "CFE_ES_CDSEarlyInit",
     .FuncPtrUnion.FunctionPtr = CFE_ES_CDS_EarlyInit},
    {.ObjectType = CFE_ES_NULL_ENTRY},
    {.ObjectType               = CFE_ES_FUNCTION_CALL,
     .ObjectName               = "CFE_EVS_EarlyInit",
     .FuncPtrUnion.FunctionPtr = CFE_EVS_EarlyInit},
    {.ObjectType = CFE_ES_NULL_ENTRY},
    {.ObjectType               = CFE_ES_FUNCTION_CALL,
     .ObjectName               = "CFE_SB_EarlyInit",
     .FuncPtrUnion.FunctionPtr = CFE_SB_EarlyInit},
    {.ObjectType = CFE_ES_NULL_ENTRY},
    {.ObjectType               = CFE_ES_FUNCTION_CALL,
     .ObjectName               = "CFE_TIME_EarlyInit",
     .FuncPtrUnion.FunctionPtr = CFE_TIME_EarlyInit},
    {.ObjectType = CFE_ES_NULL_ENTRY},
    {.ObjectType               = CFE_ES_FUNCTION_CALL,
     .ObjectName               = "CFE_TBL_EarlyInit",
     .FuncPtrUnion.FunctionPtr = CFE_TBL_EarlyInit},
    {.ObjectType = CFE_ES_NULL_ENTRY},
    {.ObjectType               = CFE_ES_FUNCTION_CALL,
     .ObjectName               = "CFE_FS_EarlyInit",
     .FuncPtrUnion.FunctionPtr = CFE_FS_EarlyInit},

    /*
    ** Spare entries
    */
    {.ObjectType = CFE_ES_NULL_ENTRY},
    {.ObjectType               = CFE_ES_FUNCTION_CALL,
     .ObjectName               = "CFE_SRL_EarlyInit",
     .FuncPtrUnion.FunctionPtr = CFE_SRL_EarlyInit},
    {.ObjectType = CFE_ES_NULL_ENTRY},

    /*
    ** cFE core tasks
    */
    {.ObjectType               = CFE_ES_CORE_TASK,
     .ObjectName               = "CFE_EVS",
     .FuncPtrUnion.MainTaskPtr = CFE_EVS_TaskMain,
     .ObjectPriority           = CFE_PLATFORM_EVS_START_TASK_PRIORITY,
     .ObjectSize               = CFE_PLATFORM_EVS_START_TASK_STACK_SIZE},
    {.ObjectType = CFE_ES_NULL_ENTRY},
    {.ObjectType               = CFE_ES_CORE_TASK,
     .ObjectName               = "CFE_SB",
     .FuncPtrUnion.MainTaskPtr = CFE_SB_TaskMain,
     .ObjectPriority           = CFE_PLATFORM_SB_START_TASK_PRIORITY,
     .ObjectSize               = CFE_PLATFORM_SB_START_TASK_STACK_SIZE},
    {.ObjectType = CFE_ES_NULL_ENTRY},
    {.ObjectType               = CFE_ES_CORE_TASK,
     .ObjectName               = "CFE_ES",
     .FuncPtrUnion.MainTaskPtr = CFE_ES_TaskMain,
     .ObjectPriority           = CFE_PLATFORM_ES_START_TASK_PRIORITY,
     .ObjectSize               = CFE_PLATFORM_ES_START_TASK_STACK_SIZE},
    {.ObjectType = CFE_ES_NULL_ENTRY},
    {.ObjectType               = CFE_ES_CORE_TASK,
     .ObjectName               = "CFE_TIME",
     .FuncPtrUnion.MainTaskPtr = CFE_TIME_TaskMain,
     .ObjectPriority           = CFE_PLATFORM_TIME_START_TASK_PRIORITY,
     .ObjectSize               = CFE_PLATFORM_TIME_START_TASK_STACK_SIZE},
    {.ObjectType = CFE_ES_NULL_ENTRY},
    {.ObjectType               = CFE_ES_CORE_TASK,
     .ObjectName               = "CFE_TBL",
     .FuncPtrUnion.MainTaskPtr = CFE_TBL_TaskMain,
     .ObjectPriority           = CFE_PLATFORM_TBL_START_TASK_PRIORITY,
     .ObjectSize               = CFE_PLATFORM_TBL_START_TASK_STACK_SIZE},

    /*
    ** Spare entries
    */
    //{.ObjectType = CFE_ES_NULL_ENTRY},
    {.ObjectType               = CFE_ES_CORE_TASK,
     .ObjectName               = "CFE_SRL",
     .FuncPtrUnion.MainTaskPtr = CFE_SRL_TaskMain,
     .ObjectPriority           = CFE_PLATFORM_SRL_START_TASK_PRIORITY,
     .ObjectSize               = CFE_PLATFORM_SRL_START_TASK_STACK_SIZE},
    {.ObjectType = CFE_ES_NULL_ENTRY},
    {.ObjectType = CFE_ES_NULL_ENTRY}};
