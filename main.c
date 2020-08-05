/**
 * main.c
 */

#include "FreeRTOS.h"
#include "os_task.h"
#include "sci.h"
#include "string.h"
#include "os_queue.h"
#include "stdio.h"


TaskHandle_t task1Handler = NULL;
TaskHandle_t task2Handler = NULL;

QueueHandle_t myQueue;
void task1(void *p){

    char txBuffer[18]; // Character buffer to store 30 characters.

    // Create Queue of 5 blocks with each block the size of 30 characters.
    myQueue = xQueueCreate(5,sizeof(txBuffer));

    // Populate txBuffer
    sprintf(txBuffer, "QueueDataBlock1.\r\n");

    // Push txBuffer contents to the Front of the Queue
    xQueueSendToFront(myQueue, (void *)txBuffer, (TickType_t)0);

    // Update txBuffer
    sprintf(txBuffer, "QueueDataBlock2.\r\n");

    // Push updated txBuffer contents to the front of the Queue, i.e. this will push previous element down/one block over
    xQueueSendToFront(myQueue,(void*)txBuffer,(TickType_t) 0);

    // Update txBuffer again
    sprintf(txBuffer, "QueueDataBlock3.\r\n");

    // Push updated txBuffer contents to the front of the Queue, i.e. this will push previous element down/one block over
    xQueueSendToFront(myQueue,(void*)txBuffer,(TickType_t) 0);

    while(1){

            //uint8 message [] = "Task 1 is executing every 1 second.\r\n";
            //sciSend(scilinREG, (uint32_t) sizeof(message), &message[0]);
            //vTaskDelay(500);

    }
}

void task2(void *p){

    // Create a rxDataBuffer of same size as the buffer used by task 1 to fill Queue blocks
    char rxBuffer[18];

    while(1){

        // check if Queue is populated/ There is data to be received from the Queue

        if(myQueue!=0){

            if(xQueueReceive(myQueue, (void *)rxBuffer, (TickType_t)5)){

                   // Print each message received from Task 1
                sciSend(scilinREG, (uint32_t)sizeof(rxBuffer), &rxBuffer[0]);
                vTaskDelay(1000); // A 1 second delay
            }
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
                    &task1Handler);

    vTaskStartScheduler();

	while(1);
}
