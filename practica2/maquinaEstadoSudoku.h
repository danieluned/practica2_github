/*********************************************************************************************
* Fichero:		maquinaEstadoSudoku.c
* Autor:
* Descrip:		Automata y funciones auxiliares usadas para jugar al sudoku
* Version:
*********************************************************************************************/

/* guarda para evitar inclusiones múltiples ("include guard") */
#ifndef MAQUINA_H
#define MAQUINA_H


void maquinaEstadosSudoku();
volatile int botonPulsado;
volatile int  permitirPulsacionLarga;
volatile int pausaCalculo;

volatile int decimasSegundos ;
volatile int segundos ;
volatile int dosdecimilisegundosCalculo;
volatile int segundosCalculo;

#endif /* MAQUINA_H */
