#include "8led.h"
#include "sudoku_2016.h"


enum {
	//ESTADOS
    INICIO      					= 0,   // no se ha pulsado nada
    APARECER_F						= 1,
    ESPERANDO_IZQUIERDO    			= 2,
    MOSTRAR_NUMEROS_1_9_F			= 3,
    APARECER_C						= 4,
    MOSTRAR_NUMEROS_1_9_C			= 5,
    APARECER_V						= 6,
    MOSTRAR_NUMEROS_0_9_V			= 7

};

// Variables y valores iniciales
volatile int  botonPulsado = 0;
volatile int  permitirPulsacionLarga = 0;
volatile int estado = INICIO;

volatile int numeroAmostrar = 1;
volatile int filaConfirmada= -1;
volatile int columnaConfirmada = -1;
volatile int valorConfirmado = -1;
//CONSTANTES
int BOTONIZQUIERDO = 4;
int BOTONDERECHO = 8;
// funciones que se necesitan
/* *****************************************************************************
 * modifica el valor almacenado en la celda indicada, devuelve 0 si NO modifica bit error, 1 si SI que lo modifica */
static inline int
celda_ponerValor(CELDA *celdaptr, uint8_t val)
{

	//Quitamos bit de error siempre, si es necesario se activa a continuaci�n
    *celdaptr = (*celdaptr & 0xBFF0) | (val & 0x000F);


    //C�lculo del bit de error
    uint16_t propa = 1<<(val+3);
    if ( val != 0 && (*celdaptr & propa) == 0 ){
    	//no podia ser ese valor un candidato, hay que activar el bit de error
    	*celdaptr = (*celdaptr | 0x4000);
    	return 1; //Ha activado bit de error
    }
    return 0;
}

/* *****************************************************************************
 * extrae el valor almacenado en los 16 bits de una celda */
static inline uint8_t
celda_leerValor(CELDA celda)
{
    return (celda & 0x000F);
}

void borrarValorEn(uint8_t fila,uint8_t columna){
	celda_ponerValor(&cuadricula[fila][columna],0);
	sudoku_candidatos_init_arm_thumb(cuadricula);
}
//Devuelve 1 si hay error (no est� en candidatos), 0 si no hay error
int introducirValorEn(uint8_t fila,uint8_t columna,uint8_t valor){
	int hayError = 0; //nohay error = 0, error si = 1;
	if (celda_leerValor(cuadricula[fila][columna]) != 0){
		hayError= celda_ponerValor(&cuadricula[fila][columna],valor);
		sudoku_candidatos_init_arm_thumb(cuadricula);
	}else{
		hayError = celda_ponerValor(&cuadricula[fila][columna],valor);
		if(hayError==0) { //Si no ha habido error, propago
			sudoku_candidatos_propagar_thumb(cuadricula,fila,columna);
		}

	}
	return hayError;
}

int esPista(int fila,int columna){

	CELDA c = cuadricula[fila][columna];

	uint16_t a = (c & 0x8000);

	if( a == 0x8000 ){
		return 0;
	}else{
		return 1;
	}

}
//MAQUINA DE ESTADOS
void maquinaEstadosSudoku(){

	while(1){
		switch (estado){

			case INICIO: //Estado inicial, pulsar boton izq para ir al siguiente estado
					if (botonPulsado!=0){
						botonPulsado = 0;
						sudoku_candidatos_init_arm_thumb(cuadricula);
						estado = APARECER_F;
						permitirPulsacionLarga = 0;
					}
					break;
			case APARECER_F: //Comienza introducci�n de fila, pulsar izq
					D8Led_symbol(15);
					if (botonPulsado==BOTONIZQUIERDO ){
						botonPulsado = 0;
						estado = MOSTRAR_NUMEROS_1_9_F;
						permitirPulsacionLarga = 1;
					}
					break;
			case MOSTRAR_NUMEROS_1_9_F: //Elecci�n de fila
					D8Led_symbol(numeroAmostrar);
					if (botonPulsado==BOTONIZQUIERDO){ //Aumenta n�mero
						botonPulsado = 0;
						numeroAmostrar ++;
						if (numeroAmostrar >9){
							numeroAmostrar=1;
						}

					}
					if (botonPulsado==BOTONDERECHO){ //Confirma n�mero
						botonPulsado = 0;
						filaConfirmada = numeroAmostrar -1;
						numeroAmostrar = 1;
						estado = APARECER_C;
						permitirPulsacionLarga = 0;
					}
					break;
			case APARECER_C: //Comienza introduci�n de columna, pulsar izq
				D8Led_symbol(12);
				if (botonPulsado==BOTONIZQUIERDO ){
					botonPulsado = 0;
					estado = MOSTRAR_NUMEROS_1_9_C;
					permitirPulsacionLarga = 1;
				}
				break;
			case MOSTRAR_NUMEROS_1_9_C: //Elecci�n de columna
				D8Led_symbol(numeroAmostrar);
				if (botonPulsado==BOTONIZQUIERDO){ //Aumenta n�mero
					botonPulsado = 0;
					numeroAmostrar ++;
					if (numeroAmostrar >9){
						numeroAmostrar=1;
					}

				}
				if (botonPulsado==BOTONDERECHO){ //Confirma n�mero
					botonPulsado = 0;
					columnaConfirmada = numeroAmostrar - 1;
					numeroAmostrar = 1;
					if (esPista(filaConfirmada,columnaConfirmada)==0){ //Si es pista vuelve al inicio
						estado = APARECER_F;
						permitirPulsacionLarga = 0;
					}else{ //No es pista, confirmamos celda
						estado = APARECER_V;
						permitirPulsacionLarga = 0;
					}

				}
				break;
			case APARECER_V: //Comienza elecci�n del valor de la casilla
				D8Led_symbol(10);
				if (botonPulsado==BOTONIZQUIERDO ){
					botonPulsado = 0;
					estado = MOSTRAR_NUMEROS_0_9_V;
					permitirPulsacionLarga = 1;
				}
				break;
			case MOSTRAR_NUMEROS_0_9_V: //Elecci�n de valor
				D8Led_symbol(numeroAmostrar);
				if (botonPulsado==BOTONIZQUIERDO){ //Aumenta n�mero
					botonPulsado = 0;
					numeroAmostrar ++;
					if (numeroAmostrar >9){
						numeroAmostrar=0;
					}

				}
				if (botonPulsado==BOTONDERECHO){ //Confirma n�mero
					botonPulsado = 0;
					valorConfirmado = numeroAmostrar;
					numeroAmostrar = 1;
					int hayError= 0;
					if (valorConfirmado == 0){	//Borrado de esa celda
						borrarValorEn(filaConfirmada,columnaConfirmada);
					}else{ 						//Introducir ese valor
						hayError = introducirValorEn(filaConfirmada,columnaConfirmada,valorConfirmado);
					}

					if (hayError==0){


						estado = APARECER_F;	//Vuelve al inicio
						permitirPulsacionLarga = 0;
					}else{
						estado = APARECER_V;
						permitirPulsacionLarga = 0;
					}
				}
				break;
		}
	}
}

