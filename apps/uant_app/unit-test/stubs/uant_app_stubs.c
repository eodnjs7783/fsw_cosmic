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
 *
 * Auto-Generated stub implementations for functions defined in uant_app header
 */

#include "uant_app.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for UANT_APP_Init()
 * ----------------------------------------------------
 */
CFE_Status_t UANT_APP_Init(void)
{
    UT_GenStub_SetupReturnBuffer(UANT_APP_Init, CFE_Status_t);

    UT_GenStub_Execute(UANT_APP_Init, Basic, NULL);

    return UT_GenStub_GetReturnValue(UANT_APP_Init, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for UANT_APP_Main()
 * ----------------------------------------------------
 */
void UANT_APP_Main(void)
{

    UT_GenStub_Execute(UANT_APP_Main, Basic, NULL);
}
