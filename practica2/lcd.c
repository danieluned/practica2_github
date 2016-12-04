/*********************************************************************************************
* Fichero:	lcd.c
* Autor:	
* Descrip:	funciones de visualizacion y control LCD
* Version:	<P6-ARM>
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "def.h"
#include "44b.h"
#include "44blib.h"
#include "lcd.h"
#include "bmp.h"
#include "maquinaEstadoSudoku.h"
#include "itoa.h"
//mis includes

/*--- definicion de macros ---*/
#define DMA_Byte  (0)
#define DMA_HW    (1)
#define DMA_Word  (2)
#define DW 		  DMA_Byte		//configura  ZDMA0 como media palabras
	
/*--- variables externas ---*/
extern INT8U g_auc_Ascii8x16[];
extern STRU_BITMAP Stru_Bitmap_gbMouse;

//Nuestras variables

int tiempo2 = 0;
extern int segundos, minutos;
extern short int pintar;
static INT8U *ind;
static INT8U *literal_pool;
/*--- codigo de la funcion ---*/
void Lcd_Init(void)
{       
	rDITHMODE=0x1223a;
	rDP1_2 =0x5a5a;      
	rDP4_7 =0x366cd9b;
	rDP3_5 =0xda5a7;
	rDP2_3 =0xad7;
	rDP5_7 =0xfeda5b7;
	rDP3_4 =0xebd7;
	rDP4_5 =0xebfd7;
	rDP6_7 =0x7efdfbf;

	rLCDCON1=(0)|(1<<5)|(MVAL_USED<<7)|(0x0<<8)|(0x0<<10)|(CLKVAL_GREY16<<12);
	rLCDCON2=(LINEVAL)|(HOZVAL<<10)|(10<<21); 
	rLCDSADDR1= (0x2<<27) | ( ((LCD_ACTIVE_BUFFER>>22)<<21 ) | M5D(LCD_ACTIVE_BUFFER>>1));
 	rLCDSADDR2= M5D(((LCD_ACTIVE_BUFFER+(SCR_XSIZE*LCD_YSIZE/2))>>1)) | (MVAL<<21);
	rLCDSADDR3= (LCD_XSIZE/4) | ( ((SCR_XSIZE-LCD_XSIZE)/4)<<9 );
	// enable,4B_SNGL_SCAN,WDLY=8clk,WLH=8clk,
	rLCDCON1=(1)|(1<<5)|(MVAL_USED<<7)|(0x3<<8)|(0x3<<10)|(CLKVAL_GREY16<<12);
	rBLUELUT=0xfa40;
	//Enable LCD Logic and EL back-light.
	rPDATE=rPDATE&0x0e;
	
	//DMA ISR
	rINTMSK &= ~(BIT_GLOBAL|BIT_ZDMA0);
    pISR_ZDMA0=(int)Zdma0Done;
}

/*********************************************************************************************
* name:		Lcd_Active_Clr()
* func:		clear LCD screen
* para:		none 
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_Active_Clr(void)
{
	INT32U i;
	INT32U *pDisp = (INT32U *)LCD_ACTIVE_BUFFER;
	
	for( i = 0; i < (SCR_XSIZE*SCR_YSIZE/2/4); i++ )
	{
		*pDisp++ = WHITE;
	}
}

/*********************************************************************************************
* name:		Lcd_GetPixel()
* func:		Get appointed point's color value
* para:		usX,usY -- pot's X-Y coordinate 
* ret:		pot's color value
* modify:
* comment:		
*********************************************************************************************/
INT8U LCD_GetPixel(INT16U usX, INT16U usY)
{
	INT8U ucColor;

	ucColor = *((INT8U*)(LCD_VIRTUAL_BUFFER + usY*SCR_XSIZE/2 + usX/8*4 + 3 - (usX%8)/2));
	ucColor = (ucColor >> ((1-(usX%2))*4)) & 0x0f;
	return ucColor;
}

