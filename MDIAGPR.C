

			/********************************/
			/*         MDIAGPR.C            */
			/* MODULO PRINCIPAL DO DIAGRAMA */
			/********************************/

#include <graphics.h>
#include <alloc.h>
#include <dos.h>
#include <stdio.h>
#include "diagr.h"

int g_mode,
	g_driver,
	top_x,
	i,
	x,
	segr,
	inicio,
	offset,
	tam_op_x,
	opcao,
	st_disk,
	tec,
	nsai;


int extern pos_x,pos_y,stbut,diagr,ind_tab,qtd_tela[125];
void *b_temp,*b_tecla_segr;

char menu_principal [NR_OP][5] = { {"SEs"},
								   {"IMP"},
								   {"FIM"} };

char msg_light [] = {"Light"};

int vet_tela [] = {0,0,639,0,639,349,0,349,0,0};

int vet0 [] = {150,40,182,40,105,100,205,100,190,115,55,115,150,40};
int vet1 [] = {50,120,185,120,90,201,69,190,140,135,30,135,50,120};

FILE extern *in;
/*=================================================================*/

void tela_inicial ()
{

	cleardevice ();
	setcolor (GREEN);
	setfillstyle (INTERLEAVE_FILL,GREEN);
	fillpoly (5,vet_tela);
	setcolor (WHITE);
	setfillstyle (SOLID_FILL,WHITE);
	fillellipse (120,120,100,80);
	settextstyle (TRIPLEX_FONT,HORIZ_DIR,0);
	setusercharsize (4,1,4,1);
	outtextxy (270,50,msg_light);
	setcolor (GREEN);
	setfillstyle (INTERLEAVE_FILL,GREEN);
	setlinestyle (DOTTED_LINE,0,NORM_WIDTH);
	fillpoly (7,vet0);
	fillpoly (7,vet1);
	setlinestyle (SOLID_LINE,0,NORM_WIDTH);
	setcolor (WHITE);
	line (20,225,590,225);
	setlinestyle (SOLID_LINE,0,THICK_WIDTH);
	line (20,230,590,230);
	setusercharsize (1,1,1,1);
	outtextxy (90,250,"DIAGRAMAS DE OPERACAO");
	settextstyle (DEFAULT_FONT,HORIZ_DIR,0);
	setlinestyle (SOLID_LINE,0,NORM_WIDTH);
	outtextxy (150,295,"Autores: Marcio Egydio S. Rondon - DTAO");
	outtextxy (2,340,"Versao 2.3");
	outtextxy (350,340,"Para iniciar, digite qualquer tecla.");
	getch ();
}

/*=================================================================*/

int sel_opcao ()
{
    int escolha,flag;

    flag = 0;
    do
	{
		get_mouse_buttom (0);                         /* verifica se apertou botao 1 */
		if ((pos_y >= INI_OP_Y + 3) &&
			(pos_y <= FIM_OP_Y - 3) &&
			(pos_x >= 5) &&
			(pos_x <= ((tam_op_x + 7) * NR_OP)))
		{
			escolha = (pos_x-5)/(tam_op_x+7);               /* identifica opcao */
			top_x = ((tam_op_x+7) * escolha) + 5;
			esconde_mouse ();
			aperta_tecla (top_x,INI_OP_Y+3,top_x+tam_op_x,FIM_OP_Y-3,&b_temp);
			settextstyle (SMALL_FONT,HORIZ_DIR,0);
			setfillstyle (SOLID_FILL,WHITE);
			bar (top_x+7,INI_OP_Y+3+2,top_x+7+16,INI_OP_Y+3+2+8);
			setcolor (LIGHTBLUE);
			outtextxy (top_x+8,INI_OP_Y+3+2,&menu_principal [escolha][0]);
			settextstyle (DEFAULT_FONT,HORIZ_DIR,1);
			aparece_mouse ();
			flag = TRUE;
		}
		else
		{
			tam_op_x = 14;
			if ((segr) &&
				(pos_y >= INI_OP_Y + 3) &&
				(pos_y <= FIM_OP_Y - 3) &&
				(pos_x >= offset) &&
				(pos_x <= ((tam_op_x + 7) * qtd_tela[ind_tab])+offset))
				{
					escolha = (pos_x - offset);
					escolha /= (tam_op_x + 7);
					escolha += NR_OP;
					flag = TRUE;
				}
				else
				{
					if (pos_y > FIM_OP_Y)
					{
						escolha = 19;
						flag = TRUE;
					}
				}
		}
		tam_op_x = 28;
		get_mouse_release (0);
	}
    while (flag == 0);
	return (escolha);
}

