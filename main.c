/**
 * main.c
 */

#include "FreeRTOS.h"
#include "os_task.h"
#include "sci.h"
#include "string.h"
#include "stdio.h"


TaskHandle_t task1Handler = NULL;
TaskHandle_t task2Handler = NULL;


char sharedResource[10]; // The shared Resource written to by both task1 & task2


/*
 *  We will setup a scenario where concurrency happens and this will allow to understand the importance of mutexes.
 */

void task1(void *p){

    TickType_t myLastTickCount;
    myLastTickCount = xTaskGetTickCount();

    char task1Buffer[] = "potatoes";
    char task1Message[19];

    while(1){

         int i;
         for(i=0; i<9 ; i++){

             // Write one character at a time
             sharedResource[i] = task1Buffer[i];
             vTaskDelayUntil(&myLastTickCount, pdMS_TO_TICKS(50)); // 50 millisecond delay
         }
         // Null character at the end to indicate end of C - string - @i=9 insert NULL character
         sharedResource[i] = '\0'; // is it '0' or '/0'??

         sprintf(task1Message,"T1:%s.\r\n",sharedResource);
         sciSend(scilinREG, (uint32_t)sizeof(task1Message), (uint8 *)task1Message);
    }
}


void task2(void *p){


    TickType_t myLastTickCount;
    myLastTickCount = xTaskGetTickCount();

    char task2Buffer[] = "tomatoes";
    char task2Message[19];

    while(1){

         int i;
         for(i=0; i<9 ; i++){

             // Write one character at a time
             sharedResource[i] = task2Buffer[i];
             vTaskDelayUntil(&myLastTickCount, pdMS_TO_TICKS(50)); // 50 millisecond delay
         }
         // Null character at the end to indicate end of C - string - @i=9 insert NULL character
          sharedResource[i] = '\0'; // is it '0' or '/0'??

          sprintf(task2Message,"T2:%s.\r\n",sharedResource);
          sciSend(scilinREG, (uint32_t)sizeof(task2Message),(uint8 *) task2Message);
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
