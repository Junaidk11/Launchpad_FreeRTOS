/**
 * main.c
 */

#include "FreeRTOS.h"
#include "os_task.h"
#include "sci.h"
#include "string.h"



TaskHandle_t task1Handler = NULL;
TaskHandle_t task2Handler = NULL;

void task1(void *p){

    int counter = 0;
    // P is type-casted as a pointer to an integer -> (int *)p, the int[(int*)p] type-casts the pointer to a data variable of type 'int', and the resulting value is assigned to the integer variable messageTypeRetrieving
    int messageTypeRetriving = (int)(int *)p;  // The void pointer is type-casted to ensure it is pointing to data type of intended data// Arrays are immutable - once defined, cannot be changed without re-initialization.

    while(1){

        if (messageTypeRetriving!=2){
            uint8 message [] = "Task 1 is executing every 1 second.\r\n";
            sciSend(scilinREG, (uint32_t) sizeof(message), &message[0]);
            counter++;
            vTaskDelay(1000);

        }else if(messageTypeRetriving==2){
            uint8 message [] = "Task 1 is executing every 0.5 second.\r\n";
            sciSend(scilinREG, (uint32_t) sizeof(message), &message[0]);
            counter++;
            vTaskDelay(500);
        }

        if (counter == 15){

            vTaskDelete(task1Handler); // Deleting task after counter reaches 10, argument is your task's handler.
        }
    }
}


void task2(void *p){

    TickType_t myLastTickCount;
    myLastTickCount = xTaskGetTickCount(); // Get current tick count

    uint8 message[] ="Task 2 has suspended Task 1. \r\n";
    uint8 message1[] = "After suspending task 1, 1 Second Delay starting now.\r\n";
    uint8 message2[] ="Resuming Task 1 now.\r\n";

    while(1){
            // Create 2 Second Delay before suspending Task 1 -> This will allow Task 1 to run for atleast 2 seconds.
            vTaskDelayUntil(&myLastTickCount, pdMS_TO_TICKS(2000));
            // Suspend Task 1  using vTaskSuspend(taskHandler)
            vTaskSuspend(task1Handler);

            sciSend(scilinREG, (uint32_t)sizeof(message), &message[0]);

           // Create a delay of 1 Seconds after Suspending Task 1.
           sciSend(scilinREG, (uint32_t)sizeof(message1), &message1[0]);
           vTaskDelayUntil(&myLastTickCount, pdMS_TO_TICKS(1000));

           // Resuming Task 1 now.
           sciSend(scilinREG, (uint32_t)sizeof(message2), &message2[0]);
           vTaskResume(task1Handler);

           // Allow Task 1 to run for 2 seconds
           vTaskDelayUntil(&myLastTickCount, pdMS_TO_TICKS(2000));

    }
}

int main(void)
{
    // Arrays are immutable - once defined, cannot be changed without re-initialization.
    sciInit();

    int messageType = 2;
    uint8 message [] = "Inside Main.\r\n";
    sciSend(scilinREG, (uint32_t) sizeof(message), &message[0]);

    // Creating two Tasks

    xTaskCreate(task1,
                "task1",
                200,
                (void *)messageType,
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
