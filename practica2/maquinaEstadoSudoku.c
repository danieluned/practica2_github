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
volatile int  tX=0, tY=0;
//Pintar LCD
volatile int pintar = 0;		//Indica si se ha de pintar de nuevo el tiempo
volatile int pintarTodo = 0;
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
volatile int empezarTiempo = 0;
//Para saber si ha terminado
volatile int casillasVacias = -1;

//CONSTANTES
int BOTONIZQUIERDO = 4;
int BOTONDERECHO = 8;
// funciones que se necesitan
void pintarCandidatos();
void pintarValores();
void pintarTablero();
void pintarCandidatosRegion();
void pintarValoresRegion();
void pintarRegion();
void pintarNumero24x48(int x,int y,int num,char color);
void pintarLeyenda();
void pintarCalculos();
void pintarTiempoTotal();
void pintarFinal();
int calcularRegion();
void pintarNormal();
void pintarPre();
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
						empezarTiempo = 1;
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

		if (touchPulsado!=0){
			touchPulsado=0;


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

	Lcd_Clr();
	Lcd_Active_Clr();
	Lcd_DspAscII8x16(100,25,BLACK,"Toque la pantalla para jugar");
	Lcd_Dma_Trans();
}
void pintarNormal(){
	if (pintarTodo==1){


	Lcd_Clr();
	Lcd_Active_Clr();
	}
	//Pintar "tiempo total"
	if (pintar==1 || pintarTodo==1){
		pintar=0;
		pintarTiempoTotal();
		//Pintar "cálculos"
		pintarCalculos();
	}
	if (pintarTodo==1)pintarTodo=0;
	//pintar leyenda "Introduzca Fila A para acabar la partida"
	pintarLeyenda();
	//si hay zoom
	//modoZoom =1;
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


	//touch varia en x desde :89 a 340
	// 				y desde : 745 a 518
    //normalizar a 320x240
	// tX= (tX-89);
	//tX/340-89 = x / 320

	//0		1   	2     	3
	//80    140     198		267

	//0		1			2			3
	//710   685-673  	620-621		550 504
	int v0=80,v1=140,v2=198,v3=257; // Separaciones verticales

	int h0=710,h1=673,h2=620,h3=504;

	if (v0 <= tX && tX < v1 && h1 < tY && tY <= h0){
		regX=0;regY=0;
		return 1;
	}else if (v1 <= tX && tX< v2 && h1 < tY && tY <= h0){
		regX=0;regY=1;
		return 2;
	}else if (v2 <= tX && tX< v3 && h1 < tY && tY <= h0){
		regX=0;regY=2;
		return 3;
	}else if (v0 <= tX && tX < v1 && h2 < tY && tY <= h1){
		regX=1;regY=0;
		return 4;
	}else if (v1 <= tX && tX < v2 && h2 < tY && tY <= h1){
		regX=1;regY=1;
		return 5;
	}else if (v2 <= tX && tX< v3 && h2 < tY  && tY<= h1){
		regX=1;regY=2;
		return 6;
	}else if (v0 <= tX && tX< v1 && h3 < tY  && tY<= h2){
		regX=2;regY=0;
		return 7;
	}else if (v1 <= tX && tX< v2 && h3 < tY && tY <= h2){
		regX=2;regY=1;
		return 8;
	}else if (v2 <= tX && tX< v3 && h3 < tY && tY <= h2){
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

	LcdClrRect(textColum,34,textColum+70,34+16,WHITE);
	Lcd_DspAscII8x16(textColum,0, BLACK, "Tiempo");
	Lcd_DspAscII8x16(textColum,17, BLACK, "total:");
	char str[10];
	strcpy(str,"");
	strcat(str,  itoa(segundos));
	strcat(str, ",");
	strcat(str, itoa(decimasSegundos));
	str[9]='\0';
	Lcd_DspAscII8x16(textColum,34,BLACK,str);
}
void pintarCalculos(){
	Lcd_DspAscII8x16(textColum,51, BLACK, "Calculos");
	//precision  de (0,0000 segundos) cuatro decimales; se multipla para obtener el resultado correcto
	int deci= dosdecimilisegundosCalculo*2;
	char str[10];
	strcpy(str,"");
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
	Lcd_DspAscII8x16(textColum,102, BLACK, "Ponga");
	Lcd_DspAscII8x16(textColum,119, BLACK, "Fila A ");
	Lcd_DspAscII8x16(textColum,136, BLACK, "para ");
	Lcd_DspAscII8x16(textColum,153, BLACK, "acabar ");
	Lcd_DspAscII8x16(textColum,170, BLACK, "la ");
	Lcd_DspAscII8x16(textColum,187, BLACK, "partida");
}

void pintarNumero24x48(int x,int y,int num,char color){
	switch (num){
	case 0:
		Lcd_Draw_HLine(x,x+24,y,color,1);
		Lcd_Draw_HLine(x,x+24,y+48,color,1);

		Lcd_Draw_VLine(y,y+48,x,color,1);
		Lcd_Draw_VLine(y,y+48,x+24,color,1);


		break;
	case 1:
		Lcd_Draw_VLine(y,y+48,x+24,color,1);

		break;
	case 2:
		Lcd_Draw_HLine(x,x+24,y,color,1);
		Lcd_Draw_HLine(x,x+24,y+24,color,1);
		Lcd_Draw_HLine(x,x+24,y+48,color,1);

		Lcd_Draw_VLine(y,y+24,x+24,color,1);
		Lcd_Draw_VLine(y+24,y+48,x,color,1);
		break;
	case 3:
		Lcd_Draw_HLine(x,x+24,y,color,1);
		Lcd_Draw_HLine(x,x+24,y+24,color,1);
		Lcd_Draw_HLine(x,x+24,y+48,color,1);

		Lcd_Draw_VLine(y,y+24,x+24,color,1);
		Lcd_Draw_VLine(y+24,y+48,x+24,color,1);
		break;
	case 4:
		Lcd_Draw_VLine(y,y+24,x,color,1);
		Lcd_Draw_VLine(y,y+48,x+24,color,1);

		Lcd_Draw_HLine(x,x+24,y+24,color,1);
		break;
	case 5:
		Lcd_Draw_HLine(x,x+24,y,color,1);
		Lcd_Draw_HLine(x,x+24,y+24,color,1);
		Lcd_Draw_HLine(x,x+24,y+48,color,1);

		Lcd_Draw_VLine(y,y+24,x,color,1);
		Lcd_Draw_VLine(y+24,y+48,x+24,color,1);

		break;
	case 6:
		Lcd_Draw_HLine(x,x+24,y,color,1);
		Lcd_Draw_HLine(x,x+24,y+24,color,1);
		Lcd_Draw_HLine(x,x+24,y+48,color,1);

		Lcd_Draw_VLine(y,y+48,x,color,1);
		Lcd_Draw_VLine(y+24,y+48,x+24,color,1);
		break;
	case 7:
		Lcd_Draw_HLine(x,x+24,y,color,1);

		Lcd_Draw_VLine(y,y+48,x+24,color,1);
		break;
	case 8:
		Lcd_Draw_HLine(x,x+24,y,color,1);
		Lcd_Draw_HLine(x,x+24,y+24,color,1);
		Lcd_Draw_HLine(x,x+24,y+48,color,1);

		Lcd_Draw_VLine(y,y+48,x,color,1);
		Lcd_Draw_VLine(y,y+48,x+24,color,1);
		break;
	case 9:
		Lcd_Draw_HLine(x,x+24,y,color,1);
		Lcd_Draw_HLine(x,x+24,y+24,color,1);
		Lcd_Draw_HLine(x,x+24,y+48,color,1);

		Lcd_Draw_VLine(y,y+24,x,color,1);
		Lcd_Draw_VLine(y,y+48,x+24,color,1);
		break;
	default:
		break;

	}
}
//PINTAR REGION

void pintarRegion(){



	int i;
	for (i=0;i<3;i++){
		Lcd_Draw_HLine(regStartX[0],regStartX[3],regStartY[i],BLACK,2);
		Lcd_Draw_VLine(regStartY[0],regStartY[3],regStartX[i],BLACK,2);

		//pintar numeros borde
		char v[2] ;
		//numeros verticales
		v[0]= '1'+(i+regX*3);
		v[1]= '\0';
		Lcd_DspAscII8x16(regStartX[0]-17,regStartY[i],BLACK,v);
		//numeros horizontales
		v[0]= '1'+(i+regY*3);
		v[1]= '\0';
		Lcd_DspAscII8x16(regStartX[i],regStartY[0]-17,BLACK,v);
	}
	Lcd_Draw_HLine(regStartX[0],regStartX[3],regStartY[3]-1,BLACK,2);
	Lcd_Draw_VLine(regStartY[0],regStartY[3],regStartX[3],BLACK,2);
}
void pintarValoresRegion(){
	int i,j;
	char v[2];
	for (i=0;i<3;i++){
		for (j=0;j<3;j++){
			int valor= celda_leerValor(cuadricula[i+regX*3][j+regY*3]);
			if (valor != 0) {
				switch (esPista(i+regX*3,j+regY*3)) {
				case 0:

					v[0] = '0';
					v[0] += valor;
					v[1]='\0';
					//Lcd_DspAscII8x16(regStartX[j], regStartY[i], DARKGRAY, v);
					pintarNumero24x48(regStartX[j]+25, regStartY[i]+15,valor,DARKGRAY);
					break;
				case 1:
					v[0] = '0';
					v[0] += valor;
					v[1]='\0';
					//Lcd_DspAscII8x16(regStartX[j], regStartY[i], BLACK, v);
					pintarNumero24x48(regStartX[j]+25, regStartY[i]+15,valor,BLACK);
					break;
				default:
					break;
				}
				if (esError(cuadricula[i+regX*3][j+regY*3])==TRUE) {
					Lcd_Draw_Box(28+(j-1)*20, 58+(i-1)*20, 15+28+(j-1)*20, 15+58+(i-1)*20, BLACK);
				}
			}
		}
	}
}

void pintarCandidatosRegion(){
	int i, j;
	char v[2];
	for (i=0; i<3; i++) {
		for (j=0; j<3; j++) {
			int valor=celda_leerValor(cuadricula[i+regX*3][j+regY*3]);
			if (valor == 0) {
				CELDA candidatos = cuadricula[i+regX*3][j+regY*3];
				candidatos &= 0x1FF0;
				candidatos = candidatos >> 4;

				v[1]='\0';
				int desplazador;
				for (desplazador=0; desplazador<9; desplazador++) {
				if ((candidatos & (1 << desplazador)) != 0){
					v[0]='1'+desplazador;
					Lcd_DspAscII8x16(25*(desplazador%3)+regStartX[j]+10, 25*(desplazador/3)+regStartY[i]+5, BLACK, v);
				}


				}
			}
		}
	}
}
//PINTAR TAblero

void pintarTablero(){
	int i;
	for (i=0; i<9; i++) {
		//pintar numeros borde
		char v[2] ;
		//numeros verticales y horizontales
		v[0]= '1'+i;
		v[1]= '\0';
		Lcd_DspAscII8x16(regStartX[0]-17,regStartY[0]+25*i,BLACK,v);
		Lcd_DspAscII8x16(regStartX[0]+25*i,regStartY[0]-17,BLACK,v);

		//pintar lineas
		if (i==0 || i==3 || i==6) {
			Lcd_Draw_HLine(regStartX[0],regStartX[3],regStartY[0]+i*25,BLACK,2);
			Lcd_Draw_VLine(regStartY[0],regStartY[3],regStartX[0]+i*25,BLACK,2);
		}
		else {
			Lcd_Draw_HLine(regStartX[0],regStartX[3],regStartY[0]+i*25,BLACK,1);
			Lcd_Draw_VLine(regStartY[0],regStartY[3],regStartX[0]+i*25,BLACK,1);
		}

	}
	Lcd_Draw_HLine(regStartX[0],regStartX[3],regStartY[3]-1,BLACK,2);
	Lcd_Draw_VLine(regStartY[0],regStartY[3],regStartX[3],BLACK,2);
}

void pintarValores(){
	short int i, j;
	char v[2] ;
	for (i=0; i<9; i++) {
		for (j=0; j<9; j++) {
			uint8_t valor;
			if ((valor=celda_leerValor(cuadricula[i][j])) != 0) {
				switch (esPista(i,j)) {
				case 0:

					v[0] = '0';
					v[0] += valor;
					v[1]='\0';
					Lcd_DspAscII8x16(regStartX[j/3]+8+25*(j%3), regStartY[i/3]+8+25*(i%3), DARKGRAY, v);
					break;
				case 1:
					v[0] = '0';
					v[0] += valor;
					v[1]='\0';
					Lcd_DspAscII8x16(regStartX[j/3]+8+25*(j%3), regStartY[i/3]+8+25*(i%3), BLACK, v);
					break;
				default:
					break;
				}
				if (esError(cuadricula[i][j])==TRUE) {
					Lcd_Draw_Box(regStartX[j/3]+25*(j%3), regStartY[i/3]+25*(i%3), regStartX[(1+j)/3]+25*((1+j)%3), regStartY[(i+1)/3]+25*((i+1)%3), BLACK);
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
				candidatos &= 0x1FF0;
				candidatos = candidatos >> 4;
				int desplazador;
				for (desplazador=0; desplazador<9; desplazador++) {
					if ((candidatos & (1 << desplazador)) != 0){
						Lcd_DspAscII8x16(regStartX[j/3]+3+25*(j%3)+(desplazador%3)*6, regStartY[i/3]-2+25*(i%3)+(desplazador/3)*6, BLACK,".");
						//Lcd_DspAscII8x16(regStartX[0]++5+6*(j%3), regStartY[i/3]+5+6*(i%3), DARKGRAY, ".");
					}

				}
			}
		}
	}
}
