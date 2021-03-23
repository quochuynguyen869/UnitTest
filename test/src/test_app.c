#include "test_app.h"
char UserOption[4] = {'0', '1', '2', '\0'};
char RX_buff[DATA_LENGTH_RX] = {'\0'};

osThreadId UartReceiveTaskHandle;
osThreadId UartHandlerTaskHandle;
osThreadId UserNotifyTaskHandle;

void (*func_table []) (int value) =
  {
	SpiTest,
	I2cTest,
	TestAll,
  };
	
char menu[] = "**************************************\n\
	Please select your test:\n\
	0. Test SPI\n\
	1. Test I2C\n\
	2. Test All\n";

void TestAppInit(){
	UartInit();
	
  osThreadDef(UartReceiveTask, StartUartReceiveTask, osPriorityNormal, 0, 128);
  UartReceiveTaskHandle = osThreadCreate(osThread(UartReceiveTask), NULL);

  osThreadDef(UartHandlerTask, StartUartHandlerTask, osPriorityIdle, 0, 128);
  UartHandlerTaskHandle = osThreadCreate(osThread(UartHandlerTask), NULL);

  osThreadDef(UserNotifyTask, StartUserNotifyTask, osPriorityIdle, 0, 128);
  UserNotifyTaskHandle = osThreadCreate(osThread(UserNotifyTask), NULL);
	
	osKernelStart();
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	static int RX_index = 0;
	if(huart->Instance == huart2.Instance){
		HAL_UART_Receive_IT(&huart2,(uint8_t*)RX_data, ONEBYTE);
		if(RX_index < DATA_LENGTH_RX){
			RX_buff[RX_index] = RX_data[0];
			RX_index++;
			if (RX_data[0] == NULL){
				xTaskNotifyFromISR(UartReceiveTaskHandle, 0, eNoAction, NULL);
				RX_index = 0;
			}
		}	
	}
}

void StartUartReceiveTask(void const * argument)
{
	static int firstInit = 1;
  for(;;)
  {
		if (firstInit){
			xTaskNotifyFromISR(UserNotifyTaskHandle, 0, eNoAction, NULL);
			firstInit --;
		}
		xTaskNotifyWait(0, 0x00, NULL ,portMAX_DELAY);
		if (strstr(UserOption, RX_buff)){
			xTaskNotify(UartHandlerTaskHandle, atoi(RX_buff), eSetBits);
		}
		else{
			UartSend("!!! Invalid option. Please choose again !!!\n");
			
		}
		memset(RX_buff,'\0',sizeof(RX_buff));
		osDelay(10);
  }
}


void StartUartHandlerTask(void const * argument)
{

	uint32_t ulNotifiedValue;
  for(;;)
  {
		xTaskNotifyWait(1, 0xffffffff, &ulNotifiedValue ,portMAX_DELAY);
		((*func_table [ulNotifiedValue]) (0));
		xTaskNotifyFromISR(UserNotifyTaskHandle, 0, eNoAction, NULL);
    osDelay(1);
  }

}

void StartUserNotifyTask(void const * argument)
{
  for(;;)
  {
		xTaskNotifyWait(0, 0x00, NULL ,portMAX_DELAY);
		UartSend(menu);
    osDelay(1);
  }

}

void SpiTest (int value)
{
	UartSend("-> -> Test SPI is executed\n");
}

void I2cTest (int value)
{
	UartSend("-> -> Test I2C is executed\n");
}

void TestAll (int value)
{
	UartSend("-> Test All\n");
	int cnt;
	// except Test All
	for(cnt = 0; cnt < NUM_TC - 1; cnt++){
		((*func_table [cnt]) (0));
	}		
}
