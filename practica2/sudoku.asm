/*
 * sudoku.asm
 *
 *  Created on: 28/09/2016
 *      Author: daniel
 */
.arm
.text


#- Mis funciones a implementar

# ARM
# calcula todas las listas de candidatos (9x9)
# necesario tras borrar o cambiar un valor (listas corrompidas)
# retorna el numero de celdas vacias 


.global sudoku_candidatos_init_arm
sudoku_candidatos_init_arm:

		//Apila los registros que modificarán después
		stmfd   sp!, {r3,r4, r5, r6, r7, r8, r9, sl, fp, lr}


        # Poned el código aquí: (r0 sera el resultado de vacios

      	/*PRIMER BUCLE PARA PONER LA LISTA DE CANDIDADTOS si no es una pista */
        //En r0 tenemos la @cuadricula
        //hay que recorrerselas 9 filas y 16 columnas
        //r1 servira para llevar el contador que valdra 9,

		LDR r2, =0x1FF01FF0 // para iniciar la lista de candidatos
		LDR r8, =0x1FF0	// para iniciar la lista de candidatos de la ultima columna
		mov r10, r0  //en r10 guardamos la @ por si acaso

		//1 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//2 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//3 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//4 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//5 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//6 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//7 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//8 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//9 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//a partir de aqui ya estan colocados correctamente las pistas

		//SEGUNDO BUCLE PARA RECORRER LA CUADRICULA Y LLAMAR A PROPAGAR si tiene valor  y sino aumentar contador de vacios
		mov r0,r10 		//r0 - recuperamos el valor inicial de cuadricula
		mov r8,r10
		MOV r11, #0x0 	//r1 es un contador que inicialmente vale 0
		mov r9, #0x0	//r9 es un contador de celdas vacías

salto2:
		//Recorrer cuadricula y si tiene valor llamar a propagar, sino incrementar valor celdas vacias
		LDMIA r10, {r3-r7}

		// Llamamos a otra funcion que necesita que r0 tenga cuadricula
		// que en r1 este la fila y que en r2 este la columna

		//ponemos registros por si acaso la funcion ha modificado r0,r1,r2
		mov r0, r8
		mov r1, r11
		ANDS r2,r3, #0xf 			// Comprobamos si en r3 hay valor aplicando máscara de la parte BAJA
		addeq r9,r9,#0x1		// si no hay valor sumamos vacios +1
		movne r2, #0x0			// si hay valor, al llamar a la funcion la columna para propagar sera r2
		movne         lr, pc	//		(solo faltaba poner el valor de la columna a r2 antes de llamarla)
        blne          sudoku_candidatos_propagar_arm

		mov r0, r8
		mov r1, r11
        ANDS r2,r3,#0xf0000			// Comprobamos si en r3 hay valor aplicando máscara de la parte ALTA
		addeq r9,r9,#0x1   		// si no hay valor sumamos vacios +1
		movne r2, #0x1
		movne         lr, pc
        blne          sudoku_candidatos_propagar_arm

		mov r0, r8
		mov r1, r11
        ANDS r2,r4,#0xf
		addeq r9,r9,#0x1
		movne r2, #0x2
		movne         lr, pc
        blne          sudoku_candidatos_propagar_arm

		mov r0, r8
		mov r1, r11
        ANDS r2,r4,#0xf0000
		addeq r9,r9,#0x1
		movne r2, #0x3
		movne         lr, pc
        blne          sudoku_candidatos_propagar_arm

		mov r0, r8
		mov r1, r11
        ANDS r2,r5,#0xf
		addeq r9,r9,#0x1
		movne r2, #0x4
		movne         lr, pc
        blne          sudoku_candidatos_propagar_arm

		mov r0, r8
		mov r1, r11
        ANDS r2,r5,#0xf0000
		addeq r9,r9,#0x1
		movne r2, #0x5
		movne         lr, pc
        blne          sudoku_candidatos_propagar_arm

		mov r0, r8
		mov r1, r11
        ANDS r2,r6,#0xf
		addeq r9,r9,#0x1
		movne r2, #0x6
		movne         lr, pc
        blne          sudoku_candidatos_propagar_arm

		mov r0, r8
		mov r1, r11
        ANDS r2,r6,#0xf0000
		addeq r9,r9,#0x1
		movne r2, #0x7
		movne         lr, pc
        blne          sudoku_candidatos_propagar_arm

		mov r0, r8
		mov r1, r11
        ANDS r2,r7,#0xf
		addeq r9,r9,#0x1
		movne r2, #0x8
		movne         lr, pc
        blne          sudoku_candidatos_propagar_arm
		//fin de la llamada a otra funcion


		ADD r11, r11, #0x1 //Incrementa fila
		ADD r10,r10, #32 //Aumenta 32bytes la dirección para pasar a la siguiente fila
		CMP r11,#9 //Comprueba si es la última fila
		Bne salto2

		//dejamos el valor de celdas vacías en r0
		mov r0,r9
        # restore the original registers

   		 ldmfd   sp!, {r3,r4, r5, r6, r7, r8, r9, sl, fp, lr}
   		 bx  lr
