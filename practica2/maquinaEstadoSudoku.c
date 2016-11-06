#include "8led.h"
#include "sudoku_2016.h"

int led[10]={
    ~0xED,
    ~0x60,
    ~0xCE,
    ~0xEA,
    ~0x63,
    ~0xAB,
    ~0x2F,
    ~0xE0,
    ~0xEF,
    ~0xE3
};

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
int botonPulsado = 0;
int estado = INICIO;
int numeroAmostrar = 1;
int filaConfirmada= -1;
int columnaConfirmada = -1;
int valorConfirmado = -1;
//CONSTANTES
int BOTONIZQUIERDO = 4;
int BOTONDERECHO = 8;
// funciones que se necesitan
/* *****************************************************************************
 * modifica el valor almacenado en la celda indicada */
static inline void
celda_ponerValor(CELDA *celdaptr, uint8_t val)
{
    *celdaptr = (*celdaptr & 0xFFF0) | (val & 0x000F);
}

/* *****************************************************************************
 * extrae el valor almacenado en los 16 bits de una celda */
static inline uint8_t
celda_leerValor(CELDA celda)
{
    return (celda & 0x000F);
}

void borrarValorEn(int fila,int columna){
	celda_ponerValor(&cuadricula[fila][columna],0);
	sudoku_candidatos_init_arm_thumb(cuadricula);
}
void introducirValorEn(int fila,int columna,int valor){
	if (celda_leerValor(cuadricula[fila][columna]) != 0){
		celda_ponerValor(&cuadricula[fila][columna],valor);
		sudoku_candidatos_init_arm_thumb(cuadricula);
	}else{
		celda_ponerValor(&cuadricula[fila][columna],valor);
		sudoku_candidatos_propagar_thumb(cuadricula,fila,columna);
	}
}

int esPista(int fila,int columna){
	return (cuadricula[fila][columna] & 0x8000) == 0x8000;

}
//MAQUINA DE ESTADOS
void maquinaEstadosSudoku(){
	switch (estado){
	case INICIO:
			if (botonPulsado!=0){
				botonPulsado = 0;
				estado = APARECER_F;
			}
		break;
	case APARECER_F:
			D8Led_symbol(F);
			if (botonPulsado==BOTONIZQUIERDO ){
				botonPulsado = 0;
				estado = MOSTRAR_NUMEROS_1_9_F;
			}
		break;


	case MOSTRAR_NUMEROS_1_9_F:
			D8Led_symbol(led[numeroAmostrar]);
			if (botonPulsado==BOTONIZQUIERDO){
				botonPulsado = 0;
				numeroAmostrar ++;
				if (numeroAmostrar >9){
					numeroAmostrar=1;
				}

			}
			if (botonPulsado==BOTONDERECHO){
				botonPulsado = 0;
				filaConfirmada = numeroAmostrar;
				numeroAmostrar = 1;
				estado = APARECER_C;
			}
		break;
	case APARECER_C:
		D8Led_symbol(C);
		if (botonPulsado==BOTONIZQUIERDO ){
			botonPulsado = 0;
			estado = MOSTRAR_NUMEROS_1_9_C;
		}
		break;
	case MOSTRAR_NUMEROS_1_9_C:
		D8Led_symbol(led[numeroAmostrar]);
		if (botonPulsado==BOTONIZQUIERDO){
			botonPulsado = 0;
			numeroAmostrar ++;
			if (numeroAmostrar >9){
				numeroAmostrar=1;
			}

		}
		if (botonPulsado==BOTONDERECHO){
			botonPulsado = 0;
			columnaConfirmada = numeroAmostrar;
			numeroAmostrar = 1;
			if (esPista(filaConfirmada,columnaConfirmada)){
				estado = APARECER_F;
			}else{
				estado = APARECER_V;
			}

		}
	break;
	case APARECER_V:
		D8Led_symbol(A);
		if (botonPulsado==BOTONIZQUIERDO ){
			botonPulsado = 0;
			estado = MOSTRAR_NUMEROS_0_9_V;
		}
	break;
	case MOSTRAR_NUMEROS_0_9_V:
		D8Led_symbol(led[numeroAmostrar]);
		if (botonPulsado==BOTONIZQUIERDO){
			botonPulsado = 0;
			numeroAmostrar ++;
			if (numeroAmostrar >9){
				numeroAmostrar=0;
			}

		}
		if (botonPulsado==BOTONDERECHO){
			botonPulsado = 0;
			valorConfirmado = numeroAmostrar;
			numeroAmostrar = 1;

			if (valorConfirmado == 0){
				//Borrado de esa celda
				borrarValorEn(filaConfirmada,columnaConfirmada);
			}else{
				//Introducir ese valor
				introducirValorEn(filaConfirmada,columnaConfirmada,valorConfirmado);
			}
			estado = APARECER_F;
		}
	break;
	}
}

