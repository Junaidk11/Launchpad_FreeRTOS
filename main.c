/**
 * main.c
 */

#include "FreeRTOS.h"
#include "os_task.h"
#include "sci.h"
#include "string.h"
#include "stdio.h"
#include "os_semphr.h" // <-- Include semaphore header file to get Mutex APIs
#include "os_queue.h"



TaskHandle_t task1Handler = NULL;
TaskHandle_t task2Handler = NULL;


char sharedResource[10]; // The shared Resource written to by both task1 & task2

//SemaphoreHandle_t xMutex; //1. Create a semaphore handle, but in the main function you need to tell the compiler it is a MUTEX

QueueHandle_t xMutex;

/*
 *  We will setup a scenario where concurrency happens and this will allow to understand the importance of mutexes.
 *
 *  Now use mutex to protect critical sections of each task to protect the shared resource
 */

//3. Define Critical Sections of your 2 tasks that will write to the shared Resource.
//4. Place critical section inside if block.
//5. Place xSemaphoreTake(MutexHandler,BlockingTime) as if-block conditional statement. The function xSemaphoreTake will return true if the Mutex is unlock, i.e. available, else false
//6. Before exiting critical section, return unlock the mutex guarding the shared resource -> xSemaphoreGive(MutexHandler);

void task1(void *p){

    TickType_t myLastTickCount;
    myLastTickCount = xTaskGetTickCount();

    char task1Buffer[] = "potatoes";
    char task1Message[19];
    char task1Message2[37];

    while(1){

        /**************************************** Critical Section of this Task ****************************************/
        if(xSemaphoreTake(xMutex,portMAX_DELAY)){ // using indefinite blocking time for now.
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

         /**************************************** Critical Section of this Task ****************************************/

             xSemaphoreGive(xMutex);
        }else{

            sprintf(task1Message2,"Shared Resource Protected by Mutex.\r\n");
            sciSend(scilinREG, (uint32_t)sizeof(task1Message2), (uint8 *)task1Message2);
            vTaskDelayUntil(&myLastTickCount, pdMS_TO_TICKS(1000)); // 1000 millisecond delay
        }
    }
}


void task2(void *p){


    TickType_t myLastTickCount;
    myLastTickCount = xTaskGetTickCount();

    char task2Buffer[] = "tomatoes";
    char task2Message[19];
    char task2Message2[37];

    while(1){

        /**************************************** Critical Section of this Task ****************************************/
        if(xSemaphoreTake(xMutex,portMAX_DELAY)){ // using indefinite blocking time for now.
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
        /**************************************** Critical Section of this Task ****************************************/

              xSemaphoreGive(xMutex);
        }else{
            sprintf(task2Message2,"Shared Resource Protected by Mutex.\r\n");
            sciSend(scilinREG, (uint32_t)sizeof(task2Message2),(uint8 *) task2Message2);
            vTaskDelayUntil(&myLastTickCount, pdMS_TO_TICKS(1000)); // 1000 millisecond delay
        }
    }
}
int main(void)
{
    sciInit();

    uint8 message [] = "Inside Main.\r\n";
    sciSend(scilinREG, (uint32_t) sizeof(message), &message[0]);

    xMutex = xSemaphoreCreateMutex(); // 2. Tell main function to initialize semaphore as a Mutex


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
