
/*******************************************************************************
* File Name:   main.c
*
* Description: This example demonstrates a Position Interface (POSIF) module
*              in Quadrature Decoder (QD) mode and uses the CCU40 module to
*              determine the speed of rotation and the position of the encoder.
*              LED1 turns ON when the encoder rotates in clockwise direction;
*              LED2 turns ON when the rotation is anti-clockwise.
*
* Related Document: See README.md
*
********************************************************************************
*
* Copyright (c) 2022, Infineon Technologies AG
* All rights reserved.
*
* Boost Software License - Version 1.0 - August 17th, 2003
* Permission is hereby granted, free of charge, to any person or organization
* obtaining a copy of the software and accompanying documentation covered by
* this license (the "Software") to use, reproduce, display, distribute,
* execute, and transmit the Software, and to prepare derivative works of the
* Software, and to permit third-parties to whom the Software is furnished to
* do so, all subject to the following:
*
* The copyright notices in the Software and this entire statement, including
* the above license grant, this restriction and the following disclaimer,
* must be included in all copies of the Software, in whole or in part, and
* all derivative works of the Software, unless such copies or derivative
* works are solely in the form of machine-executable object code generatd by
* a source language processor.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
* SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
* FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*
*******************************************************************************/

#include "cybsp.h"
#include "cy_utils.h"
#include <stdio.h>
#include "cy_retarget_io.h"

/* Define macro to enable/disable printing of debug messages */
#define ENABLE_XMC_DEBUG_PRINT (0)

/* Define macro to set the loop count before printing debug messages */
#if ENABLE_XMC_DEBUG_PRINT
static bool loop_entered = false;
static bool LED_turned_on_once = false;
#endif

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function. It starts POSIF Module in Quadrature Decoder (QD)
* mode and uses the CCU40 module to determine the speed of rotation of the
* encoder. LED1 turns ON when the encoder rotates in clockwise direction;
* LED2 turns ON when the rotation is anti-clockwise. Relative position and
* speed are displayed in serial terminal
*
* Parameters:
*  none
* Return:
*  int
*
*******************************************************************************/
 int main(void)
{
    cy_rslt_t result;

    /* POSITION_COUNTER timer value on TIMESTAMP_TRIGGER slice Status Signal falling edge */
    uint16_t position_value = 0;
    /* TIME_BETWEEN_TICKS timer value on QUADRATURE_DECODER Period Clock rising edge */
    uint16_t time_between_ticks = 0;
    /* Direction of rotation */
    XMC_POSIF_QD_DIR_t direction;
    /* Counters for slower printing on serial terminal */
    int32_t phase_b_pulse_counter = 0;
    int32_t previous_phase_b_pulse_counter = -1;

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Initialize retarget-io to use the debug UART port */
    cy_retarget_io_init(CYBSP_DEBUG_UART_HW);

    #if ENABLE_XMC_DEBUG_PRINT
    printf("Initialization done\r\n");
    #else
    /* Show welcome message on terminal during initialization */
    printf("======================== \r\n");
    printf("XMC POSIF QD example \r\n");
    printf("======================== \r\n");
    #endif

    /* Start the Encoder */
    XMC_POSIF_Start(QUADRATURE_DECODER_HW);

    /* Start CCU8 timers */
    XMC_CCU8_SLICE_StartTimer(PHASE_A_HW);
    XMC_CCU8_SLICE_StartTimer(PHASE_B_HW);

    /* Start CCU4 timers */
    XMC_CCU4_SLICE_StartTimer(POSITION_COUNTER_HW);
    XMC_CCU4_SLICE_StartTimer(TIME_BETWEEN_TICKS_HW);

    while (1)
    {
        /* Check for a Period Match while Counting UP event for TIMESTAMP_TRIGGER slice */
        if (XMC_CCU4_SLICE_GetEvent(TIMESTAMP_TRIGGER_HW, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH))
        {
            /* Clear event*/
            XMC_CCU4_SLICE_ClearEvent(TIMESTAMP_TRIGGER_HW, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH);

            /* Get captured timers value on Period Match while Counting UP event for TIMESTAMP_TRIGGER slice falling edge */
            position_value = XMC_CCU4_SLICE_GetCaptureRegisterValue(POSITION_COUNTER_HW, (uint8_t)1);
            time_between_ticks = XMC_CCU4_SLICE_GetCaptureRegisterValue(TIME_BETWEEN_TICKS_HW, (uint8_t)1);

            /* Check the direction of rotation of the motor */
            direction = XMC_POSIF_QD_GetDirection(QUADRATURE_DECODER_HW);

            /* Check the current state and direction */
            if (direction == 1)
            {
                /* Clockwise rotation */
                XMC_GPIO_SetOutputLevel(CYBSP_USER_LED1_PORT, CYBSP_USER_LED1_PIN, XMC_GPIO_OUTPUT_LEVEL_HIGH);
                XMC_GPIO_SetOutputLevel(CYBSP_USER_LED2_PORT, CYBSP_USER_LED2_PIN, XMC_GPIO_OUTPUT_LEVEL_LOW);
            }
            else
            {
                /* Anti-clockwise rotation */
                XMC_GPIO_SetOutputLevel(CYBSP_USER_LED1_PORT, CYBSP_USER_LED1_PIN, XMC_GPIO_OUTPUT_LEVEL_LOW);
                XMC_GPIO_SetOutputLevel(CYBSP_USER_LED2_PORT, CYBSP_USER_LED2_PIN, XMC_GPIO_OUTPUT_LEVEL_HIGH);
            }
            #if ENABLE_XMC_DEBUG_PRINT
            if(!LED_turned_on_once)
            {
                printf("Direction LED Turned on\r\n");
                LED_turned_on_once = true;
            }
            #endif
        }

        /* Slowing down serial print operation */
        if(XMC_CCU8_SLICE_GetEvent(PHASE_B_HW, XMC_CCU8_SLICE_IRQ_ID_PERIOD_MATCH))
        {
            ++phase_b_pulse_counter;
            if(previous_phase_b_pulse_counter != phase_b_pulse_counter)
            {
            #if ENABLE_XMC_DEBUG_PRINT
                if(!loop_entered)
                {
                    printf("Results printed\r\n");
                    loop_entered = true;
                }
            #else
                printf("POSITION: %5d INTERVAL: %lu\r\n", position_value, TIME_BETWEEN_TICKS_TICK_NS * (unsigned long)time_between_ticks);
            #endif
            }
            XMC_CCU8_SLICE_ClearEvent(PHASE_B_HW, XMC_CCU8_SLICE_IRQ_ID_PERIOD_MATCH);
         }
        previous_phase_b_pulse_counter = phase_b_pulse_counter;
    }
}