################################################################################

################################################################################
# - Mis funciones a implementar
.extern  sudoku_candidatos_propagar_c

# ARM -> C (función propagar de C)
# calcula todas las listas de candidatos (9x9)
# necesario tras borrar o cambiar un valor (listas corrompidas)
# retorna el numero de celdas vacias

.global sudoku_candidatos_init_arm_c
sudoku_candidatos_init_arm_c:

		//Apila los registros que modificarán después
		stmfd   sp!, {r3,r4, r5, r6, r7, r8, r9, sl, fp, lr}


        # Poned el código aquí: (r0 sera el resultado de vacios

      	/*PRIMER BUCLE PARA PONER LA LISTA DE CANDIDADTOS si no es una pista */
        //En r0 tenemos la @cuadricula
        //hay que recorrerselas 9 filas y 16 columnas
        //r1 servira para llevar el contador que valdra 9,

		LDR r2, =0x1FF01FF0 // para iniciar la lista de candidatos
		LDR r8, =0x1FF0	// para iniciar la lista de candidatos de la ultima columna
		mov r10, r0  //en r10 guardamos la @ por si acaso

		//1 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//2 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//3 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//4 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//5 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//6 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//7 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//8 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//9 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//a partir de aqui ya estan colocados correctamente las pistas

		//SEGUNDO BUCLE PARA RECORRER LA CUADRICULA Y LLAMAR A PROPAGAR si tiene valor  y sino aumentar contador de vacios
		mov r0,r10 		//r0 - recuperamos el valor inicial de cuadricula
		mov r8,r10
		MOV r11, #0x0 	//r1 es un contador que inicialmente vale 0
		mov r9, #0x0	//r9 es un contador de celdas vacías

