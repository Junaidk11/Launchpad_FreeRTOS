/**
 * main.c
 */

#include "FreeRTOS.h"
#include "os_task.h"
#include "sci.h"
#include "string.h"
#include <stdio.h>


TaskHandle_t task1Handler = NULL;
TaskHandle_t task2Handler = NULL;

void task1(void *p){

     TickType_t myLastTickCount;

     myLastTickCount = xTaskGetTickCount();
    // Send notification from task1 to task2


    while(1){

            /*
             *  The following function will send a 32-bit value to task2. The 32-bit value by default is '0'.
             *  When this function is called, the 32-bit value is incremented by 1 and sent to task2
             */

            // Send notification 1
          xTaskNotifyGive(task2Handler);
           // Send notification 2
          xTaskNotifyGive(task2Handler);
           // Send notification 3
          xTaskNotifyGive(task2Handler);
           // Send notification 4
          xTaskNotifyGive(task2Handler);

          // Create a 5 second delay
          vTaskDelayUntil(&myLastTickCount, pdMS_TO_TICKS(5000));



    }
}

void task2(void *p){


    int notificationValue;
    char messageBuffer[30];
    while(1){

        /*
         *  The following function will receive a 32-bit value from task1. The 32-bit value by default is '0'.
         *  When this function is called, the second argument is used to tell FreeRTOS kernel to either decrement the 32-bit value received  by 1 or reset it.
         *      uint32_t value returned  <---  ulTaskNotifyTake(DecrementOrReset, BlockingTime);
         *
         *      if first argument = 0 -> the 32-bit value received from task 1 is decremented by 1
         *      if first argument = 1 -> the 32-bit value received from task 1 is Reset after reading.
         *
         */
        notificationValue = ulTaskNotifyTake(pdFALSE, (TickType_t)portMAX_DELAY);


        if(notificationValue > 0){
            sprintf(messageBuffer, "Notification Received: %d. \r\n", notificationValue);
            sciSend(scilinREG, (uint32_t)sizeof(messageBuffer), (uint8 *)messageBuffer);
        }



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

    xTaskCreate(task2,
                "task2",
                200,
                (void *)0,
                tskIDLE_PRIORITY,
                &task2Handler);

    vTaskStartScheduler();

	while(1);
}
