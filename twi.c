/*
 * twi.c
 *
 *  Created on: Dec 12, 2019
 *      Author: Ghanie
 */
#include "twi.h"


/*********************************************** Delay Function ***********************************************
 * Description: this busy delay is used after writing to I2CMCS Register and before polling the I2CMasterBUSY()
 ************************************************************************************************************** */
void delay(void)
{
    uint8_t i=0;
    for(i=0;i<10;++i);
}


/*********************************************** Pin Init *****************************************************
 * Description: This function is to set pins to I2C module 0 \b for now \b
 ************************************************************************************************************** */
void I2C_pinInit(void)
{
    /* The I2C0 peripheral must be enabled before use.*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

    /* For this example I2C0 is used with PortB[3:2].  The actual port and
     // pins used may be different on your part, consult the data sheet for
     // more information.  GPIO port B needs to be enabled so these pins can
     // be used.
     // TODO: change this to whichever GPIO port you are using. */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    //
    /* Configure the pin muxing for I2C0 functions on port B2 and B3.
     // This step is not necessary if your part does not support pin muxing.
     // TODO: change this to select the port/pin you are using.
     */
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);

    //
    /* Select the I2C function for these pins.  This function will also
     // configure the GPIO pins pins for I2C operation, setting them to
     // open-drain operation with weak pull-ups.  Consult the data sheet
     // to see which functions are allocated per pin.
     // TODO: change this to select the port/pin you are using.
     */
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
}


/*********************************************** Module Init *************************************************
 * Description: This function is to Start I2C module 0 \b for now \b
 ************************************************************************************************************** */
void I2C_init(uint32_t SystemClock)
{
    I2C_pinInit();
    /* Enable and initialize the I2C0 master module.  Use the system clock for
     the I2C0 module.  The last parameter sets the I2C data transfer rate.
     If false the data rate is set to 100kbps and if true the data rate will
     be set to 400kbps.  For this example we will use a data rate of 100kbps.*/
    I2CMasterInitExpClk(I2C0_BASE, SystemClock, false);
}


/*********************************************** Slave Address Set ********************************************
 * Description: This function is to set I2C Slave Address with Read/write condition
 * \b if operation is false --> Write
 * \b if operation is true  --> Read
 ************************************************************************************************************** */
void I2C_slaveAddressSet(bool operation)
{
     // Tell the master module what address it will place on the bus when
     // communicating with the slave.  Set the address to SLAVE_ADDRESS
     // (as set in the slave module).  The receive parameter is set to false
     // which indicates the I2C Master is initiating a \b writes \b to the slave.  If
     // true, that would indicate that the I2C Master is initiating reads from
     // the slave.
    I2CMasterSlaveAddrSet(I2C0_BASE, SLAVE_ADDRESS, operation);
}


/*********************************************** I2C Write *****************************************************
 * Description: This function is to start writing process to I2C module 0 \b for now \b
 * 1- Clear interrupt, 2- Put Data in I2CMDR Register, 3- Start writing condition
 * 4- polling on BUSYBIT, 5- sending remain data, 6- STOP Bit
 ************************************************************************************************************** */
void I2C_write(uint8_t address, uint16_t data)
{
    /*Clear IC BIT interrupt*/
    I2CMasterIntClear(I2C0_BASE);

    /*Send Slave address with Write*/
    I2C_slaveAddressSet(false);

    /*Send Memory Address */
    I2CMasterDataPut(I2C0_BASE, address);

    /*start condition*/
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    /*delay from 0 to 10 using while loop*/
    delay();

    /*wait for busy bit to clear*/
    while (I2CMasterBusy(I2C0_BASE) == true);
    /*put data in Master Register*/
    I2CMasterDataPut(I2C0_BASE, (uint8_t) ((Data >> 8) & 0x00FF));
    /*Continue transmitting*/
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
    /**/
    delay();
    /*wait for busy bit to clear*/
    while (I2CMasterBusy(I2C0_BASE) == true);
    /*Put the other LSB Data in Master Register*/
    I2CMasterDataPut(I2C0_BASE, (uint8_t) (Data));
    /*Send Finish ACK*/
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    /**/
    delay();
    /*wait for busy bit to clear*/
    while (I2CMasterBusy(I2C0_BASE) == true);
    /*Send Stop Bit*/
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_STOP);
}


/*********************************************** I2C Read *****************************************************
 * Description: This function is to start reading process to I2C module 0 \b for now \b
 * 1- Clear interrupt, 2- Start Reading condition, 3-
 * 4- polling on BUSYBIT, 5- sending remain data, 6- STOP Bit
 ************************************************************************************************************** */
uint16_t I2C_readData(uint8_t address)
{
    uint16_t data;

    /*Clear IC BIT interrupt*/
    I2CMasterIntClear(I2C0_BASE);

    /*Send Slave address with Write*/
    I2C_slaveAddressSet(false);

    /*address*/
    I2CMasterDataPut(I2C0_BASE, address);

    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    /**/
    delay();

    /*wait until the Masteis idle*/
    while (I2CMasterBusy(I2C0_BASE) == true);

    /*Send Slave Address with Read*/
    I2C_slaveAddressSet(true);

    /*send control byte and read from the register from the MCU*/
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);

    /**/
   delay();

    /*wait for MCU to finish transaction*/
    while (I2CMasterBusy(I2C0_BASE) == true);

    /*put data in data register*/
    data = I2CMasterDataGet(I2C0_BASE);

    /*Send Finish ACK*/
     I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

    /**/
    delay();

    /*wait for MCU to finish transaction*/
    while (I2CMasterBusy(I2C0_BASE) == true);

    /*Receive LSB of Data*/
    data =(data<<8)| I2CMasterDataGet(I2C0_BASE);

    /*Stop Bit*/
   I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_STOP);

   return data;
}

