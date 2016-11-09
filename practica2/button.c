/*********************************************************************************************
* Fichero:	button.c
* Autor:
* Descrip:	Funciones de manejo de los pulsadores (EINT6-7)
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "button.h"
#include "8led.h"
#include "44blib.h"
#include "44b.h"
#include "def.h"
#include "pilaDepuracion.h"
#include <inttypes.h>
#include "maquinaEstadoSudoku.h"
/*--- variables globales del módulo ---*/

/* ESTADOS (Hay que actualizarlo con el diseño nuevo)
 * INICIAL, estado inicial, no se ha pulsado boton
 *
 * PULSADO se ha detectado una pulsacion
 * 		deshabilitar interrupciones de los botones
 * 		programar temporizador con retardo inicial
 *
 * RETINICIAL,
 * 		identificar boton pulsado
 * 		programar temporizador cada 10ms para ver si sigue pulsado
 * LEVANTADO, se ha levantado
 * 		introducir retardo final
 * HABILITAR,
 * 		habilitar la IRQ al boton
 */
enum {
	//ESTADOS
    inicial      					= 0,
    esperandoRetardoInicial      	= 1,
    esperandoPulsado       			= 2,
    esperandoRetardoFinal      		= 3,
    //TIEMPOS ms
    TRP = 130, //placa 5: 200, placa 13 130
    TRD = 130, //placa 5: 130, placa 13 130
    TI = 50

};

//Variables para controlar el automata
int ESTADO = inicial;
int interrupcionBoton = 0;
int interrupcionTimer = 0;
int tiempo = 0;
int botonAntes = 0;
int botonAhora = 0;
int which_int;
uint32_t estadoBoton;
/*--- codigo de las funciones ---*/
int transcurrido = 0;  // ms
void timer0_on(){
	/* establecer update=manual (bit 1) + inverter=on (¿? será inverter off un cero en el bit 2 pone el inverter en off)*/
	rTCON = (rTCON & 0xfffffff0) | 0x2 ;
	/* iniciar timer (bit 0) con auto-reload (bit 3)*/
	rTCON = (rTCON & 0xfffffff0) | 0x9;
}
void timer0_off(){
	/* iniciar timer (bit 0) con auto-reload (bit 3)*/
		rTCON = (rTCON & 0xfffffff0) | 0x0;
}
 void  timer0_reset(){
	transcurrido = 0;
}
// Maquina de estados para la eliminación de los rebotes
void maquinaEstados(){
	switch(ESTADO){
	case inicial: 	if (interrupcionBoton==1){
						//Deshabilitar Irq de botones
						rINTMSK    |= BIT_EINT4567;
						//Llamar a temporizador con retardo inicial
						//push_debug(666,1);
						timer0_on();
						timer0_reset();
						//Siguiente estado
						ESTADO = esperandoRetardoInicial;
						interrupcionBoton = 0;
				 	 }
		break;
	case esperandoRetardoInicial:
					if (interrupcionTimer==1){
						if (transcurrido >= TRP){
							//temporizador de intervalo
							//push_debug(666,1); // Debug para validar que el timer 0
							timer0_reset();
							//Siguiente estado
							ESTADO = esperandoPulsado;

						}
						interrupcionTimer=0;
					}
		break;
	case esperandoPulsado:
					if (interrupcionTimer==1){
						if (transcurrido >= TI){
							//boton ahora
							if (which_int==4){
								//Pulsador eint6
								estadoBoton = rPDATG & 0x40 ;
							}
							if (which_int==8){
								//Pulsador eint7
								estadoBoton = rPDATG & 0x80 ;
							}
							if (estadoBoton != 0){ // cero es cuando esta pulsado
								//Llamar a temporizador con retardo final
								//push_debug(666,1);
								timer0_reset();
								//Siguiente estado
								ESTADO = esperandoRetardoFinal;
							}else{
								//Si no ha levantado el boton, volver a mirar dentro de un rato
								//push_debug(666,1);
								timer0_reset();
							}

						}
						interrupcionTimer=0;
					}
		break;
	case esperandoRetardoFinal:
					if (interrupcionTimer==1){
						if (transcurrido >= TRD){
							//Habilitar IRQ
							rEXTINTPND = 0x0f;
							rI_ISPC   |= BIT_EINT4567;
							rINTMSK &= ~BIT_EINT4567;
							//y desactivamos timer
							timer0_off();
							//Siguiente estado
							ESTADO = inicial;
						}

						interrupcionTimer=0;
					}
		break;
	}
}
/* declaración de función que es rutina de servicio de interrupción
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void Eint4567_ISR(void) __attribute__((interrupt("IRQ")));


int interrupcion = 0;
/*--- codigo de funciones ---*/
void Eint4567_ISR(void)
{
    interrupcion +=1;
	push_debug(interrupcion,rEXTINTPND);

	interrupcionBoton = 1;
	which_int = rEXTINTPND;


	//Implementación del automataca
	maquinaEstados();

	rEXTINTPND = 0xf;				// borra los bits en EXTINTPND
	rI_ISPC   |= BIT_EINT4567;		// borra el bit pendiente en INTPND

	// Asignar el boton pulsado a la variable que esta escuchando
	// la maquina de sudoku
	botonPulsado = which_int;
}
void Eint4567_init(void)
{
	/* Configuracion del controlador de interrupciones. Estos registros están definidos en 44b.h */
	rI_ISPC    = 0x3ffffff;	// Borra INTPND escribiendo 1s en I_ISPC
	rEXTINTPND = 0xf;       // Borra EXTINTPND escribiendo 1s en el propio registro
	rINTMOD    = 0x0;		// Configura las linas como de tipo IRQ
	rINTCON    = 0x1;	    // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK    &= ~(BIT_GLOBAL | BIT_EINT4567);

	/* Establece la rutina de servicio para Eint4567 */
	pISR_EINT4567 = (int) Eint4567_ISR;

	/* Configuracion del puerto G */
	rPCONG  = 0xffff;        		// Establece la funcion de los pines (EINT0-7)
	rPUPG   = 0x0;                  // Habilita el "pull up" del puerto
	rEXTINT = rEXTINT | 0x22222222;   // Configura las lineas de int. como de flanco de bajada

	/* Por precaucion, se vuelven a borrar los bits de INTPND y EXTINTPND */
	rI_ISPC    |= (BIT_EINT4567);
	rEXTINTPND = 0xf;

}
/*******TIMER 0******************/
/*--- variables globales ---*/
int timer0_num_int=0;

