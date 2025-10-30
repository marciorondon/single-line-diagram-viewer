
		  /******************************/
		  /*      MDIAGARQ.C            */
		  /* MODULO DE TRAT. ARQUIVOS   */
		  /******************************/


#include <dir.h>
#include <alloc.h>
#include <graphics.h>
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <dos.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "diagr.h"


/* variaveis de ff () */

int box [] = {BOTX+1,TOPY-1,
			  BOTX+20,TOPY-1,
			  BOTX+20,BOTY+1,
			  BOTX+1,BOTY+1,
			  BOTX+1,BOTY-TOPY/2,
			  BOTX+20,BOTY-TOPY/2};

int seta_up [] = {	BOTX+4+6,TOPY+5+3,
					BOTX+4+6+4,TOPY+5+3+4,
					BOTX+4+6-4,TOPY+5+3+4,
					BOTX+4+6,TOPY+5+3};

int seta_down [] = {BOTX+4+6,TOPY+52,
					BOTX+4+6+3,TOPY+52-3,
					BOTX+4+6-3,TOPY+52-3,
					BOTX+4+6,TOPY+52};


typedef struct
	{
		unsigned int qtd;
		unsigned char valor;
	} st_basica;

st_basica registro;

FILE *in;

int extern x,i,j,plano,segr,offset,tam_op_x,inicio,nsai;
char extern far *ptr_video;
char extern /* nome_abr[125][5], nome_completo[125][23],*/ *menu[];
int extern qtd_tela [];
void extern *b_tecla_segr;

char arq_dig[30], arq_dig_aux [30];
char *nomes;
int so_ana,*indice,*p_ana;
int qtd_nomes, ind_tab,diagr,desistiu;
char ext[] = {".EDT"};
char drive [30] = {"A:"};
char menu_diagr [16][4] = { {"01"},
						   {"02"},
						   {"03"},
						   {"04"},
						   {"05"},
						   {"06"},
						   {"07"},
						   {"08"},
						   {"09"},
						   {"10"},
						   {"11"},
						   {"12"},
						   {"13"},
						   {"14"},
						   {"15"},
						   {"16"} };

char menu_din [3][10] = { {"ABERTO"},{"FECHADO"},{"IMPEDIDO"} };

char nome_arq [20],nome_arq_ant [20];

int extern pos_x,pos_y,stbut;
unsigned char extern disja[],disjf[],disji[],facaa[],facaf[],facai[],
			 disja_p[],disjf_p[],disji_p[],facaa_p[],facaf_p[],facai_p[],
			 facaa_pp [],facaf_pp[],facai_pp[],disja_pp[],disjf_pp[],disji_pp[];
typedef struct
	{
		int est;
		int eixox;
		int eixoy;
		char tipo;
	} tela_dig;


tela_dig *ptr_dig,*ptr_aux_dig;
tela_dig  extern ponto_dig;
long tam_arq_dig;

unsigned tamanho;
void *area;

typedef struct
{
	char nome_completo [23];
	char nome_abr [5];
} reg_se;

reg_se *ptr_arq;
int	qtd_se;
char nome_completo [23];

/*===========================================================*/

void faz_retangulo ()
{
	tamanho = imagesize (TOPX,TOPY,BOTX,BOTY);
	area = malloc (tamanho);
	getimage (TOPX,TOPY,BOTX,BOTY,area);
	setcolor (BROWN);                                      /* faz retangulos */
	setfillstyle (SOLID_FILL,BROWN);
	disable ();
	bar (TOPX,TOPY,BOTX,BOTY);
	enable ();
	setfillstyle (SOLID_FILL,CYAN);
	disable ();
	bar (TOPX+4,TOPY+4,BOTX-4,BOTY-4);
	enable ();
	setcolor (BLACK);
	rectangle (TOPX+1,TOPY+1,BOTX-1,BOTY-1);
}

/*===========================================================*/

void retira_retangulo ()
{
	putimage (TOPX,TOPY,area,COPY_PUT);
	free (area);
}

