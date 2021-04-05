/*
 * main.c
 *
 *  Created on: 9 mar. 2021
 *      Author: adidi
 */

#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
void HolaMundoTask( void *pvParameters )
{
	printf("Hello\n");

	while(1);
}

int main(void)
{
	xTaskCreate(HolaMundoTask, (const signed char *)"HM",
			128,NULL, 0, NULL);
	vTaskStartScheduler();
	//We should never get here
	while(1) {
		;
	}
	return 0;
}
