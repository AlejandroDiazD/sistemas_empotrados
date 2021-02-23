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
 * Práctica 1. Entrada/salida digital: pulsadores y LEDs
 * 
 * El programa debe realizar los siguientes pasos:
 * 
 * - Configurar el display de cristal líquido y las entradas y 
 *   salidas digitales y analógicas
 * - Tomar una primera medida de temperatura y definir el rango inicial
 * - Mostrar cada 250 milisegundos el siguiente mensaje en la línea 0:
 *   - Degrees: DD.DD
 *   - Donde DD.DD es la temperatura media en grados centígrados de los últimos 30 segundos
 * - Mostrar el siguiente mensaje en la línea 1:
 *   - Range: [XX, YY]
 *   donde XX e YY son los límites inferior y superior del rango de temperaturas
 * - Cada vez que se pulse el pulsador 1, decrementar los límites en una unidad 
 *   y actualizar el display
 * - Cada vez que se pulse el pulsador 2, incrementar los límites en una unidad 
 *   y actualizar el display
 * - Si la temperatura es menor que el límite inferior del rango, encender el LED azul
 * - Si la temperatura es mayor que el límite superior del rango, encender el LED rojo
 * 
 * Para ello se proponen las siguientes funciones:
 * 
 * 
 * - void pinMode(pin, mode)
 * 
 * Parámetros: 
 * 
 * - pin: identificador del pin digital que se va a configurar.
 * - mode: modo de configuración. Puede ser INPUT, OUTPUT, o INPUT_PULLUP
 * 
 * Configura el pin pasado como parámetro como entrada (INPUT), salida (OUTPUT) o
 * entrada con la resistencia de Pull-Up activada (INPUT_PULLUP)
 * 
 * 
 * - int digitalRead(pin)
 * 
 * Parámetros: 
 * 
 * - pin: identificador del pin digital que se va a leer.
 * 
 * Función que devuelve el valor de un pin digital configurado como entrada. El valor puede
 * ser HIGH o LOW.
 * 
 * 
 * - void digitalWrite(pin, value)
 * 
 * Parámetros: 
 * 
 * - pin: identificador del pin digital que se va a escribir.
 * - value: valor a escribir (HIGH o LOW)
 * 
 * Función que permite escribir un valor HIGH o LOW en un pin digital configurado como
 * salida.
 * 
 * 
 * - void attachInterrupt(digitalPinToInterrupt(pin), ISR, mode)
 * 
 * Parámetros:
 * 
 * - pin: identificador del pin digital en el que se va a instalar el manejador de
 *   interrupción. En Arduino Uno, solo pueden ser los pines 2 o 3.
 * - ISR: nombre de la rutina de atención a la interrupción
 * - mode: define cuándo se ha de disparar la interrupción. Los posibles valores son:
 *   - LOW: se activará siempre que el pin esté a nivel bajo. 
 *   - CHANGE: se activará cuando el pin cambie de valor.
 *   - RISING: se activará cuando el pin cambie de nivel bajo a nivel alto.
 *   - FALLING se activará cuando el pin cambie de nivel alto a nivel bajo.
 * 
 * Esta rutina permite instalar un manejador de interrupción para un pin digital configurado
 * como entrada. Los únicos pines disponibles en el Arduino Uno son los pines 2 y 3.
 * 
 * 
 */


// biblioteca del driver del display
#include <LiquidCrystal.h>

// objeto para manejar el driver. Inicialización de los pines.
LiquidCrystal lcd(9, 8, 7, 6, 5, 4);

// global variables
float measures[20];
int cnt = 0;
int flag_up = 0;
int flag_down = 0;
int low_range, high_range;
  
void setup() {
  
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(3), pls_up, RISING);
  attachInterrupt(digitalPinToInterrupt(2), pls_down, RISING);
  for(int i=0; i<20; i++){
    float volt = analogRead(0)*5.0/1024.0-0.5;
    int temp = volt/0.01;
    measures[i] = temp;
    }
  low_range = measures[19] - 5;
  high_range = measures[19] + 5;
}

void loop() {

  // Check range flags 
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

  // Get new measure
  float volt = analogRead(0)*5.0/1024.0-0.5;
  int temp_i = volt/0.01;

  // Update cnt
  cnt %= 20;
  measures[cnt] = temp_i;
  cnt += 1;

  // Calculate measure average
  int sum = 0;
  for (int i=0; i<20; i++){
    sum += measures[i];
    }
  int avg_temp = sum/20;

  // Light leds if out of range
  if (avg_temp < low_range){
    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);
    }
  else if (avg_temp > high_range){
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
    }
  else{
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
    }

  // Update display 
  lcd.setCursor(0, 0);
  lcd.print("Degrees C:");
  lcd.print(avg_temp);
  
  lcd.setCursor(0, 1);
  lcd.print("Range:");
  lcd.print(low_range);
  lcd.print(" ; ");
  lcd.print(high_range);
  lcd.print(" ");
 
  delay(250);
  
}

void pls_up(){
  flag_up = 1;
  }

void pls_down(){
  flag_down = 1;
  }