/*===========================================================*/

int escreve_msg_drv_a (char *arq)
{
	int c;

	faz_retangulo ();
	outtextxy (TOPX+120,TOPY+19,arq);
	outtextxy (TOPX+60,TOPY+10,"Arquivo nao econtrado em A:");
	outtextxy (TOPX+15,TOPY+35,"Tecle ESC para sair e qualquer outra");
	outtextxy (TOPX+45,TOPY+48,"quando o drive estiver pronto.");
	putch (07);
	c = getch ();
	retira_retangulo ();
	return (c);
}

/*===========================================================*/

void escreve_msg_drv_c (char *arq)
{
	faz_retangulo ();
	outtextxy (TOPX+120,TOPY+20,arq);
	outtextxy (TOPX+115,TOPY+8,"Erro Grave!");
	outtextxy (TOPX+8,TOPY+30,"Arquivo nao econtrado no Disco Rigido!");
	outtextxy (TOPX+50,TOPY+40,"Pressione qualquer tecla para");
	outtextxy (TOPX+100,TOPY+50,"retornar ao DOS.");
	putch (07);
	delay (200);
	putch (07);
	delay (200);
	putch (07);
	getch ();
	retira_retangulo ();
}

/*===========================================================*/

int existe (char *ptr)
{

	int ti,result;
	reg_se *ptr_arq_aux;

	ptr_arq_aux = ptr_arq;
	ti = 0;
	do
	{
		result = strncmp (ptr,ptr_arq_aux->nome_abr,3);
		if (result == 0)
			ind_tab = ti;
		ti++;
		ptr_arq_aux += 1;
	}
	while ((result != 0) && (ti < qtd_se));
	if (!result)
		return (TRUE);
	else
		return (FALSE);
}

/*===========================================================*/

void dir_janela (int i)
{
	int cont,y,k;

	y=YINIC;
	cont = k = 0;
	while ((i<qtd_nomes) && (k<MAX))
	{
		if (cont < 8)
		{
			outtextxy (XINIC+(cont*DESLOC_H),y,&nomes[i*TAM_NOME]);
			cont++;
			i++;
			k++;
		}
	else
		{
			y+=DESLOC_V;
			cont=0;
		}
	}
}
/*==================================================================*/

void ordena ()
{
	int k,w,indice_aux,p_ana_aux,qtd_tela_aux;
	char nome_aux [9];

	for (k = 0;k < (qtd_nomes-1);k++)
	{
		for (w = k+1; w < qtd_nomes;w++)
		{
			if (strcmp (&nomes[k*TAM_NOME],&nomes[w*TAM_NOME]) > 0)
			{
			   strcpy (&nome_aux,&nomes[k*TAM_NOME]);
			   strcpy (&nomes[k*TAM_NOME],&nomes[w*TAM_NOME]);
			   strcpy (&nomes[w*TAM_NOME],&nome_aux);

			   indice_aux = indice[k];
			   indice[k]  = indice[w];
			   indice[w]  = indice_aux;

			   p_ana_aux = p_ana [k];
			   p_ana [k] = p_ana [w];
			   p_ana [w] = p_ana_aux;
			}
		}
	}
}

/*==================================================================*/


