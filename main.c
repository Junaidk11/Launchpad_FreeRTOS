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

    TickType_t myLastTickCount;
    myLastTickCount = xTaskGetTickCount();
    while(1){

            uint8 message [] = "Task 1 is executing every 2 second.\r\n";
            sciSend(scilinREG, (uint32_t) sizeof(message), &message[0]);
            vTaskDelayUntil(&myLastTickCount, pdMS_TO_TICKS(2000)); // 2 Second Delay

    }
}


void task2(void *p){


    char myTaskList[300];  // A buffer to hold the information about all tasks running on the kernel.
    TickType_t myLastTickCount;
    myLastTickCount = xTaskGetTickCount();


    uint8 message2 [] ="State of Task 1:\r\n";
    uint8 message3 [] ="State of Task 2:\r\n";
    uint8 message4 [] ="Displaying information on all tasks running on the kernel.\r\n";
    //eTaskState taskStates;
    //uint8 taskState[]="0";
    while(1){

            uint8 message [] = "Task 2 is executing every 1 second.\r\n";
            sciSend(scilinREG, (uint32_t) sizeof(message), &message[0]);
            vTaskDelayUntil(&myLastTickCount, pdMS_TO_TICKS(1000)); // 1 Second Delay

            //taskStates = eTaskGetState(task1Handler); // Get state of task1
            //taskState = taskStates;
            sciSend(scilinREG, (uint32_t) sizeof(message2), &message2[0]);
            //sciSend(scilinREG, (uint32_t) sizeof(taskState), &taskState);



            //taskStates = eTaskGetState(task1Handler); // Get state of task2
            //taskStateHolder[0] = (uint8)taskStates;


            sciSend(scilinREG, (uint32_t) sizeof(message3), &message3[0]);
            //sciSend(scilinREG, (uint32_t) sizeof(taskStates), &taskStates);

            vTaskDelayUntil(&myLastTickCount, pdMS_TO_TICKS(1000)); // 1 Second Delay - this will allow Task 1 to run

            vTaskList(myTaskList); // This function will store the task information of all the tasks running of the freeRTOS kernel.
            sciSend(scilinREG, (uint32_t) sizeof(message4), &message4[0]);
            sciSend(scilinREG, (uint32_t) sizeof((uint8)myTaskList), &myTaskList[0]);

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
                tskIDLE_PRIORITY+1,
                &task2Handler);
        vTaskStartScheduler();

	while(1);
}