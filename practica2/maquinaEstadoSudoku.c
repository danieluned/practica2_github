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
    uint16_t propa = 1<<(val+3);
    if ( (*celdaptr & propa) != 0 ){
    	//podia ser un candidato
    	;
    }else{
    	//no podia ser ese valor un candidato, hay que poner el bit de error
    	*celdaptr = (*celdaptr | 0x2000);
    }
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
void introducirValorEn(uint8_t fila,uint8_t columna,uint8_t valor){
	if (celda_leerValor(cuadricula[fila][columna]) != 0){
		celda_ponerValor(&cuadricula[fila][columna],valor);
		sudoku_candidatos_init_arm_thumb(cuadricula);
	}else{
		celda_ponerValor(&cuadricula[fila][columna],valor);
		sudoku_candidatos_propagar_thumb(cuadricula,fila,columna);
	}
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

			case INICIO:
					if (botonPulsado!=0){
						botonPulsado = 0;
						sudoku_candidatos_init_arm_thumb(cuadricula);
						estado = APARECER_F;
					}
					break;
			case APARECER_F:
					D8Led_symbol(15);
					if (botonPulsado==BOTONIZQUIERDO ){
						botonPulsado = 0;
						estado = MOSTRAR_NUMEROS_1_9_F;
					}
					break;
			case MOSTRAR_NUMEROS_1_9_F:
					D8Led_symbol(numeroAmostrar);
					if (botonPulsado==BOTONIZQUIERDO){
						botonPulsado = 0;
						numeroAmostrar ++;
						if (numeroAmostrar >9){
							numeroAmostrar=1;
						}

					}
					if (botonPulsado==BOTONDERECHO){
						botonPulsado = 0;
						filaConfirmada = numeroAmostrar -1;
						numeroAmostrar = 1;
						estado = APARECER_C;
					}
					break;
			case APARECER_C:
				D8Led_symbol(12);
				if (botonPulsado==BOTONIZQUIERDO ){
					botonPulsado = 0;
					estado = MOSTRAR_NUMEROS_1_9_C;
				}
				break;
			case MOSTRAR_NUMEROS_1_9_C:
				D8Led_symbol(numeroAmostrar);
				if (botonPulsado==BOTONIZQUIERDO){
					botonPulsado = 0;
					numeroAmostrar ++;
					if (numeroAmostrar >9){
						numeroAmostrar=1;
					}

				}
				if (botonPulsado==BOTONDERECHO){
					botonPulsado = 0;
					columnaConfirmada = numeroAmostrar - 1;
					numeroAmostrar = 1;
					if (esPista(filaConfirmada,columnaConfirmada)==0){
						estado = APARECER_F;
					}else{
						estado = APARECER_V;
					}

				}
				break;
			case APARECER_V:
				D8Led_symbol(10);
				if (botonPulsado==BOTONIZQUIERDO ){
					botonPulsado = 0;
					estado = MOSTRAR_NUMEROS_0_9_V;
				}
				break;
			case MOSTRAR_NUMEROS_0_9_V:
				D8Led_symbol(numeroAmostrar);
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
}