/*********************************************************************************************
* name:		Lcd_Active_Clr()
* func:		clear virtual screen
* para:		none 
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_Clr(void)
{
	INT32U i;
	INT32U *pDisp = (INT32U *)LCD_VIRTUAL_BUFFER;
	
	for( i = 0; i < (SCR_XSIZE*SCR_YSIZE/2/4); i++ )
	{
		*pDisp++ = WHITE;
	}
}

/*********************************************************************************************
* name:		LcdClrRect()
* func:		fill appointed area with appointed color
* para:		usLeft,usTop,usRight,usBottom -- area's rectangle acme coordinate
*			ucColor -- appointed color value
* ret:		none
* modify:
* comment:	also as clear screen function 
*********************************************************************************************/
void LcdClrRect(INT16 usLeft, INT16 usTop, INT16 usRight, INT16 usBottom, INT8U ucColor)
{
	INT16 i,k,l,m;
	
	INT32U ulColor = (ucColor << 28) | (ucColor << 24) | (ucColor << 20) | (ucColor << 16) | 
				     (ucColor << 12) | (ucColor << 8) | (ucColor << 4) | ucColor;

	i = k = l = m = 0;	
	if( (usRight-usLeft) <= 8 )
	{
		for( i=usTop; i<=usBottom; i++)
		{
			for( m=usLeft; m<=usRight; m++)
			{
				LCD_PutPixel(m, i, ucColor);
			}
		}	
		return;
	}

	/* check borderline */
	if( 0 == (usLeft%8) )
		k=usLeft;
	else
	{
		k=(usLeft/8)*8+8;
	}
	if( 0 == (usRight%8) )
		l= usRight;
	else
	{
		l=(usRight/8)*8;
	}

	for( i=usTop; i<=usBottom; i++ )
	{
		for( m=usLeft; m<=(k-1); m++ )
		{
			LCD_PutPixel(m, i, ucColor);
		}
		for( m=k; m<l; m+=8 )
		{
			(*(INT32U*)(LCD_VIRTUAL_BUFFER + i * SCR_XSIZE / 2 + m / 2)) = ulColor;
		}
		for( m=l; m<=usRight; m++ )
		{
			LCD_PutPixel(m, i, ucColor);
		}
	}
}

