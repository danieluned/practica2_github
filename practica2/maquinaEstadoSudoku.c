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

enum {
	//ESTADOS LÓGICA JUEGO
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
    //ESTADOS DE LA PANTALLA
    PINTAR_PRE						= 9,
    PINTAR_NORMAL					= 10,
    PINTAR_FIN						= 11
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
extern unsigned int  Vx, Vy;
//Pintar LCD
volatile int pintar = 0;
volatile int estadoPintar = PINTAR_PRE;
volatile int region=-1;
//valor region pinchada
int regX=0,regY=0;

//valores para dibujar tablero
int regStartX[4] ={ 18, 92, 166, 240};
int regStartY[4] ={ 18, 92, 166, 240};
int textColum = 258;
//Tiempo total de partida en segundos
volatile int decimasSegundos = 0;
volatile int segundos = 0;
volatile int dosdecimilisegundosCalculo = 0;
volatile int segundosCalculo=0;
volatile int pausaCalculo=1;
//Para saber si ha terminado
volatile int casillasVacias = -1;

//CONSTANTES
int BOTONIZQUIERDO = 4;
int BOTONDERECHO = 8;
// funciones que se necesitan
/* *****************************************************************************
 * modifica el valor almacenado en la celda indicada, devuelve 0 si NO modifica bit error, 1 si SI que lo modifica */
static inline int
celda_ponerValor(CELDA *celdaptr, uint8_t val)
{

	//Quitamos bit de error siempre, si es necesario se activa a continuación
    *celdaptr = (*celdaptr & 0xBFF0) | (val & 0x000F);


    //Cálculo del bit de error
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
int esError(CELDA celda){
	return (celda & 0x4000);
}
void borrarValorEn(uint8_t fila,uint8_t columna){
	celda_ponerValor(&cuadricula[fila][columna],0);
	sudoku_candidatos_init_arm_thumb(cuadricula);
}
//Devuelve 1 si hay error (no está en candidatos), 0 si no hay error
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
			case PRE_INICIO: //Pintar instrucciones y esperar a que pulse la pantalla con touch
					//si a pulsado el touch saltar a inicio
					if (touchPulsado!=0){
						touchPulsado = 0;
						//action en este estado
						//Siguiente estado
						estado= INICIO;
						permitirPulsacionLarga = 0;
						estadoPintar= PINTAR_NORMAL;
					}
					break;
			case INICIO: //Estado inicial, pulsar boton izq para ir al siguiente estado
					if (botonPulsado!=0){
						botonPulsado = 0;
						sudoku_candidatos_init_arm_thumb(cuadricula);
						estado = APARECER_F;
						permitirPulsacionLarga = 0;


					}
					break;
			case APARECER_F: //Comienza introducción de fila, pulsar izq
					D8Led_symbol(15);
					if (botonPulsado==BOTONIZQUIERDO ){
						botonPulsado = 0;
						estado = MOSTRAR_NUMEROS_1_9_F_A;
						permitirPulsacionLarga = 1;
					}
					break;
			case MOSTRAR_NUMEROS_1_9_F: //Elección de fila
					D8Led_symbol(numeroAmostrar);
					if (botonPulsado==BOTONIZQUIERDO){ //Aumenta número
						botonPulsado = 0;
						numeroAmostrar ++;
						if (numeroAmostrar >9){
							numeroAmostrar=1;
						}

					}
					if (botonPulsado==BOTONDERECHO){ //Confirma número
						botonPulsado = 0;
						filaConfirmada = numeroAmostrar -1;
						numeroAmostrar = 1;
						estado = APARECER_C;
						permitirPulsacionLarga = 0;
					}
					break;
			case MOSTRAR_NUMEROS_1_9_F_A: //Elección de fila
								D8Led_symbol(numeroAmostrar);
								if (botonPulsado==BOTONIZQUIERDO){ //Aumenta número
									botonPulsado = 0;
									numeroAmostrar ++;
									if (numeroAmostrar >10){
										numeroAmostrar=1;
									}

								}
								if (botonPulsado==BOTONDERECHO){ //Confirma número
									botonPulsado = 0;
									filaConfirmada = numeroAmostrar -1;
									if (numeroAmostrar == 10){
										numeroAmostrar = 1;
										estado = APARECER_C;
										permitirPulsacionLarga = 0;
									}else{
										numeroAmostrar = 1;
										estado = APARECER_C;
										permitirPulsacionLarga = 0;
									}

								}
								break;
			case APARECER_C: //Comienza introdución de columna, pulsar izq
				D8Led_symbol(12);
				if (botonPulsado==BOTONIZQUIERDO ){
					botonPulsado = 0;
					estado = MOSTRAR_NUMEROS_1_9_C;
					permitirPulsacionLarga = 1;
				}
				break;
			case MOSTRAR_NUMEROS_1_9_C: //Elección de columnaa
				D8Led_symbol(numeroAmostrar);
				if (botonPulsado==BOTONIZQUIERDO){ //Aumentar número
					botonPulsado = 0;
					numeroAmostrar ++;
					if (numeroAmostrar >9){
						numeroAmostrar=1;
					}

				}
				if (botonPulsado==BOTONDERECHO){ //Confirma número
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
			case APARECER_V: //Comienza elección del valor de la casilla
				D8Led_symbol(10);
				if (botonPulsado==BOTONIZQUIERDO ){
					botonPulsado = 0;
					estado = MOSTRAR_NUMEROS_0_9_V;
					permitirPulsacionLarga = 1;
				}
				break;
			case MOSTRAR_NUMEROS_0_9_V: //Elección de valor
				D8Led_symbol(numeroAmostrar);
				if (botonPulsado==BOTONIZQUIERDO){ //Aumenta número
					botonPulsado = 0;
					numeroAmostrar ++;
					if (numeroAmostrar >9){
						numeroAmostrar=0;
					}

				}
				if (botonPulsado==BOTONDERECHO){ //Confirma número
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
						if(casillasVacias !=0){
							estado = APARECER_F;	//Vuelve al inicio
						}else{
							//Si ya no hay mas casillas vacias, ha terminado
							estado = FIN_ESTADISTICAS;
							estadoPintar= PINTAR_FIN;
						}

						permitirPulsacionLarga = 0;

					}else{
						estado = APARECER_V;
						permitirPulsacionLarga = 0;
					}
				}
				break;
			case FIN_ESTADISTICAS:
				if (touchPulsado!=0){
					touchPulsado = 0;
					//action en este estado
					//Siguiente estado
					estado= INICIO;
					permitirPulsacionLarga = 0;
					estadoPintar= PINTAR_NORMAL;
				}
				break;
		}//fin switch