salto2_ac:
		//Recorrer cuadricula y si tiene valor llamar a propagar, sino incrementar valor celdas vacias
		LDMIA r10, {r3-r7}

		// Llamamos a otra funcion que necesita que r0 tenga cuadricula
		// que en r1 este la fila y que en r2 este la columna

		//ponemos registros por si acaso la funcion ha modificado r0,r1,r2
		mov r0, r8
		mov r1, r11
		ANDS r2,r3, #0xf 			// Comprobamos si en r3 hay valor aplicando máscara de la parte BAJA
		addeq r9,r9,#0x1		// si no hay valor sumamos vacios +1
		movne r2, #0x0			// si hay valor, al llamar a la funcion la columna para propagar sera r2
		movne         lr, pc	//		(solo faltaba poner el valor de la columna a r2 antes de llamarla)
        blne          sudoku_candidatos_propagar_c

		mov r0, r8
		mov r1, r11
        ANDS r2,r3,#0xf0000			// Comprobamos si en r3 hay valor aplicando máscara de la parte ALTA
		addeq r9,r9,#0x1   		// si no hay valor sumamos vacios +1
		movne r2, #0x1
		movne         lr, pc
        blne          sudoku_candidatos_propagar_c

		mov r0, r8
		mov r1, r11
        ANDS r2,r4,#0xf
		addeq r9,r9,#0x1
		movne r2, #0x2
		movne         lr, pc
        blne          sudoku_candidatos_propagar_c

		mov r0, r8
		mov r1, r11
        ANDS r2,r4,#0xf0000
		addeq r9,r9,#0x1
		movne r2, #0x3
		movne         lr, pc
        blne          sudoku_candidatos_propagar_c

		mov r0, r8
		mov r1, r11
        ANDS r2,r5,#0xf
		addeq r9,r9,#0x1
		movne r2, #0x4
		movne         lr, pc
        blne          sudoku_candidatos_propagar_c

		mov r0, r8
		mov r1, r11
        ANDS r2,r5,#0xf0000
		addeq r9,r9,#0x1
		movne r2, #0x5
		movne         lr, pc
        blne          sudoku_candidatos_propagar_c

		mov r0, r8
		mov r1, r11
        ANDS r2,r6,#0xf
		addeq r9,r9,#0x1
		movne r2, #0x6
		movne         lr, pc
        blne          sudoku_candidatos_propagar_c

		mov r0, r8
		mov r1, r11
        ANDS r2,r6,#0xf0000
		addeq r9,r9,#0x1
		movne r2, #0x7
		movne         lr, pc
        blne          sudoku_candidatos_propagar_c

		mov r0, r8
		mov r1, r11
        ANDS r2,r7,#0xf
		addeq r9,r9,#0x1
		movne r2, #0x8
		movne         lr, pc
        blne          sudoku_candidatos_propagar_c
		//fin de la llamada a otra funcion


		ADD r11, r11, #0x1 //Incrementa fila
		ADD r10,r10, #32 //Aumenta 32bytes la dirección para pasar a la siguiente fila
		CMP r11,#9 //Comprueba si es la última fila
		Bne salto2_ac

		//dejamos el valor de celdas vacías en r0
		mov r0,r9
        # restore the original registers

   		 ldmfd   sp!, {r3,r4, r5, r6, r7, r8, r9, sl, fp, lr}
   		 bx  lr
################################################################################
#
# propaga el valor de una determinada celda
# para actualizar las listas de candidatos
# de las celdas en su su fila, columna y región 