/*********************************************************************************************
* name:		Lcd_Draw_Box()
* func:		Draw rectangle with appointed color
* para:		usLeft,usTop,usRight,usBottom -- rectangle's acme coordinate
*			ucColor -- appointed color value
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_Draw_Box(INT16 usLeft, INT16 usTop, INT16 usRight, INT16 usBottom, INT8U ucColor)
{
	Lcd_Draw_HLine(usLeft, usRight,  usTop,    ucColor, 1);
	Lcd_Draw_HLine(usLeft, usRight,  usBottom, ucColor, 1);
	Lcd_Draw_VLine(usTop,  usBottom, usLeft,   ucColor, 1);
	Lcd_Draw_VLine(usTop,  usBottom, usRight,  ucColor, 1);
}

/*********************************************************************************************
* name:		Lcd_Draw_Line()
* func:		Draw line with appointed color
* para:		usX0,usY0 -- line's start point coordinate
*			usX1,usY1 -- line's end point coordinate
*			ucColor -- appointed color value
*			usWidth -- line's width
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_Draw_Line(INT16 usX0, INT16 usY0, INT16 usX1, INT16 usY1, INT8U ucColor, INT16U usWidth)
{
	INT16 usDx;
	INT16 usDy;
	INT16 y_sign;
	INT16 x_sign;
	INT16 decision;
	INT16 wCurx, wCury, wNextx, wNexty, wpy, wpx;

	if( usY0 == usY1 )
	{
		Lcd_Draw_HLine (usX0, usX1, usY0, ucColor, usWidth);
		return;
	}
	if( usX0 == usX1 )
	{
		Lcd_Draw_VLine (usY0, usY1, usX0, ucColor, usWidth);
		return;
	}
	usDx = abs(usX0 - usX1);
	usDy = abs(usY0 - usY1);
	if( ((usDx >= usDy && (usX0 > usX1)) ||
        ((usDy > usDx) && (usY0 > usY1))) )
    {
        GUISWAP(usX1, usX0);
        GUISWAP(usY1, usY0);
    }
    y_sign = (usY1 - usY0) / usDy;
    x_sign = (usX1 - usX0) / usDx;

    if( usDx >= usDy )
    {
        for( wCurx = usX0, wCury = usY0, wNextx = usX1,
             wNexty = usY1, decision = (usDx >> 1);
             wCurx <= wNextx; wCurx++, wNextx--, decision += usDy )
        {
            if( decision >= usDx )
            {
                decision -= usDx;
                wCury += y_sign;
                wNexty -= y_sign;
            }
            for( wpy = wCury - usWidth / 2;
                 wpy <= wCury + usWidth / 2; wpy++ )
            {
                LCD_PutPixel(wCurx, wpy, ucColor);
            }

            for( wpy = wNexty - usWidth / 2;
                 wpy <= wNexty + usWidth / 2; wpy++ )
            {
                LCD_PutPixel(wNextx, wpy, ucColor);
            }
        }
    }
    else
    {
        for( wCurx = usX0, wCury = usY0, wNextx = usX1,
             wNexty = usY1, decision = (usDy >> 1);
             wCury <= wNexty; wCury++, wNexty--, decision += usDx )
        {
            if( decision >= usDy )
            {
                decision -= usDy;
                wCurx += x_sign;
                wNextx -= x_sign;
            }
            for( wpx = wCurx - usWidth / 2;
                 wpx <= wCurx + usWidth / 2; wpx++ )
            {
                LCD_PutPixel(wpx, wCury, ucColor);
            }

            for( wpx = wNextx - usWidth / 2;
                 wpx <= wNextx + usWidth / 2; wpx++ )
            {
                LCD_PutPixel(wpx, wNexty, ucColor);
            }
        }
    }
}

/*********************************************************************************************
* name:		Lcd_Draw_HLine()
* func:		Draw horizontal line with appointed color
* para:		usX0,usY0 -- line's start point coordinate
*			usX1 -- line's end point X-coordinate
*			ucColor -- appointed color value
*			usWidth -- line's width
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_Draw_HLine(INT16 usX0, INT16 usX1, INT16 usY0, INT8U ucColor, INT16U usWidth)
{
	INT16 usLen;

    if( usX1 < usX0 )
    {
        GUISWAP (usX1, usX0);
    }

    while( (usWidth--) > 0 )
    {
        usLen = usX1 - usX0 + 1;
        while( (usLen--) > 0 )
        {
        	LCD_PutPixel(usX0 + usLen, usY0, ucColor);
        }
        usY0++;
    }
}

/*********************************************************************************************
* name:		Lcd_Draw_VLine()
* func:		Draw vertical line with appointed color
* para:		usX0,usY0 -- line's start point coordinate
*			usY1 -- line's end point Y-coordinate
*			ucColor -- appointed color value
*			usWidth -- line's width
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_Draw_VLine (INT16 usY0, INT16 usY1, INT16 usX0, INT8U ucColor, INT16U usWidth)
{
	INT16 usLen;

    if( usY1 < usY0 )
    {
        GUISWAP (usY1, usY0);
    }

    while( (usWidth--) > 0 )
    {
        usLen = usY1 - usY0 + 1;
        while( (usLen--) > 0 )
        {
        	LCD_PutPixel(usX0, usY0 + usLen, ucColor);
        }
        usX0++;
    }
}

/*********************************************************************************************
* name:		Lcd_DspAscII8x16()
* func:		display 8x16 ASCII character string 
* para:		usX0,usY0 -- ASCII character string's start point coordinate
*			ForeColor -- appointed color value
*			pucChar   -- ASCII character string
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_DspAscII8x16(INT16U x0, INT16U y0, INT8U ForeColor, INT8U * s)
{
	INT16 i,j,k,x,y,xx;
	INT8U qm;
	INT32U ulOffset;
	INT8 ywbuf[16],temp[2];
    
	for( i = 0; i < strlen((const char*)s); i++ )
	{
		if( (INT8U)*(s+i) >= 161 )
		{
			temp[0] = *(s + i);
			temp[1] = '\0';
			return;
		}
		else
		{
			qm = *(s+i);
			ulOffset = (INT32U)(qm) * 16;		//Here to be changed tomorrow
			for( j = 0; j < 16; j ++ )
			{
				ywbuf[j] = g_auc_Ascii8x16[ulOffset + j];
            }

            for( y = 0; y < 16; y++ )
            {
            	for( x = 0; x < 8; x++ ) 
               	{
                	k = x % 8;
			    	if( ywbuf[y]  & (0x80 >> k) )
			       	{
			       		xx = x0 + x + i*8;
			       		LCD_PutPixel(xx, y + y0, (INT8U)ForeColor);
			       	}
			   	}
            }
		}
	}
}

/*********************************************************************************************
* name:		ReverseLine()
* func:		Reverse display some lines 
* para:		ulHeight -- line's height
*			ulY -- line's Y-coordinate
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void ReverseLine(INT32U ulHeight, INT32U ulY)
{
	INT32U i, j, temp;
	
	for( i = 0; i < ulHeight; i++ )
	{
		for( j = 0; j < (SCR_XSIZE/4/2) ; j++ )
		{
			temp = *(INT32U*)(LCD_VIRTUAL_BUFFER + (ulY+i)*SCR_XSIZE/2 + j*4);
			temp ^= 0xFFFFFFFF;
			*(INT32U*)(LCD_VIRTUAL_BUFFER + (ulY+i)*SCR_XSIZE/2 + j*4) = temp;
		}
	}
}

/*********************************************************************************************
* name:		Zdma0Done()
* func:		LCD dma interrupt handle function
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
static INT8U ucZdma0Done=1;	//When DMA is finish,ucZdma0Done is cleared to Zero
void Zdma0Done(void)
{
	rI_ISPC=BIT_ZDMA0;	    //clear pending
	ucZdma0Done=0;
}

/*********************************************************************************************
* name:		Lcd_Dma_Trans()
* func:		dma transport virtual LCD screen to LCD actual screen
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_Dma_Trans(void)
{
	INT8U err;
	
	ucZdma0Done=1;
	//#define LCD_VIRTUAL_BUFFER	(0xc400000)
	//#define LCD_ACTIVE_BUFFER	(LCD_VIRTUAL_BUFFER+(SCR_XSIZE*SCR_YSIZE/2))	//DMA ON
	//#define LCD_ACTIVE_BUFFER	LCD_VIRTUAL_BUFFER								//DMA OFF
	//#define LCD_BUF_SIZE		(SCR_XSIZE*SCR_YSIZE/2)
	//So the Lcd Buffer Low area is from LCD_VIRTUAL_BUFFER to (LCD_ACTIVE_BUFFER+(SCR_XSIZE*SCR_YSIZE/2))
	rNCACHBE1=(((unsigned)(LCD_ACTIVE_BUFFER)>>12) <<16 )|((unsigned)(LCD_VIRTUAL_BUFFER)>>12);
  	rZDISRC0=(DW<<30)|(1<<28)|LCD_VIRTUAL_BUFFER; // inc
  	rZDIDES0=( 2<<30)  |(1<<28)|LCD_ACTIVE_BUFFER; // inc
        rZDICNT0=( 2<<28)|(1<<26)|(3<<22)|(0<<20)|(LCD_BUF_SIZE);
        //                      |            |            |             |            |---->0 = Disable DMA
        //                      |            |            |             |------------>Int. whenever transferred
        //                      |            |            |-------------------->Write time on the fly
        //                      |            |---------------------------->Block(4-word) transfer mode
        //                      |------------------------------------>whole service
	//reEnable ZDMA transfer
  	rZDICNT0 |= (1<<20);		//after ES3
    rZDCON0=0x1; // start!!!  

	//Delay(500);
	while(ucZdma0Done);		//wait for DMA finish
}

/*********************************************************************************************
* name:		Lcd_Test()
* func:		LCD test function
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_Test(void)
{
	/* initial LCD controller */
	Lcd_Init();
	/* clear screen */
	Lcd_Clr();
	Lcd_Active_Clr();

	/* draw rectangle pattern */ 
    #ifdef Eng_v // english version
	Lcd_DspAscII8x16(10,0,DARKGRAY,"Embest S3CEV40 ");
	#else
