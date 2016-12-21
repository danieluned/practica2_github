/*********************************************************************************************
* Fichero:		maquinaEstadoSudoku.c
* Autor:
* Descrip:		Automata y funciones auxiliares usadas para jugar al sudoku
* Version:
*********************************************************************************************/

/* guarda para evitar inclusiones múltiples ("include guard") */
#ifndef PINTAR_H
#define PINTAR_H



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

#endif /* PINTAR_H */
