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
 * @file    main.c
 * @brief   LED Strip Example
 * @details This example sends a rainbow effect down an RGB LED Strip
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include "mxc_device.h"
#include "led.h"
#include "board.h"
#include "gpio.h"
#include "mxc_delay.h"
#include "IN-PI33TBTPRPGPB.h"

/***** Definitions *****/
#define DIM_FACTOR 0        //Set the dimming factor (0:Full Brightness; 7: Minimum brightness)
#define CYCLES 0            //Set the number of cycles to run through the rainbow (0 = infinite)

#if defined(BOARD_PIHAT_V1)
mxc_gpio_cfg_t LEDStripDataPin = { MXC_GPIO0, MXC_GPIO_PIN_19, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE,
                                    MXC_GPIO_VSSEL_VDDIOH };
#endif


#ifdef BOARD_PIHAT_V1   //Using the MAX78000_RPIHat

#define NUM_LEDS        4           //LEDs on board

#else   //Not using the MAX78000_RPIHat

#define NUM_LEDS        0 //Number of LEDs in chain

#endif

/***** Globals *****/


/***** Functions *****/

// *****************************************************************************
int main(void)
{

    printf("\n***** LED Strip Example *****\n");

#ifdef BOARD_PIHAT_V1
    printf("This example sends a rainbow animation through the onboard RGB LEDs.\n");
    printf("LED Strip Length: %d LEDs\n",NUM_LEDS);
    printf("LED Strip Resolution: %d bits\n",LED_RES);
    printf("Sitback and enjoy the show!\n");
#else
    printf("This example sends rainbow animation data for a NeoPixel LED Strip.\n");
    printf("Connect the MAX78000's P%d.%d to the DIN pin of the powered LED Strip.\n",LEDStripDataPin.port,LEDStripDataPin.pin);
    printf("LED Strip Length: %d LEDs\n",NUM_LEDS);
    printf("LED Strip Resolution: %d bits\n",LED_RES);
    printf("Sitback and enjoy the show!\n");
#endif

    //Intialize data output pin
    if(LEDStrip_Init(&LEDStripDataPin)!=E_NO_ERROR){
        printf("Error Initalizing the LED Strip Data Pin!\n");
        while(1){}
    }
    
    //Switch to 100MHz clock
    MXC_SYS_Clock_Select(MXC_SYS_CLOCK_IPO);

    //Cycles = 0 will run this test loop indefintly
    LEDSTRIP_StrandTest(CYCLES, DIM_FACTOR);

    while (1) 
    {
        //Should not reach this loop
    }
}