		//despues de calcular las modificaciones del tablero
		//pasamos a pintar,
		//pintar es puesto a 1 por la interrupcion de reloj del timer2 cada cierto tiempo
		if (pintar!=0){
			pintar = 0;
			if (touchPulsado!=0){
				touchPulsado=0;


				if (modoZoom==1){
					modoZoom=0;
				}else{
					region = calcularRegion();
					//si ha pulsado en una zona correcta, activamos zoom
					if (region != -1)
					modoZoom=1;
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
			}
		}
	}//fin while
}

//FUNCIONES DE PINTADO
//Se tiene una pantalla de 320x240, si se usa el tablero

/*
 * 		16 numero
 * 16 numero
 *
 * 	______________
 * 	240 - 20, 220, -> 72 pixeles por region |
 *
 * 	de margen derecho queda 80 pixeles -> 10 letras y 13 lineas sie
 */
void pintarPre(){
	Lcd_Init();
	Lcd_Clr();
	Lcd_Active_Clr();
	Lcd_DspAscII8x16(100,25,BLACK,"Toque la pantalla para jugar");
	Lcd_Dma_Trans();
}
void pintarNormal(){
	Lcd_Clr();
	Lcd_Active_Clr();
	//Pintar "tiempo total"
	pintarTiempoTotal();
	//Pintar "cálculos"
	pintarCalculos();
	//pintar leyenda "Introduzca Fila A para acabar la partida"
	pintarLeyenda();
	//si hay zoom
	if (modoZoom==1){
		//Region numerada
		pintarRegion();
		//pintar celdas region diferenciando pistas de valores del usuario
		pintarValoresRegion();
		//pintar celdas vacias region con listas de candidatos
		//resaltar error region
		pintarCandidatosRegion();
	}else{
	//sino

		//tablero numerado
		pintarTablero();
		//pintar celdas diferenciando pistas de valores del usuario
		pintarValores();
		//pintar celdas vacias con listas de candidatos
		//resaltar error tablero
		pintarCandidatos();
	}
	Lcd_Dma_Trans();
}


