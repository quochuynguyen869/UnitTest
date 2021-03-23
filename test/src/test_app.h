#ifndef __TEST_APP_H
#define __TEST_APP_H
//#include "cmsis_os.h"
//#include "uart.h"

typedef int osThreadId;



void UartInit(void);

typedef void (*fp)(void const*); //function pointer
int UartReceiveTask, osPriorityNormal;
void osThreadDef(int UartReceiveTask, fp StartUartReceiveTask, int osPriorityNormal, int a, int b);

int osThread(int UartReceiveTask);
int osThreadCreate(int, int *p);

int  osPriorityIdle, UartHandlerTask;
void osThreadDef(int UartHandlerTask, fp StartUartHandlerTask,int osPriorityIdle, int a, int b);

int UserNotifyTask;
void osThreadDef(int UserNotifyTask, int StartUserNotifyTask,int osPriorityIdle, int a, int b);

void osKernelStart(void);

void HAL_UART_RxCpltCallback(void const * argument);






///////////////////////////////////

#include <stdlib.h>
#define DATA_LENGTH_RX (int)20
#define NUL (int)0
#define NUM_TC (int)3
void StartUartReceiveTask(void const * argument);
void StartUartHandlerTask(void const * argument);
void StartUserNotifyTask(void const * argument);

void TestAppInit(void);
void SpiTest (int value);
void I2cTest (int value);
void TestAll (int value);
#endif