.global sudoku_candidatos_propagar_arm
sudoku_candidatos_propagar_arm:
		stmfd   sp!, {r3,r4, r5, r6, r7, r8, r9, sl, fp, lr}

        # Poned el código aquí:
		// En r0 esta la @cuadricula, en r1 fila , en r2 columna

		 /* recorrer fila descartando valor de listas candidatos */
      	mov r8,#32
      	mla r8,r1,r8,r0    // r8=fila*32+@cuadrícula
		LDMIA r8, {r3-r7}  // hemos obtenido la fila

		//obtenemos el numero que nos piden
		mov r9,#2  // para calcular la columna donde esta, constante 2
		mla r9,r2,r9,r8  // r9 tiene la direccion de donde esta el valor r9=columna*2+@inicialFila
		ldrh r9, [r9] // en r9 esta la celda que nos piden
		AND r9, r9, #0xf // r9 = valor, el numero que nos piden

		//calcular la mascara
		add r10,r9,#3  //  r10 = (valor+3)
		mov r11,#1		 //constante 1 para desplazar posteriormente
		mov r10, r11 , LSL r10 // en r10 esta la mascara sin negar
		add r10,r10, LSL #16  // mascara sin negar para dos celdas
		mvn r10, r10 // r10 = mascara ,r8= @filaInicialdelvalor

		// PONER MASCARA A LA FILA
		and r3, r3, r10
		and r4, r4, r10
		and r5, r5, r10
		and r6, r6, r10
		and r7, r7, r10
		STMIA r8, {r3-r7}

		 /* recorrer columna descartando valor de listas candidatos */
 		// PONER MASCARA A LA COLUMNA

 		MOV r9, #0x0 	//r1 es un contador que inicialmente vale 0
 						//usado para el bucle de 0..9
 		mov r4,#0b10  	// para calcular la columna donde esta
		mla r4,r2,r4,r0 // r4 tiene la direccion de donde esta el valor r4=columna*2+@inicialFila
 		//mascara

 salto3:
 		// ¿r9 == fila?	para no tener que realizar la carga de un dato que ya se ha colocado
 		CMP r9, r10

 		//cogemos dato
 		ldrneh r3,[r4]	//Si es otra fila distinta a la actual r3=valor
		//aplicamos mascara
		andne r3, r3, r10
		//se escribe dato en memoria
 		strneh r3, [r4]


 		ADD r9, r9, #0x1 //Incrementa contador
		add r4,#32 // saltamos a la siguiente fila
		CMP r9,#0x9 //Comprueba si es la última fila
		BNE salto3
		//Ya se ha terminado de

		// PONER MASCARA A LA REGION
		//Calculamos direccion de inicio de la region:

		// Para realizar un if else if sin saltos:
		// nos ayudamos de la variable r5, que tomara el valor de la columna y de la fila posteriormente
		// entonces si se cumple una condicion, es menor que 3,6,9..
		// realizo la operacion e incremento a r5 por encima del valor del ultimo test,
		// asi no activara ninguna guarda posterior, siempre jugando con "es menor que?"

		//Sumamos las COLUMNAS
		mov r5, r2	//r5=columna
		//if (r5 <3)
		CMP r5,#3
		addlt r5,#10
		//else if (r5 <6)
		CMP r5,#6
		addlt r0,r0,#0x6 //@inicial de la región, región 2
		addlt r5,#10
		//else if (r5<9)
		CMP r5,#9
		addlt r0,r0,#0xc //@inicial de la región, región 3

		// Sumamos las FILAS
		mov r5,r1 	//r5=fila
		//if (r5 <3)
		CMP r5,#3
		addlt r5,#10
		//else if (r5 <6)
		CMP r5,#6
		addlt r0,r0,#96 //@inicial de la región
		addlt r5,#10
		//else if (r5<9)
		CMP r5,#9
		addlt r0,r0,#192 //@inicial de la región

		//Apartir de aqui r0 tiene la direccion inicial de la region
		// para terminar de coger la region, se realizan tres cargas en dos registros cada una,
		// cada una de ellas recogera 4 numeros, nos sobra uno que sera de otra region
		//
		// para saber de que region se trata ese que sobra, miramos la alineacion de la @ que hemos calcula en r0



		//Filtros para la mascara que esta en r10
		ldr r7,=0xffff0000	// para dejar intacto la parte superior
		ldr r8,=0xffff		// para dejar intacto la parte inferior
		//Miramos si la direccion esta alineada o no
		ands r5,r0,#0b11	// Para saber si esta alineado o no, alineada=multiplo de 4
		bne sinalineare // Saltamos si esta sin alinear

		//Para region alineada
		ldmia r0,{r3,r4}
		and r3,r3,r10 // Aplicamos máscara 
		orr r6,r10,r7 // Aplicamos filtro parte superior
		and r4,r4,r6
		stmia r0,{r3,r4}

		add r0,r0,#32	//saltamos de columna

		ldmia r0,{r3,r4}
		and r3,r3,r10
		orr r6,r10,r7 // Aplicamos filtro
		and r4,r6
		stmia r0,{r3,r4}

		add r0,r0,#32 // saltamos de columna

		ldmia r0,{r3,r4}
		and r3,r3,r10
		orr r6,r10,r7 // Aplicamos filtro
		and r4,r4,r6
		stmia r0,{r3,r4}
		b continuar

sinalineare:
		//Para region sin alinear
		sub r0,r0,#2
		ldmia r0,{r3,r4}	//Se calcula con una direccion un
		orr r6,r10,r8 // dejamos la mascara inferior
		and r3,r3,r6
		and r4,r4,r10
		stmia r0,{r3,r4}

		add r0,r0,#32	//saltamos columna

		ldmia r0,{r3,r4}
		orr r6,r10,r8 // dejamos la mascara inferior
		and r3,r3,r6
		and r4,r4,r10
		stmia r0,{r3,r4}

		add r0,r0,#32	//saltamos columna

		ldmia r0,{r3,r4}
		orr r6,r10,r8 // dejamos la mascara inferior
		and r3,r3,r6
		and r4,r4,r10
		stmia r0,{r3,r4}

continuar:
        # restore the original registers
    ldmfd   sp!,  {r3,r4, r5, r6, r7, r8, r9, sl, fp, lr}
    bx  lr
################################################################################
################################################################################
#- Mis funciones a implementar

