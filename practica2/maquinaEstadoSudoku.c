/*********************************************************************************************
* Fichero:		maquinaEstadoSudoku.c
* Autor:
* Descrip:		Automata y funciones auxiliares usadas para jugar al sudoku
* Version:
*********************************************************************************************/
#include "8led.h"
#include "sudoku_2016.h"
#include "lcd.h"
#include "itoa.h"
#include "44blib.h"
#include "pintarSudoku.h"

enum {
	//ESTADOS L�GICA JUEGO
    INICIO      					= 0,
    APARECER_F						= 1,
    ESPERANDO_IZQUIERDO    			= 2,
    MOSTRAR_NUMEROS_1_9_F			= 3,
    APARECER_C						= 4,
    MOSTRAR_NUMEROS_1_9_C			= 5,
    APARECER_V						= 6,
    MOSTRAR_NUMEROS_0_9_V			= 7,
    FIN_ESTADISTICAS				= 12,
    PRE_INICIO 						= 8,
    MOSTRAR_NUMEROS_1_9_F_A			= 13,
    MOSTRAR_NUMEROS_1_3				= 14,
    //ESTADOS DE LA PANTALLA
    PINTAR_PRE						= 9,
    PINTAR_NORMAL					= 10,
    PINTAR_FIN						= 11,
    PINTAR_SELECCION				= 15
};

// Variables y valores iniciales
volatile int  botonPulsado = 0;
volatile int  permitirPulsacionLarga = 0;
volatile int estado = PRE_INICIO; // Cual es el inicio

volatile int numeroAmostrar = 1;
volatile int filaConfirmada= -1;
volatile int columnaConfirmada = -1;
volatile int valorConfirmado = -1;
//Touchpad
volatile int touchPulsado = 0;
volatile int modoZoom=0;
volatile int  tX=0, tY=0;
//Pintar LCD
volatile int pintar = 0;		//Indica si se ha de pintar de nuevo el tiempo
volatile int pintarTodo = 0;
volatile int estadoPintar = PINTAR_PRE;
volatile int region=-1;
volatile int unaVez=0;
//valor region pinchada
volatile int regX=0,regY=0;

//valores para dibujar tablero
volatile int regStartX[4] ={ 18, 92, 166, 240};
volatile int regStartY[4] ={ 18, 92, 166, 240};
volatile int textColum = 258;
//Tiempo total de partida en segundos
volatile int decimasSegundos = 0;
volatile int segundos = 0;
volatile int dosdecimilisegundosCalculo = 0;
volatile int segundosCalculo=0;
volatile int pausaCalculo=1;
volatile int empezarTiempo = 0;
//Para saber si ha terminado
volatile int casillasVacias = -1;
volatile int rebotesTouch=0;
volatile int microSegundos=0;
//CONSTANTES
volatile int BOTONIZQUIERDO = 4;
volatile int BOTONDERECHO = 8;

/* *****************************************************************************
 * modifica el valor almacenado en la celda indicada, devuelve 0 si NO modifica bit error, 1 si SI que lo modifica */
static inline int
celda_ponerValor(CELDA *celdaptr, uint8_t val)
{

	//Quitamos bit de error siempre, si es necesario se activa a continuaci�n
    *celdaptr = (*celdaptr & 0xFFF0) | (val & 0x000F);
    return 0;
}

/* *****************************************************************************
 * extrae el valor almacenado en los 16 bits de una celda */