void ff (char *espec)
{


	int tam_opcao,tam_janela,tam_tecla;
	char *pnt,*buffer_opcao,*tecla,*janela;
	int qtd,inicio,fim,aux1,aux2,aux3,aux4,topo;
	int ref,refx,refy,ref_ant,refx_ant,refy_ant,ref_mouse;
	struct ffblk ffblk;
	int achou,ind,opcao,desistiu;
	void *b_seta;

	int arq;

	unsigned tam;

	arq = open ("estacoes.dat",O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
	tam = filelength (arq);
	qtd_se = tam / sizeof (reg_se);
	ptr_arq = malloc (tam);
	read (arq,ptr_arq,tam);
	close (arq);

	for (j = 0; j < qtd_se; j++)
		qtd_tela [j] = 0;
	desistiu = FALSE;
	qtd_nomes = 0;									/* qtd de nomes encontrados */
	achou = findfirst (espec,&ffblk,0);             /* procura pelo primeiro arquivo */
	if (achou == 0)									/* se nao achou nao faz nada */
	{

		esconde_mouse();
		get_mouse_release(0);
		tam_janela = imagesize(TOPX-4,TOPY-3,BOTX+23,BOTY+3);	/* salva area da tela onde aparecera a janela */
		janela = malloc (tam_janela);
		getimage (TOPX-4,TOPY-3,BOTX+23,BOTY+3,janela);
		setfillstyle (SOLID_FILL,BLACK);
		bar (TOPX-4,TOPY-3,BOTX+23,BOTY+3);
		tam_opcao = imagesize (2,2,DESLOC_H-2,DESLOC_V-4);		/* reserva area para iluminar opcao corrente */
		buffer_opcao = malloc (tam_opcao);
		ind = 0;									/* posicao na matriz de nomes */
//		ffblk.ff_name [3] = 0;
		if (existe (&ffblk.ff_name[0]))
		{
			if (qtd_tela [ind_tab] == 0)
			{
				nomes = malloc (TAM_NOME);  				/* reserva 9 posicoes para colocar nulo no final */
				indice = malloc (sizeof (int));
				p_ana = malloc (sizeof (int));              /* cria tabela de .ANA */
				if ((ffblk.ff_name[5] == '_') && (ffblk.ff_name[6] == 'A'))   /* verifica extensao _A */
					p_ana [qtd_nomes] = TRUE;
				else
					p_ana [qtd_nomes] = FALSE;
				ffblk.ff_name[3] = 0;
				strncpy (&nomes[ind],ffblk.ff_name,4);
				indice [qtd_nomes] = ind_tab;
				qtd_nomes++;
				ind += TAM_NOME;
			}
			qtd_tela [ind_tab]++;
		}
		achou = findnext (&ffblk);			 		/* procura o proximo */
		while (achou == 0)
		{

//			ffblk.ff_name [3] = 0;
			if (existe (&ffblk.ff_name[0]))
			{
				if (qtd_tela [ind_tab] == 0)
				{
					if (qtd_nomes == 0)
					{
						nomes = malloc (TAM_NOME);  				/* reserva 9 posicoes para colocar nulo no final */
						indice = malloc (sizeof (int));
						p_ana = malloc (sizeof (int));
						if ((ffblk.ff_name[5] == '_') && (ffblk.ff_name[6] == 'A'))
							p_ana [qtd_nomes] = TRUE;
						else
							p_ana [qtd_nomes] = FALSE;
					}
					else
					{
						nomes = realloc (nomes,ind+TAM_NOME);  		/* realoca mais area para armazenar o novo nome */
						indice = realloc (indice,ind+sizeof(int));
						p_ana = realloc (p_ana,ind+sizeof (int));
						if ((ffblk.ff_name[5] == '_') && (ffblk.ff_name[6] == 'A'))
							p_ana [qtd_nomes] = TRUE;
						else
							p_ana [qtd_nomes] = FALSE;
					}
					ffblk.ff_name [3] = 0;
					strncpy (&nomes[ind],ffblk.ff_name,4);
					indice [qtd_nomes] = ind_tab;
					qtd_nomes++;
					ind += TAM_NOME;
				}
				qtd_tela [ind_tab]++;
			}
			achou = findnext (&ffblk);          		/* procura o proximo */
		}

		ordena ();

		setcolor (BROWN);							/* desenha janela em marrom */
		setfillstyle (SOLID_FILL,BROWN);
		rectangle (TOPX-1,TOPY-1,BOTX+1,BOTY+1);
		rectangle (TOPX-4,TOPY-3,BOTX+23,BOTY+3);
		drawpoly (6,box);
		floodfill (BOTX+2,TOPY+2,BROWN);
		floodfill (BOTX+2,BOTY-2,BROWN);

		faz_tecla (BOTX+4,TOPY+5,BOTX+4+12,TOPY+5+15);

		faz_tecla (BOTX+4,TOPY+35+5,BOTX+4+12,TOPY+35+5+15);

		setcolor (BLACK);
		setfillstyle (SOLID_FILL,BLACK);						/* desenha seta para cima */
		fillpoly (4,seta_up);
		line (BOTX+4+6,TOPY+5+3+4,BOTX+4+6,TOPY+5+3+4+5);
		line (BOTX+4+5,TOPY+5+3+4,BOTX+4+5,TOPY+5+3+4+5);
		line (BOTX+4+7,TOPY+5+3+4,BOTX+4+7,TOPY+5+3+4+5);
		fillpoly (4,seta_down);									/* desenha seta para baixo */
		putpixel (BOTX+4+6,TOPY+52,BLACK);
		line (BOTX+4+6,TOPY+52-4,BOTX+4+6,TOPY+52-9);
		line (BOTX+4+5,TOPY+52-4,BOTX+4+5,TOPY+52-9);
		line (BOTX+4+7,TOPY+52-4,BOTX+4+7,TOPY+52-9);
		desloc_mouse_x (TOPX,BOTX+19);							/* prepara mouse */
		desloc_mouse_y (TOPY,BOTY);
		posiciona_mouse (TOPX,TOPY);
		setviewport (TOPX,TOPY,BOTX,BOTY,1);					/* abre viewport para apresentar a janela */
		clearviewport();
		setcolor (RED);
		setfillstyle (SOLID_FILL,CYAN);
		floodfill (0,0,BROWN);
		topo = ref_ant = refx_ant = refy_ant = ref_mouse = ind = 0;
		dir_janela(topo);							/* preenche janela com nomes dos arquivos */
		inicio = TRUE;
		do
		{
			get_mouse_position();
			refx = pos_x;
			refx -= TOPX;
			refx /= DESLOC_H;
			refy = pos_y;
			refy -= TOPY;
			refy /= DESLOC_V;
			if (pos_x > BOTX)						/* verifica se mouse esta na area das setas */
			{
				if (pos_y < (HY/2))
					ref = 32;						/* mouse na seta para cima */
				else
					ref = 33;                       /* mouse na seta para baixo */
			}
			else
				ref = (QTD_H*refy)+refx;            /* ref de 0 ate 15 = mouse em alguma posicao da janela de nomes */
			switch (ref)
			{
				case  0: case  1: case  2: case  3: case  4: case  5: case  6: case  7:	 /* de 0 a 31 o tratamento e o mesmo */
				case  8: case  9: case 10: case 11: case 12: case 13: case 14: case 15:
				case 16: case 17: case 18: case 19: case 20: case 21: case 22: case 23:
				case 24: case 25: case 26: case 27: case 28: case 29: case 30: case 31:


							if ((ref_mouse == 32)||(ref_mouse == 33))     /* se retornou das teclas */
							{
								esconde_mouse();
								ref_mouse = 0;
							}
							if ((ref_ant != ref) || (inicio == TRUE))     				/* se nova posicao ou 									*/
							{                                               			/* inicio = true (primeira vez ou retornou das teclas) 	*/
								aux1 = DESLOC_H * refx_ant;                 			/* calcula posicoes para controlar o movimento na janela */
								aux2 = DESLOC_V * refy_ant;
								aux3 = DESLOC_H * refx;
								aux4 = DESLOC_V * refy;
								if (inicio != TRUE)
									putimage (aux1+2,aux2+2,buffer_opcao,COPY_PUT); 	/* apaga iluminacao da opcao anterior */
								else
									inicio = FALSE;
								getimage (aux3+2,aux4+2,aux3+DESLOC_H-2,aux4+DESLOC_V-4,buffer_opcao);	/* le area da opcao corrente */
								putimage (aux3+2,aux4+2,buffer_opcao,NOT_PUT);                          /* escreve mesma area com iluminacao */
								refx_ant = refx;														/* salva referencias */
								refy_ant = refy;
								ref_ant  = ref;
							}
							if ((stbut & 1) == 1)            		/* verifica se pressionou botao do mouse */
							{
								get_mouse_release(0);
								if ((topo+ref) < qtd_nomes)         /* se mouse esta em cima de um nome */
									fim=TRUE;
							}
							if ((stbut & 2) == 2)
							{
								get_mouse_release (1);
								fim = desistiu = TRUE;
							}
							break;

				case 32:    if ((ref_ant != ref) && (inicio == FALSE))       								/* se nova posicao e  */
							{                                                   							/* veio da janela 	  */
								putimage (DESLOC_H*refx_ant+2,DESLOC_V*refy_ant+2,buffer_opcao,COPY_PUT);   /* coloca ultima posicao da janela em condicao normal */
								aparece_mouse();															/* mostra mouse para selecionar tecla */
								inicio = TRUE;
								ref_mouse = ref;
							}
							if (((stbut & 1) == 1) && (topo != 0))	/* se pressionou botao e pode fazer scroll */
							{
								setviewport (0,0,639,349,1);							/* volta tela como era antes */
								esconde_mouse ();
								aperta_tecla (BOTX+4,TOPY+5,BOTX+4+12,TOPY+5+15,&b_seta);
								delay (190);
								solta_tecla (BOTX+4,TOPY+5,&b_seta);
								aparece_mouse ();
								setviewport (TOPX,TOPY,BOTX,BOTY,1);
								setcolor (CYAN); 					/* escreve mesma janela na cor do fundo */
								dir_janela (topo);
								setcolor (RED);						/* escreve janela 4 posicoes acima */
								topo -= 4;
								dir_janela (topo);
								get_mouse_release(0);

							}
							if ((stbut & 2) == 2)
							{
								get_mouse_release (1);
								esconde_mouse ();
								fim = desistiu = TRUE;
							}
							break;

				case 33:    if ((ref_ant != ref) && (inicio == FALSE))
							{
								putimage (DESLOC_H*refx_ant+2,DESLOC_V*refy_ant+2,buffer_opcao,COPY_PUT);
								aparece_mouse();
								inicio = TRUE;
								ref_mouse = ref;
							}
							if (((stbut & 1) == 1) && (topo+MAX < qtd_nomes))
							{
								setviewport (0,0,639,349,1);							/* volta tela como era antes */
								esconde_mouse ();
								aperta_tecla (BOTX+4,TOPY+35+5,BOTX+4+12,TOPY+35+5+15,&b_seta);
								delay (190);
								solta_tecla (BOTX+4,TOPY+35+5,&b_seta);
								aparece_mouse ();
				setviewport (TOPX,TOPY,BOTX,BOTY,1);
								setcolor (CYAN);
								dir_janela (topo);
								setcolor (RED);
								topo += 4;
								dir_janela (topo);
								get_mouse_release (0);

							}
							if ((stbut & 2) == 2)
							{
								get_mouse_release (1);
								esconde_mouse ();
								fim = desistiu = TRUE;
							}
							break;
			}
		}
		while ( fim != TRUE);  								 	/* executa ate selecionar um nome */
		if (desistiu)
			nome_arq [0] = 0;
		else
		{
			strncpy (nome_arq,&nomes[(topo+ref)*(TAM_NOME)],TAM_NOME);    /* monta extensao do arquivo */
			strncat (nome_arq,"01",2);
			so_ana = p_ana [topo+ref];                                    /* marca que so tem .ANA */
			if (so_ana)
				strncat (nome_arq,"_A.EDT",6);
			else
				strncat (nome_arq,".EDT",4);
			ind_tab = indice [topo+ref];
			ptr_arq += ind_tab;
			strcpy (nome_completo,ptr_arq->nome_completo);
		}
		free (nomes);											/* libera areas */
		free (buffer_opcao);
		free (indice);
		free (ptr_arq);
		setviewport (0,0,639,349,1);							/* volta tela como era antes */
		putimage (TOPX-4,TOPY-3,janela,COPY_PUT);
		free (janela);
		aparece_mouse();
		desloc_mouse_x (0,639);
		desloc_mouse_y (0,349);
	}
}

/*=====================================================*/

void altera_estado (char *nomef,char *nomed,char *nomef_p,char *nomed_p,char *nomef_pp,char *nomed_pp)
{
	switch (ptr_aux_dig -> tipo)
	{
		case 'F':
			  putimage (ptr_aux_dig -> eixox,ptr_aux_dig -> eixoy,nomef,COPY_PUT);
			  break;

		case 'D':
			  putimage (ptr_aux_dig -> eixox,ptr_aux_dig -> eixoy,nomed,COPY_PUT);
			  break;

		case 'f':
			  putimage (ptr_aux_dig -> eixox,ptr_aux_dig -> eixoy,nomef_p,COPY_PUT);
			  break;

		case 'd':
			  putimage (ptr_aux_dig -> eixox,ptr_aux_dig -> eixoy,nomed_p,COPY_PUT);
			  break;

		case '0':
			  putimage (ptr_aux_dig -> eixox,ptr_aux_dig -> eixoy,nomef_pp,COPY_PUT);
			  break;

		case '1':
			  putimage (ptr_aux_dig -> eixox,ptr_aux_dig -> eixoy,nomed_pp,COPY_PUT);
			  break;


		default: break;
	}
}

/*=========================================================*/

void mostra_dig (char *arquivo)
{


	int k,starq_dig,starq_dig_aux,le_disco;
	char c = '.';
	char *pont_nome;

	char dig[] = {"DIG"};

	strcpy (arq_dig,arquivo);
	pont_nome = strchr (arq_dig,c);
	strcpy ((pont_nome+1),dig);
	strcpy (arq_dig_aux,arq_dig);
	strcat (drive,arq_dig_aux);
	strcpy (arq_dig_aux,drive);
	drive [2] = 0;
	do
	{
		starq_dig = open (arq_dig,O_RDWR|O_BINARY,S_IREAD|S_IWRITE);         /* drive C: */
//		starq_dig_aux = open (arq_dig_aux,O_RDWR|O_BINARY,S_IREAD|S_IWRITE); /* drive A: */
		if ((starq_dig != -1) /* && (starq_dig_aux != -1)*/ )
		{
			le_disco = FALSE;
			tam_arq_dig = filelength (starq_dig);
			ptr_dig = (tela_dig *) malloc (tam_arq_dig);
			read (starq_dig,ptr_dig,tam_arq_dig);
			close (starq_dig);
//			close (starq_dig_aux);
			ptr_aux_dig = ptr_dig;
			for (k = 0;k < (tam_arq_dig/sizeof(tela_dig));k++)
			{
				switch (ptr_aux_dig -> est)
				{
					case 0: altera_estado (facaa,disja,facaa_p,disja_p,facaa_pp,disja_pp);
							break;

					case 1: altera_estado (facaf,disjf,facaf_p,disjf_p,facaf_pp,disjf_pp);
							break;

					case 2: altera_estado (facai,disji,facai_p,disji_p,facai_pp,disji_pp);
							break;

					default: break;
				}
				ptr_aux_dig += 1;
			}
			ptr_aux_dig = ptr_dig;
		}
		else
		{
			if (starq_dig == -1)                 /* se drive C NOK */
			{
				escreve_msg_drv_c (&arq_dig[0]);
				nsai = le_disco = FALSE;          /* aborta */
				if (starq_dig_aux != -1)         /* se abriu arquivo em A */
					close (starq_dig_aux);       /* fecha */
			}
			else
			{
				if (escreve_msg_drv_a (&arq_dig[0]) == 0x1B)  /* se drive A NOK e teclou ESC */
					nsai = le_disco = FALSE;                   /* aborta */
				else
					le_disco = TRUE;                          /* tenta outra vez */
				close (starq_dig);                            /* fecha arquivo do drive C */
			}
		}
	}
	while (le_disco);                                         /* enquanto tenta le o disco */
}

/*====================================================================*/
void descompac ()
{
		int i;

		for (plano = 1;plano < 9;plano <<=1)
		{
			ptr_video = (unsigned char *)0xA0000640; /* aponta inicio da tela, inicio da mem.video */
			outportb (0x3C4,2);                      /* seleciona plano */
			outportb (0x3C5,plano);
			j = 0;
			while (j < 26400)
			{
				fread (&registro,3,1,in);
				for (i = 0;i < registro.qtd;i++)              /* escreve dado na tela  */
				{
					*(ptr_video+j) = registro.valor;
					j++;
				}
			}
		}
		outportb (0x3C4,2);
		outportb (0x3C5,0xFF);
}
/*====================================================================*/
void le_arq ()
{
	int starq;


	ff ("*.EDT");                                                   /* seleciona arquivo */
	if (nome_arq [0] != 0)
	{
		esconde_mouse ();
		if (in != NULL)
			fclose (in);
		in = fopen (nome_arq,"rb");
		if (in != NULL)
		{
			descompac ();                                        /* descompacta os dados */
//			fclose (in);
			strcpy (nome_arq_ant,nome_arq);
			if (!so_ana)
				mostra_dig (nome_arq);
			mostra_ana (nome_arq);
			if (nsai)
			{
				setfillstyle (SOLID_FILL,WHITE);
				bar (458,4,630,15);
				setcolor (BLACK);
				rectangle (457,3,631,16);
				line (456,3,456,16);
				line (632,3,632,16);
				setcolor (LIGHTGRAY);
				line (458,4,458,15);
				line (459,4,459,15);
				line (459,4,630,4);
				setcolor (RED);
				outtextxy (462,7,nome_completo);
				setfillstyle (SOLID_FILL,CYAN);
				tam_op_x = 14;
				bar (offset-5,0,offset+(16*(tam_op_x+7)),19);
				setcolor (BLACK);
				   /* outtextxy (offset-80,7,"DIAGRAMAS:"); */
				settextstyle (SMALL_FONT,HORIZ_DIR,0);
				for (i = 0;i < qtd_tela[ind_tab];i++)
				{
					x = ((tam_op_x + 7) * i) + (offset);
					faz_tecla (x,INI_OP_Y+3,x + tam_op_x,FIM_OP_Y-3);
					setcolor (BLACK);
					outtextxy (x+1,INI_OP_Y+4,&menu_diagr[i][0]);
				}
				segr = (qtd_tela[ind_tab] > 1) ? TRUE : FALSE;
				aperta_tecla (offset,INI_OP_Y+3,offset+tam_op_x,FIM_OP_Y-3,&b_tecla_segr);
				settextstyle (SMALL_FONT,HORIZ_DIR,0);
				setfillstyle (SOLID_FILL,WHITE);
				bar (offset+2,INI_OP_Y+3+4,offset+2+11,INI_OP_Y+3+2+10);
				setcolor (LIGHTBLUE);
				outtextxy (offset+4,INI_OP_Y+3+2,&menu_diagr [0][0]);
				settextstyle (DEFAULT_FONT,HORIZ_DIR,1);
				diagr = 0;
			}
		}
		else
		{
			faz_retangulo ();
//			outtextxy (TOPX+60,TOPY+16,"Insira um disco no drive A!");
			outtextxy (TOPX+18,TOPY+32,"A Estacao esta em uso");
//			outtextxy (TOPX+15,TOPY+45,"tecla quando o drive estiver pronto.");
			putch (07);
			getch ();
			retira_retangulo ();
		}

		tam_op_x = 28;
	}
	else
	{
		desistiu = TRUE;
	}
	aparece_mouse ();
}

/*================================================================*/

void muda_diagrama (int c,int n)
{

	if ( c < 10 )
	{
		nome_arq[3] = '0';
		itoa( c, &nome_arq[4], 10 );
	}
	else
		itoa( c, &nome_arq[3], 10 );

	nome_arq[5] = '.';
	if (strcmp (nome_arq,nome_arq_ant) != 0)
	{
		esconde_mouse ();

		fclose (in);
		in = fopen (nome_arq,"rb");
		if (in != NULL)
		{
			tam_op_x = 14;
			descompac ();                                        /* descompacta os dados */
//			fclose (in);
			strcpy (nome_arq_ant,nome_arq);
			mostra_dig (nome_arq);
			x = ((tam_op_x + 7) * n) + offset;
			solta_tecla (x,INI_OP_Y+3,&b_tecla_segr);
			n = c - 1;
			x = ((tam_op_x + 7) * n) + offset;
			aperta_tecla (x,INI_OP_Y+3,x+tam_op_x,FIM_OP_Y-3,&b_tecla_segr);
			settextstyle (SMALL_FONT,HORIZ_DIR,0);
			setfillstyle (SOLID_FILL,WHITE);
			bar (x+2,INI_OP_Y+3+2,x+2+11,INI_OP_Y+3+2+9);
			setcolor (LIGHTBLUE);
			outtextxy (x+2+1,INI_OP_Y+3+1,&menu_diagr [n][0]);
			settextstyle (DEFAULT_FONT,HORIZ_DIR,1);
			mostra_dig (nome_arq);
			tam_op_x = 28;
		}
/*		else
			escreve_msg (&nome_arq[0],"Arquivo nao encontrado !"); */
		aparece_mouse ();
	}
}

/*===============================================================*/

void tela ()
{
	get_mouse_release (0);
	if ((inicio) || (desistiu))
	{
		inicio = FALSE;
		desistiu = FALSE;
	}
	else
	{
		free (b_tecla_segr);
		free (ptr_dig);
	}
	le_arq ();
}

/*====================================================*/

void procura ()
{
	int k,difx,dify,x,y,opcao,starq_dig,starq_dig_aux;

	get_mouse_release (0);
	k = 0;
	do
	{
		difx = pos_x - (ptr_aux_dig -> eixox);
		dify = pos_y - (ptr_aux_dig -> eixoy);
		ptr_aux_dig += 1;
		k += sizeof (tela_dig);
	}
	while (((difx < 0) || (difx > 20) || (dify < 0) || (dify > 10)) &&
		  (k < tam_arq_dig));
	if ((difx >= 0) && (difx <= 20) && (dify >= 0) && (dify <= 10))
	{
		for (opcao = 0;opcao < 3;opcao++)
			menu [opcao] = &menu_din [opcao][0];
		ptr_aux_dig -= 1;
		x = (ptr_aux_dig -> eixox) + 30;
		y = (ptr_aux_dig -> eixoy);
		if ((x+82) > 639)
			x = (ptr_aux_dig -> eixox) - 30 - 82;
		if ((y+33) > 349)
			y = (ptr_aux_dig -> eixoy) - 25;
		opcao = opcoes (x,y,3);
		esconde_mouse ();
		switch (opcao)
		{
			case 0: ptr_aux_dig -> est = 0;
					altera_estado (facaa,disja,facaa_p,disja_p,facaa_pp,disja_pp);
					break;

			case 1: ptr_aux_dig -> est = 1;
					altera_estado (facaf,disjf,facaf_p,disjf_p,facaf_pp,disjf_pp);
					break;

			case 2: ptr_aux_dig -> est = 2;
					altera_estado (facai,disji,facai_p,disji_p,facai_pp,disji_pp);
					break;

			default: break;
		}
		starq_dig = open (arq_dig,O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
//		starq_dig_aux = open (arq_dig_aux,O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
		lseek (starq_dig,0,SEEK_SET);
		lseek (starq_dig_aux,0,SEEK_SET);
		write (starq_dig,ptr_dig,tam_arq_dig);
//		write (starq_dig_aux,ptr_dig,tam_arq_dig);
		close (starq_dig);
//		close (starq_dig_aux);
		aparece_mouse ();
	}
	ptr_aux_dig = ptr_dig;
}

