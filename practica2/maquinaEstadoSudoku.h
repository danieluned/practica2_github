/*********************************************************************************************
* Fichero:		maquinaEstadoSudoku.c
* Autor:
* Descrip:		Automata y funciones auxiliares usadas para jugar al sudoku
* Version:
*********************************************************************************************/

/* guarda para evitar inclusiones múltiples ("include guard") */
#ifndef MAQUINA_H
#define MAQUINA_H
#include "sudoku_2016.h"

void maquinaEstadosSudoku();
volatile int botonPulsado;
volatile int  permitirPulsacionLarga;


static inline int
celda_ponerValor(CELDA *celdaptr, uint8_t val);
static inline uint8_t
celda_leerValor(CELDA celda);

void borrarValorEn(uint8_t fila,uint8_t columna);
//Devuelve 1 si hay error (no está en candidatos), 0 si no hay error
int introducirValorEn(uint8_t fila,uint8_t columna,uint8_t valor);

int esPista(int fila,int columna);

#endif /* MAQUINA_H */
