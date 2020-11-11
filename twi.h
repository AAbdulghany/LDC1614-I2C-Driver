/*
 * i2c.h
 * this file to use API's of the I2C driver from ti
 *
 *  Created on: Dec 12, 2019
 *  Author: Ghanie
 */

#ifndef TWI_H_
#define TWI_H_

#include <stdbool.h>
#include <stdint.h>
#include "driverlib/pin_map.h"
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"

#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
/***********************************************************************************************
 *                                   Preprocessor Macros                                       *
 ***********************************************************************************************/
/*Slave Address of LDC
 * if ADDR = L ---> 0x2A
 * if ADDR = H ---> 0x2B */
#define SLAVE_ADDRESS 0x2A

/*Numbers of Data*/
#define NUM_I2C_DATA  3

#define GPIO_PB2_I2C0SCL        0x00010802
#define GPIO_PB3_I2C0SDA        0x00010C02
/***********************************************************************************************
 *                                   Functions prototypes                                      *
 ***********************************************************************************************/
void I2C_pinInit(void);                                  /*Configure Required Ports and clocks*/
void I2C_init(uint32_t SystemClock);
void I2C_slaveAddressSet(bool operation);
void I2C_write(uint8_t address, uint16_t data);
uint16_t I2C_readData(uint8_t address);
void delay(void);
#endif /* TWI_H_ */
