/**
 * Copyright (C) 2017 UAH. Departamento de Automática

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * 
 * Práctica 2. Ejecutivo cíclico: control de una planta
 * 
 * En el arranque del sistema, el programa debe realizar las siguientes
 * operaciones:
 *
 * - Configurar la puerta serie a 9600 baudios.
 * - Tomar una primera medida de temperatura y definir el rango inicial.
 * - Configurar las entradas y salidas digitales y analógicas de acuerdo con el
 *   enunciado de la práctica.
 * 
 * El programa debe implementar las siguientes tareas periódicas:
 * 
 * - Cada segundo debe de encender o apagar el LED_BUILTIN para indicar que el
 *   programa está funcionando correctamente.
 * - Cada 500 milisegundos debe tomar una muestra del sensor de temperatura. A
 *   continuación, debe calcular la media de las últimas 20 tomas de temperatura
 *   para obtener el valor de temperatura representativo.
 * - Cada segundo debe realizar el control del sistema de acuerdo con los
 *   siguientes requisitos:
 *   - Si el pulsador 1 ha sido pulsado al menos una vez durante el último
 *     segundo, decrementar los límites del rango en una unidad.
 *   - Si el pulsador 2 ha sido pulsado al menos una vez durante el último
 *     segundo, incrementar los límites del rango en una unidad.
 *   - Si la temperatura es menor que el límite inferior del rango, pasar a o
 *     permanecer en modo HEATING.
 *   - Si la temperatura es mayor que el límite superior del rango, pasar a o
 *     permanecer en modo COOLING.
 *   - Si la temperatura está dentro del rango, pasar a o permanecer en modo
 *     STANDBY.
 * - En modo HEATING:
 *   - El LED rojo debe de estar encendido permanentemente.
 *   - Cada segundo, girar 90 grados el "agitador" (servo motor) [0, 90, 180]
 * - En modo COOLING:
 *   - El motor de continua debe estar en funcionamiento permanentemente con un
 *     ciclo de trabajo del 10%.
 *
 * El sistema siempre arranca en modo STANDBY.
 * 
 * Cada vez que se inicie un subperiodo del ejecutivo cíclico, el sistema
 * deberá mostrar el siguiente mensaje por la puerta serie:
 * - [XXXX]: Frame Y
 * donde XXXX son los milisegundos desde el arranque e Y es el número de marco
 * o subperiodo.
 *
 *
 * Para ello se proponen las siguientes funciones:
 * 
 * 
 * - void analogWrite(pin, value)
 * 
 * Parámetros: 
 * 
 * - pin: identificador del pin digital que se va a escribir.
 * - value: el ciclo de trabajo, entre 0 (siempre apagado) y 255 (siempre
 *   encendido).
 * 
 * Función que permite generar una función PWM a través de un PIN digital con
 * capacidad para ello.  Los pines digitales con posible salida PWM son los
 * pines 3, 5, 6, 9, 10, y 11.
 *
 *
 * - void servo.attach(pin)
 * 
 * Parámetros: 
 * 
 * - pin: identificador del pin digital al que está conectado el servo. El pin
 *   ha de tener capacidad de generar un PWM.
 *
 * Función que asocia el objeto Servo definido globalmente con el pin al que el
 * corresponiente motor está conectado. Esta función ha de ser llamada durante
 * el proceso de inicilización.
 * 
 *
 * - void servo.write(angle)
 * 
 * Parámetros: 
 * 
 * - angle: el valor del ángulo que debe de tomar el servo. Acepta valores
 *   entre 0 y 180.
 *
 * Función que escribe un valor en el servo. Dicho valor será el ángulo en
 * grados que tiene que adoptar el servo.
 *
 *
 */

#include <Servo.h>

#define RED_LED 12
#define MOTOR 11

#define TOTAL_MEASURES 20

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

// Variables globales
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

void setup() {
  // Configuramos los baudios del puerto serie
  Serial.begin(9600);

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
}

void loop() {

  // 1. Cada 500 ms: 
  // - 1.1 Mostrar tiempo ejecucion por puerto serie
  unsigned long Te = millis();
  
  // - 1.2. Leer sensor
  float volt = analogRead(0)*5.0/1024.0-0.5;
  int temp_i = volt/0.01;

  // - 1.3. Actualizar contador y guardar medida
  cnt = cnt%20;                // Esto permite que el contador nunca pase de 20
  measures[cnt] = temp_i;   // Almacena la nueva medida
  cnt += 1;                 // Actualiza el contador
  
  // - 1.4. Calcular media ultimas 20 medidas
  int sum = 0;
  for (int i=0; i<20; i++){
    sum += measures[i];
    }
  int avg_temp = sum/20;
  // Imprimir por puerto serie 
  Serial.print(" , Mode = ");
  Serial.println(mode);
  // 2. Cada 1000ms: 
  if (timer_flag == true){  
    // - 2.1. Parpadeo LED BUILTIN
    // Si esta encendido lo apaga y viceversa. Lo comprueba mediante
    // el estado de la variable 'led_flag'
    if (led_flag == false){
      digitalWrite(LED_BUILTIN, HIGH);
      }
    else{
      digitalWrite(LED_BUILTIN, LOW);
      }
    led_flag = !led_flag;
    
    // - 2.2. Acciones modo
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
      analogWrite(11, 100);        // Encender motor DC al 10%
      servo.write(angle);          // Servo parado 
      }
    
    // - 2.3. Ajustar rango 
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
      
    // - 2.4. Cambiar modo
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
  
  // Actualizar flag de los 500ms y frame
  timer_flag = !timer_flag;
  frame = frame + 1;
  
  // Realiza un delay de 500 ms menos el tiempo que ha tardado en 
  // ejecutar todas las acciones anteriores, con ayuda de la variable
  // auxiliar Te que se almacena al principio del bucle.
  delay(Te + 500 - millis());
}

// Cada interrupcion activa el flag correspondiente
void pls_up(){
  flag_up = 1;
  }

void pls_down(){
  flag_down = 1;
  }
