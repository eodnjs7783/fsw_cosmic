/************************************************************************
** File:
**    uant_app_events.h 
**
** Purpose: 
**  Define UANT App Events IDs
**
** Notes:
**
**
*************************************************************************/
#ifndef _UANT_EVENTS_H_
#define _UANT_EVENTS_H_

typedef enum {
    UANT_RESERVED_EID,
    UANT_STARTUP_INF_EID,
    UANT_COMMAND_ERR_EID,
    UANT_COMMANDNOP_INF_EID,
    UANT_COMMANDRST_INF_EID,
    UANT_INVALID_MSGID_ERR_EID,
    UANT_LEN_ERR_EID,
    UANT_PIPE_ERR_EID,
    UANT_CMD_ERR_EID,
}uant_events_n;

#endif /* _uant_app_events_h_ */

/************************/
/*  End of File Comment */
/************************/
