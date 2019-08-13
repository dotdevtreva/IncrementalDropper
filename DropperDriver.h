/*### INCREMENTAL-ENCODED GEARMOTOR DROPPER DRIVER ###

Contacto: 		prog.gabrieltreva@outlook.com
Desarrollador: 	Gabriel I. Vázquez
Creación: 		12/08/19
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

**/

/* Traducciones del compilador */

//	Estados lógicos
#define HIGH    1
#define LOW     0
#define TRUE	1
#define FALSE	0
#define	INPUT	1
#define	OUTPUT	0

//	Tiempos de retraso
#define TLCD 	1000
#define TGEN	500
#define TWAIT	5

/* Variables globales */
unsigned char prevCountVal = 0;
unsigned char countStart = 235;
unsigned char slotNumber = 1;
unsigned short int runningInfo = 0;

//	Subrutina de reestablecimiento del contador
void Dropper_ResetCount(){
	
	// Asignar valores de inicio
	TMR0L = countStart;
	prevCountVal = countStart;
	
	// Limpiar indicador de desborde de TMR0
	INTCON.B2 = FALSE;
	
}

//	Subrutina de configuración inicial del dispensador
void Dropper_Init(){
	
/* ### Establecer la dirección de I/O de los puetos ### */     

	// Puerto A:
	TRISA.B4 = INPUT;				// Entrada en RA4 (T0CKI)
    
	// Puerto B:
	TRISB = TRISB & 0b11000000;		// Salidas en RB0 - RB5 (Activadores)
	
	// Puerto C:
	TRISC.B2 = OUTPUT;				// Salida en RC2 (CCP1/PWM1)
	
/* ### Establecer configuraciones de lectores análogos ### */

	// Apagado del módulo A/D
	ADCON0.B0 = FALSE;
          
	// Establecer todo el módulo A/D como D
	ADCON1 = ADCON1 | 0b00001111;
	
/* ### Establecer configuraciones del moduo PWM ### */
      
	// Inicializar PWM1 a 10KHz
	PWM1_Init(10000);
      
	// Establecer ciclo de trabajo en 75%
	PWM1_Set_Duty(192);
	
	// Mantener el PWM apagado
	PWM1_Stop();
      
/* ### Establecer configuraciones de la interrupción ### */
      
	// Activación de Timer 0 como contador en RA4, con flanco de caida, modo 8bit y sin preescalador
	T0CON = T0CON | 0b11111000;
    
	// Limpieza de memoria para el Timer1
	Dropper_ResetCount();
      
}

//	Subrutina para establecer el valor inicial del contador en 255-N para usar N cuentas al desborde de interrupción
void Dropper_SetRevLimit(unsigned char MaxCount){
	
	countStart = 255 - MaxCount;
	Dropper_ResetCount();
	
}	// 20 cuentas por default (1 revolucion para encoders con 20 divisiones).

//	Subrutina para disparar el funcionamiento de determinado motor en la matriz
void Dropper_Activate(unsigned char ColumnX, unsigned char RowY){
	
	// Activar GIE, PEIE y TMR0E
	INTCON = INTCON | 0b11100000;
	
	// Determinar posición por habilitar mediante función "3(y-1)+x"
	slotNumber = (( (3*(RowY-1)) ) + ColumnX);
	
	// Apagar habilitaciones previas
	LATB = 0x00;
	
	// Determinar la salida digital a habilitar 
	switch (slotNumber){
		case    1:   LATB.B0 = HIGH; break; // Slot 11
		case    2:   LATB.B1 = HIGH; break; // Slot 12
		case    3:   LATB.B2 = HIGH; break; // Slot 13
		case    4:   LATB.B3 = HIGH; break; // Slot 21
		case    5:   LATB.B4 = HIGH; break; // Slot 22
		case    6:   LATB.B5 = HIGH; break; // Slot 23
		default  :   LATB = 0x00     break; // Sin activación
	}
	
	// Arranque del motor con amortiguación de cambios
	Delay_ms(TWAIT);
	PWM1_Start();
	
}

//	Subrutina de acción en interrupción de Timer1
void interrupt (void){
	
	// Apagado de GIE y Timer1 en espera de una nueva instrucción de habilitación
	INTCON = INTCON & 0b00011011;
		
	// Salva de estado de memoria volatil en memoria local
	runningInfo = STATUS;

	// Apagado de motor activo y habilitaciones
	PWM1_Stop(); LATB = 0x00;
	
	// Reestablecimiento de estado de memoria volatil y cuenta de revoluciones
	STATUS = runningInfo;
	Dropper_ResetCount();
}