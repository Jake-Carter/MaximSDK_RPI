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

#include "mxc_device.h"
#include "IN-PI33TBTPRPGPB.h"
#include "mxc_delay.h"
#include "pt_regs.h"
#include "gcr_regs.h"
#include "ptg_regs.h"

/*  Globals */

mxc_pt_regs_t *PT_Inst = NULL;  //Pulse Train Peripheral Address
RGB_Pixel_t LED_Pixel[NUM_LEDS] = {0};              //Hold pixel value
uint32_t    Pixel_Bits[NUM_LEDS*LED_RES + 1] = {0}; //Hold bit values for each pixel

/*  Private Function Prototype  */

void LEDStrip_Reset(void);
void LEDStrip_PackArray(void);


/******************************************************************************/
int LEDStrip_Init(void)
{
    int err = E_NO_ERROR;

    //Get pointer to PT instance
    PT_Inst = (mxc_pt_regs_t *)MXC_PT_GET_PT(PT_CH);

    //setup PT configuration
    mxc_pt_cfg_t ptConfig;
    ptConfig.channel = PT_CH;
    ptConfig.bps = PT_FREQ; //bit rate
    ptConfig.ptLength = PT_Resolution; //bits
    ptConfig.pattern = 0x00;
    ptConfig.loop = 0;
    ptConfig.loopDelay = 0;
    
    MXC_PT_Init(PT_Divider); //initialize pulse train engine
    err = MXC_PT_Config(&ptConfig);
    return (err);
}

//******************************************************************************
int LEDStrip_SetVoltage(mxc_gpio_vssel_t vssel){
    
    int err = E_NO_ERROR;
    mxc_gpio_cfg_t temp = gpio_cfg_pt1;

    switch (PT_CH){
        case 0:
            temp = gpio_cfg_pt0;
        break;
        case 1:
            temp = gpio_cfg_pt1;
        break;
        case 2:
            temp = gpio_cfg_pt2;
        break;
        case 3:
            temp = gpio_cfg_pt3;
        break;
        default:
            return(E_BAD_PARAM);
        break;
    }
    temp.vssel = vssel;
    err = MXC_GPIO_Config(&temp);
    return(err);
}

//******************************************************************************
void LEDStrip_Update(void)
{
    LEDStrip_PackArray();
    LEDStrip_Reset();
    PT_Inst->train = 0x7FF;
    PT_Inst->loop = 1;
    MXC_PT_Start(1 << PT_CH);   //start channel so it goes high
}

//******************************************************************************
void LEDStrip_Off(void)
{
    //TODO: Turn off LEDS
}

//******************************************************************************
void LEDStrip_SetPixel_RGB(uint32_t LEDStrip_index, uint8_t r,uint8_t g, uint8_t b)
{
    LED_Pixel[LEDStrip_index].RGB_dat.Red_value = r;
    LED_Pixel[LEDStrip_index].RGB_dat.Green_value = g;
    LED_Pixel[LEDStrip_index].RGB_dat.Blue_value = b;
}

//******************************************************************************
void LEDStrip_SetPixel_24b(uint32_t LEDStrip_index, uint32_t pixel_val)
{
    LED_Pixel[LEDStrip_index].val = pixel_val;
}

//******************************************************************************
void LEDSTRIP_IRQHandler(){
    
    static volatile uint32_t LEDStrip_SendIndex = 0;

    PT_Inst->train =Pixel_Bits[LEDStrip_SendIndex];
    MXC_PT_Start(1 << PT_CH);
    
    if (LEDStrip_SendIndex++ >= NUM_LEDS*LED_RES){
        MXC_PT_Stop(1 << PT_CH);
        MXC_PT_DisableInt(1 << PT_CH); //disable interrupts for PT    
        LEDStrip_SendIndex = 0;
    }

}

/****************************** Private Functions ******************************/
void LEDStrip_PackArray(void){

    for(int i = 0; i < NUM_LEDS;i++){
        for(int j=0;j<LED_RES;j++){
            if(((LED_Pixel[i].val >> j) & 1))
                Pixel_Bits[i*24+j] = PT_ONE;
            else
                Pixel_Bits[i*24+j] = PT_ZERO;
        }
    }
    Pixel_Bits[NUM_LEDS*LED_RES] = 0xFFFFFFFF;
}

//******************************************************************************
void LEDStrip_Reset(void){
    PT_Inst->train = 0xFFF;
    PT_Inst->loop = 0;
    MXC_PT_Start(1 << PT_CH);   //start channel so it goes high
    MXC_Delay(100);             //Reset Pulse high


    PT_Inst->train = 0x800;
    MXC_PT_Stop(1 << PT_CH);    //Reset Pulse Low
    MXC_Delay(100);

    NVIC_EnableIRQ(PT_IRQn);    //enabled default interrupt handler
    MXC_PT_EnableInt(0x02);     //enabled interrupts for PT

    PT_Inst->train = 0xFFF;
    MXC_PT_Start(1 << PT_CH);   //start channel so it goes high
    MXC_Delay(100);

    MXC_PT_Stop(1 << PT_CH);    //Reset Pulse Low 
}
