/* Auto-Generated file. Never change this code! */
/**
 * Required header files
*/
#include "cfe_srl_module_all.h"

/**
 * Global data
*/
CFE_SRL_IO_Handle_t *Handles[CFE_SRL_GNRL_DEVICE_NUM];
/**************************************************
 * Index of Each device
 * 0 : I2C0 Handle
 * 1 : RS422 Handle
 **************************************************/




/************************************************************************
 * Early Initialization function executed at cFE ES
 * Append object to `cfe_es_objtab.c`
 * Declaration is located at
 * `cfe/modules/core_private/fsw/inc/cfe_srl_core_internal.h`
 ************************************************************************/
int32 CFE_SRL_EarlyInit(void) {
	int32 Status;

	Status = CFE_SRL_PriorInit();
	if(Status != CFE_SUCCESS) return Status;
	CFE_ES_WriteToSysLog("%s: Prior Initialized.", __func__);
	/**************************************************
	 * Serial Comm. Init
 	 * Only `ready == true` interface is initialized
	 **************************************************/
	/* I2C0 Init */
	Status = CFE_SRL_HandleInit(&Handles[CFE_SRL_I2C0_HANDLE_INDEXER], "I2C0", "/dev/i2c-0", SRL_DEVTYPE_I2C, CFE_SRL_I2C0_HANDLE_INDEXER, 0, 0);
	if (Status != CFE_SUCCESS) {
		CFE_ES_WriteToSysLog("%s: I2C0 Initialization failed! RC=%d\n", __func__, Status);
		return CFE_SRL_I2C0_INIT_ERR;
	}
	CFE_ES_WriteToSysLog("%s: I2C0 Initialized. FD=%d || DevName=%s\n", __func__, Handles[CFE_SRL_I2C0_HANDLE_INDEXER]->FD, ((CFE_SRL_Global_Handle_t *)Handles[CFE_SRL_I2C0_HANDLE_INDEXER])->DevName);

	
return CFE_SUCCESS;
}
