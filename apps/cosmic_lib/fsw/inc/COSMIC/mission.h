#ifndef _COSMIC_MISSION_H_
#define _COSMIC_MISSION_H_

#define COSMIC_PIPE_DEPTH   32
#define MAX_TM_DATASIZE     170
#define MAX_RAW_DATASIZE    200
#define MAX_TC_DATASIZE     170

#define STORE_OPSDATA_FILEPATH      "cf/opsdata.bin"

typedef enum {
    COSMIC_MODE_EARLY_ORBIT,
    COSMIC_MODE_NORMAL,
    COSMIC_MODE_COMMUNICAION,
    HYVIRD_MODE_COMMISSIONING,
    COSMIC_MODE_EMERGENCY,
    COSMIC_MODE_MISSION,
    
    //Don't use this.
    COSMIC_MODE_DEATH,
}COSMIC_ModType_n;


typedef enum {
    SUBMODE_EO_OK,
    SUBMODE_EO_STANDBY,
    SUBMODE_EO_INITIALIZE,
    SUBMODE_EO_INITIAL_AD,
    SUBMODE_EO_START_TELEMETRY_COLLECT,
    SUBMODE_EO_START_BEACON,
    SUBMODE_EO_DEPLOY_SP,
    SUBMODE_EO_DEPLOY_MMT,
    SUBMODE_EO_DEPLOY_UANT,
    SUBMODE_EO_UPDATE_INITIAL_PRO,
    SUBMODE_EO_START_DETUMBLING,
}HYVIRD_SubmodeType_EO_n;

typedef enum {
    SUBMODE_EM_UNKNOWN,
    SUBMODE_EM_READ_ERROR,
    SUBMODE_EM_EPS,
    SUBMODE_EM_DETUMBLE,
}COSMIC_SUbmodeType_EM_n;

typedef enum {
    COSMIC_RUNSTATUS_LIB,
    COSMIC_RUNSTATUS_CI,
    COSMIC_RUNSTATUS_EPS,
    COSMIC_RUNSTATUS_PAYR,
    COSMIC_RUNSTATUS_UANT,
    COSMIC_RUNSTATUS_TO,
    COSMIC_RUNSTATUS_FM,
    COSMIC_RUNSTATUS_ADCS,
    COSMIC_RUNSTATUS_GRX,
    COSMIC_RUNSTATUS_IFCAPP,
    COSMIC_RUNSTATUS_PAYC,
    COSMIC_RUNSTATUS_PAYS,
    COSMIC_RUNSTATUS_SN,
    COSMIC_RUNSTATUS_STX,
    COSMIC_RUNSTATUS_UTRX,
    COSMIC_RUNSTATUS_LOG,
    COSMIC_RUNSTATUS_DS,
    COSMIC_RUNSTATUS_HK,
    COSMIC_RUNSTATUS_SCH,
}COSMIC_Runstatus_n;

// typedef struct {
//     unsigned int eo_flag_no_effect : 1;
//     unsigned int eo_flag_
// }

typedef struct {
    uint8 CurrentMode;
    uint8 PreviousMode;
}COSMIC_OpsMode_t;

typedef struct {
    uint8 PreviousSubmode;
    uint8 CurrentSubmode;
}COSMIC_OpsSubMode_t;

typedef struct {
    uint8 CurrentModeFlag;
    uint8 PreviousModeFlag;
}COSMIC_OpsFlag_t;

typedef struct{
    COSMIC_OpsMode_t mode;
    COSMIC_OpsSubMode_t submode[COSMIC_MODE_DEATH];
    COSMIC_OpsFlag_t flag[COSMIC_MODE_DEATH];
    uint16 RebootCnt;
    uint8 RebootCause;
}COSMIC_OpsData_t;


typedef struct{
    uint8 MaxBuffer[MAX_TM_DATASIZE];
}COSMIC_MaxTelemetryBuffer_t;

void COSMIC_SetRunStatusOn(uint8_t entry);
void COSMIC_SetRunStatusOff(uint8_t entry);
void COSMIC_GetRunStatus(uint32_t* status);
void COSMIC_GetOpsData(COSMIC_OpsData_t* ops);
void COSMIC_GetMode(uint8* mode);
void COSMIC_GetSubMode(uint8* submode);
void COSMIC_GetSubModeEntry(COSMIC_OpsSubMode_t* submode, uint8 modeentry);
void COSMIC_GetModeFlag(uint8* flag);
void COSMIC_SetMode(uint8 mode, uint8 submode);
void COSMIC_ConvertMode(uint8 mode);
void COSMIC_SetSubMode(uint8 submode);
void COSMIC_SetFlag(uint8 flag);
int COSMIC_SaveOpts(COSMIC_OpsData_t *data);
int COSMIC_LoadOpts(void);

#endif