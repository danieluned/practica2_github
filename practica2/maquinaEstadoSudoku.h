/*********************************************************************************************
* Fichero:		maquinaEstadoSudoku.c
* Autor:
* Descrip:		Automata y funciones auxiliares usadas para jugar al sudoku
* Version:
*********************************************************************************************/

/* guarda para evitar inclusiones m�ltiples ("include guard") */
#ifndef MAQUINA_H
#define MAQUINA_H
#include "sudoku_2016.h"

void maquinaEstadosSudoku();



volatile int botonPulsado;
volatile int  permitirPulsacionLarga;
volatile int pausaCalculo;

volatile int decimasSegundos ;
volatile int segundos ;
volatile int dosdecimilisegundosCalculo;
volatile int segundosCalculo;
volatile int pintar;
volatile int empezarTiempo;

//////////////////////////
// Variables y valores iniciales

volatile int estado ; // Cual es el inicio

volatile int numeroAmostrar ;
volatile int filaConfirmada;
volatile int columnaConfirmada ;
volatile int valorConfirmado ;
//Touchpad
volatile int touchPulsado;
volatile int modoZoom;
volatile int  tX, tY;
//Pintar LCD
volatile int pintar;		//Indica si se ha de pintar de nuevo el tiempo
volatile int pintarTodo;
volatile int estadoPintar;
volatile int region;
volatile int unaVez;
//valor region pinchada
volatile int regX,regY;

//valores para dibujar tablero
volatile int regStartX[4];
volatile int regStartY[4];
volatile int textColum;
//Tiempo total de partida en segundos
volatile int decimasSegundos;
volatile int segundos;
volatile int dosdecimilisegundosCalculo;
volatile int segundosCalculo;
volatile int pausaCalculo;
volatile int empezarTiempo;
volatile int microSegundos;
//Para saber si ha terminado
volatile int casillasVacias;
volatile int rebotesTouch;
#endif /* MAQUINA_H */
