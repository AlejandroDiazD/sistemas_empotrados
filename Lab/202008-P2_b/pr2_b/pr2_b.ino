/* DUDAS 
 * 
 * 1. Las variables que antes utilizabamos como globales, seguimos utilizando globales??
 * o hay que tenerlo en cuenta de alguna forma de cara al tamano de la pila que se reserva
 * para cada tarea??
 * 
 * 2. Los pinMode se realizan en cada tarea o en el setup global?
 * 
 * 3. Cuantas tareas y para que cada una tenemos que hacer?
 * 
 * 4. Revisar si hacemos con delay o delayuntil
 * 
 * 5. Ver por que se sube la temperatura del sensor
*/
#include <Arduino_FreeRTOS.h>
#include <Servo.h>


void TaskBlinkWatchdog( void *pvParameters );
void TaskTempRead( void *pvParameters );
void TaskRunMode( void *pvParameteres );


// Definimos un tipo enum para los modos de 
// la maquina de estados
typedef enum mode {
  STANDBY = 0,
  HEATING = 1,
  COOLING = 2
} e_mode;

// Inicialmente, la maquina arranca en STANDBY
e_mode mode = STANDBY;

Servo servo;


// Global variables
int cnt_watchdog = 0;           // Contador que utiliza el watchdog

float measures[20];       // Array que almacena las 20 ultimas medidas
                          // con las que realizar la media
int cnt = 0;              // Contador que sirve como indice para el array
                          // de 20 muestras
int frame = 0;            // Numero de frame actual
int flag_up = 0;          // Flag auxiliar para interrupcion
int flag_down = 0;        // Glaf auxiliar para interrupcion
int low_range, high_range;// Rangos maximo y minimo de temperatura
int angle = 0;            // Angulo del servo
int estado = 0;           // Estado para ir moviendo 90 grados el servo
bool timer_flag = false;  // Flag que permite realizar unas tareas cada
                          // 500 ms y otras cada 1000 ms
bool led_flag = false;    // Flag auxiliar para hacer parpadear el LED BUILTIN
int avg_temp = 0;         // Temperatura media de las 20 ultimas medidas

void setup() {
  
  Serial.begin(9600); // initialize serial communication at 9600 bits per second

  // Configuramos los pines In/Out y las interrupciones
  pinMode(2, INPUT_PULLUP);     // Pulsador 1
  pinMode(3, INPUT_PULLUP);     // Pulsador 2
  pinMode(5, OUTPUT);           // Servo 
  pinMode(11, OUTPUT);          // Motor DC
  pinMode(12, OUTPUT);          // LED rojo 
  pinMode(13, OUTPUT);          // LED azul
  pinMode(LED_BUILTIN, OUTPUT); // LED BUILTIN
  attachInterrupt(digitalPinToInterrupt(3), pls_up, RISING);   // Interrupcion
  attachInterrupt(digitalPinToInterrupt(2), pls_down, RISING); // Interrupcion
  servo.attach(5);      // Configuracion servo 

  // Realizamos inicialmente 20 medidas de temperatura para rellenar el array
  // con el que se calcula la media.
  // La lectura de voltaje que se realiza en la entrada analogica se convierte
  // a temperatura mediante las siguientes ecuaciones.
  for(int i=0; i<20; i++){
    float volt = analogRead(0)*5.0/1024.0-0.5;
    int temp = volt/0.01;
    measures[i] = temp;
    }
  // Se da un valor inicial a los rangos maximo y minimo a partir de la medida 
  // realizada y un offset
  low_range = measures[19] - 5;
  high_range = measures[19] + 5;
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }

  xTaskCreate(
    TaskBlinkWatchdog
    ,  "Blink"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskRunMode
    ,  "RunMode"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskTempRead
    ,  "TempRead"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );
  
}

void loop() {
  // Empty. Things are done in Tasks.
}

