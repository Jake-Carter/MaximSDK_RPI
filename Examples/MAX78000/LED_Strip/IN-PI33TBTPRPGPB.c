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

#include "IN-PI33TBTPRPGPB.h"
#include "mxc_device.h"
#include "mxc_delay.h"



/*  Globals */

/*  LED Strand Variables    */
RGB_Pixel_t LEDStrip_PixelData[NUM_LEDS] = {0};              //Hold pixel value
mxc_gpio_cfg_t LEDStripDINPin = {0};

/*  Private Function Prototype  */

void LEDStrip_Reset(void);
void LEDStrip_PackArray(void);
uint32_t Wheel(uint8_t pos, uint8_t DIM_FACTOR);
void LEDStrip_BitBang();

/******************************************************************************/
int LEDStrip_Init(mxc_gpio_cfg_t *DataPin)
{
    LEDStripDINPin = *DataPin;
    MXC_GPIO_Config(&LEDStripDINPin);
    MXC_GPIO0->out_clr = LEDStripDINPin.mask;
    
    return (E_NO_ERROR);
}

//******************************************************************************
void LEDStrip_Update(void)
{
    LEDStrip_BitBang();
}

//******************************************************************************
void LEDStrip_Off(void)
{
    //TODO: Add in deinit strip
}

//******************************************************************************
void LEDStrip_ClearStrip(void)
{
    for (int i=0;i<NUM_LEDS;i++){
        LEDStrip_PixelData[i].val = 0x00;
    }
}

//******************************************************************************
int LEDStrip_SetPixel_RGB(uint32_t LEDStrip_index, uint8_t r,uint8_t g, uint8_t b)
{
    if(LEDStrip_index >= NUM_LEDS){
        return(E_BAD_PARAM);
    }

    LEDStrip_PixelData[LEDStrip_index].RGB_dat.Red_value = r;
    LEDStrip_PixelData[LEDStrip_index].RGB_dat.Green_value = g;
    LEDStrip_PixelData[LEDStrip_index].RGB_dat.Blue_value = b;

    return(E_NO_ERROR);
}

//******************************************************************************
void LEDStrip_SetPixel_24b(uint32_t LEDStrip_index, uint32_t pixel_val)
{
    LEDStrip_PixelData[LEDStrip_index].val = pixel_val;
}

//******************************************************************************


void LEDSTRIP_StrandTest(uint32_t cycles, uint8_t dimFactor){
    uint16_t pos, j, i;
    uint32_t color, counter =0;
    while(counter <= cycles){
        for(j=0; j<256; j++)
        { 
            for(i=0; i<NUM_LEDS; i++)
            {
                // generate a value between 0~255 according to the position of the pixel
                // along the strip
                pos = ((i*256/NUM_LEDS)+j) & 0xFF;
                // calculate the color for the ith pixel
                color = Wheel(pos, dimFactor);
                // set the color of the ith pixel
                LEDStrip_SetPixel_24b(i, color);
            }
            LEDStrip_Update();
            MXC_Delay(5000);
        }  
        if(cycles)
            counter++;
    }
}



//******************************************************************************
void LEDStrip_Reset(void){
  
}

uint32_t Wheel(uint8_t pos, uint8_t DIM_FACTOR){
  pos = 255 - pos;
  RGB_Pixel_t color = {0};
  if(pos < 85) 
  {
    color.RGB_dat.Red_value = ((255 - pos * 3) >> DIM_FACTOR);
    color.RGB_dat.Green_value = (0);
    color.RGB_dat.Blue_value = ((pos * 3) >> DIM_FACTOR);
    return(color.val);
  } else if(pos < 170) 
  {
    pos -= 85;
    color.RGB_dat.Red_value = (0);
    color.RGB_dat.Green_value = ((pos * 3 ) >> DIM_FACTOR);
    color.RGB_dat.Blue_value = ((255 - pos * 3) >> DIM_FACTOR);
    return(color.val);
  } else 
  {
    pos -= 170;
    color.RGB_dat.Red_value = ((pos * 3) >> DIM_FACTOR);
    color.RGB_dat.Green_value = ((255 - pos * 3) >> DIM_FACTOR);
    color.RGB_dat.Blue_value = (0);
    return(color.val);
  }
}

#define ONE_HIGHTICK_OFFSET 7
#define ZERO_HIGHTICK_OFFSET 2
#define ONE_LOWTICK_OFFSET 2
#define ZERO_LOWTICK_OFFSET 7

void LEDStrip_BitBang(){


    volatile int j;
    uint32_t LED_BitMask;
    for(int i=0; i<NUM_LEDS;i++){
        LED_BitMask = 0x01 << (LED_RES-1);
        while(LED_BitMask)
        {
            LEDStripDINPin.port->out_set = LEDStripDINPin.mask;
            if ((LEDStrip_PixelData[i].val & LED_BitMask)){

                    /*  '1' Bit */
                    LEDStripDINPin.port->out_set = LEDStripDINPin.mask;   //Pin HIGH
                    LED_BitMask >>= 1;                      //Shift mask
                    for( j=0;j<ONE_HIGHTICK_OFFSET;j++){}   //Intentional delay
                    LEDStripDINPin.port->out_clr = LEDStripDINPin.mask;   //Pin LOW
                    for( j=0;j<ONE_LOWTICK_OFFSET;j++){}    //Intentional delay
            }
            else
            {        
                    /*  '0' Bit */
                    LEDStripDINPin.port->out_set = LEDStripDINPin.mask;   //Pin HIGH
                    LED_BitMask >>= 1;                      //Shift mask
                    for( j=0;j<ZERO_HIGHTICK_OFFSET;j++){}  //Intentional delay
                    LEDStripDINPin.port->out_clr = LEDStripDINPin.mask;   //Pin LOW
                    for( j=0;j<ZERO_LOWTICK_OFFSET;j++){}   //Intentional delay
            }
        }
    }

}