//	Lcd_DspHz16(10,0,DARKGRAY,"Ó¢ÝíÌØÈýÐÇÊµÑéÆÀ¹À°å");
	#endif
	Lcd_DspAscII8x16(10,20,BLACK,"Codigo del puesto: ");
	Lcd_Draw_Box(10,40,310,230,14);
	/*
	Lcd_Draw_Box(20,45,300,225,13);
	Lcd_Draw_Box(30,50,290,220,12);
	Lcd_Draw_Box(40,55,280,215,11);
	Lcd_Draw_Box(50,60,270,210,10);
	Lcd_Draw_Box(60,65,260,205,9);
	Lcd_Draw_Box(70,70,250,200,8);
	Lcd_Draw_Box(80,75,240,195,7);
	Lcd_Draw_Box(90,80,230,190,6);
	Lcd_Draw_Box(100,85,220,185,5);
	Lcd_Draw_Box(110,90,210,180,4);
	Lcd_Draw_Box(120,95,200,175,3);
	Lcd_Draw_Box(130,100,190,170,2);
	*/
	Lcd_Draw_Box(10,40,310,230,14);
	Lcd_Draw_Box(20,45,300,225,13);
	Lcd_Draw_Box(30,50,290,220,12);
	Lcd_Draw_Box(40,55,280,215,11);
	Lcd_Draw_Box(50,60,270,210,10);
	Lcd_Draw_Box(60,65,260,205,9);
	Lcd_Draw_Box(70,70,250,200,8);
	Lcd_Draw_Box(80,75,240,195,7);
	Lcd_Draw_Box(90,80,230,190,6);
	Lcd_Draw_Box(100,85,220,185,5);
	Lcd_Draw_Box(110,90,210,180,4);
	Lcd_Draw_Box(120,95,200,175,3);
	Lcd_Draw_Box(130,100,190,170,2);
	BitmapView(125,135,Stru_Bitmap_gbMouse);
	Lcd_Dma_Trans();

}

