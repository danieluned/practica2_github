/*********************************************************************************************
* Fichero:		timer2.h
* Autor:
* Descrip:		funciones de control del timer2 del s3c44b0x
* Version:
*********************************************************************************************/

#ifndef _TIMER2_H_
#define _TIMER2_H_
#include <inttypes.h>
/*--- variables visibles del m�dulo timer.c/timer.h ---*/
volatile int timer2_num_int;
volatile int pruebaTimer2;
/*--- declaracion de funciones visibles del m�dulo timer.c/timer.h ---*/

/*
 * configura el timer 2 para que trabaje a la m�xima preci�n posible. El reloj
 * de la placa est� configurado a 64MHz. Para aumentar el rango del contador,
 * el temporizador generar� una interrupci�n cada vez que haga una cuenta completa
 * (queremos que la cuenta sea lo mayor posible para no sobrecargar en exceso al sistema con
 * interrupciones). La biblioteca dispondr� de la variable interna timer2_num_int,
 * compartida entre la interrupcion y el resto de funciones de ese m�dulo, que
 * llevara la cuenta de los peridos completos del temporizador. Al acabar la cuenta completa
 * el temporizador se reinicia al valor inicial y seguir� contando.
 */
void timer2_inicializar();

/*
 * reinicia la cuenta de tiempo (contador y la variable), y comienza a medir.
 */
void timer2_empezar();

/*
 * lee la cuenta actual del temporizador y el n�mero de interrupciones generadas, y
 * devuelve el tiempo transcurrido en microsegundos.
 */
float timer2_leer();

#endif /* _TIMER_H_ */
