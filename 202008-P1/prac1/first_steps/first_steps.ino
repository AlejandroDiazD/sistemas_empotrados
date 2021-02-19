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
 * Práctica 1. Primeros pasos.
 * 
 * El programa debe realizar los siguientes pasos:
 * 
 * - Configurar la puerta serie a 9600 baudios - Imprimir cada segundo el 
 *   siguiente mensaje: 
 *   - Serial output. Time XXX s
 *   donde XXX son los segundos transcurridos desde el arranque.
 * 
 * Para ello se proponen las siguientes funciones:
 * 
 * - void Serial.begin(speed)
 * 
 * Parámetros: 
 * 
 * - speed: velocidad de transmisión en baudios.
 * 
 * Selecciona la tasa de transmisión de datos en bits por segundo (baudios)
 * para la puerta serie. Los valores posibles para comunicarse con el PC son
 * los siguientes: 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800,
 * 38400, 57600, o 115200.  La función ha de ser llamada una única vez antes de
 * poder utilizar la puerta serie.
 * 
 * 
 * - void Serial.print(val)
 * 
 * Parámetros: 
 * 
 * - val: valor a imprimir. Puede ser de cualquier tipo.
 * 
 * Imprime el valor "val" por la puerta serie. Este valor puede ser una cadena
 * de caracteres o un valor numérico de tipo entero o en coma flotante.  Por
 * ejemplo: 
 * - Serial.print("Hola Mundo\n"); 
 * - Serial.print(1.2345); 
 * - Serial.print(345);
 *  
 *  
 * - unsigned long millis()
 * 
 * Parámetros: ninguno.
 * 
 * Retorna el número de milisegundos desde que comenzó el programa.
 * 
 * 
 * - void delay(ms)
 * 
 * Parámetros: 
 * 
 * - ms: milisegundos de duración de la pausa.
 * 
 * Pausa la ejecución del sistema durante un número de milisegundos.
 * 
 * 
 */

#include <LiquidCrystal.h>

void setup() {
  
  // Código de configuración. Solo se ejecutará al comienzo de la aplicación.
  Serial.begin(9600);
  
}

void loop() {
  
  // Código principal de la aplicación. Se ejecutará de forma indefinida.
  unsigned long val;
  val = millis();
  Serial.println(val);
  delay(1000);
  
}
