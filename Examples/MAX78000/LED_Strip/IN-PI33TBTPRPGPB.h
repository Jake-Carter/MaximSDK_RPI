/**
 * @file    IN-PI33TBTPRPGPB.h
 * @brief   LED Strip driver API.
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

#ifdef __cplusplus
extern "C" {
#endif


/***** TypeDef *****/

/**
 * @brief Structure Type for defining one LED Pixel
 */
typedef union RGB_LED{
    struct {
        uint8_t Red_value;
        uint8_t Blue_value;
        uint8_t Green_value;
    }RGB_dat;
    uint32_t val;
}RGB_Pixel_t;

/***** Definitions *****/

#define LED_RES         24          //24-bit led resolution

#if defined(BOARD_PIHAT_V1)   
#define NUM_LEDS        4          
#else   
#define NUM_LEDS        0
#endif
/***** Globals *****/


/* **** Function Prototypes **** */

/**
 * @brief Initalize the data pin for sending LED Strip data
 *  
 * @param PT_Channel Pulse Train Engine channel connected to LED Strip DataIn
 * 
 * @return int #E_NO_ERROR if everything is successful.
 */
int LEDStrip_Init(mxc_gpio_cfg_t *LEDStripDataPin);

/**
 * @brief Send LED Strip data on initialized Pulse Train Channel
 * 
 */
void LEDStrip_Update(void);

/**
 * @brief Turn off LED Strip -- Current Pixels are Saved
 * 
 */
void LEDStrip_Off(void);

/**
 * @brief Sets all pixels to OFF -- Does not update LED Strip
 * 
 */
void LEDStrip_ClearStrip(void);

/**
 * @brief Set LED Strand pixel based on separate R, G and B values
 * 
 * @param LEDStrip_index LED Strip index number
 * @param r Red pixel value (8-bit max)
 * @param g Green pixel value (8-bit max)
 * @param b Blue pixel value (8-bit max)
 * @return  int #E_NO_ERROR if everything is successful.
 */
int LEDStrip_SetPixel_RGB(uint32_t LEDStrip_index, uint8_t r,uint8_t g, uint8_t b);

/**
 * @brief Set LED Strand pixel based on 24-bit RBG value
 * 
 * @param LEDStrip_index 
 * @param pixel_val 
 */
void LEDStrip_SetPixel_24b(uint32_t LEDStrip_index, uint32_t pixel_val);

/**
 * @brief IRQ handler for sending individual bits using Pulse Train Engine.
 * This function must be placed in the PT_IRQHandler() function so it is 
 * executed after each PT interrupt.
 */
void LEDSTRIP_IRQHandler();

/**
 * @brief Rainbow strand test to verify LED Strip is functional
 * 
 * @param cycles Number of times to run rainbow test animation (value
 *  of 0 results in running indefintley)
 * @param dimFactor Dimming factor for LED brightness 
 * Must be between 0 (Full bright) - 7 (Least Bright)
 */
void LEDSTRIP_StrandTest(uint32_t cycles, uint8_t dimFactor);

#ifdef __cplusplus
}
#endif
/**@}*/
#endif // LIBRARIES_MISCDRIVERS_IN_PI33TBTPRPGPB_H_