/* declaración de función que es rutina de servicio de interrupción
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void timer0_ISR(void) __attribute__((interrupt("IRQ")));



void timer0_ISR(void)
{
	transcurrido++;
	//push_debug(999,0);
	interrupcionTimer=1;
	maquinaEstados();
	/* borrar bit en I_ISPC para desactivar la solicitud de interrupción*/
	rI_ISPC |= BIT_TIMER0; // BIT_TIMER1 está definido en 44b.h y pone un uno en el bit que correponde al Timer2
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
void timer0_inicializar(){

	/* Configuraion controlador de interrupciones */
	rINTMOD=0x0; // Configura las linas como de tipo IRQ
	rINTCON=0x1; // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK &= ~(BIT_GLOBAL |BIT_TIMER0);
	/* Establece la rutina de servicio para TIMER0 */
	pISR_TIMER0 = (unsigned) timer0_ISR;

	/* Configura el Timer0 para avisar cada 1 ms */
	rTCFG0 =(rTCFG0 & 0xffffff00)| 0x0 ; // ajusta el preescalado
	rTCFG1 =(rTCFG1 & 0xfffffff0) |0x4 ; //divisor de 32
	rTCNTB0 = 2000;// valor inicial de cuenta (la cuenta es descendente)
	rTCMPB0 = 0;// valor de comparación


}


/*
Calculos para obtener los 2000 de la cuenta
F = MCLK / ((preescalado+1)(valordeldivisor))

		Ticks (c)/FrecuenciaEfectiva(c/s)=s


			-----------
			 ticks * (press+1) * div    == time
			MLCK
			___________
			ticks * (pre+1) *div  * 1000 =  ms
			64000000
			___________
			ticks * (pre+1)*div  = ms
			64000
			___________
			x * 0+1 * div = ms
			64000
		   ____________
		   x * div = ms
		   64000
*/