# ARM -> THUMB
# calcula todas las listas de candidatos (9x9)
# necesario tras borrar o cambiar un valor (listas corrompidas)
# retorna el numero de celdas vacias

.global sudoku_candidatos_init_arm_thumb
sudoku_candidatos_init_arm_thumb:

		//Apila los registros que modificarán después
		stmfd   sp!, {r3,r4, r5, r6, r7, r8, r9, sl, fp, lr}


        # Poned el código aquí: (r0 sera el resultado de vacios

      	/*PRIMER BUCLE PARA PONER LA LISTA DE CANDIDADTOS si no es una pista */
        //En r0 tenemos la @cuadricula
        //hay que recorrerselas 9 filas y 16 columnas
        //r1 servira para llevar el contador que valdra 9,

		LDR r2, =0x1FF01FF0 // para iniciar la lista de candidatos
		LDR r8, =0x1FF0	// para iniciar la lista de candidatos de la ultima columna
		mov r10, r0  //en r10 guardamos la @ por si acaso

		//1 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//2 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//3 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//4 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//5 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//6 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//7 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//8 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//9 FILA
		LDMIA r0, {r3-r7}  // obtenemos fila, cada registro tiene dos numeros
						   // r3(col1,col2),r4(col3,col4),r5(col5,col6),r6(col7,col8),r7(col9)
		//Aplicando máscaras comprobamos si cada una de las casillas son pista
		ORR r3,r3, r2	//col 1 y 2
		ORR r4,r4, r2	//col 3 y 4
		ORR r5,r5, r2	//col 5 y 6
		ORR r6,r6, r2	//col 7 y 8
		ORR r7,r7, r8	//col 9
		STMIA r0, {r3-r7}
		ADD r0,r0, #0b100000

		//a partir de aqui ya estan colocados correctamente las pistas

		//recuperamos el valor inicial de cuadricula
		mov r0,r10
		MOV r1, #0x0 	//r1 es un contador que inicialmente vale 0
		mov r9, #0x0	//para llevar contador de vacios
		LDR r8 ,=0xF
salto2_at:
		//Recorrer cuadricula y si tiene valor llamar a propagar, sino incrementar valor celdas vacias
		LDMIA r10, {r3-r7}

		//Llamamos a otra funcion que necesita que r0 tenga cuadricula
		// que en r1 este la fila y que en r2 este la columna


		ANDS r2,r3,r8 			// Comparacion para calcular vacios
		addeq r9,r9,#0x1		// si no hay valor sumamos vacios +1
		ANDs r2, r3,#0x4000		//Comprobacion para saltar si hay error a la siguiente celda
		bne si1
		ANDS r2,r3,r8 			// Comprobamos si en r3 hay valor
		movne r2, #0x0			// si hay valor, al llamar a la funcion la columna para propagar sera r2
		ADRL    r11, sudoku_candidatos_propagar_thumb+1 // Se usa ADRL ya que la distancia es demasiado grande para usar ADR
		movne lr, pc	//		(solo faltaba poner el valor de la columna a r2 antes de llamarla)
        bxne  r11
si1:
		LDR r11, =0xF0000
        ANDS r2,r3,r11
        addeq r9,r9,#0x1
		ANDs r2, r3,#0x40000000		//Comprobacion para saltar si hay error a la siguiente celda
		bne si2
		ANDS r2,r3,r11
		movne r2, #0x1
		ADRL    r11, sudoku_candidatos_propagar_thumb+1 // Se usa ADRL ya que la distancia es demasiado grande para usar ADR
		movne lr, pc	//		(solo faltaba poner el valor de la columna a r2 antes de llamarla)
        bxne  r11
si2:
        ANDS r2,r4,r8
		addeq r9,r9,#0x1
		ANDs r2, r4,#0x4000		//Comprobacion para saltar si hay error a la siguiente celda
		bne si3
		 ANDS r2,r4,r8
		movne r2, #0x2
		movne lr, pc	//		(solo faltaba poner el valor de la columna a r2 antes de llamarla)
        bxne  r11
