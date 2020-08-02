/**
 * main.c
 */

#include "FreeRTOS.h"
#include "os_task.h"
#include "sci.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

TaskHandle_t task1Handler = NULL;

void task1(void *p){


    while(1){


            uint8 message [] = "Task 1 is executing every 1 second.\r\n";
            sciSend(scilinREG, (uint32_t) sizeof(message), &message[0]);
            vTaskDelay(1000);
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
