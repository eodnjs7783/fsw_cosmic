/* Auto-Generated file */
#ifndef CFE_SRL_MISSION_CFG_H
#define CFE_SRL_MISSION_CFG_H

#include "cfe_srl_interface_cfg.h"

typedef enum {
	CFE_SRL_SOCAT_HANDLE_INDEXER,
} CFE_SRL_Handle_Indexer_t;

/* \# of General serial device */
#define CFE_SRL_GNRL_DEVICE_NUM		(CFE_SRL_SOCAT_HANDLE_INDEXER + 1)

/* \# of used gpio pin */
#define CFE_SRL_TOT_GPIO_NUM		1

typedef enum {
	CFE_SRL_ADCS_EN_GPIO_INDEXER,
} CFE_SRL_GPIO_Indexer_t;

#endif /* CFE_SRL_MISSION_CFG_H */