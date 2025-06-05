#include <cfe.h>
#include <COSMIC/cosmic.h>

int32 HVD_LibInit(void)
{
    COSMIC_LoadOpts();
    COSMIC_SetRunStatusOn(COSMIC_RUNSTATUS_LIB);
    OS_printf ("COSMIC Lib Initialized.\n");
                
    return OS_SUCCESS;
}
