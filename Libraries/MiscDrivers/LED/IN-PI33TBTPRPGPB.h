/**
 * @file
 * @brief   LED driver API.
 */

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

#ifndef LIBRARIES_MISCDRIVERS_IN_PI33TBTPRPGPB_H_
#define LIBRARIES_MISCDRIVERS_IN_PI33TBTPRPGPB_H_

#include "mxc_assert.h"
#include "board.h"
#include "gpio.h"
#include "pt.h"

#ifdef __cplusplus
extern "C" {
#endif


/***** TypeDef *****/

typedef union RGB_LED{
    struct {
        uint8_t Red_value;
        uint8_t Green_value;
        uint8_t Blue_value;
        uint8_t RSVD;
    }RGB_dat;
    uint32_t val;
}RGB_Pixel_t;

/***** Definitions *****/

#ifdef BOARD_PIHAT_V1   //Using the MAX78000_RPIHat

/*  LED Setup   */
#define NUM_LEDS        4           //LEDs on board
#define LED_RES         24          //24-bit led resolution

/* PulseTrain Setup */
#define PT_CH           1               //PT channel 1
#define PT_FREQ         17000000        //Bits/sec on PT
#define PT_Resolution   12              //PT resolution
#define PT_Divider      MXC_PT_CLK_DIV1 //PT Clock Divider

#define PT_ONE 0x1FF                //0 bit .3us high .9us low
#define PT_ZERO 0x00F               //1 bit .9us high .0us low

#else   //Not using the MAX78000_RPIHat

/*  LED Setup   */
#define NUM_LEDS        //Number of LEDs in chain
#define LED_RES         //LED Resolution in bits

/* PulseTrain Setup */
#define PT_CH           //Pulse Train Channel
#define PT_FREQ         //Bits/second for PT 
#define PT_Resolution   //Number of bits per pulse-train
#define PT_Divider

#define PT_ONE          //Pulse Train value for '1'
#define PT_ZERO         //Pulse Train value for '0'

#endif

/***** Globals *****/


/* **** Function Prototypes **** */
int LEDStrip_Init(void);

int LEDStrip_SetVoltage(mxc_gpio_vssel_t vssel);

void LEDStrip_Update(void);

void LEDStrip_Off(void);

void LEDStrip_SetPixel_RGB(uint32_t LEDStrip_index, uint8_t r,uint8_t g, uint8_t b);

void LEDStrip_SetPixel_24b(uint32_t LEDStrip_index, uint32_t pixel_val);

void LEDSTRIP_IRQHandler();

#ifdef __cplusplus
}
#endif
/**@}*/
#endif // LIBRARIES_MISCDRIVERS_IN_PI33TBTPRPGPB_H_
