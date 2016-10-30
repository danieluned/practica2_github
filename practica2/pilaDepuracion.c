#include <inttypes.h>
#include "timer2.h"
/*
#Memory Area
#GCS6    8M 16bit(8MB) DRAM/SDRAM(0xc000000-0xc7fffff)
#APP     RAM=0xc000000~0xc7effff
#EV_boot RAM=0xc7f0000-0xc7ff000 // if EV_boot
#STACK	 =0xc7ffa00
*/

//.equ 	MiStack,	_ISR_STARTADDRESS-0xf00-256		/* c7fef00 */

//Con estos valores y configuración hay para 16 eventos.
uint32_t LIMITESUPERIOR = 0xc7fef00 ;
uint32_t LIMITEINFERIOR = 0xc7fee00;
uint32_t actual = LIMITEINFERIOR;
/*Esta función escribirá en la pila tres enteros, por un lado
 * los parámetros ID_evento que permita identificar qué interrupción ha saltado,
 * y el parámetro auxData, con datos auxiliares aclaratorios, y por otro
 * el momento exacto en que se ha invocado a la función. Para ello hará uso
 * de la biblioteca de medidas de tiempo.
 */
void push_debug(int ID_evento,int auxData){
	int time = timer2_leer();
	asm(
		"str %1, [%0]\n\t"
		"str %2, [%0,#4]\n\t"
		"str %3, [%0,#8]\n\t"

		:: "r"(actual),"r"(ID_evento),"r"(auxData),"r"(time)
	);

	actual = actual + 0x10;
	if ( (actual+0x10) > LIMITESUPERIOR ){
		actual= LIMITEINFERIOR;
	}
}

void probarPila(){
	//Probar pila
	push_debug(1,3);
	push_debug(2,3);
	push_debug(3,3);
	push_debug(4,3);
	push_debug(5,3);
	push_debug(6,3);
	push_debug(7,3);
	push_debug(8,3);
	push_debug(9,3);
	push_debug(10,3);
	push_debug(11,3);
	push_debug(12,3);
	push_debug(13,3);
	push_debug(14,3);
	push_debug(15,3);
	push_debug(16,3);
	push_debug(17,3);
	push_debug(18,3);
	push_debug(19,3);
}
