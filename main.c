/*******************************************************************************
* File Name:   main.c
*
* Description: This example demonstrates a Position Interface (POSIF) module
*              in Quadrature Decoder (QD) mode and uses the CCU40 module to
*              determine the speed of rotation of the encoder. LED1 turns ON
*              when the encoder rotates in clockwise direction; LED2 turns ON
*              when the rotation is anti-clockwise.
*
* Related Document: See README.md
*
********************************************************************************
*
* Copyright (c) 2020, Infineon Technologies AG
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
#include "xmc_posif.h"
#include "retarget_io.h"
#include "stdio.h"

/*******************************************************************************
*  Macros
*******************************************************************************/
/* Define macros for XMC14x Boot kit */
#ifdef TARGET_KIT_XMC14_BOOT_001
#define INPUT_0  (XMC_POSIF_INPUT_PORT_B)     /* Choice of INPUT_PORT_B */
#define INPUT_1  (XMC_POSIF_INPUT_PORT_A)     /* Choice of INPUT_PORT_A */
#define INPUT_2  (XMC_POSIF_INPUT_PORT_A)     /* Choice of INPUT_PORT_A */

#define LED_ON   (XMC_GPIO_OUTPUT_LEVEL_LOW)  /* LED ON for XMC14x Boot kit */
#define LED_OFF  (XMC_GPIO_OUTPUT_LEVEL_HIGH) /* LED OFF for XMC14x Boot kit */

/* Get the prescaler value used in CCU40 Slice */
#define PRESCALER_VALUE  (1 << (XMC_CCU4_SLICE_GetPrescaler(CAPTURE_0_HW)))

/* Get ccu clock frequency in MegaHertz */
#define CLOCK            (XMC_SCU_CLOCK_GetFastPeripheralClockFrequency()/1000000)
#endif

/* Define macros for XMC47x Relax kit */
#ifdef TARGET_KIT_XMC47_RELAX_V1
#define INPUT_0  (XMC_POSIF_INPUT_PORT_B)     /* Choice of INPUT_PORT_B */
#define INPUT_1  (XMC_POSIF_INPUT_PORT_B)     /* Choice of INPUT_PORT_B */
#define INPUT_2  (XMC_POSIF_INPUT_PORT_A)     /* Choice of INPUT_PORT_A */

#define LED_OFF  (XMC_GPIO_OUTPUT_LEVEL_LOW)  /* LED OFF for XMC47x Relax kit */
#define LED_ON   (XMC_GPIO_OUTPUT_LEVEL_HIGH) /* LED ON for XMC47x Relax kit */

/* Get the prescaler value used in CCU40 Slice */
#define PRESCALER_VALUE  (1 << (XMC_CCU4_SLICE_GetPrescaler(CAPTURE_0_HW)))

/* Get ccu clock frequency in MegaHertz */
#define CLOCK            (XMC_SCU_CLOCK_GetCcuClockFrequency()/1000000)
#endif

/* Every time that a capture trigger 0 occurs, CCcapt0, the actual value of the
 * timer is captured into the capture register 1 and the previous value stored in
 * this register is transferred into capture register 0
 */
#define CAPTURE_0_VALUE_RISING_EDGE_REGISTER  (1U)

/*******************************************************************************
* Data Structures
*******************************************************************************/
/* POSIF module configuration */
XMC_POSIF_CONFIG_t posif_config =
{
    .mode   = XMC_POSIF_MODE_QD,              /* POSIF Operational mode */
    .input0 = INPUT_0,                        /* Choice of input for Phase-A */
    .input1 = INPUT_1,                        /* Choice of input for Phase-B */
    .input2 = INPUT_2,                        /* Choice of input for Index */
    .filter = XMC_POSIF_FILTER_2_CLOCK_CYCLE, /* Input filter configuration */
};