//apartir de los valores pulsados obtenemos que region es
int calcularRegion(){
	if (regStartX[0] <= Vx && Vx < regStartX[1] && regStartY[0] <= Vy && Vy < regStartY[1]){
		regX=0;regY=0;
		return 1;
	}else if (regStartX[1] <= Vx && Vx< regStartX[2] && regStartY[0] <= Vy && Vy < regStartY[1]){
		regX=0;regY=1;
		return 2;
	}else if (regStartX[2] <= Vx && Vx< regStartX[3] && regStartY[0] <= Vy && Vy < regStartY[1]){
		regX=0;regY=2;
		return 3;
	}else if (regStartX[0] <= Vx && Vx < regStartX[1] && regStartY[1] <= Vy && Vy < regStartY[2]){
		regX=1;regY=0;
		return 4;
	}else if (regStartX[1] <= Vx&& Vx < regStartX[2] && regStartY[1] <= Vy && Vy < regStartY[2]){
		regX=1;regY=1;
		return 5;
	}else if (regStartX[2] <= Vx && Vx< regStartX[3] && regStartY[1] <= Vy  && Vy< regStartY[2]){
		regX=1;regY=2;
		return 6;
	}else if (regStartX[0] <= Vx && Vx< regStartX[1] && regStartY[2] <= Vy  && Vy< regStartY[3]){
		regX=2;regY=0;
		return 7;
	}else if (regStartX[1] <= Vx && Vx< regStartX[2] && regStartY[2] <= Vy && Vy < regStartY[3]){
		regX=2;regY=1;
		return 8;
	}else if (regStartX[2] <= Vx && Vx< regStartX[3] && regStartY[2] <= Vy && Vy < regStartY[3]){
		regX=2;regY=2;
		return 9;
	}
	return -1;
}
void pintarFinal(){
	Lcd_Clr();
	Lcd_Active_Clr();
	// Pintar resultado
	// pintar leyenda "Toque la pantalla para jugar"
	Lcd_DspAscII8x16(50,60, BLACK, "Tiempo total:");
	Lcd_DspAscII8x16(274,60,BLACK, itoa(decimasSegundos));
	Lcd_DspAscII8x16(50,80, BLACK, "Calculos");
	Lcd_DspAscII8x16(274,80,BLACK, itoa(dosdecimilisegundosCalculo*2));
	Lcd_DspAscII8x16(240,1, BLACK, "Toque la pantalla para jugar");
	Lcd_Dma_Trans();
}
//FUNCIONEs AUXILIARes cuando es pintar normal
void pintarTiempoTotal(){
	Lcd_DspAscII8x16(textColum,0, BLACK, "Tiempo");
	Lcd_DspAscII8x16(textColum,17, BLACK, "total:");
	char *str= "";
	strcat(str,  itoa(segundos));
	strcat(str, ",");
	strcat(str, itoa(decimasSegundos));
	strcat(str,"\0");
	Lcd_DspAscII8x16(textColum,34,BLACK,str);
}
void pintarCalculos(){
	Lcd_DspAscII8x16(textColum,51, BLACK, "Calculos");
	//precision  de (0,0000 segundos) cuatro decimales; se multipla para obtener el resultado correcto
	int deci= dosdecimilisegundosCalculo*2;
	char *str= "";
	strcat(str,  itoa(segundosCalculo));

	if (deci <10){
		strcat(str, ",000");
	}else if (deci<100){
		strcat(str, ",00");
	}else if (deci<1000){
		strcat(str, ",0");
	}else{
		strcat(str, ",");
	}
	strcat(str, itoa(dosdecimilisegundosCalculo*2));
	strcat(str,"\0");
	Lcd_DspAscII8x16(textColum,68,BLACK, str);
}
void pintarLeyenda(){
	Lcd_DspAscII8x16(textColum,102, BLACK, "Introduzca");
	Lcd_DspAscII8x16(textColum,119, BLACK, "Fila A ");
	Lcd_DspAscII8x16(textColum,136, BLACK, "para ");
	Lcd_DspAscII8x16(textColum,153, BLACK, "acabar ");
	Lcd_DspAscII8x16(textColum,170, BLACK, "la ");
	Lcd_DspAscII8x16(textColum,187, BLACK, "partida");
}


