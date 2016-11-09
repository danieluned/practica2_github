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
#include "sudoku_2016.h"
#include "pilaDepuracion.h"
#include "maquinaEstadoSudoku.h"
/*--- variables globales ---*/



/*--- codigo de funciones ---*/
void Main(void)
{
	/* Inicializa controladores */
	sys_init();         // Inicializacion de la placa, interrupciones y puertos
	excepciones_inicializar();

	timer2_inicializar();  //TIMER 2 , para el tiempo global

	timer0_inicializar();
	//timer_init();	    // Inicializacion del temporizador NO SE USA
	Eint4567_init();	// inicializamos los pulsadores. Cada vez que se pulse se verá reflejado en el 8led
	D8Led_init();       // inicializamos el 8led


	/* Valor inicial de los leds */
	leds_off();
	led1_on();

	limpiadoPila();
	//Iniciamos el tiempo del sistema.
	timer2_empezar();






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
	/*
	asm(
			"ldr    R0,=0xffffffff\n\t"
			"MOV   PC, r0"

	);
	*/
	/*
	// Otra alternativa seria pasar a cada uno de los modos...
	// modo que se quiere EJEMPLO DE Undef, en 44binit.asm estan los diferentes nombres linea 420.
	asm(

			"mrs r0,cpsr"  			"\n\t"
			"bic ro,r0, #0x1f"		"\n\t"
			"orr r1,r0, #0x1b"		"\n\t"
			"msr cpsr_cxsf.r1"		"\n\t"
			"ldr sp,=UndefStack"

	);
	*/
	//sudoku9x9(cuadricula,1);

	//Probar pila
	//probarPila();


	/*
	// Pruebas para el timer0
	timer0_reset();
	Delay(5000); //esta en decimas de milisegundo
	push_debug(888,transcurrido); //transcurrido es en ms tendria que estas alrededor de 500

	timer0_reset();
	Delay(1300); //esta en decimas de milisegundo
	push_debug(888,transcurrido); //transcurrido es en ms , transcurrido alrededor de 130

	timer0_reset();
	Delay(100); //esta en decimas de milisegundo
	push_debug(888,transcurrido); //transcurrido es en ms , transcurrido alrededor de 10
	*/
	/*
	//probando botones
	while (1){
		int a = 0;

	}
	*/

	maquinaEstadosSudoku();


}
