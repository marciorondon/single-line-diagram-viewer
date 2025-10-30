
			  /*********************************/
			  /*          MDIAGAUX.C           */
			  /*     ROTINAS  AUXILIARES       */
			  /*       MOUSE, MENU, ETC        */
			  /*********************************/


#include <dos.h>
#include <graphics.h>
#include <alloc.h>
#include "diagr.h"

int stbut,
	pos_x,
	pos_y;

char *menu [3];
unsigned wit,comp;

union REGS regs;
struct SREGS sregs;

/*========================================================*/

void posiciona_cursor (int lin,int col)
{

	regs.h.ah = 0x02;
	regs.h.bh = 2;
	regs.h.dh = lin;
	regs.h.dl = col;
	int86 (CURSOR,&regs,&regs);
}

/*==============================================================*/

void freia_mouse()
{
    regs.x.ax = 0x000f;
    regs.x.cx = 16;
    regs.x.dx = 32;
	int86 (MOUSE,&regs,&regs);
}

/*=======================================================================*/

void acelera_mouse()
{
    regs.x.ax = 0x000f;
    regs.x.cx = 8;
	regs.x.dx = 16;
    int86 (MOUSE,&regs,&regs);

}
/*==========================================================================*/
void get_mouse_release (int tecla)
{

    int mask;
    do
    {
		mask = 1;
		regs.x.ax = 0x0006;
		regs.x.bx = tecla;
		int86 (MOUSE,&regs,&regs);
		stbut = regs.x.ax;
		pos_x = regs.x.cx;
		pos_y = regs.x.dx;
		if (tecla != 0)
			mask <<= tecla;
    }
    while ((stbut & mask) != 0);
}
/*==================================================================*/
void get_mouse_position ()
{
	regs.x.ax = 0x0003;
	int86 (MOUSE,&regs,&regs);
    stbut = regs.x.bx;
    pos_x = regs.x.cx;
	pos_y = regs.x.dx;
}

/*======================================================================*/
void chk_mouse ()
{

    regs.x.ax = 0;
    int86(MOUSE,&regs,&regs);
/*	if (regs.x.ax == 0)
	{
		closegraph ();
		puts ("Mouse nao instalado !");
		exit (255);
	} */
}


void get_mouse_buttom (int tecla)
{
    int mask;

	get_mouse_release (tecla);
    do
	{
	mask = 1;
	regs.x.ax = 0x0005;
	regs.x.bx = tecla;
	int86 (MOUSE,&regs,&regs);
	stbut = regs.x.ax;
	pos_x = regs.x.cx;
	pos_y = regs.x.dx;
	if (tecla != 0)
	   mask <<= tecla;
    }
	while ((stbut & mask) != mask);
}

/*============================================================================*/

void esconde_mouse ()
{
    regs.x.ax = 0x0002;
    int86 (MOUSE,&regs,&regs);
}

/*===================================================*/

void aparece_mouse ()
{
    regs.x.ax = 0x0001;
    int86 (MOUSE,&regs,&regs);
}

/*===================================================*/

void posiciona_mouse (int x, int y)
{
    regs.x.ax = 0x0004;
    regs.x.cx = x;
	regs.x.dx = y;
	int86 (MOUSE,&regs,&regs);
}

/*====================================================*/

void desloc_mouse_y (int min, int max)
{
    regs.x.ax = 0x0008;
    regs.x.cx = min;
    regs.x.dx = max;
    int86 (MOUSE,&regs,&regs);
}

/*====================================================*/

void desloc_mouse_x (int min, int max)
{
	regs.x.ax = 0x0007;
	regs.x.cx = min;
	regs.x.dx = max;
	int86 (MOUSE,&regs,&regs);
}
/*=======================================================================*/
void get_buttom_pressed ()
{
	get_mouse_release (0);
	get_mouse_release (1);
	do
	  get_mouse_position ();
	while (((stbut & 1) != 1) && ((stbut & 2) != 2));
}

/*======================================================*/

