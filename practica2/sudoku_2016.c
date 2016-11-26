/*********************************************************************************************
* Fichero:		sudoku_2016.c
* Autor:
* Descrip:		Funciones criticas del sudoku para la version c y
* 				pruebas de tiempos para las distintas versiones (sudoku9x9)
* Version:
*********************************************************************************************/
#include "sudoku_2016.h"
#include "timer2.h"
/* *****************************************************************************
 * Funciones privadas (static)
 * (no pueden ser invocadas desde otro fichero) */
 
/* *****************************************************************************
 * modifica el valor almacenado en la celda indicada */
static inline void
celda_poner_valor(CELDA *celdaptr, uint8_t val)
{
    *celdaptr = (*celdaptr & 0xFFF0) | (val & 0x000F);
}

/* *****************************************************************************
 * extrae el valor almacenado en los 16 bits de una celda */
static inline uint8_t
celda_leer_valor(CELDA celda)
{
    return (celda & 0x000F);
}

/* *****************************************************************************
 * propaga el valor de una determinada celda
 * para actualizar las listas de candidatos
 * de las celdas en su su fila, columna y región */
void
sudoku_candidatos_propagar_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS],
                             uint8_t fila, uint8_t columna)
{
    /* valor que se propaga */
    uint8_t valor = celda_leer_valor(cuadricula[fila][columna]);

    //Calculo del bit para la posicion en los 9bits de ayuda:

    uint16_t propa = 1<<(valor+3); //Se tiene un 1 en la posicion que queremos de la celda
    // hacemos la mascara negando lo anterior
    propa = ~propa; // en teoria se tiene una mascara para poder usarla con una and para eliminar ese numero en
    	//las demas filas columnas y region.

    /* recorrer fila descartando valor de listas candidatos */
    int i;
    for (i=0; i<9; i++){
    	cuadricula[i][columna]= cuadricula[i][columna] & propa;
    }

    /* recorrer columna descartando valor de listas candidatos */
    for (i=0; i<9; i++){
		cuadricula[fila][i]= cuadricula[fila][i] & propa;

    }
    /* recorrer region descartando valor de listas candidatos */
    //Averiguar region para no saltarse de los limites

    int addfil = 0;
    int addcol = 0;
    if (columna<3&&fila<3){
    	//region1
    	addfil = 0;
    	addcol = 0;
    }else if(columna<6&&fila<3){
    	//region2
    	addfil = 0;
    	addcol = 3;
    }else if (columna<9&&fila<3){
    	//region3
    	addfil = 0;
    	addcol = 6;
    }else if(columna<3&&fila<6){
    	//region4
    	addfil = 3;
    	addcol = 0;
    }else if(columna<6&&fila<6){
		//region5
		addfil = 3;
		addcol = 3;
	}else if (columna<9&&fila<6){
		//region6
		addfil = 3;
		addcol = 6;
	}else if(columna<3&&fila<9){
    	//region7
    	addfil = 6;
    	addcol = 0;
    }else if(columna<6&&fila<9){
		//region8
		addfil = 6;
		addcol = 3;
	}else {
		//region9
		addfil = 6;
		addcol = 6;
	}
    cuadricula[addfil][addcol]= cuadricula[addfil][addcol] & propa;
	cuadricula[addfil][addcol+1]= cuadricula[addfil][addcol+1] & propa;
	cuadricula[addfil][addcol+2]= cuadricula[addfil][addcol+2] & propa;
	cuadricula[addfil+1][addcol]= cuadricula[addfil+1][addcol] & propa;
	cuadricula[addfil+1][addcol+1]= cuadricula[addfil+1][addcol+1] & propa;
	cuadricula[addfil+1][addcol+2]= cuadricula[addfil+1][addcol+2] & propa;
	cuadricula[addfil+2][addcol]= cuadricula[addfil+2][addcol] & propa;
	cuadricula[addfil+2][addcol+1]= cuadricula[addfil+2][addcol+1] & propa;
	cuadricula[addfil+2][addcol+2]= cuadricula[addfil+2][addcol+2] & propa;
}

/* *****************************************************************************
 * calcula todas las listas de candidatos (9x9)
 * necesario tras borrar o cambiar un valor (listas corrompidas)
 * retorna el numero de celdas vacias */
