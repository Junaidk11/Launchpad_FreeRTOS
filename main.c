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
    /*
     * Looking at a variation of Sending Notification from one task to another.
     * In this setup, we set the desired bits of the 32-bit unsigned notification value and send them./
     * The different bits of the 32-bit notification value can be used as flags by the receiving task.
     */


    while(1){

            xTaskNotify(task2Handler, (1<<3) | (1<<2) | (1<<1) | (1<<0),eSetBits);  //Set bit 0, 1, 2
            //vTaskDelayUntil(&myLastTickCount, pdMS_TO_TICKS(1000)); // 1 Second delay



            //xTaskNotify(task2Handler, (1<<3),eSetValueWithOverwrite);  //Set bit 3 of Notification value, and then increment the Notification value by 1.
            vTaskDelayUntil(&myLastTickCount, pdMS_TO_TICKS(1000)); // 1 Second delay

    }
}

void task2(void *p){

    TickType_t myLastTickCount;
    myLastTickCount = xTaskGetTickCount();
    char messageBuffer[15];
    uint32_t ulNotificationValue; // To store the received notification value
    while(1){

        if(xTaskNotifyWait(( (1<<3) | (1<<2) | (1<<1) |(1<<0)), 0, &ulNotificationValue, portMAX_DELAY)== pdTRUE){
            // Notification value received, cleared bits 0,1,2, not clearing bit 3
            if (ulNotificationValue & (7<<0)){  // Checking if all 3 bits were set in the notification
                sprintf(messageBuffer, "Green.\r\n");
                sciSend(scilinREG,(uint32_t)sizeof(messageBuffer),(uint8 *)messageBuffer);
            }else if(ulNotificationValue & (1<<3)){  // Checking if bit 3 of the Notification value is set
                sprintf(messageBuffer, "Blue.\r\n");
                sciSend(scilinREG,(uint32_t)sizeof(messageBuffer),(uint8 *)messageBuffer);
            }

            if(ulNotificationValue & (1<<3)){  // Checking if bit 3 of the Notification value is set
                sprintf(messageBuffer, "Blue.\r\n");
                sciSend(scilinREG,(uint32_t)sizeof(messageBuffer),(uint8 *)messageBuffer);
            }
        }
        vTaskDelayUntil(&myLastTickCount, pdMS_TO_TICKS(1000)); // Block Task, allowing the next notification to come in.

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
