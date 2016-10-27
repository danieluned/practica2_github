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
#include "timer.h"
#include "44blib.h"
#include "44b.h"

/*--- variables globales ---*/



/*--- codigo de funciones ---*/
void Main(void)
{


	/* Inicializa controladores */
	sys_init();         // Inicializacion de la placa, interrupciones y puertos
	timer_init();	    // Inicializacion del temporizador
	Eint4567_init();	// inicializamos los pulsadores. Cada vez que se pulse se verá reflejado en el 8led
	D8Led_init();       // inicializamos el 8led
	timer2_inicializar();


#define noreal
	/* Valor inicial de los leds */
	leds_off();
	led1_on();

	int tiempo=0;
	int asd = 1;
	while (1)
	{
		if (switch_leds=1){
			leds_switch();
			switch_leds = 0;
		}
		int decimaldemilisegundo = 1000;
		tiempo=0;
		timer2_empezar();
#ifdef real

		Delay(decimaldemilisegundo);
#else
		DelayMs(decimaldemilisegundo);
#endif
		tiempo = timer2_leer();

	}

}
