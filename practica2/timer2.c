/*********************************************************************************************
* Fichero:		timer2.c
* Autor:
* Descrip:		funciones de control del timer2 del s3c44b0x
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "timer2.h"
#include "44b.h"
#include "44blib.h"
#include <inttypes.h>

/*--- variables globales ---*/
volatile int timer2_num_int=0;
volatile int pruebaTimer2=0;
/* declaración de función que es rutina de servicio de interrupción
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void timer2_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de las funciones ---*/
void timer2_ISR(void)
{
	timer2_num_int = 1+timer2_num_int;

	/* borrar bit en I_ISPC para desactivar la solicitud de interrupción*/
	rI_ISPC |= BIT_TIMER2; // BIT_TIMER2 está definido en 44b.h y pone un uno en el bit que correponde al Timer2
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
void timer2_inicializar(){

	rINTMOD = 0x0; // Configura las linas como de tipo IRQ
	rINTCON = 0x1; // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK &= ~(BIT_GLOBAL | BIT_TIMER2); // Emascara todas las lineas excepto Timer2 y el bit global (bits 26 y 11, BIT_GLOBAL y BIT_TIMER2 están definidos en 44b.h)

	/* Establece la rutina de servicio para TIMER0 */
	pISR_TIMER2 = (unsigned) timer2_ISR;

	/* Configura el Timer2 */
	rTCFG0 &= 0xffff00ff; // ajusta el preescalado, 255
	rTCFG1 &= 0xfffff0ff; // selecciona la entrada del mux que proporciona el reloj. La 00 corresponde a un divisor de 1/2.
	rTCNTB2 = 65535;// valor inicial de cuenta (la cuenta es descendente)
	rTCMPB2 = 0;// valor de comparación

}


/*
 * reinicia la cuenta de tiempo (contador y la variable), y comienza a medir.
 */
void timer2_empezar(){
	//Ponemos pause al timer2

	timer2_num_int =0;
	/* establecer update=manual (bit 1) + inverter=on (¿? será inverter off un cero en el bit 2 pone el inverter en off)*/
	rTCON = (rTCON & 0xffff0fff) | 0x2000 ;
	rTCON = (rTCON & 0xffff0fff) | 0x9000;


}
//double frecuenciaEfectiva = 64000000/((0+1)*2) ;
/*
 * lee la cuenta actual del temporizador y el número de interrupciones generadas, y
 * devuelve el tiempo transcurrido en microsegundos.
 */
float timer2_leer(){

	// es de 64Mhz, quiere decir 64.000.000 de ciclos por segundo
	//Ticks totales:

	//Mejor int ,

	return timer2_num_int*(65535/32) + (65535-rTCNTO2)/32;
	//return ( ( 65535 * timer2_num_int + ( 65535 - (rTCNTO2 %65535) ) ) / frecuenciaEfectiva )*1000000 ;
}