void TaskBlinkWatchdog(void *pvParameters)  // This is a task.
{
  (void) pvParameters; 
/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
*/
  for (;;) // A Task shall never return or exit.
  {
    cnt_watchdog += 1;
    if (cnt_watchdog >= 2){
      vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
    }
    else{
      if (led_flag == false){
        led_flag = true;
        digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on
      }
      else{
        led_flag = false;
        digitalWrite(LED_BUILTIN, LOW);    // turn the LED off 
      }
    }
  }
}

void TaskTempRead(void *pvParameters)
{
  (void) pvParameters;
  /*
    Leer temperatura cada 500 ms
  */
  for (;;)
  {
    // Manda senal al watchdog
    cnt_watchdog = 0;
    // - Leer sensor
    float volt = analogRead(0)*5.0/1024.0-0.5;
    int temp_i = volt/0.01;
  
    // - Actualizar contador y guardar medida
    cnt = cnt%20;                // Esto permite que el contador nunca pase de 20
    measures[cnt] = temp_i;   // Almacena la nueva medida
    cnt += 1;                 // Actualiza el contador
    
    // - Calcular media ultimas 20 medidas
    int sum = 0;
    for (int i=0; i<20; i++){
      sum += measures[i];
    }
    avg_temp = sum/20;
    vTaskDelay( 500 / portTICK_PERIOD_MS ); // Espera 500 ms
  }
}

void TaskRunMode(void *pvParameters)
{
  (void) pvParameters;
  /* 
    Ejecuta toda la logica para que funcione la planta cada 1000 ms
  */
  for (;;)
  {
    // Manda senal al watchdog
    cnt_watchdog = 0;
    // - Acciones modo
    if (mode == STANDBY){
      digitalWrite(12, LOW);  // Apagar LED rojo
      analogWrite(11, 0);     // Apagar motor DC
      servo.write(angle);     // Servo parado 
    } 
    else if (mode == HEATING){
      digitalWrite(12, HIGH);      // Encender LED rojo
      analogWrite(11, 0);         // Apagar motor DC
      if (estado == 0){            // Mover 90 grados el servo 
          angle = 90;
          estado = 1;
      }
      else if (estado == 1){
          angle = 180;
          estado = 2;
      }
      else if(estado == 2){
          angle = 90;
          estado = 3;
      } 
      else if (estado == 3){
          angle = 0;
          estado = 0;
      }
      servo.write(angle);
    }
    else if (mode == COOLING){
      digitalWrite(12, LOW);       // Apagar LED rojo
      analogWrite(11, 50);        // Encender motor DC al 10%
      servo.write(angle);          // Servo parado 
    }
    
    // - Ajustar rango 
    // Haciendo uso de estas flags junto con las interrupciones, se logra
    // aumentar o disminuir el valor de los rangos cuando se pulsan los
    // pulsadores, filtrando de manera software los posibles rebotes.
    if (flag_up == 1){
      low_range += 1;
      high_range += 1;
      flag_up = 0;
    }

    if (flag_down == 1){
      low_range -= 1;
      high_range -= 1;
      flag_down = 0;
    }
      
    // - Cambiar modo
    // Cambia de modo comprobando las condiciones de funcionamiento
    // definidas segun la temperatura y los rangos.
    if (avg_temp < low_range){
      mode = HEATING;
    }
    else if (avg_temp > high_range){
      mode = COOLING;
    }
    else{
      mode = STANDBY;
    }

    // Imprimir toda la informacion 
    Serial.print(millis());
    Serial.print(": Frame ");
    Serial.print(frame);
    Serial.print(" , LR = ");
    Serial.print(low_range);
    Serial.print(" , HR = ");
    Serial.print(high_range);
    Serial.print(" , Temp = ");  
    Serial.print(avg_temp);
    Serial.print(" , Angle = ");
    Serial.print(angle);  
    Serial.print(" , Mode = ");
    Serial.println(mode);

    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // Espera 1000 ms
  }

}

// Cada interrupcion activa el flag correspondiente
void pls_up()
{
  flag_up = 1;
}

void pls_down()
{
  flag_down = 1;
}
