/*********************************************************************************************
* Fichero:	main.c
* Autor:
* Descrip:	punto de entrada de C
* Version:  <P4-ARM.timer-leds>
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "8led.h"
#include "button.h"
#include "led.h"
#include "timer2.h"
#include "44blib.h"
#include "44b.h"
#include "excepciones.h"
#include <inttypes.h>

/*--- variables globales ---*/


void iniciar(){
	/* PARA EL TIMER 2 */
	rINTMOD = 0; // Configura la lina del timer 2 como de tipo IRQ
	rINTCON = 0x1; // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK = ~(BIT_GLOBAL | BIT_TIMER2); // Activo desenmarcaro el Timer2
}
/*--- codigo de funciones ---*/
void Main(void)
{


	/* Inicializa controladores */
	sys_init();         // Inicializacion de la placa, interrupciones y puertos
	excepciones_inicializar();
	iniciar();
	timer2_inicializar();

	//timer_init();	    // Inicializacion del temporizador NO SE USA
	//Eint4567_init();	// inicializamos los pulsadores. Cada vez que se pulse se verá reflejado en el 8led
	D8Led_init();       // inicializamos el 8led




	/* Valor inicial de los leds */
	leds_off();
	led1_on();



	/*//EJEMPLO PARA PROBAR EL TIMER2
	 * 	int tiempo=0;
	while (1)
	{

		int decimaldemilisegundo = 100;
		tiempo=0;
		timer2_empezar();
		Delay(decimaldemilisegundo);
		tiempo = timer2_leer();
		tiempo /= 100;
	}
	*/

	//EJEMPLOS PARA PROBAR LAS EXCEPCIONES
	/*
	 *  ; load CPSR into R0
	 *  ; clear mode field
	 *  ; user mode code
	 *  ; store modified CPSR into SPSR
	 *   ; context switch and branch
	 */
	asm(
			"ldr    R0,=0xffffffff\n\t"
			"MOV   PC, r0"

	);

}