/* POSIF quadrature decoder configuration */
XMC_POSIF_QD_CONFIG_t posif_qd_config =
{
    .mode          = XMC_POSIF_QD_MODE_QUADRATURE, /* Standard Quadrature Mode */
    .phase_a       = 0U,                           /* Phase A is active High */
    .phase_b       = 0U,                           /* Phase B is active High */
    .phase_leader  = 0U,                           /* Phase A is the leading signal for clockwise rotation */
    .index         = 0U                            /* No index marker generation on POSIF0.OUT3 */
};

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function. It starts POSIF Module in Quadrature Decoder (QD)
* mode and uses the CCU40 module to determine the speed of rotation of the
* encoder. LED1 turns ON when the encoder rotates in clockwise direction;
* LED2 turns ON when the rotation is anti-clockwise. Index signal time period is
* displayed on terminal.
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

    /* Current state of phase signals */
    uint8_t state;

    /* Capture timer value on index signal rising edge */
    uint16_t capture_value;

    /* Index signal time period */
    uint32_t index_signal;

    /* Direction of rotation */
    XMC_POSIF_QD_DIR_t direction;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Initialize retarget-io to use the debug UART port */
    retarget_io_init();

    printf("======================== \r\n");
    printf("XMC POSIF QD example \r\n");
    printf("======================== \r\n");

    /* Initialize POSIF module */
    XMC_POSIF_Init(POSIF0, &posif_config);

    /* Initializes quadrature decoder control mode */
    XMC_POSIF_QD_Init(POSIF0, &posif_qd_config);

    /* Start the Encoder */
    XMC_POSIF_Start(POSIF0);

    /* Start CCU4 timer */
    XMC_CCU4_SLICE_StartTimer(CAPTURE_0_HW);

    while (1)
    {
        /* Check for a rising edge event in the index signal */
        if (XMC_CCU4_SLICE_GetEvent(CAPTURE_0_HW, XMC_CCU4_SLICE_IRQ_ID_EVENT0))
        {
            /* Clear event*/
            XMC_CCU4_SLICE_ClearEvent(CAPTURE_0_HW, XMC_CCU4_SLICE_IRQ_ID_EVENT0);

            /* Get captured timer value on index signal rising edge */
            capture_value = XMC_CCU4_SLICE_GetCaptureRegisterValue(CAPTURE_0_HW, CAPTURE_0_VALUE_RISING_EDGE_REGISTER);

            /* Calculates the Index signal time period */
            index_signal = (uint32_t)((capture_value * PRESCALER_VALUE)/CLOCK);

            /* Prints Index signal time period in micro seconds */
            printf(" Index signal time period: %luus\r\n", index_signal);

            /* Check the direction of rotation of the motor */
            direction = XMC_POSIF_QD_GetDirection(POSIF0);

            /* Check the current state of phase signals in quadrature decoder mode */
            state = XMC_POSIF_QD_GetCurrentState(POSIF0);

            /* Check the current state and direction */
            if( (direction == 1) && (state != 0))
            {
                /* Clockwise rotation */
                XMC_GPIO_SetOutputLevel(CYBSP_USER_LED1_PORT, CYBSP_USER_LED1_PIN, LED_ON);
                XMC_GPIO_SetOutputLevel(CYBSP_USER_LED2_PORT, CYBSP_USER_LED2_PIN, LED_OFF);
            }
            else if( (direction == 0) && (state != 0))
            {
                /* Anti-clockwise rotation */
                XMC_GPIO_SetOutputLevel(CYBSP_USER_LED1_PORT, CYBSP_USER_LED1_PIN, LED_OFF);
                XMC_GPIO_SetOutputLevel(CYBSP_USER_LED2_PORT, CYBSP_USER_LED2_PIN, LED_ON);
            }
            else
            {
                /* No rotation */
                XMC_GPIO_SetOutputLevel(CYBSP_USER_LED1_PORT, CYBSP_USER_LED1_PIN, LED_OFF);
                XMC_GPIO_SetOutputLevel(CYBSP_USER_LED2_PORT, CYBSP_USER_LED2_PIN, LED_OFF);
            }
        }
    }
}

/* [] END OF FILE */