/**
 * Funciones para el dibujado del sudoku
 */

void Lcd_PantallaInicial(void)
{
	Lcd_Init();		//inicializa el controlador


	ind = (INT8U*)malloc(sizeof(INT8U));
	literal_pool = (INT8U*)malloc(9*sizeof(INT8U));
	literal_pool[0] = '1';
	literal_pool[1] = '2';
	literal_pool[2] = '3';
	literal_pool[3] = '4';
	literal_pool[4] = '5';
	literal_pool[5] = '6';
	literal_pool[6] = '7';
	literal_pool[7] = '8';
	literal_pool[8] = '9';

    /* clear screen */
    Lcd_Clr();
    Lcd_Active_Clr();

    Lcd_DspAscII8x16(85, 25, DARKGRAY, "PROYECTO HARDWARE");
    Lcd_DspAscII8x16(132, 65, BLACK, "SUDOKU");
    Lcd_DspAscII8x16(10, 105, BLACK, "Instrucciones:");
    Lcd_DspAscII8x16(10, 122, BLACK, "  - Boton Izdo: Seleccionar Fil/Col/Val");
    Lcd_DspAscII8x16(10, 135, BLACK, "  - Boton Dcho: Confirmar");
    Lcd_DspAscII8x16(55, 175, BLACK, "Pulse un boton para jugar...");
    Lcd_DspAscII8x16(165, 207, BLACK, "Daniel Martinez");
    Lcd_DspAscII8x16(165, 220, BLACK, "Oscar Saboya");

    Lcd_Dma_Trans();
}

inline void Lcd_pintarNumero(short int i, short int j, int num, INT8U colour)
{
	INT8U a = literal_pool[num-1];
	Lcd_DspAscII8x16(12+20*j, 37+20*i, colour, &a);
}

void Lcd_indicesTablero(void)
{
	short int i;
	*ind = '1';
	for (i=1; i<=9; i++) {
		Lcd_DspAscII8x16(12+20*i,37,BLACK,ind);
		Lcd_DspAscII8x16(10,37+20*i,BLACK,ind);
		*ind=*ind+1;
	}
}

void Lcd_tableroVacio(void)
{
	/* Muestra el tablero inicial vacio y el mensaje de acabar */
	short int i;
	Lcd_DspAscII8x16(0,8,BLACK,"Introduzca fila A para acabar");
	for (i=1; i<=9; i++) {
		if (i==1 || i==4 || i==7) {
			Lcd_Draw_HLine(25,205,55+(i-1)*20,BLACK,2);
			Lcd_Draw_VLine (55,235,25+(i-1)*20,BLACK,2);
		}
		else {
			Lcd_Draw_HLine(25,205,55+(i-1)*20,BLACK,1);
			Lcd_Draw_VLine (55,235,25+(i-1)*20,BLACK,1);
		}
	}
	Lcd_Draw_HLine(25,205,235,BLACK,2);
	Lcd_Draw_VLine (55,235,205,BLACK,2);
}

inline void Lcd_filaAcabarBlack(void)
{
	Lcd_DspAscII8x16(0,8,BLACK,"Introduzca fila A para acabar");
}

inline void Lcd_filaAcabarWhite(void)
{
	Lcd_DspAscII8x16(0,8,WHITE,"Introduzca fila A para acabar");
}

void Lcd_filaAcabadaMensaje(void)
{
	Lcd_filaAcabarWhite();
	Lcd_DspAscII8x16(0,4,BLACK,"Partida finalizada.");
	Lcd_DspAscII8x16(0,17,BLACK,"Pulse un boton para volver a jugar");
}

