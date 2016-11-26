/*********************************************************************************************
* Fichero:		excepciones.c
* Autor:
* Descrip:		funciones de control para las excepciones
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "excepciones.h"
#include "44b.h"
#include "44blib.h"
#include <inttypes.h>
#include "8led.h"
#include "pilaDepuracion.h"

//Variable global
volatile uint32_t error; //
volatile uint32_t instruccionCausante;

/* declaración de función que es rutina de servicio de interrupción
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void excepciones_ISR(void) __attribute__((interrupt("ABORT")));
void excepciones_ISR(void) __attribute__((interrupt("FIQ")));
void excepciones_ISR(void) __attribute__((interrupt("UNDEF")));
void excepciones_ISR(void) __attribute__((interrupt("SWI")));
void excepciones_ISR(void) __attribute__((interrupt("IRQ")));

void parpadear(int cod){

	while(1){
		D8Led_symbol(cod);
		Delay(1000);
		D8Led_symbol(16);
		Delay(500);
	}

}
/*--- codigo de las funciones ---*/
void excepciones_ISR(void)
{
	uint32_t value;

	// CALCULAR EL TIPO
	asm volatile("mrs %0,CPSR" : "=r" (value));
    error = value & 0xf;
    //Calculo de la instruccion causante
    //R14 es la de retorno, r14 -4 es la causante
    uint32_t instruccion;
    asm volatile("mov %0,r14" : "=r" (instruccion));
    switch (error) {
		case 7:
			// ABORT
			//causante = causante - 4; // for a prefetch abort
			instruccionCausante = instruccion - 8; // for a data abort
			push_debug(error,instruccion);
			parpadear(error);
			break;
		case 1:
			// FIQ int. rapdias
			instruccionCausante = instruccion - 8;
			push_debug(error,instruccion);
			parpadear(error);
			break;
		case 2:
			// int. lentas
			// IRQ
			instruccionCausante = instruccion -8;
			push_debug(error,instruccion);
			parpadear(error);
			break;
		case 3:
			//SWI Software interrupt
			instruccionCausante = instruccion -4;
			push_debug(error,instruccion);
			parpadear(error);
			break;
		case 6:
			// Undefined intruction
			instruccionCausante = instruccion -4;
			push_debug(error,instruccion);
			parpadear(error);
			break;
		default:
			// desconocido
			instruccionCausante = instruccion -4;
			push_debug(error,instruccion);
			parpadear(error);
			break;
	}


}

/*
 *
	fiq 10001 Servicio de int. rápidas
	irq 10010 Servicio de int. lentas
	svc 10011 Modo protegido para sistema operativo (int. sw)
	abt 10111 Procesado de fallos de acceso a mem
	und 11011 Manejo de instrucc. indefinidas

 *
 */

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


	pISR_UNDEF = (unsigned) excepciones_ISR;
	pISR_SWI = (unsigned) excepciones_ISR;
	pISR_PABORT = (unsigned) excepciones_ISR;
	pISR_DABORT = (unsigned) excepciones_ISR;
	pISR_FIQ = (unsigned) excepciones_ISR;
	pISR_IRQ = (unsigned) excepciones_ISR;



}