si3:
		LDR r11, =0xF0000
        ANDS r2,r4,r11
		addeq r9,r9,#0x1
		ANDs r2, r4,#0x40000000		//Comprobacion para saltar si hay error a la siguiente celda
		bne si4
		 ANDS r2,r4,r11
		movne r2, #0x3
		ADR   r11, sudoku_candidatos_propagar_thumb+1
		movne lr, pc	//		(solo faltaba poner el valor de la columna a r2 antes de llamarla)
        bxne  r11
si4:
        ANDS r2,r5,r8
		addeq r9,r9,#0x1
		ANDs r2, r5,#0x4000		//Comprobacion para saltar si hay error a la siguiente celda
		bne si5
		 ANDS r2,r5,r8
		movne r2, #0x4
		movne lr, pc	//		(solo faltaba poner el valor de la columna a r2 antes de llamarla)
        bxne  r11
si5:
		LDR r11, =0xF0000
        ANDS r2,r5,r11
		addeq r9,r9,#0x1
		ANDs r2, r5,#0x40000000		//Comprobacion para saltar si hay error a la siguiente celda
		bne si6
		ANDS r2,r5,r11
		movne r2, #0x5
		ADR   r11, sudoku_candidatos_propagar_thumb+1
		movne lr, pc	//		(solo faltaba poner el valor de la columna a r2 antes de llamarla)
        bxne  r11
si6:
        ANDS r2,r6,r8
		addeq r9,r9,#0x1
		ANDs r2, r6,#0x4000		//Comprobacion para saltar si hay error a la siguiente celda
		bne si7
		ANDS r2,r6,r8
		movne r2, #0x6
		movne lr, pc	//		(solo faltaba poner el valor de la columna a r2 antes de llamarla)
        bxne  r11
si7:
		LDR r11, =0xF0000
        ANDS r2,r6,r11
		addeq r9,r9,#0x1
		ANDs r2, r6,#0x40000000		//Comprobacion para saltar si hay error a la siguiente celda
		bne si8
		ANDS r2,r6,r11
		movne r2, #0x7
		ADR   r11, sudoku_candidatos_propagar_thumb+1
		movne lr, pc	//		(solo faltaba poner el valor de la columna a r2 antes de llamarla)
        bxne  r11
si8:
        ANDS r2,r7,r8
		addeq r9,r9,#0x1
		ANDs r2, r7,#0x4000		//Comprobacion para saltar si hay error a la siguiente celda
		bne si9
		ANDS r2,r7,r8
		movne r2, #0x8
		movne lr, pc	//		(solo faltaba poner el valor de la columna a r2 antes de llamarla)
        bxne  r11
		//fin de la llamada a otra funcion
si9:
		ADD r1, r1, #0x1 //Incrementa fila
		ADD r10,r10, #0b100000
		CMP r1,#9 //Comprueba si es la última fila
		Bne salto2_at

		//dejamos el valor en r0
		mov r0,r9
        # restore the original registers

   		 ldmfd   sp!, {r3,r4, r5, r6, r7, r8, r9, sl, fp, lr}
   		 bx  lr
################################################################################
.global sudoku_candidatos_propagar_thumb
# THUMB
# propaga el valor de una determinada celda
# para actualizar las listas de candidatos
# de las celdas en su su fila, columna y región 