void Lcd_tiempos(void)
{
	/* Muestra los tiempos */
	Lcd_DspAscII8x16(240,60, BLACK, "Tiempo");
	Lcd_DspAscII8x16(215,77, BLACK, "transcurrido:");
	Lcd_Draw_HLine(213,316,95,BLACK,1);
	Lcd_DspAscII8x16(250,100,BLACK,"00:00");
	Lcd_DspAscII8x16(230,140, BLACK, "Tiempo de");
	Lcd_DspAscII8x16(237,157, BLACK, "calculo:");
	Lcd_Draw_HLine(227,305,175,BLACK,1);
}

void Lcd_Cuadricula(void)
{
	  Lcd_Clr();
	    Lcd_Active_Clr();
	Lcd_filaAcabarBlack();
	Lcd_tableroVacio();
	Lcd_indicesTablero();
	Lcd_tiempos();
	Lcd_Dma_Trans();
}

static inline uint8_t
celda_leerValor(CELDA celda)
{
    return (celda & 0x000F);
}

void Lcd_mostrarCandidatos(CELDA tablero[9][9])
{
	short int i, j;
	for (i=0; i<9; i++) {
		for (j=0; j<9; j++) {

			if (celda_leerValor(tablero[i][j]) == 0) {
				CELDA candidatos = tablero[i][j];
				candidatos &= 0x01FF;
				short int it;
				CELDA aux;
				for (it=0; it<9; it++) {
					aux = candidatos & (0x01 << it);
					//Mostrar punto correspondiente
					if ((candidatos & (0x01 << it)) != 0)	Lcd_DspAscII8x16(6*(it%3)+6+20*(j+1), 30+5*(it/3)+20*(i+1), BLACK, ".");
				}
			}
		}
	}
}
//Devuelve 1 si está el bit de error en la celda, 0 si no está el bit de error
int esError(CELDA celda){
	if((celda & 0x4000)!=0)
		return 1;
	else
		return 0;

}

void Lcd_rellenarValoresTab(CELDA tablero[9][9])
{
	short int i, j;
	for (i=1; i<=9; i++) {
		for (j=1; j<=9; j++) {
			uint8_t valor;
			if ((valor=celda_leerValor(tablero[i-1][j-1])) != 0) {
				switch (esPista(i-1,j-1)) {
				case 0:
					Lcd_pintarNumero(i,j,valor,BLACK);//DARKGRAY
					break;
				case 1:
					Lcd_pintarNumero(i,j,valor,BLACK);
					break;
				default:
					break;
				}
				if (esError(tablero[i-1][j-1])!=TRUE) {
					//Lcd_Draw_Box(28+(j-1)*20, 58+(i-1)*20, 15+28+(j-1)*20, 15+58+(i-1)*20, BLACK);
				}
			}
		}
	}
	Lcd_Dma_Trans();
}

void Lcd_escribir_tiempo()
{
	LcdClrRect(245,102,310,125,WHITE);

	if(0<10) {//if(minutos<10) {
		Lcd_DspAscII8x16(250,100,BLACK,"0");
		Lcd_DspAscII8x16(258,100,BLACK,itoa(0)); //Lcd_DspAscII8x16(258,100,BLACK,itoa(minutos));
	}
	else {
		Lcd_DspAscII8x16(250,100,BLACK,itoa(0)); //Lcd_DspAscII8x16(250,100,BLACK,itoa(minutos));
	}

	Lcd_DspAscII8x16(266,100,BLACK,":");

	if(0<10) {//if(segundos<10) {
		Lcd_DspAscII8x16(274,100,BLACK,"0");
		Lcd_DspAscII8x16(282,100,BLACK,itoa(0)); //Lcd_DspAscII8x16(282,100,BLACK,itoa(segundos));
	}
	else{
		Lcd_DspAscII8x16(274,100,BLACK,itoa(0)); //Lcd_DspAscII8x16(274,100,BLACK,itoa(segundos));
	}

	//pintar=0;

	Lcd_Dma_Trans();
}

void Lcd_felicitacion(void)
{
	Lcd_filaAcabarWhite();
	Lcd_DspAscII8x16(0,4,BLACK,"Sudoku finalizado.");
	Lcd_DspAscII8x16(0,17,BLACK,"Pulse un boton para volver a jugar");
}

void Lcd_noHuboModificacion(void)
{
	LcdClrRect(215,178,310,230,WHITE);
	Lcd_DspAscII8x16(240,180,BLACK,"Sin");
	Lcd_DspAscII8x16(217,193,BLACK,"modificacion");
	Lcd_Dma_Trans();
}