static int
sudoku_candidatos_init_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS])
{

	int celdas_vacias = 0;

    /* recorrer cuadricula celda a celda */
     // filas
	int i,j;
    for (i=0; i<9; i++){
    	 // columnas
    	for ( j=0; j<9; j++){
    		/* inicializa lista de candidatos */
    		cuadricula[i][j]= cuadricula[i][j] | 0b0001111111110000; //0x1ff0 //0001 1111 1111 0000

    	}

    }

    /* recorrer cuadricula celda a celda */
   // filas
    for ( i=0; i<9; i++){
	     // columnas
    	for ( j=0; j<9; j++){

			/* si celda tiene valor */
			if (celda_leer_valor(cuadricula[i][j]) != 0){
				/*    sudoku_candidatos_propagar_c(...); */
				sudoku_candidatos_propagar_c(cuadricula,i,j);
			}else{
				/* else actualizar contador de celdas vacias */
				celdas_vacias++;
			}

		}

	}
   /* retorna el numero de celdas vacias */
    return (celdas_vacias);
}
/* *****************************************************************************
 * calcula todas las listas de candidatos (9x9)
 * necesario tras borrar o cambiar un valor (listas corrompidas)
 * retorna el numero de celdas vacias */
static int
sudoku_candidatos_init_c_thumb(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS])
{

	int celdas_vacias = 0;

    /* recorrer cuadricula celda a celda */
     // filas
	int i,j;
    for ( i=0; i<9; i++){
    	 // columnas

    	for ( j=0; j<9; j++){
    		/* inicializa lista de candidatos */

    		cuadricula[i][j]= cuadricula[i][j] | 0b0001111111110000;



    	}

    }
    /* recorrer cuadricula celda a celda */
    // filas
    for (i=0; i<9; i++){
	     // columnas
    	for ( j=0; j<9; j++){

			/* si celda tiene valor */
			if (celda_leer_valor(cuadricula[i][j]) != 0){
				/*    sudoku_candidatos_propagar_c(...); */
				sudoku_candidatos_propagar_thumb(cuadricula,i,j);
			}else{
				/* else actualizar contador de celdas vacias */
				celdas_vacias++;
			}

		}

	}

   /* retorna el numero de celdas vacias */
    return (celdas_vacias);
}
/* *****************************************************************************
 * calcula todas las listas de candidatos (9x9)
 * necesario tras borrar o cambiar un valor (listas corrompidas)
 * retorna el numero de celdas vacias */
static int
sudoku_candidatos_init_c_arm(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS])
{

	int celdas_vacias = 0;

    /* recorrer cuadricula celda a celda */
    int i=0; // filas
    int j=0;
    for (i=0; i<9; i++){
    	int j=0; // columnas
    	 for (j=0; j<9; j++){
    		/* inicializa lista de candidatos */

    		cuadricula[i][j]= cuadricula[i][j] | 0b0001111111110000;


    	}

    }
    /* recorrer cuadricula celda a celda */
    // filas
    for (i=0; i<9; i++){
	     // columnas
    	 for (j=0; j<9; j++){

			/* si celda tiene valor */
			if (celda_leer_valor(cuadricula[i][j]) != 0){
				/*    sudoku_candidatos_propagar_c(...); */
				sudoku_candidatos_propagar_arm(cuadricula,i,j);
			}else{
				/* else actualizar contador de celdas vacias */
				celdas_vacias++;
			}

		}

	}

   /* retorna el numero de celdas vacias */
    return (celdas_vacias);
}
/* *****************************************************************************
 * Funciones públicas
 * (pueden ser invocadas desde otro fichero) */
int cuadricula_candidatos_verificar(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS],CELDA cuadriculaCorrecta[NUM_FILAS][NUM_COLUMNAS] ){
	int i=0;
	int j=0;
	 for (i=0; i<NUM_FILAS; i++){

		 for (j=0; j<NUM_COLUMNAS; j++){
			if(cuadricula[i][j]!=cuadriculaCorrecta[i][j]){
				return 0;
			}

		}

	}
	return 1;
}
/* *****************************************************************************
 * Funciones públicas
 * (pueden ser invocadas desde otro fichero) */