.thumb
.thumb_func
sudoku_candidatos_propagar_thumb:
		push {r0,r1,r2,r3,r4,r5,r6,r7}

        # Poned el código aquí:
		// En r0 esta la @cuadricula, en r1 fila , en r2 columna
		//saltamos las filas necesarias
      	mov r3,#0b100000
      	mul r3, r1,r3
      	add r3,r3,r0

		//obtenemos el numero que nos piden
		mov r4,#0b10  // para calcular la columna donde esta
		mul r4,r2,r4  //se multiplica por 2, que es el número de bytes de cada casilla
		add r4,r4,r3

		ldrh r4, [r4] // en r4 esta la celda que nos piden
		mov r5,#0b1111
		AND r4, r4, r5 // r4 = valor, el numero que nos piden

		//calcular la mascara
		add r6,r4,#0x3  //  r6 = (columna+3)
		mov r5,#1
		LSL r5,r6		// en r5 esta la mascara sin negar
		LSL r6,r5,#16	// r6 mascara sin negar superior
		add r5,r5,r6		//r5 mascara doble sin negar
		mvn r5, r5 // r5 = mascara ,r3= @filaInicialdelvalor

		//r5r10, r3=r8
		push {r0,r1,r2}  // APILADOS @cuadricula , fila
		mov r0,r3
		mov r1,r5  		// en r0 ahora esta @, r1=mascara
		mov r2,r0
		// PONER MASCARA A LA FILA
		LDMIA r0, {r3-r7}  // hemos obtenido la fila
		and r3, r3, r1
		and r4, r4, r1
		and r5, r5, r1
		and r6, r6, r1
		and r7, r7, r1

		STMIA r2, {r3-r7}
		//llevamos la mascara a otro registro
		mov r3,r1
		pop {r0,r1,r2}


		//r0=@cuadricula , r1=fila, r2=columna, r3=mascara
 		// PONER MASCARA A LA COLUMNA
 		MOV r4, #0x0 	//r4 es un contador i que inicialmente vale 0 hasta 9

 		//Para calcular donde empieza la columna pedida
		mov r5,#0b10
 		mul r5,r2,r5
 		add r5,r0      // r5 = @inicial de la columna

 salto4:
 		// r4->i  == fila
 		CMP r4, r1

		beq salto4a	//Si es la fila del dato la saltamos
 		//cogemos dato
 		ldrh r6,[r5]
		//aplicamos mascara
		and r6, r6,r3   //r3=mascara
		//se escribe dato en memoria
 		strh r6, [r5]

salto4a:
 		ADD r4, r4, #0x1 //Incrementa contador
		add r5,#32 // saltamos a la siguiente fila
		CMP r4,#0x9 //Comprueba si es la última fila
		BNE salto4

		// PONER MASCARA A LA REGION
		//Calculamos direccion de inicio de la region
		//Sumamos las columnas
		mov r5, r2
		//if (r5 <3)
		CMP r5,#3
		bge siguienteColumna
		b inicioFila
		//else if (r5 <6)
siguienteColumna:
		CMP r5,#6
		bge siguienteColumna2
		add r0,r0,#0x6
		b inicioFila
		//else if (r5<9)
siguienteColumna2:
		add r0,r0,#0xc

inicioFila:
		// Sumamos las filas
		mov r5,r1
		//if (r5 <3)
		CMP r5,#3
		bge siguienteFila
		b mirarAlineacion
siguienteFila:
		//else if (r5 <6)
		CMP r5,#6
		bge siguienteFila2
		add r0,r0,#96
		b mirarAlineacion
siguienteFila2:
		//else if (r5<9)
		add r0,r0,#192

mirarAlineacion:
		//Uso de registros: r0=@direccion inicial de la region , r3=mascara
		//Miramos si la direccion esta alineada o no

		mov r5,#0b11
		and r5,r0

		//Filtros para la mascara que esta en r10
		ldr r1,=0xffff0000	// para dejar intacto la parte superior
		orr r1,r3
		ldr r2,=0xffff		// para dejar intacto la parte inferior
		orr r2,r3
		////Uso de registros: r0=@direccion inicial de la region , r1=mascarasuperior, r2=mascarainferior r3=mascaradoble
		mov r4,r0
		CMP r5,#0 // Para saber si esta alineado o no
		bne sinalinear2 // Saltamos si esta sin alinear

		//Para region alineada

		////Uso de registros: r0=@direccion inicial de la region , r1=mascarasuperior
		///						r2=mascarainferior r3=mascaradoble, r4=copia de r0
		ldmia r0,{r5,r6}
		and r5,r3
		and r6,r1
		mov r0,r4
		stmia r0,{r5,r6}

		add r4,r4,#32	//saltamos de columna
		mov r0,r4

		ldmia r0,{r5,r6}
		and r5,r3
		and r6,r1
		mov r0,r4
		stmia r0,{r5,r6}

		add r4,r4,#32	//saltamos de columna
		mov r0,r4

		ldmia r0,{r5,r6}
		and r5,r3
		and r6,r1
		mov r0,r4
		stmia r0,{r5,r6}

		b continuar2