void pintarNumero24x48(int x,int y,int num){
	switch (num){
	case 0:
		Lcd_Draw_HLine(x,x+24,y,BLACK,1);
		Lcd_Draw_HLine(x,x+24,y+48,BLACK,1);

		Lcd_Draw_VLine(y,y+48,x,BLACK,1);
		Lcd_Draw_VLine(y,y+48,x+24,BLACK,1);


		break;
	case 1:
		Lcd_Draw_VLine(y,y+48,x+24,BLACK,1);

		break;
	case 2:
		Lcd_Draw_HLine(x,x+24,y,BLACK,1);
		Lcd_Draw_HLine(x,x+24,y+24,BLACK,1);
		Lcd_Draw_HLine(x,x+24,y+48,BLACK,1);

		Lcd_Draw_VLine(y,y+24,x+24,BLACK,1);
		Lcd_Draw_VLine(y+24,y+48,x,BLACK,1);
		break;
	case 3:
		Lcd_Draw_HLine(x,x+24,y,BLACK,1);
		Lcd_Draw_HLine(x,x+24,y+24,BLACK,1);
		Lcd_Draw_HLine(x,x+24,y+48,BLACK,1);

		Lcd_Draw_VLine(y,y+24,x+24,BLACK,1);
		Lcd_Draw_VLine(y+24,y+48,x+24,BLACK,1);
		break;
	case 4:
		Lcd_Draw_VLine(y,y+24,x,BLACK,1);
		Lcd_Draw_VLine(y,y+48,x+24,BLACK,1);

		Lcd_Draw_HLine(x,x+24,y+24,BLACK,1);
		break;
	case 5:
		Lcd_Draw_HLine(x,x+24,y,BLACK,1);
		Lcd_Draw_HLine(x,x+24,y+24,BLACK,1);
		Lcd_Draw_HLine(x,x+24,y+48,BLACK,1);

		Lcd_Draw_VLine(y,y+24,x,BLACK,1);
		Lcd_Draw_VLine(y+24,y+48,x+24,BLACK,1);

		break;
	case 6:
		Lcd_Draw_HLine(x,x+24,y,BLACK,1);
		Lcd_Draw_HLine(x,x+24,y+24,BLACK,1);
		Lcd_Draw_HLine(x,x+24,y+48,BLACK,1);

		Lcd_Draw_VLine(y,y+48,x,BLACK,1);
		Lcd_Draw_VLine(y+24,y+48,x+24,BLACK,1);
		break;
	case 7:
		Lcd_Draw_HLine(x,x+24,y,BLACK,1);

		Lcd_Draw_VLine(y,y+48,x+24,BLACK,1);
		break;
	case 8:
		Lcd_Draw_HLine(x,x+24,y,BLACK,1);
		Lcd_Draw_HLine(x,x+24,y+24,BLACK,1);
		Lcd_Draw_HLine(x,x+24,y+48,BLACK,1);

		Lcd_Draw_VLine(y,y+48,x,BLACK,1);
		Lcd_Draw_VLine(y,y+48,x+24,BLACK,1);
		break;
	case 9:
		Lcd_Draw_HLine(x,x+24,y,BLACK,1);
		Lcd_Draw_HLine(x,x+24,y+24,BLACK,1);
		Lcd_Draw_HLine(x,x+24,y+48,BLACK,1);

		Lcd_Draw_VLine(y,y+24,x,BLACK,1);
		Lcd_Draw_VLine(y,y+48,x+24,BLACK,1);
		break;
	default:
		break;

	}
}
//PINTAR REGION
void pintarRegion(){
	int i;
	for (i=0;i<4;i++){
		Lcd_Draw_HLine(regStartX[0],regStartX[3],regStartY[i],BLACK,1);
		Lcd_Draw_HLine(regStartY[0],regStartY[3],regStartX[i],BLACK,1);

		char v[2] ;
		v[0]= '1'+(i+regX);
		v[1]= '\0';
		Lcd_DspAscII8x16(regStartX[0],regStartY[i],BLACK,v);
		v[0]= '1'+(i+regY);
		v[1]= '\0';
		Lcd_DspAscII8x16(regStartX[i],regStartY[0],BLACK,v);
	}

}
void pintarValoresRegion(){
	int i,j;
	for (i=0;i<3;i++){
		for (j=0;j<3;j++){
			int valor= celda_leerValor(cuadricula[i+regX][j+regY]);
			pintarNumero24x48(regStartX[0]+17*i,regStartY[0]+17*j,valor);
		}

	}
}
void pintarCandidatosRegion(){
	int i, j;
	for (i=0; i<3; i++) {
		for (j=0; j<3; j++) {
			if (celda_leerValor(cuadricula[i+regX][j+regY]) == 0) {
				CELDA candidatos = cuadricula[i+regX][j+regY];
				candidatos &= 0x01FF;
				int z;
				for (z=0; z<9; z++) {
					if ((candidatos & (1 << z)) != 0)
						Lcd_DspAscII8x16(6*(z)+6+20*(j+1), 30+5*(z/3)+20*(i+1), BLACK, ".");
				}
			}
		}
	}
}
//PINTAR TAblero
void pintarTablero(){
	int i;
	for (i=1; i<=9; i++) {
		if (i==1 || i==4 || i==7) {
			Lcd_Draw_HLine(25,205,55+(i-1)*20,BLACK,2);
			Lcd_Draw_VLine (55,235,25+(i-1)*20,BLACK,2);
		}
		else {
			Lcd_Draw_HLine(25,205,55+(i-1)*20,BLACK,1);
			Lcd_Draw_VLine (55,235,25+(i-1)*20,BLACK,1);
		}
	}
	Lcd_Draw_HLine(25,205,235,BLACK,2);
	Lcd_Draw_VLine (55,235,205,BLACK,2);
}
void pintarValores(){
	short int i, j;
	for (i=1; i<=9; i++) {
		for (j=1; j<=9; j++) {
			uint8_t valor;
			if ((valor=celda_leerValor(cuadricula[i-1][j-1])) != 0) {
				switch (esPista(i-1,j-1)) {
				case 0:
					//Lcd_pintarNumero(i,j,valor,DARKGRAY);
					break;
				case 1:
					//Lcd_pintarNumero(i,j,valor,BLACK);
					break;
				default:
					break;
				}
				if (esError(cuadricula[i-1][j-1])==TRUE) {
					Lcd_Draw_Box(28+(j-1)*20, 58+(i-1)*20, 15+28+(j-1)*20, 15+58+(i-1)*20, BLACK);
				}
			}
		}
	}
}
void pintarCandidatos(){
	short int i, j;
	for (i=0; i<9; i++) {
		for (j=0; j<9; j++) {
			if (celda_leerValor(cuadricula[i][j]) == 0) {
				CELDA candidatos = cuadricula[i][j];
				candidatos &= 0x01FF;
				short int it;
				CELDA aux;
				for (it=0; it<9; it++) {
					aux = candidatos & (0x01 << it);
					if ((candidatos & (0x01 << it)) != 0)	Lcd_DspAscII8x16(6*(it%3)+6+20*(j+1), 30+5*(it/3)+20*(i+1), BLACK, ".");
				}
			}
		}
	}
}
