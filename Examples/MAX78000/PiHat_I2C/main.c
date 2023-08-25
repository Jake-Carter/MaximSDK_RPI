/******************************************************************************
 * Copyright (C) 2023 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 ******************************************************************************/

/**
 * @file        main.c
 * @brief       I2C Loopback Example
 * @details     This example uses the I2C Master to read/write from/to the I2C Slave. For
 *              this example you must connect P0.10 to P0.16 (SCL) and P0.11 to P0.17 (SCL). The Master
 *              will use P0.10 and P0.11. The Slave will use P0.16 and P0.17. You must also
 *              connect the pull-up jumpers (JP21 and JP22) to the proper I/O voltage.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "board.h"
#include "mxc_device.h"
#include "mxc_delay.h"
#include "nvic_table.h"
#include "i2c.h"
#include "dma.h"

#ifdef BOARD_EVKIT_V1
#warning This example is not supported by the standard EvKit.
#endif

/***** Definitions *****/
#define RPI_I2C MXC_I2C1

#define I2C_FREQ 100000
#define MCU_SLAVE_ADDR (0x55)
#define MAX_BYTES_RX 128
#define MAX_BYTES_TX 52

/***** Globals *****/
int rxnum = 0;
int txnum = 0;
int txcnt = 0;
uint8_t packet_ready = 0;
uint8_t Srxdata[MAX_BYTES_RX];
uint8_t Stxdata[MAX_BYTES_TX];

/***** Functions *****/
int slaveHandler(mxc_i2c_regs_t* i2c, mxc_i2c_slave_event_t event, void* data){
switch (event) {
    case MXC_I2C_EVT_MASTER_WR:
        // If we're being written to
        // Clear bytes written
        rxnum = 0;
        break;
        
    case MXC_I2C_EVT_MASTER_RD:
    
        txnum = MAX_BYTES_TX;
        txcnt = 0;

        break;
        
    case MXC_I2C_EVT_RX_THRESH:
    case MXC_I2C_EVT_OVERFLOW:
        rxnum += MXC_I2C_ReadRXFIFO(i2c, &Srxdata[rxnum], MXC_I2C_GetRXFIFOAvailable(i2c));
        packet_ready = 1;    
        break;
        
    case MXC_I2C_EVT_TX_THRESH:
    case MXC_I2C_EVT_UNDERFLOW:
    
        // Write as much data as possible into TX FIFO
        // Unless we're at the end of the transaction (only write what's needed)
        if (txcnt >= txnum) {
            break;
        }
        
        int num = MXC_I2C_GetTXFIFOAvailable(i2c);
        num = (num > (txnum - txcnt)) ? (txnum - txcnt) : num;
        txcnt += MXC_I2C_WriteTXFIFO(i2c, &Stxdata[txcnt], num);
        break;
        
    default:
        if (* ((int*) data) == E_COMM_ERR) {
            printf("I2C Slave Error!\n");
            printf("i2c->intfl0 = 0x%08x\n", i2c->intfl0);
            printf("i2c->status  = 0x%08x\n", i2c->status);
            return 1;
        }
        else if (* ((int*) data) == E_NO_ERROR) {
            rxnum += MXC_I2C_ReadRXFIFO(i2c, &Srxdata[rxnum], MXC_I2C_GetRXFIFOAvailable(i2c));
            return 1;
        }
    }
    return E_NO_ERROR;
}

void I2C1_IRQHandler(void)
{
    NVIC_DisableIRQ(I2C1_IRQn);
    
    MXC_I2C_AsyncHandler(RPI_I2C);

    NVIC_EnableIRQ(I2C1_IRQn);
    return;
}

int main()
{
    MXC_Delay(MXC_DELAY_MSEC(500)); //Wait for PMIC to power-up
    printf("\n******** PiHat I2C Demo *********\n");

    printf("\nThis demo uses I2C to change the state of the\n");
    printf("on-board RGB LEDs.\n");


    int error, i = 0;

    // Initialize test data
    for (i = 0; i < MAX_BYTES_TX; i++) {
        Stxdata[i] = 'a'+i;
        Srxdata[i] = 0xFF;
    }
    
    //Setup the I2CS
    error = MXC_I2C_Init(RPI_I2C, 1, MCU_SLAVE_ADDR);

    NVIC_EnableIRQ(I2C1_IRQn);
    MXC_I2C_SetFrequency(RPI_I2C, I2C_FREQ);

    uint8_t rx_data[10] = {0};
    while(1){
        if ((error = MXC_I2C_SlaveTransactionAsync(RPI_I2C, slaveHandler)) != 0) {
            printf("\nError Starting Slave Transaction %d\n", error);
            return -1;
        }
        while(!packet_ready);
        MXC_Delay(100000);
        packet_ready = 0;
    }
}