sinalinear2:
		//Para region sin alinear
		////Uso de registros: r0=@direccion inicial de la region , r1=mascarasuperior
		////				  r2=mascarainferior r3=mascaradoble, r4=copia de r0
		sub r0,r0,#2
		sub r4,r4,#2
		ldmia r0,{r5,r6}
		and r5,r2
		and r6,r3
		mov r0,r4
		stmia r0,{r5,r6}

		add r4,r4,#32	//saltamos de columna
		mov r0,r4

		ldmia r0,{r5,r6}
		and r5,r2
		and r6,r3
		mov r0,r4
		stmia r0,{r5,r6}

		add r4,r4,#32	//saltamos de columna
		mov r0,r4

		ldmia r0,{r5,r6}
		and r5,r2
		and r6,r3
		mov r0,r4
		stmia r0,{r5,r6}

continuar2:
        # restore the original registers

    	 pop  {r0,r1,r2,r3,r4, r5, r6, r7}
    	 bx r14

################################################################################
.data
.ltorg
.align 5    /* guarantees 32-byte alignment (2^5) */

# huecos para cuadrar
.global cuadricula
cuadricula:
     /* SUDOKU 2 - Otro ejemplo */
    //.hword   0x0000,0x0000,0x0000,	0x0000,0x0000,0x0000,	0x0000,0x0000,0x0000,	0,0,0,0,0,0,0
	//.hword   0x0000,0x0000,0x0000,	0x0000,0x0000,0x8003,	0x0000,0x8008,0x8005,	0,0,0,0,0,0,0
	//.hword   0x0000,0x0000,0x8001,	0x0000,0x8002,0x0000,	0x0000,0x0000,0x0000,	0,0,0,0,0,0,0

	//.hword   0x0000,0x0000,0x0000,	0x8005,0x0000,0x8007,	0x0000,0x0000,0x0000,	0,0,0,0,0,0,0
	//.hword   0x0000,0x0000,0x8004,	0x0000,0x0000,0x0000,	0x8001,0x0000,0x0000,	0,0,0,0,0,0,0
	//.hword   0x0000,0x8009,0x0000,	0x0000,0x0000,0x0000,	0x0000,0x0000,0x0000,	0,0,0,0,0,0,0

	//.hword   0x8005,0x0000,0x0000,	0x0000,0x0000,0x0000,	0x0000,0x8007,0x8003,	0,0,0,0,0,0,0
	//.hword   0x0000,0x0000,0x8002,	0x0000,0x8001,0x0000,	0x0000,0x0000,0x0000,	0,0,0,0,0,0,0
	//.hword   0x0000,0x0000,0x0000,	0x0000,0x8004,0x0000,	0x0000,0x0000,0x8009,	0,0,0,0,0,0,0




    /* SUDOKU 1 - Ejemplo inicial */
	/* 9 filas de 16 entradas para facilitar la visualizacion, 16 bits por celda */
    .hword   0x8005,0x0000,0x0000,	0x8003,0x0000,0x0000,	0x0000,0x0000,0x0000,0,0,0,0,0,0,0
    .hword   0x0000,0x0000,0x0000,	0x0000,0x8009,0x0000,	0x0000,0x0000,0x8005,0,0,0,0,0,0,0
    .hword   0x0000,0x8009,0x8006,	0x8007,0x0000,0x8005,	0x0000,0x8003,0x0000,0,0,0,0,0,0,0

    .hword   0x0000,0x8008,0x0000,	0x8009,0x0000,0x0000,	0x8006,0x0000,0x0000,0,0,0,0,0,0,0
    .hword   0x0000,0x0000,0x8005,	0x8008,0x8006,0x8001,	0x8004,0x0000,0x0000,0,0,0,0,0,0,0
    .hword   0x0000,0x0000,0x8004,	0x8002,0x0000,0x8003,	0x0000,0x8007,0x0000,0,0,0,0,0,0,0

    .hword   0x0000,0x8007,0x0000,	0x8005,0x0000,0x8009,	0x8002,0x8006,0x0000,0,0,0,0,0,0,0
    .hword   0x8006,0x0000,0x0000,	0x0000,0x8008,0x0000,	0x0000,0x0000,0x0000,0,0,0,0,0,0,0
    .hword   0x0000,0x0000,0x0000,	0x0000,0x0000,0x8002,	0x0000,0x0000,0x8001,0,0,0,0,0,0,0

.end
#        END

