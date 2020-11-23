/**
 * main.c : This main function is the test code written to implement interrupt handling in FreeRTOS
 */

#include "FreeRTOS.h"
#include "os_task.h"
#include "sci.h"
#include "string.h"


TaskHandle_t task1Handler = NULL;
TaskHandle_t interruptHandler = NULL;
TaskHandle_t task3Handler = NULL;

volatile int counter = 0;

void task1(void *p){


    while(1){

            uint8 message [] = "Task 1 is executing every 1 second.\r\n";
            sciSend(scilinREG, (uint32_t) sizeof(message), &message[0]);
            vTaskDelay(1000);
            counter+=10; // Increment counter by  10 after delay.

    }
}

void interruptRoutineTask(void *p){
    uint8 message [] = "Interrupt Handling Task is suspended.\r\n";
    uint8 message1 [] = "Interrupt Handling Task is resumed.\r\n";
    while(1){
        /*
         * The following line will suspend the task itself.
         * This task will suspend itself after this line and won't execute anything after until it is resumed by the ISR handler.
         */
        sciSend(scilinREG, (uint32_t) sizeof(message), &message[0]);
        vTaskSuspend(NULL);
        sciSend(scilinREG, (uint32_t) sizeof(message1), &message1[0]);
        counter = 0; // Refresh the counter.

    }
}
void ISR_Handler(){

    uint8 message [] = "ISR Handler is resuming the task now.\r\n";
    sciSend(scilinREG, (uint32_t) sizeof(message), &message[0]);
    vTaskResume(interruptHandler);

    // Yield?
    //vPortYield();   // Not sure if we need to Yield here because this function is not a task.


    /*
     *  The Following Lines are used if hardware interrupt used. In this example we're emulating a software interrupt.
     */
    //BaseType_t checkIfYieldRequired;
    //checkIfYieldRequired = xTaskResumeFromISR(interruptHandler);
    //portYIELD_FROM_ISR(checkIfYieldRequired);



}

void emulatingInterrupt(void *p){

    TickType_t lastTickCount;
    lastTickCount = xTaskGetTickCount();

    uint8 message [] = "CPU Interrupted.\r\n";
    while(1){

        vTaskDelayUntil(&lastTickCount, pdMS_TO_TICKS(5000));  // Create a Delay of 5 seconds, this will block this task for 5 seconds, allowing task 1 to run.
        sciSend(scilinREG, (uint32_t) sizeof(message), &message[0]);
        ISR_Handler();  // Call the ISR handler
    }
}


int main(void)
{
    sciInit();

    uint8 message [] = "Inside Main.\r\n";
    sciSend(scilinREG, (uint32_t) sizeof(message), &message[0]);
        xTaskCreate(task1,
                    "task1",
                    150,
                    (void *)0,
                    tskIDLE_PRIORITY,
                    &task1Handler);

        xTaskCreate(interruptRoutineTask,
                    "interruptRoutineTask",
                    150,
                    (void *)0,
                    tskIDLE_PRIORITY+1,  // Higher priority than the previous task. Getting Garbage output on terminal when both at same priority.
                    &interruptHandler);

        xTaskCreate(emulatingInterrupt,
                            "EmulatingAnInterrupt",
                            150,
                            (void *)0,
                            tskIDLE_PRIORITY,
                            &task3Handler);

        vTaskStartScheduler();

	while(1);
}
