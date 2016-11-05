#include "8led.h"

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
void borrarValorEn(int fila,int columna);
void introducirValorEn(int fila,int columna);

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

	}
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
// funciones que se necesitan
void borrarValorEn(int fila,int columna){
	celda_poner_valor(cuadricula[fila][columna],0);
	sudoku_candidatos_init_thumb(cuadricula);
}
void introducirValorEn(int fila,int columna,int valor){
	if (celda_leer_valor(cuadricula[fila][columna]) != 0){
		celda_poner_valor(cuadricula[fila][columna],valor);
		sudoku_candidatos_init_thumb(cuadricula);
	}else{
		celda_poner_valor(cuadricula[fila][columna],valor);
		sudoku_candidatos_propagar_thumb(cuadricula);
	}
}
