/*********************************************************************************************
* Fichero:	button.h
* Autor:
* Descrip:	Funciones de manejo de los pulsadores (EINT6-7)
* Version:
*********************************************************************************************/

#ifndef _BUTTON_H_
#define _BUTTON_H_

/*--- declaracion de funciones visibles del m�dulo button.c/button.h ---*/
void Eint4567_init(void);
void timer0_inicializar(void);

int transcurrido;  // ms

void  timer0_reset();
#endif /* _BUTTON_H_ */
