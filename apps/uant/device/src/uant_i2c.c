#include <IFC/ifc_mission_defs.h>
#include <IFC/ifc_io_generic.h>
#include <IFC/ifc_i2c.h>

#include <HYVRID/hyvrid.h>


#define ISIS_UANT_I2C_ADDRESS   0x31
#define ISIS_UANT_I2C_DEV       IFC_MISSION_DEV_I2C2
#define ISIS_UANT_I2C_MUTEX_ID  IFC_MISSION_MUTEX_ID_I2C2

static ifc_i2c_handle_t* i2c;

int isis_uant_i2c_init(void) {
    int error;
    int status;
    status = ifc_i2c_allocate(&i2c,
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
    uint8_t cmd[2] = {reg, data};

    ifc_io_mutex_lock(i2c);

    ifc_i2c_set_slave_address(i2c, ISIS_UANT_I2C_ADDRESS);    
    status = ifc_io_write(i2c, cmd, noarg ? 1 : 2);

    ifc_io_mutex_unlock(i2c);

    if (status != IFC_OK) {

    }

    return DEVICE_SUCCESS;
}

int isis_uant_i2c_read(uint8_t reg,
                       uint8_t* data,
                       uint8_t size) {
    if (ifc_i2c_transaction(i2c,
                            ISIS_UANT_I2C_ADDRESS,
                            &reg,
                            1,
                            data,
                            size) != IFC_OK) {
        return DEVICE_ERR_READ;
    }
    return DEVICE_SUCCESS;
}
