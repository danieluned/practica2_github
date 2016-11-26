/*********************************************************************************************
* Fichero:		excepciones.c
* Autor:
* Descrip:		funciones de control para las excepciones
* Version:
*********************************************************************************************/

#ifndef _EXC_H
#define _EXC_H
#include <inttypes.h>
/*--- variables visibles del módulo timer.c/timer.h ---*/

volatile uint32_t error;  // 7 data abort,
volatile uint32_t instruccionCausante;
void excepciones_inicializar();
#endif /* _TIMER_H_ */
