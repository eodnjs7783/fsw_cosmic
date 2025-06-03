#ifndef _UANT_TLM_H_
#define _UANT_TLM_H_

#include <cfe.h>
#include <HYVRID/hyvrid.h>

typedef struct {
    uint16 DeployState;
} OS_PACK UANT_BCN_t;

typedef struct {
    uint16 DeployState;
} OS_PACK UANT_HK_t;

typedef struct {
    uint8 TlmHeader[CFE_SB_TLM_HDR_SIZE];
    HYVRID_CmdExecutionReportMsg_t Report;
    union {
        HYVRID_AppCount_t AppCounter;
        UANT_HK_t hk;
        UANT_BCN_t bcn;
        uint8 u8;
        uint16 u16;
        uint32 u32;
        int8 s8;
        int16 s16;
        int32 s32;
        float f;
        bool boolean;
        uint8 Byte[MAX_TM_DATASIZE];
        HYVRID_MaxTelemetryBuffer_t MaxBuffer;
    } Data;
} UANT_Tlm_t;

#endif