int  opcoes (int xinic, int yinic, int nropcoes)
{

								 /* constantes */

#define POSICAO_MOUSE 5
#define LARGURA_OPCAO 12
#define COMPRIM_OPCAO 83
#define DESLOC_H_TEXTO 2
#define DESLOC_V_TEXTO 3

								  /* variaveis */

void     *buffer_menu,*buffer_opcao;
unsigned size_vazio,size_menu;
int      xfinal,yfinal,pos,posant,yaux,i,inicio,antx,anty;


   esconde_mouse ();
   xfinal = xinic + COMPRIM_OPCAO - 1;                           /* salva area do menu para restaurar no final */
   yfinal = yinic + ((LARGURA_OPCAO - 1) * nropcoes);
   size_menu = imagesize (xinic,yinic,xfinal,yfinal);
   buffer_menu = malloc(size_menu);
   getimage (xinic,yinic,xfinal,yfinal,buffer_menu);
   setfillstyle (SOLID_FILL,BLACK);
   bar (xinic,yinic,xfinal,yfinal);
   size_vazio = imagesize (xinic+1,yinic+1,xfinal-1,yinic+LARGURA_OPCAO-2);        /* area de uma opcao */
   buffer_opcao = malloc (size_vazio);                               /* inicializa buffer para armazenar opcao */
   setcolor (BROWN);                                                 /* desenha menu */
   rectangle (xinic,yinic,xfinal,yfinal);
   yaux = yinic;
   for (i=0;i< nropcoes;i++)
   {
	   setcolor (WHITE);
       outtextxy (xinic+DESLOC_H_TEXTO,yaux+DESLOC_V_TEXTO,menu[i]);
	   yaux += LARGURA_OPCAO-1;
	   setcolor (BROWN);
	   line (xinic,yaux,xfinal,yaux);
   }

   posiciona_mouse (xfinal + POSICAO_MOUSE , yinic + LARGURA_OPCAO / 2);        /* posiciona mouse no incio , ao lado do menu */
   desloc_mouse_y (yinic,yfinal-1);                                          /* limita movimento do mouse ao eixo y */
   desloc_mouse_x (xfinal + POSICAO_MOUSE , xfinal + POSICAO_MOUSE);
									 /* obtem posicao do mouse  e retorna quando selecionar opcao */
   antx = anty = posant = inicio = 0;
   do
   {
	  get_mouse_position();
      pos = (pos_y - yinic) / LARGURA_OPCAO;
      if((posant != pos) || (inicio == 0))
      {
	 yaux = (yinic + pos * (LARGURA_OPCAO-1));
	 if (inicio != 0)
	    putimage (antx,anty,buffer_opcao,COPY_PUT);
	 else
	    inicio = 1;
	 getimage (xinic+1,yaux+1,xfinal-1,yaux+LARGURA_OPCAO-2,buffer_opcao);
	 putimage (xinic+1,yaux+1,buffer_opcao,NOT_PUT);
	 antx = xinic + 1;
	 anty = yaux + 1;
	 posant = pos;
      }
   }
   while (( stbut & 1 ) != 1);
   putimage (xinic,yinic,buffer_menu,COPY_PUT);
   get_mouse_release (0);
   aparece_mouse();
   desloc_mouse_y (21,342);
   desloc_mouse_x (0,639);
   free (buffer_menu);
   free (buffer_opcao);
   acelera_mouse ();
   return (pos);

}
/*======================================================*/

void faz_tecla (int xt,int yt,int xb,int yb)
{
	setfillstyle (SOLID_FILL,WHITE);
	bar (xt,yt,xb,yb);
	setcolor (BLACK);
	rectangle (xt-1,yt-1,xb+1,yb+1);
	line (xt+2,yb+2,xb+1,yb+2);
	line (xb+2,yt+1,xb+2,yb+2);
	setcolor (LIGHTGRAY);
	line (xb,yt,xb,yb-1);
	line (xb-1,yt+1,xb-1,yb-1);
	line (xt,yb,xb,yb);
	line (xt+1,yb-1,xb,yb-1);
}

/*========================================*/

void aperta_tecla (int xt,int yt,int xb,int yb,void *(*tecla_ant))
{
	void *tecla_mov;

	comp = imagesize (xt-3,yt-3,xb+4,yb+4);
	(*tecla_ant) = malloc (comp);
	getimage (xt-3,yt-3,xb+4,yb+4,(*tecla_ant));
	wit = imagesize (xt-3,yt-2,xb+1,yb);
	tecla_mov = malloc (wit);
	getimage (xt-3,yt-2,xb+1,yb,tecla_mov);
	putimage (xt-1,yt-1,tecla_mov,COPY_PUT);
	free (tecla_mov);
}

/*======================================*/

void solta_tecla (int xt,int yt,void *(*tecla_ant))
{
	putimage (xt-3,yt-3,(*tecla_ant),COPY_PUT);
	free (*tecla_ant);
}

/*======================================*/

int get_disk_status ()
{
	regs.x.ax = 0x0400;
	regs.x.cx = 0;
	regs.x.dx = 0;
	int86 (DISK,&regs,&regs);
	return (regs.h.ah);
}
/*======================================*/

int get_printer_status ()
{
	regs.h.ah = 0x01;
	regs.x.dx = 0;
	int86 (PRINTER,&regs,&regs);
	return (regs.x.ax);
}