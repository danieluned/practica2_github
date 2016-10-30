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

/*--- variables globales del módulo ---*/
/* int_count la utilizamos para sacar un número por el 8led.
  Cuando se pulsa un botón sumamos y con el otro restamos. ¡A veces hay rebotes! */
static unsigned int int_count = 0;

/* ESTADOS
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
    TRP = 100,
    TRD = 100,
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


// Maquina de estados para la eliminación de los rebotes
void maquinaEstados(){
	switch(ESTADO){
	case inicial: 	if (interrupcionBoton==1){
						//Deshabilitar Irq de botones
						rINTMSK    |= BIT_EINT4567;
						//Llamar a temporizador con retardo inicial
						temporizador(TRP);
						//Siguiente estado
						ESTADO = esperandoRetardoInicial;
						interrupcionBoton = 0;
				 	 }
		break;
	case esperandoRetardoInicial:
					if (interrupcionTimer==1){
						//Identificar boton
						if (which_int==4){
							//Pulsador eint6
							botonAntes = rPDATG & 0x40 ;
						}
						if (which_int==8){
							//Pulsador eint7
							botonAntes = rPDATG & 0x80 ;
						}

						//temporizador de intervalo
						temporizador(TI);
						//Siguiente estado
						ESTADO = esperandoPulsado;
						interrupcionTimer=0;
					}
		break;
	case esperandoPulsado:
					if (interrupcionTimer==1){
						//boton ahora
						if (which_int==4){
							//Pulsador eint6
							botonAhora = rPDATG & 0x40 ;
						}
						if (which_int==8){
							//Pulsador eint7
							botonAhora = rPDATG & 0x80 ;
						}
						if (botonAntes != botonAhora){
							//Llamar a temporizador con retardo final
							temporizador(TRD);
							//Siguiente estado
							ESTADO = esperandoRetardoFinal;
						}else{
							//Si no ha levantado el boton, volver a mirar dentro de un rato
							temporizador(TI);
						}
						interrupcionTimer=0;
					}
		break;
	case esperandoRetardoFinal:
					if (interrupcionTimer==1){
						//Habilitar IRQ
						rINTMSK    &= BIT_EINT4567;
						//Siguiente estado
						ESTADO = inicial;
						interrupcionTimer=0;
					}
		break;
	}
}
/* declaración de función que es rutina de servicio de interrupción
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void Eint4567_ISR(void) __attribute__((interrupt("IRQ")));


/*--- codigo de funciones ---*/
void Eint4567_ISR(void)
{
	interrupcionBoton = 1;
	which_int = rEXTINTPND;
	//Implementación del automataca
	maquinaEstados();
	rEXTINTPND = 0xf;				// borra los bits en EXTINTPND
	rI_ISPC   |= BIT_EINT4567;		// borra el bit pendiente en INTPND

	/*
	Identificar la interrupcion (hay dos pulsadores)

	int which_int = rEXTINTPND;
	switch (which_int)
	{
		case 4:
			int_count++; // incrementamos el contador
			break;
		case 8:
			int_count--; // decrementamos el contador
			break;
		default:
			break;
	}

	//D8Led_symbol(int_count & 0x000f); // sacamos el valor por pantalla (módulo 16)

	// Finalizar ISR
	rEXTINTPND = 0xf;				// borra los bits en EXTINTPND
	rI_ISPC   |= BIT_EINT4567;		// borra el bit pendiente en INTPND

	*/
}
void Eint4567_init(void)
{
	/* Configuracion del controlador de interrupciones. Estos registros están definidos en 44b.h */
	rI_ISPC    = 0x3ffffff;	// Borra INTPND escribiendo 1s en I_ISPC
	rEXTINTPND = 0xf;       // Borra EXTINTPND escribiendo 1s en el propio registro
	rINTMOD    = 0x0;		// Configura las linas como de tipo IRQ
	rINTCON    = 0x1;	    // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK    = ~(BIT_GLOBAL | BIT_EINT4567 | BIT_TIMER0); // Enmascara todas las lineas excepto eint4567, el bit global y el timer0

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

/*--- codigo de las funciones ---*/
void timer0_ISR(void)
{
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


	/* Establece la rutina de servicio para TIMER0 */
	pISR_TIMER0 = (unsigned) timer0_ISR;

	/* Configura el Timer3 para avisar  */
	rTCFG0 |= 0xCF ; // ajusta el preescalado 207
	rTCFG1 |= 0x7; // selecciona la entrada del mux que proporciona el reloj. La 00 corresponde a un divisor de 1/2.
	rTCNTB0 = 65535;// valor inicial de cuenta (la cuenta es descendente)
	rTCMPB0 = 0;// valor de comparación

}

/*
 * Mandara una interrupcion dentro de ms milisegundos.
 * valor permitido entre 1 y 5000 ms
 *
 */

void temporizador(int ms){
	//Ponemos pause al timer2

	if (ms*10 > 65535){

	}else{
		rTCNTB0 = ms*10;// valor inicial de cuenta (la cuenta es descendente)
		/* establecer update=manual (bit 1) + inverter=on (¿? será inverter off un cero en el bit 2 pone el inverter en off)*/
		rTCON = 0x2;
		/* iniciar timer (bit 0) sin auto-reload (bit 3)*/
		rTCON = 0x1;
	}
}


/* CALCULOS PARA CONFIGURAR LOS TICKS

	frecuenciaEfectiva = 1000;

	66000000 /((preescalado+1)*divisor) = 1000

	66000 = (preescalado+1) * divisor

	66000 / divisor  - 1 = preescalado

	divisor {2,4,8,16,32}
	preescalado {8bits}  0 hasta 255

	PARa Cualquier divisor = 2
		NO HAY SOLoCUoN
	////
	si ponemos que los ticks sean 10 vecos de los ms = (ticks*10) * 1000

	frecuenciaEfectiva = 10000;

	66000000 /((preescalado+1)*divisor) = 10000

	6600 = (preescalado+1) * divisor

	6600 / divisor  - 1 = preescalado

	divisor {2,4,8,16,32}
	preescalado {8bits}  0 hasta 255

	si  divisor = 32
		preescalado = 207
	ticks = ms * 10;
	maximo = 6553,5 ms

*/
