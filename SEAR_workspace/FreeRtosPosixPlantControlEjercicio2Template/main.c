  #include <FreeRTOS.h>
  #include <task.h>
  #include <semphr.h>

  #include <stdio.h>


//********************** Global Handlers ****************************/

//TODO declarar el mutex CommandMutex
SemaphoreHandle_t command_mutex;

//********************** PlantControlTask ****************************/
#define CONTROL_CYCLE_MS 10*100
#define CONTROL_CYCLE_TICKS ((CONTROL_CYCLE_MS*configTICK_RATE_HZ)/1000)

void PlantControlTask( void *pvParameters )
{
  	TickType_t xLastWakeTime;


    // A
    xLastWakeTime = xTaskGetTickCount();

    // B
    while(1)
    {


        
        vTaskDelayUntil( &xLastWakeTime, CONTROL_CYCLE_TICKS );

        // Request data from the sensors.
        printf("+Req Data Sensor 1\n");
        printf("+Req Data Sensor 2\n");
        printf("+Control Algorithm\n");
        printf("+Apply Control\n\n");

    }

    // Will never get here!
}


//********************** KeySCANTask ****************************/
#define KEY_SCAN_CYCLE_MS 15*100
#define KEY_SCAN_CYCLE_TICKS ((KEY_SCAN_CYCLE_MS*configTICK_RATE_HZ)/1000)


const char *auxKeyb="KEYB CONFIG PID Kp=0.5 Ki=0.3 Kd=0.2#";
int auxpKeyb=0;

int KeyPressed( char * key ){
  int keypressed=0;
  *key=*(auxKeyb+auxpKeyb);
  if(*key!='\0'){
    if(*key!='_')
      keypressed=1;
    auxpKeyb++;
  }

 return keypressed;
}

void UpdateKeybDisplay( char Key ){

  printf("            KeybDisplay %c\n",Key);
}


void ExecKeybCommand(){
  printf("\n                                  ExecKeybCommand\n");
}


void KeyScanTask( void *pvParmeters )
{
char Key;

	TickType_t xLastWakeTime;

    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        // Wait for the next cycle.
        vTaskDelayUntil( &xLastWakeTime, KEY_SCAN_CYCLE_TICKS );

        // Scan the keyboard.
        if( KeyPressed( &Key ) )
        {
        
            if(Key=='#'){
              //TODO Ejecutar la función ExecKeybCommand() en exclusión mutua
            	xSemaphoreTake(command_mutex, portMAX_DELAY);
            	ExecKeybCommand();
            	xSemaphoreGive(command_mutex);
            }else
            	UpdateKeybDisplay( Key );            
            
        }
    }
}

//********************** RS232ScanTask ****************************/
#define RS232_SCAN_CYCLE_MS 15*100
#define RS232_SCAN_CYCLE_TICKS ((RS232_SCAN_CYCLE_MS*configTICK_RATE_HZ)/1000)


const char *auxRS232="_______________________________________RS232 CONFIG PID Kp=0.4 Ki=0.2 Kd=0.1#";

int auxpRS232=0;

int RS232_RxChar( char * rxChar ){
  int charRx=0;

  *rxChar=*(auxRS232+auxpRS232);
  if(*rxChar!='\0'){
      if(*rxChar!='_')
        charRx=1;
      auxpRS232++;
  }

 return charRx;
}

void UpdateRS232Display( Key ){

  printf("            RS232Display %c\n",Key);

}


void ExecRS232Command(){
  printf("\n                                  ExecRS232Command\n");
}



void RS232Task( void *pvParmeters )
{
	char rxChar;
    TickType_t xLastWakeTime;

    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        // Wait for the next cycle.
        vTaskDelayUntil( &xLastWakeTime, RS232_SCAN_CYCLE_TICKS );

        // Scan the RS232.
        if( RS232_RxChar( &rxChar ) )
        {

            if(rxChar=='#'){

                //TODO Ejecutar la función ExecRS232Command() en exclusión mutua
            	xSemaphoreTake(command_mutex, portMAX_DELAY);
            	ExecRS232Command();
            	xSemaphoreGive(command_mutex);
            }else
                UpdateRS232Display(rxChar);
            
        }
    }
}


//********************** LEDTask ****************************/
#define LED_CYCLE_MS 100*100
#define LED_CYCLE_TICKS ((LED_CYCLE_MS*configTICK_RATE_HZ)/1000)

#define LED_PULSE_WITH_TICKS (LED_CYCLE_TICKS/2)


void LEDTask( void *pvParmeters )
{

	//Implementación de la ejecución periodica del bucle. 
	//Al iniciar cada periodo debe de haber un encendido del LED
	//y a mitad del periodo el LED se apagará
	
	TickType_t xLastWakeTime;

    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        TickType_t xSwitchOnWakeTime;

        // Wait for the next cycle.
        vTaskDelayUntil( &xLastWakeTime, LED_CYCLE_TICKS );
        xSwitchOnWakeTime=xLastWakeTime;
        printf("    LED GREEN ON\n\n");
        // Wait for the switch the led OFF.
        vTaskDelayUntil( &xSwitchOnWakeTime, LED_PULSE_WITH_TICKS );
        
        printf("    LED OFF\n\n");
    }
}

//********************** HTTPTask ****************************/
#define HTTP_CYCLE_MS 100*100
#define HTTP_CYCLE_TICKS ((HTTP_CYCLE_MS*configTICK_RATE_HZ)/1000)



void HTTPTask( void *pvParmeters )
{

	TickType_t xLastWakeTime;

    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {


        // Wait for the next cycle.
        vTaskDelayUntil( &xLastWakeTime, HTTP_CYCLE_TICKS );

        printf("\n        HTTP TASK\n");

    }
}

/**
 *	This is the systems main entry, some call it a boot thread.
 *
 *	-- Absolutely nothing wrong with this being called main(), just it doesn't have
 *	-- the same prototype as you'd see in a linux program.
 **/
int main(void) {


  //TODO Instanciar el mutex CommandMutex
	command_mutex= xSemaphoreCreateMutex();

    xTaskHandle PlantControlTaskHandler,KeyScanTaskHandler, RS232TaskHandler;
    xTaskHandle LEDTaskHandler,HTTPTaskHandler;

	xTaskCreate(PlantControlTask, (const char *) "CTASK", 128, NULL, 2, &PlantControlTaskHandler);
	xTaskCreate(KeyScanTask, (const char *)"KTASK", 128, NULL, 1, &KeyScanTaskHandler);
	xTaskCreate(RS232Task, (const char *)"RTASK", 128, NULL, 1, &RS232TaskHandler);
	xTaskCreate(LEDTask, (const char *)"LTASK", 128, NULL, 0, &LEDTaskHandler);
	xTaskCreate(HTTPTask, (const char *)"HTASK", 128, NULL, 0, &HTTPTaskHandler);


	vTaskStartScheduler();

	/*
	 *	We should never get here, but just in case something goes wrong,
	 *	we'll place the CPU into a safe loop.
	 */
	while(1) {
		;
	}
	return 0;
}