/*=======================================================================*/

void desfaz_op()
{
		top_x = ((tam_op_x+7) * opcao) + 5;
		solta_tecla (top_x,INI_OP_Y+3,&b_temp);
}

/*=======================================================================*/

main ()
{

	g_mode = 1;
	g_driver = 3;
	if (registerbgidriver (EGAVGA_driver) < 0) exit (1);
	if (registerbgifont (sansserif_font) < 0) exit (1);
	if (registerbgifont (triplex_font) < 0) exit (1);
	if (registerbgifont (small_font) < 0) exit (1);
	initgraph (&g_driver,&g_mode," ");
	chk_mouse ();
//	st_disk = get_disk_status ();
	tela_inicial ();
	tam_op_x = 28;
	inicio = nsai = TRUE;
	in = NULL;
	setbkcolor (BLACK);
	clearviewport ();
	setfillstyle (SOLID_FILL,CYAN);
	disable ();
	bar (INI_OP_X,INI_OP_Y,FIM_OP_X,FIM_OP_Y);
	enable ();
	settextstyle (SMALL_FONT,HORIZ_DIR,0);
	for (i = 0; i < NR_OP; i++)
	{
		x = ((tam_op_x+7) * i)+5;
		faz_tecla (x,INI_OP_Y+3,x+tam_op_x,FIM_OP_Y-3);
		setcolor (BLACK);
		outtextxy (x+5,INI_OP_Y+4,&menu_principal [i][0]);
	}
	offset =  ((tam_op_x+7) * (NR_OP/* +2.5 */))+15;
	settextstyle (DEFAULT_FONT,HORIZ_DIR,0);
//	while (st_disk == 0x80)
//	{
//		faz_retangulo ();
//		outtextxy (TOPX+60,TOPY+16,"Insira um disco no drive A!");
//		outtextxy (TOPX+11,TOPY+32,"Tecle ESC para sair ou qualquer outra");
//		outtextxy (TOPX+15,TOPY+45,"tecla quando o drive estiver pronto.");
//		putch (07);
//		tec = getch ();
//		retira_retangulo ();
//		if (tec == 0x1B)
//			nsai = st_disk = FALSE;
//		else
//			st_disk = get_disk_status ();
//	}
	aparece_mouse ();
	while (nsai)
	{
		get_mouse_release (0);
		opcao = sel_opcao ();
		desloc_mouse_y (FIM_OP_Y+2,342);

		switch (opcao)
		{

			case 0: tela ();
					esconde_mouse ();
					desfaz_op ();
					aparece_mouse ();
					break;

			case 1: hardcopy ();
					esconde_mouse ();
					desfaz_op ();
					aparece_mouse ();
					break;

			case 2: nsai = FALSE;
					esconde_mouse ();
					desfaz_op ();
					aparece_mouse ();
					if (!inicio)
						free (b_tecla_segr);
					break;

			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
			case 17:
			case 18: muda_diagrama (opcao - 2,diagr);
					diagr = opcao - 3;
					break;

			case 19: procura ();
					break;

			default: break;
		}
		desloc_mouse_y (0,349);
	}
	esconde_mouse ();
    clearviewport();
    closegraph ();
	exit (0);
}