/**
 * main.c
 */

#include "FreeRTOS.h"
#include "os_task.h"
#include "sci.h"
#include "string.h"


TaskHandle_t task1Handler = NULL;

void task1(void *p){

    // P is typecasted as a pointer to an integer -> (int *)p, the int[(int*)p] type-casts the pointer to a data variable of type 'int', and the resulting value is assigned to the integer variable messageTypeRetrieving
    int messageTypeRetriving = (int)(int *)p;  // The void pointer is type-casted to ensure it is pointing to data type of intended data// Arrays are immutable - once defined, cannot be changed without re-initialization.

    while(1){

        if (messageTypeRetriving!=2){
            uint8 message [] = "Task 1 is executing every 1 second.\r\n";
            sciSend(scilinREG, (uint32_t) sizeof(message), &message[0]);
            vTaskDelay(1000);
        }else if(messageTypeRetriving==2){
            uint8 message [] = "Task 1 is executing every 0.5 second.\r\n";
            sciSend(scilinREG, (uint32_t) sizeof(message), &message[0]);
            vTaskDelay(500);
        }
    }
}

int main(void)
{
    // Arrays are immutable - once defined, cannot be changed without re-initialization.
    sciInit();

    int messageType = 2;
    uint8 message [] = "Inside Main.\r\n";
    sciSend(scilinREG, (uint32_t) sizeof(message), &message[0]);

    // Passing arguments to a task;


        xTaskCreate(task1,
                    "task1",
                    200,
                    (void *)messageType,
                    tskIDLE_PRIORITY,
                    &task1Handler);

        vTaskStartScheduler();

	while(1);
}
