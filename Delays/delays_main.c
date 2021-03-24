/**
 * main.c
 */

#include "FreeRTOS.h"
#include "os_task.h"
#include "sci.h"
#include "string.h"
#include "FreeRTOSConfig.h"
#include "os_projdefs.h"


TaskHandle_t task1Handler = NULL;

void task1(void *p){


    TickType_t myLastTickCount;
    myLastTickCount = xTaskGetTickCount();
    while(1){
            uint8 message [] = "Task 1 is executing every 1 second.\r\n";
            sciSend(scilinREG, (uint32_t) sizeof(message), &message[0]);
            //vTaskDelay(1000); // Getting a delay

            /*  For 1 second delay, use the kernel tick time defined in FreeRTOSConfig.h */
            //vTaskDelay(1000 * configTICK_RATE_HZ /1000); // This is more precise then the previous Delay, but not most accurate delay of 1 second.

            /*  Instead of writing the equation above -> "1000 * configTICK_RATE_HZ /1000" to get the number of ticks
             *  corresponding to your desired delay in millisecond, you can use a function called pdMS_TO_TICKS(millisecond), returns
             *     Tick Time*/
            //vTaskDelay(pdMS_TO_TICKS(1000));  // This gives a delay of "alteast" 1 second, but is slightly more than a 1 second -> Explained in FreeRTOS doc


            /* To get a more accurate delay of 1 Second - use vTaskDelayUntil(currentTickTime, DesiredMsInTickTime) */
            vTaskDelayUntil(&myLastTickCount,pdMS_TO_TICKS(1000)); // This will give you exactl 1 second block Time for this task.

            /* Anything after the previous statement will only be executed once the Delay is completed. */


    }
}
int main(void)
{
    sciInit();
    uint8 message [] = "Inside Main.\r\n";
    sciSend(scilinREG, (uint32_t) sizeof(message), &message[0]);
        xTaskCreate(task1,
                    "task1",
                    200,
                    (void *)0,
                    tskIDLE_PRIORITY,
                    &task1Handler);
    vTaskStartScheduler();

	while(1);
}
