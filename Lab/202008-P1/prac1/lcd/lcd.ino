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
 * Práctica 1. El display de cristal líquido.
 * 
 * El programa debe realizar los siguientes pasos:
 * 
 * - Configurar el display de cristal líquido
 * - Mostrar el siguiente mensaje en la línea 0:
 *   - LCD Display
 * - Mostrar cada segundo el siguiente mensaje en la línea 1:
 *   - Time: XXX s
 *   donde XXX son los segundos transcurridos desde el arranque
 * 
 * Para ello se proponen las siguientes funciones:
 * 
 * 
 * - void lcd.begin(cols, rows)
 * 
 * Parámetros: 
 * 
 * - cols: número de columnas del display (p. ej. 16).
 * - row: número de filas del display (p. ej. 2).
 * 
 * Inicializa la interfaz con el display y especifica el número de filas
 * y columnas.
 * 
 * 
 * - void lcd.setCursor(col, row)
 * 
 * Parámetros: 
 * 
 * - col: columna en la que posicionar el cursor (comienza en 0).
 * - row: fila en la que posicionar el cursor (comienza en 0).
 * 
 * Mueve el cursor a la posición determinada por row y col.
 * 
 * 
 * - void lcd.print(val)
 * 
 * Parámetros: 
 * 
 * - val: valor a imprimir. Puede ser de cualquier tipo.
 * 
 * Imprime el valor "val" en el display a partir de la posición del cursor.
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
  unsigned long val;
  val = millis();
  lcd.setCursor(0, 0);
  lcd.print("LCD Display");
  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  lcd.print(val);
  delay(1000);
  
}
