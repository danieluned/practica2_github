/*********************************************************************************************
* Fichero:		timer2.h
* Autor:
* Descrip:		funciones de control del timer2 del s3c44b0x
* Version:
*********************************************************************************************/

#ifndef _TIMER2_H_
#define _TIMER2_H_
#include <inttypes.h>
/*--- variables visibles del módulo timer.c/timer.h ---*/
volatile int timer2_num_int;
volatile int pruebaTimer2;
/*--- declaracion de funciones visibles del módulo timer.c/timer.h ---*/

/*
 * configura el timer 2 para que trabaje a la máxima preción posible. El reloj
 * de la placa está configurado a 64MHz. Para aumentar el rango del contador,
 * el temporizador generará una interrupción cada vez que haga una cuenta completa
 * (queremos que la cuenta sea lo mayor posible para no sobrecargar en exceso al sistema con
 * interrupciones). La biblioteca dispondrá de la variable interna timer2_num_int,
 * compartida entre la interrupcion y el resto de funciones de ese módulo, que
 * llevara la cuenta de los peridos completos del temporizador. Al acabar la cuenta completa
 * el temporizador se reinicia al valor inicial y seguirá contando.
 */
void timer2_inicializar();

/*
 * reinicia la cuenta de tiempo (contador y la variable), y comienza a medir.
 */
void timer2_empezar();

/*
 * lee la cuenta actual del temporizador y el número de interrupciones generadas, y
 * devuelve el tiempo transcurrido en microsegundos.
 */
float timer2_leer();

#endif /* _TIMER_H_ */
