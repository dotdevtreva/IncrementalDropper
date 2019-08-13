# IncrementalDropper
INCREMENTAL-ENCODED GEARMOTOR DROPPER DRIVER FOR PIC18F 

> Contacto: 		  prog.gabrieltreva@outlook.com

> Desarrollador: 	Gabriel I. Vázquez

> Creación: 		  12/08/19

Licencia:
				        This program is free software: you can redistribute it and/or modify
				        it under the terms of the GNU General Public License as published by
				        the Free Software Foundation, either version 3 of the License, or
				        (at your option) any later version.
				        This program is distributed in the hope that it will be useful,
				        but WITHOUT ANY WARRANTY; without even the implied warranty of
				        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
				        GNU General Public License for more details.
				        You should have received a copy of the GNU General Public License
				        along with this program. If not, see <https://www.gnu.org/licenses/>.
				
# Detalles

   **ADVERTENCIA**: El uso de esta librería no permite el correcto trabajo de los módulos ADC.
   Para su correcta compilación **SE REQUIERE** incluir la librería *PWM* de MikroElektronica (R).
   
   Uso de RA4 como detector de pulsos para contador
   Uso de RB0 a RB5 como activador de los diferentes motores en matriz
   Uso de RC2 como generador de PWM para el contol del motorreductor

   Este controlador modera la cantidad de revoluciones que un mecanismo motorreductor requiere llevar
   a cabo, basado en el número de pulsos que un encoder incremental acoplado al mismo
   arroja hacia la terminal de cuenta del Timer 0. 
   
   Para la selección del motor a trabajar se hace uso de salidas digitales en el puerto B
   que habiiten la lectura (en estado alto) del PWM en determinado motor dentro de una matriz 3x3.
