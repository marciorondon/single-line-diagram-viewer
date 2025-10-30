

					/**********************/
					/*       MDIHCPY      */
					/* MODULO DE HARDCOPY */
					/**********************/



#include <dos.h>
#include <graphics.h>
#include <stdio.h>
#include <conio.h>
#include "diagr.h"


int col,lin,plano,j;
char far *ptr_video;
char bufimp;
char extern nome_completo[125][23];
int extern ind_tab;
struct date hoje;
struct time agora;

/*================================*/

void hardcopy ()
{
	int k;

	if ((((get_printer_status ()) & 0xFF00) == 0x9000) ||
		(((get_printer_status ()) & 0xFF00) == 0x1000))
	{
		esconde_mouse ();
		ptr_video = (unsigned char *)0xA0000000;    /* aponta inicio da tela */
		biosprint (1,0,0);
		biosprint (0,0x1B,0);					    /* ESC */
		biosprint (0,0x41,0);					    /* A */
		biosprint (0,0x08,0);					    /* 08 */
		for (col = 0; col < 80; col++)
		{   										/* programa impressora */
			biosprint (0,0x1B,0);					/* ESC */
			biosprint (0,0x5A,0);					/* Z - Densidade Quadrupla */
			biosprint (0,0x28,0);                   /* n1 = 40 */
			biosprint (0,0x05,0);                   /* n2 = 5  */
			for (lin = 0;lin < 330;lin++)
			{
				j = ((350-lin) * 80) + col;         /* aponta byte */
				bufimp = 0;
				for (plano = 0;plano < 4;plano++)
				{
					outportb (0x3CE,04);            /* programa placa EGA */
					outportb (0x3CF,plano);         /* seleciona plano de leitura */
					bufimp |= *(ptr_video + j);     /* prepara agulhas para impressora */
				}
				for (k = 0; k < 4;k++)              /* imprime 4 vezes */
					biosprint (0,bufimp,0);			/* impressao quadrupla */
			}
			biosprint (0,0x0a,0);                   /* line feed */
		}
//		biosprint (0,0x0a,0);                   /* line feed */
//		biosprint (0,0x0a,0);                   /* line feed */
		fprintf (stdprn,"%s - ",nome_completo);
		getdate (&hoje);
		fprintf (stdprn,"%d/%d/%d ",hoje.da_day,hoje.da_mon,hoje.da_year);
		gettime (&agora);
		fprintf (stdprn,"AS %02d:%02d HS.",agora.ti_hour,agora.ti_min);
		biosprint (0,0x0C,0);                            /* form feed */
		aparece_mouse ();
	}
	else
	{
		esconde_mouse ();
		faz_retangulo ();
		outtextxy (TOPX+50,TOPY+20,"Impressora com problemas!");
		outtextxy (TOPX+30,TOPY+35,"Tecle algo para voltar ao menu.");
		getch ();
		retira_retangulo ();
		aparece_mouse ();
	}
}

