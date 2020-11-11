/**
 * main.c
 */
 
#include "twi.h"

uint16_t pui32DataTx = 0x5A10;
uint16_t pui32DataRx = 0;
//uint32_t i=0;
int main(void)
{

    //uint32_t ui32Index; 

    /*Variable to set the system clock*/
    uint32_t SystemClock;

    /*Initiate system Clock to 20MHz*/
    SystemClock = SysCtlClockFreqSet(
            (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_OSC), 120000000);

    /*Initiate Clock for I2C*/
    I2C_init(SystemClock);


    while (1)
    {
        /*This function takes 2 Arguments,
         * first the memory register address
         * second the data required to send 
         */
        I2C_write(0x08, pui32DataTx);

        /*This function takes 2 Arguments,
         * first the memory register address
         * return the data received from the LDC sensor  
         */
        pui32DataRx = I2C_readData(0x08);
    }
}
