
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "usrcmd.h"



volatile int uxTopUsedPriority ;
TaskHandle_t blinkTaskHandle;


void blink_task(void *arg)
{
    cyhal_gpio_init(CYBSP_USER_LED,CYHAL_GPIO_DIR_OUTPUT,CYHAL_GPIO_DRIVE_STRONG,0);

    for(;;)
    {
    	cyhal_gpio_toggle(CYBSP_USER_LED);
    	vTaskDelay(500);
    }
}




int main(void)
{
    cy_rslt_t result;
    uxTopUsedPriority = configMAX_PRIORITIES - 1 ; // enable OpenOCD Thread Debugging

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    __enable_irq();

    cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    // Stack size in WORDs
    // Idle task = priority 0
    xTaskCreate(blink_task, "blinkTask", configMINIMAL_STACK_SIZE,0 /* args */ ,0 /* priority */, &blinkTaskHandle);
    xTaskCreate(usrcmd_task, "nt shell task", configMINIMAL_STACK_SIZE*2,0 /* args */ ,0 /* priority */, 0);
    vTaskStartScheduler();


}

/* [] END OF FILE */
