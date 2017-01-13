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
#include "maquinaEstadoSudoku.h"
#include "pintarSudoku.h"
extern volatile int unaVez;
extern  volatile int pintarTodo;
extern  volatile int pintar;
extern  volatile int modoZoom;
extern  volatile int tX,tY;
extern  volatile int regStartY[4];
extern  volatile int regStartX[4];
extern  volatile int casillasVacias;
extern  volatile int regX,regY;
extern  volatile int segundos,decimasSegundos,dosdecimilisegundosCalculo,segundosCalculo;
extern volatile  int textColum;

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
static inline uint8_t
 celda_leerValor(CELDA celda)
 {
     return (celda & 0x000F);
 }

void pintarPre(){
	if (unaVez==0){
		Lcd_Clr();
		Lcd_Active_Clr();
		Lcd_DspAscII8x16(100,25,BLACK,"Toque la pantalla para jugar");
		Lcd_Dma_Trans();
		unaVez=1;
	}
}
void pintarSel(){
	if (unaVez==0){
		Lcd_Clr();
		Lcd_Active_Clr();
		Lcd_DspAscII8x16(5,55,BLACK,"SELECCIONE UN SODOKU CON LOS PULSADORES");
		Lcd_DspAscII8x16(100,85,BLACK,"1) Sudoku 1");
		Lcd_DspAscII8x16(100,115,BLACK,"2) Sudoku 1 casi terminado");
		Lcd_DspAscII8x16(100,145,BLACK,"3) Sudoku 2");
		Lcd_Dma_Trans();
		unaVez=1;
	}
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
		//Pintar "c�lculos"
		pintarCalculos();
	}

	//pintar leyenda "Introduzca Fila A para acabar la partida"
	pintarLeyenda();
	//si hay zoom
	//modoZoom =1;
	if (pintarTodo==1)
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


		//pintar celdas diferenciando pistas de valores del usuario
		pintarValores();
		//pintar celdas vacias con listas de candidatos
		//resaltar error tablero
		pintarCandidatos();

		//tablero numerado
		pintarTablero();
	}
	Lcd_Dma_Trans();
	if (modoZoom!=1){
		char v[2];
		v[0]= '2';
		v[1]= '\0';
		Lcd_DspAscII8x16(regStartX[0]-17,regStartY[0]+25,BLACK,v);
		v[0]= '3';
		Lcd_DspAscII8x16(regStartX[0]-17,regStartY[0]+25*2,BLACK,v);
		Lcd_Dma_Trans();
	}

	if (pintarTodo==1)pintarTodo=0;
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
	//179					502
	//
	//0		1			2			3
	//710   685-673  	620-621		550 504
	//671							200
	int xi=179;
	int xf=502;
	int yi=671;
	int yf=200;

	int dx= (xf -xi)/3;
	int dy= (yi -yf)/3;

	int v0=xi,v1=xi+dx,v2=xf-dx,v3=xf; // Separaciones verticales

	int h0=yi ,h1=yi-dy,h2=yf+dy,h3=yf;

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
	if (unaVez==0){


		Lcd_Clr();
		Lcd_Active_Clr();
		// Pintar resultado
		// pintar leyenda "Toque la pantalla para jugar"




		char str[10];
		strcpy(str,"");
		strcat(str,  itoa(segundos));
		strcat(str, ",");
		strcat(str, itoa(decimasSegundos));
		str[9]='\0';
		Lcd_DspAscII8x16(50,60, BLACK, "Tiempo total:");
		Lcd_DspAscII8x16(204,60,BLACK, str);
		Lcd_DspAscII8x16(50,80, BLACK, "Calculos");
		Lcd_DspAscII8x16(50,100, BLACK, "Casillas vacias:");
		//precision  de (0,0000 segundos) cuatro decimales; se multipla para obtener el resultado correcto
		int deci= dosdecimilisegundosCalculo*2;
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
		Lcd_DspAscII8x16(204,80,BLACK, str);
		strcpy(str,"");
		strcat(str,  itoa(casillasVacias));
		Lcd_DspAscII8x16(204,100,BLACK, str);

		Lcd_DspAscII8x16(50,10, BLACK, "Toque la pantalla para jugar");
		Lcd_Dma_Trans();
		unaVez=1;
	}
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
	LcdClrRect(textColum,68,textColum+70,68+16,WHITE);
	char str[10];
	strcpy(str,"");
	strcat(str,  itoa(microSegundos));
	strcat(str,"\0");
	Lcd_DspAscII8x16(textColum,68,BLACK, str);
	strcpy(str,"");
	strcat(str,  itoa(casillasVacias));
	Lcd_DspAscII8x16(textColum+30,210,BLACK, str);
}
void pintarLeyenda(){
	Lcd_DspAscII8x16(textColum,102, BLACK, "Ponga");
	Lcd_DspAscII8x16(textColum,119, BLACK, "Fila A ");
	Lcd_DspAscII8x16(textColum,136, BLACK, "para ");
	Lcd_DspAscII8x16(textColum,153, BLACK, "acabar ");
	Lcd_DspAscII8x16(textColum,170, BLACK, "la ");
	Lcd_DspAscII8x16(textColum,187, BLACK, "partida");
	Lcd_DspAscII8x16(textColum,210, BLACK, "V: ");
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

					if (esError(cuadricula[i+regX*3][j+regY*3])!=0) {
						LcdClrRect(regStartX[j], regStartY[i],regStartX[j]+70, regStartY[i]+70,DARKGRAY);
						pintarNumero24x48(regStartX[j]+25, regStartY[i]+15,valor,WHITE);
					}else{
						pintarNumero24x48(regStartX[j]+25, regStartY[i]+15,valor,DARKGRAY);
					}
					break;
				case 1:
					v[0] = '0';
					v[0] += valor;
					v[1]='\0';
					//Lcd_DspAscII8x16(regStartX[j], regStartY[i], BLACK, v);
					if (esError(cuadricula[i+regX*3][j+regY*3])!=0) {
						LcdClrRect(regStartX[j], regStartY[i],regStartX[j]+70, regStartY[i]+70,BLACK);
						pintarNumero24x48(regStartX[j]+25, regStartY[i]+15,valor,WHITE);
					}else{
						pintarNumero24x48(regStartX[j]+25, regStartY[i]+15,valor,BLACK);
					}

					break;
				default:
					break;
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
					if (esError(cuadricula[i][j])!=0) {
						LcdClrRect(regStartX[j/3]+8+25*(j%3)-8, regStartY[i/3]+8+25*(i%3)-8,regStartX[j/3]+8+25*(j%3)+15, regStartY[i/3]+8+25*(i%3)+15,DARKGRAY);
						Lcd_DspAscII8x16(regStartX[j/3]+8+25*(j%3), regStartY[i/3]+8+25*(i%3), WHITE, v);
					}else{
						Lcd_DspAscII8x16(regStartX[j/3]+8+25*(j%3), regStartY[i/3]+8+25*(i%3), DARKGRAY, v);
					}

					break;
				case 1:
					v[0] = '0';
					v[0] += valor;
					v[1]='\0';
					if (esError(cuadricula[i][j])!=0) {
						LcdClrRect(regStartX[j/3]+8+25*(j%3)-8, regStartY[i/3]+8+25*(i%3)-8,regStartX[j/3]+8+25*(j%3)+15, regStartY[i/3]+8+25*(i%3)+15,BLACK);
						Lcd_DspAscII8x16(regStartX[j/3]+8+25*(j%3), regStartY[i/3]+8+25*(i%3), WHITE, v);
					}else{
						Lcd_DspAscII8x16(regStartX[j/3]+8+25*(j%3), regStartY[i/3]+8+25*(i%3), BLACK, v);
					}

					break;
				default:
					break;
				}

			}else{
				if (esError(cuadricula[i][j])!=0) {
					LcdClrRect(regStartX[j/3]+8+25*(j%3)-8, regStartY[i/3]+8+25*(i%3)-8,regStartX[j/3]+8+25*(j%3)+15, regStartY[i/3]+8+25*(i%3)+15,BLACK);
					Lcd_DspAscII8x16(regStartX[j/3]+8+25*(j%3), regStartY[i/3]+8+25*(i%3), WHITE, "0");
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
