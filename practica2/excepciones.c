/*********************************************************************************************
* Fichero:		timer2.c
* Autor:
* Descrip:		funciones de control del timer2 del s3c44b0x
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "excepciones.h"
#include "44b.h"
#include "44blib.h"
#include <inttypes.h>


//Variable global
int error; // 0 no hay error, 1 reset, 2 abort, 3 fiq, 4 undef, 5 swi


/* declaración de función que es rutina de servicio de interrupción
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void excepciones_ISR(void) __attribute__((interrupt("RESET")));
void excepciones_ISR(void) __attribute__((interrupt("ABORT")));
void excepciones_ISR(void) __attribute__((interrupt("FIQ")));
void excepciones_ISR(void) __attribute__((interrupt("UNDEF")));
void excepciones_ISR(void) __attribute__((interrupt("SWI")));
/*--- codigo de las funciones ---*/
void excepciones_ISR(void)
{
	uint32_t value;

	// CALCULAR EL TIPO
	asm volatile("mrs %0,CPSR" : "=r" (value));
    error = value & 0xf;

    switch(error){
    case 0:

    	break;
    case 1:
    	break;
    case 2:
    	break;
    case 3:
    	break;
    case 4:
    	break;
    case 5:
    	break;
    default:
    	break;

    }
}


/*
 * configura el timer 2 para que trabaje a la máxima preción posible. El reloj
 * de la placa está configurado a 64MHz. Para aumentar el rango del contador,
 * el temporizador generará una interrupción cada vez que haga una cuenta completa
 * (queremos que la cuenta sea lo mayor posible para no sobrecargar en exceso al sistema con
 * interrupciones). La biblioteca dispondrá de la variable interna timer2_num_int,
 * compartida entre la interrupcion y el resto de funciones de ese módulo, que
 * llevara la cuenta de los peridos completos del temporizador. Al acabar la cuenta completa
 * el temporizador se reinicia al valor inicial y seguirá contando.
 */
void excepciones_inicializar(){
	/* Configuraion controlador de interrupciones */

    error = 0;
	/* Establece la rutina de servicio para TIMER0 */

	pISR_UNDEF = (unsigned) excepciones_ISR;
	pISR_SWI = (unsigned) excepciones_ISR;
	pISR_PABORT = (unsigned) excepciones_ISR;
	pISR_DABORT = (unsigned) excepciones_ISR;
	pISR_FIQ = (unsigned) excepciones_ISR;
	pISR_IRQ = (unsigned) excepciones_ISR;



}