static inline uint8_t
celda_leerValor(CELDA celda)
{
    return (celda & 0x000F);
}
int esError(CELDA celda){
	return (celda & 0x4000);
}
void borrarValorEn(uint8_t fila,uint8_t columna){
	celda_ponerValor(&cuadricula[fila][columna],0);
	int aux = timer2_leer();
	casillasVacias = sudoku_candidatos_init_arm_thumb(cuadricula);
	int fin = timer2_leer();
	microSegundos += (fin-aux);
}
//Devuelve 1 si hay error (no est� en candidatos), 0 si no hay error
int introducirValorEn(uint8_t fila,uint8_t columna,uint8_t valor){
	int hayError = 0; //nohay error = 0, error si = 1;
	if (celda_leerValor(cuadricula[fila][columna]) != 0){
		hayError= celda_ponerValor(&cuadricula[fila][columna],valor);
		int aux = timer2_leer();
		casillasVacias = sudoku_candidatos_init_arm_thumb(cuadricula);
		int fin = timer2_leer();
		microSegundos += (fin-aux);
	}else{
		hayError = celda_ponerValor(&cuadricula[fila][columna],valor);
		casillasVacias = casillasVacias - 1;
		int aux = timer2_leer();
		sudoku_candidatos_propagar_thumb(cuadricula,fila,columna);
		int fin = timer2_leer();
		microSegundos += (fin-aux);


	}
	return hayError;
}
void reiniciarSodu(){
	int i;
	for (i=0;i<9;i++){
		int j;
		for (j=0;j<9;j++){
			if(esPista(i,j)==0){
				cuadricula[i][j]=  cuadricula[i][j] & 0x800F;
			}else{
				cuadricula[i][j]=  cuadricula[i][j] & 0x0;
			}

		}
	}
	casillasVacias = sudoku_candidatos_init_arm_thumb(cuadricula);
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

int actualizarErrores(int *vacios){
	int errores= 0;
	*vacios = 0;

	int i;
	for (i = 0; i<9; i++ ){
		int j ;
		for (j=0; j<9;j++){
			uint8_t valor = celda_leerValor(cuadricula[i][j]);
			//Quitamos bit de error siempre, si es necesario se activa a continuaci�n
			cuadricula[i][j] = cuadricula[i][j] & 0xBFFF;

			if (valor != 0){
				//C�lculo del bit de error
				uint16_t propa = 1<<(valor+3);
				CELDA x = cuadricula[i][j] & 0xFFF0;
				if ((x & propa) == 0 ){
					//no podia ser ese valor un candidato, hay que activar el bit de error
					cuadricula[i][j] = (cuadricula[i][j] | 0x4000);
					errores = errores+ 1; //Ha activado bit de error
				}

			}else{
				*vacios = *vacios + 1;
			}

		}
	}

	return errores;
}

//MAQUINA DE ESTADOS
void maquinaEstadosSudoku(){
	touchPulsado=0;

	while(1){


		switch (estado){
			case PRE_INICIO: //Pintar instrucciones y esperar a que pulse la pantalla con touch
					//si a pulsado el touch saltar a inicio

					if (touchPulsado!=0){
						touchPulsado = 0;
						//action en este estado
						//Siguiente estado
						estado= MOSTRAR_NUMEROS_1_3;
						estadoPintar= PINTAR_SELECCION;

						pintarTodo=1;
						unaVez = 0;
					}
					break;
			case MOSTRAR_NUMEROS_1_3:
				D8Led_symbol(numeroAmostrar);
					if (botonPulsado==BOTONIZQUIERDO){ //Aumenta n�mero
						botonPulsado = 0;
						numeroAmostrar ++;
						if (numeroAmostrar >3){
							numeroAmostrar=1;
						}

					}
					if (botonPulsado==BOTONDERECHO){ //Confirma n�mero
						botonPulsado = 0;

						if ( numeroAmostrar == 1 ){
							cuadricula_copiar(cuadricula,cuadricula1);
						}else if (numeroAmostrar == 2){
							cuadricula_copiar(cuadricula,cuadricula2);
						}else{
							cuadricula_copiar(cuadricula,cuadricula3);
						}
						casillasVacias = sudoku_candidatos_init_arm_thumb(cuadricula);
						numeroAmostrar = 1;

						//Siguiente estado
						estado= APARECER_F;
						permitirPulsacionLarga = 0;
						estadoPintar= PINTAR_NORMAL;
						empezarTiempo = 1;
						pintarTodo=1;

					}
					break;
			case INICIO: //Estado inicial, pulsar boton izq para ir al siguiente estado
					if (botonPulsado!=0){
						botonPulsado = 0;

						estado = APARECER_F;
						permitirPulsacionLarga = 0;


					}
					break;
			case APARECER_F: //Comienza introducci�n de fila, pulsar izq
					D8Led_symbol(15);
					if (botonPulsado==BOTONIZQUIERDO ){
						botonPulsado = 0;
						estado = MOSTRAR_NUMEROS_1_9_F_A;
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
			case MOSTRAR_NUMEROS_1_9_F_A: //Elecci�n de fila
					D8Led_symbol(numeroAmostrar);
					if (botonPulsado==BOTONIZQUIERDO){ //Aumenta n�mero
						botonPulsado = 0;
						numeroAmostrar ++;
						if (numeroAmostrar >10){
							numeroAmostrar=1;
						}

					}
					if (botonPulsado==BOTONDERECHO){ //Confirma n�mero
						botonPulsado = 0;
						filaConfirmada = numeroAmostrar -1;
						if (numeroAmostrar == 10){
							// Ha introducido fila A para acabar
							numeroAmostrar = 1;
							estado = FIN_ESTADISTICAS;
							estadoPintar= PINTAR_FIN;
							pintarTodo=1;
							empezarTiempo = 0;
							unaVez = 0;
						}else{

							numeroAmostrar = 1;
							estado = APARECER_C;
							permitirPulsacionLarga = 0;
						}

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
			case MOSTRAR_NUMEROS_1_9_C: //Elecci�n de columnaa
				D8Led_symbol(numeroAmostrar);
				if (botonPulsado==BOTONIZQUIERDO){ //Aumentar n�mero
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
					//int hayError= 0;
					//int aux = timer2_leer();
					if (valorConfirmado == 0){	//Borrado de esa celda
						borrarValorEn(filaConfirmada,columnaConfirmada);
					}else{ 						//Introducir ese valor
						introducirValorEn(filaConfirmada,columnaConfirmada,valorConfirmado);
					}


					int errores =0, vacios = 0;


					errores = actualizarErrores(&vacios);


					//Esquema para terminar partida
					if(vacios !=0){
						//Aun faltan celdas por rellenar, el usuario aun no ha terminado la partida
						estado = APARECER_F;	//Vuelve al inicio

					}else{
						//Ya no quedan celdas vacias, el usuario terminara si no hay errores
						if (errores >0){
							estado = APARECER_F;	//Vuelve al inicio
						}else{
							estado = FIN_ESTADISTICAS;
							estadoPintar= PINTAR_FIN;
							pintarTodo=1;
							empezarTiempo = 0;
							unaVez = 0;
							numeroAmostrar = 1;
							//

						}

					}




					pintarTodo=1;
				}
				break;
			case FIN_ESTADISTICAS:
				D8Led_symbol(blank);

				if (touchPulsado!=0){

					touchPulsado = 0;
					//action en este estado
					//codigo para reiniciar el sudoku

					decimasSegundos = 0;
					segundos = 0;
					dosdecimilisegundosCalculo = 0;
					segundosCalculo=0;
					//Siguiente estado
					estado= MOSTRAR_NUMEROS_1_3;
					estadoPintar= PINTAR_SELECCION;
					microSegundos=0;
					pintarTodo=1;
					unaVez = 0;

				}
				break;
		}//fin switch

		//despues de calcular las modificaciones del tablero
		//pasamos a pintar,
		//pintar es puesto a 1 por la interrupcion de reloj del timer2 cada cierto tiempo

		if (touchPulsado!=0){
			touchPulsado=0;
			rebotesTouch=1;

			if (modoZoom==1){
				modoZoom=0;
				pintarTodo=1;
			}else{
				region = calcularRegion();
				//si ha pulsado en una zona correcta, activamos zoom
				if (region != -1){
					modoZoom=1;
					pintarTodo=1;
				}

			}
		}
		switch (estadoPintar){
			case PINTAR_PRE:
				pintarPre();
			break;
			case PINTAR_NORMAL:
				pintarNormal();
				break;
			case PINTAR_FIN:
				pintarFinal();
				break;
			case PINTAR_SELECCION:
				pintarSel();
				break;
		}

	}//fin while
}

//FUNCIONES DE PINTADO
//Se tiene una pantalla de 320x240, si se usa el tablero

