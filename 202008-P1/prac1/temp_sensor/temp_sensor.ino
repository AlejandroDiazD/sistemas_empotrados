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
 * Práctica 1. Entrada analógica: sensor de temperatura
 * 
 * El programa debe realizar los siguientes pasos:
 * 
 * - Configurar el display de cristal líquido y la entrada analógica 
 * - Mostrar cada 250 milisegundos el siguiente mensaje en la línea 0:
 *   - Degrees: DD.DD
 *   donde XXX son los segundos transcurridos desde el arranque.
 * 
 * Para ello se proponen las siguientes funciones:
 * 
 * 
 * - int analogRead(pin)
 * 
 * Parámetros: 
 * 
 * - pin: identificador del pin de entrada analógico que se desea leer.
 *   Puede ser un valor entre 0 y 5.
 * 
 * Devuelve el valor del pin de entrada analógico pasado como parámetro.
 * 
 * 
 */


// Biblioteca del driver del display
#include <LiquidCrystal.h>

// Objeto para manejar el driver. Inicialización de los pines.
LiquidCrystal lcd(9, 8, 7, 6, 5, 4);


void setup() {
  
  // Código de configuración. Solo se ejecutará al comienzo de la aplicación.
  Serial.begin(9600);
  lcd.begin(16, 2);
}

void loop() {
  
  // Código principal de la aplicación. Se ejecutará de forma indefinida.
  long temp;
  float volt;
  volt = analogRead(0)*5.0/1024.0-0.5;
  temp = volt/0.01;
  lcd.setCursor(0, 0);
  lcd.print("Degrees C:");
  lcd.print(temp);
  delay(250);
}
