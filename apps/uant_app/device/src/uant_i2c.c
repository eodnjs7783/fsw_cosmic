#include <stdint.h>
#include <stdbool.h>
#include <cfe_srl_api.h>
#include <cfe_srl_handle.h>

#include <COSMIC/cosmic.h>


#define ISIS_UANT_I2C_ADDRESS   0x31
#define ISIS_UANT_I2C_DEV       IFC_MISSION_DEV_I2C2
#define ISIS_UANT_I2C_MUTEX_ID  IFC_MISSION_MUTEX_ID_I2C2

static CFE_SRL_IO_Handle_t i2c

int isis_uant_i2c_init(void) {
    int error;
    int status;

    status = CFE_SRL_PriorInit();
    if (status != CFE_SRL_OK) {
        return DEVICE_ERROR;
    }

    status = CFE_SRL_HandleInit(&i2c,
                              "UANT_I2C",
                              ISIS_UANT_I2C_DEV,
                              ISIS_UANT_I2C_ADDRESS,
                              ISIS_UANT_I2C_MUTEX_ID,
                              &error);
    if (status != IFC_OK) {
        return DEVICE_ERROR;
    }

    return DEVICE_SUCCESS;
}

int isis_uant_i2c_write(uint8_t reg,
                        uint8_t data,
                        bool noarg) {
    int status;
    if (noarg){
        status = CFE_SRL_ApiWrite(&i2c,&reg,1,ISIS_UANT_I2C_ADDRESS);

    }
    else{
        unit8_t buf[2] = {reg, data};
        status = CFE_SRL_ApiWrite(&i2c,buf,2,ISIS_UANT_I2C_ADDRESS);

       }
    return (status == CFE_SRl_OK ? DEVICE_SUCCESS : DEVICE_ERROR);

}

int isis_uant_i2c_read(uint8_t reg,
                       uint8_t* data,
                       uint8_t size) {
    int status;
    uint32 timeout_ms = 100;

    status = CFE_SRL_ApiRead(&i2c,&reg,1,data,size,timeout_ms,ISIS_UANT_I2C_ADDRESS);

    return (status == CFE_SRL_OK ? DEVICE_SUCCESS : DEVICE_ERR_READ);
}


void isis_uant_i2c_deinit(void)
{
    CFE_SRL_ApiClose(&I2C_Handle);
}