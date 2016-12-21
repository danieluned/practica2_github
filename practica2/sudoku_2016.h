/*********************************************************************************************
* Fichero:		sudoku_2016.c
* Autor:
* Descrip:		Funciones criticas del sudoku para la version c y
* 				pruebas de tiempos para las distintas versiones (sudoku9x9)
* Version:
*********************************************************************************************/
/* guarda para evitar inclusiones múltiples ("include guard") */
#ifndef SUDOKU_H_2016
#define SUDOKU_H_2016

#include <inttypes.h>

/* Tamaños de la cuadricula */
/* Se utilizan 16 columnas para facilitar la visualización */
enum {NUM_FILAS = 9, NUM_COLUMNAS = 16};

/* Definiciones para valores muy utilizados */
enum {FALSE = 0, TRUE = 1};

typedef uint16_t CELDA;
/* La información de cada celda está codificada en 16 bits
 * con el siguiente formato, empezando en el bit más significativo (MSB):
 * 1 bit  PISTA
 * 1 bit  ERROR
 * 1 bit  no usado
 * 9 bits vector CANDIDATOS
 * 4 bits VALOR
 */


/* declaracion de funciones visibles en el exterior */
extern CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS];
extern CELDA cuadricula1[NUM_FILAS][NUM_COLUMNAS];
extern CELDA cuadricula2[NUM_FILAS][NUM_COLUMNAS];
extern CELDA cuadricula3[NUM_FILAS][NUM_COLUMNAS];

/* *****************************************************************************
 * programa principal del juego que recibe el tablero,
 * y la señal de ready que indica que se han actualizado fila y columna */
void sudoku9x9(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], char *ready);


/* declaracion de funciones ARM/thumb a implementar */

/* ARM
 * calcula todas las listas de candidatos (9x9)
 * necesario tras borrar o cambiar un valor (listas corrompidas)
 * retorna el numero de celdas vacias */
int
sudoku_candidatos_init_arm(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);

/* ARM->THUMB
 * calcula todas las listas de candidatos (9x9)
 * necesario tras borrar o cambiar un valor (listas corrompidas)
 * retorna el numero de celdas vacias */
int
sudoku_candidatos_init_arm_thumb(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);

/* ARM->C
 * calcula todas las listas de candidatos (9x9)
 * necesario tras borrar o cambiar un valor (listas corrompidas)
 * retorna el numero de celdas vacias */
int
sudoku_candidatos_init_arm_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);

/* ARM
 * propaga el valor de una determinada celda
 * para actualizar las listas de candidatos
 * de las celdas en su su fila, columna y región */
void
sudoku_candidatos_propagar_arm(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS],
                               uint8_t fila, uint8_t columna);

/* C
 * propaga el valor de una determinada celda
 * para actualizar las listas de candidatos
 * de las celdas en su su fila, columna y región */
void
sudoku_candidatos_propagar_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS],
                               uint8_t fila, uint8_t columna);

/* THUMB
 * propaga el valor de una determinada celda
 * para actualizar las listas de candidatos
 * de las celdas en su su fila, columna y región */
void
sudoku_candidatos_propagar_thumb(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS],
                                 uint8_t fila, uint8_t columna);

void
cuadricula_copiar(CELDA destino[NUM_FILAS][NUM_COLUMNAS],
					CELDA original[NUM_FILAS][NUM_COLUMNAS] );
#endif /* SUDOKU_H_2016 */
