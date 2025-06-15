#ifndef _UANT_I2C_H_
#define _UANT_I2C_H_

#include <cfe.h>
#include <stdbool.h>

int isis_uant_i2c_init(void);

int isis_uant_i2c_write(uint8_t reg,
                        uint8_t data,
                        bool noarg);

int isis_uant_i2c_read(uint8_t reg,
                       uint8_t* data,
                       uint8_t size);

#endif