void cuadricula_copiar(CELDA destino[NUM_FILAS][NUM_COLUMNAS],CELDA original[NUM_FILAS][NUM_COLUMNAS] ){
	int i=0;
	int j=0;
	for (i=0; i<NUM_FILAS; i++){
		for (j=0; j<NUM_COLUMNAS; j++){
			destino[i][j]=original[i][j];
		}

	}
}
/* *****************************************************************************
 * programa principal del juego que recibe el tablero,
 * y la señal de ready que indica que se han actualizado fila y columna */
void
sudoku9x9(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], char *ready)
{




     /* calcula lista de candidatos, versión C */
	int celdas_vacias= sudoku_candidatos_init_c(cuadricula);
    /* verificar que la lista de candidatos calculada es correcta */

    //Para verificar se realiza una comparacion del resultado de c
    CELDA cuadricula2[NUM_FILAS][NUM_COLUMNAS];
    cuadricula_copiar(cuadricula2,cuadricula);


    /* repetir para otras versiones (C optimizado, ARM, THUMB) */
    //variable correcto = 0 es incorrecto , 1 correcto
    // SERA CORRECTA CADA VERSION CUANDO tiene el mismo contenido que la cuadricula copia
    // Y ADEMAS COINCIDE EN EL NUMERO DEVUELTO

    int vacias_c_arm = sudoku_candidatos_init_c_arm(cuadricula);
    int correcto_c_arm=  cuadricula_candidatos_verificar(cuadricula2,cuadricula);

    int vacias_c_thumb= sudoku_candidatos_init_c_thumb(cuadricula);
    int correcto_c_thumb=  cuadricula_candidatos_verificar(cuadricula2,cuadricula);

    int vacias_arm = sudoku_candidatos_init_arm(cuadricula);
    int correcto_arm=  cuadricula_candidatos_verificar(cuadricula2,cuadricula);

    int vacias_arm_c = sudoku_candidatos_init_arm_c(cuadricula);
    int correcto_arm_c=  cuadricula_candidatos_verificar(cuadricula2,cuadricula);

    int vacias_arm_thumb = sudoku_candidatos_init_arm_thumb(cuadricula);
    int correcto_arm_thumb=  cuadricula_candidatos_verificar(cuadricula2,cuadricula);

    /* Calculo de tiempos con la nueva funcion*/

    //Version C
    int i =0 ;
    float tmpC;
    int repe = 1000;
    int vueltas = timer2_num_int;
    float inicio = timer2_leer();
    while (i<repe){
    	sudoku_candidatos_init_c(cuadricula);
    	i++;
    }
    tmpC = (timer2_leer()-inicio)/repe;
    push_debug(0,(int)tmpC);

    //Version C y ARM
    i = 0;
    int vueltas1 = timer2_num_int;
    float inicio1 = timer2_leer();
	while (i<repe){
		sudoku_candidatos_init_c_arm(cuadricula);
		i++;
	}
	float tmpC_ARM = (timer2_leer()-inicio1)/repe;
	push_debug(1,(int)tmpC_ARM);


	 //Version C y THUMB
    i = 0;
    int vueltas2 = timer2_num_int;
    float inicio2 = timer2_leer();
	while (i<repe){
		sudoku_candidatos_init_c_thumb(cuadricula);
		i++;
	}
	float tmpC_thumb  = (timer2_leer()-inicio2)/repe;
	push_debug(2,(int)tmpC_thumb);

	 //Version ARM
	 i = 0;
	 int vueltas3 = timer2_num_int;
	 float inicio3 = timer2_leer();
	while (i<repe){
		sudoku_candidatos_init_arm(cuadricula);
		i++;
	}
	float tmpARM = (timer2_leer()-inicio3)/repe;
	push_debug(3,(int)tmpARM);

	 //Version ARM_C
	i = 0;
	int vueltas4 = timer2_num_int;
	float inicio4 = timer2_leer();
	while (i<repe){
		sudoku_candidatos_init_arm_c(cuadricula);
		i++;
	}
	float tmpARM_C = (timer2_leer()-inicio4)/repe;
	push_debug(4,(int)tmpARM_C);

	 //Version ARM_THUMB
    i = 0;
    int vueltas5 = timer2_num_int;
    float inicio5 = timer2_leer();
	while (i<repe){
		sudoku_candidatos_init_arm_thumb(cuadricula);
		i++;
	}
	float tmpARM_thumb = (timer2_leer()-inicio5)/repe;
	int vueltas6 = timer2_num_int;
	push_debug(5,(int)tmpARM_thumb);
	float a = 0;

}

