#include <alloc.h>
#include <sys\stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <graphics.h>

/*
#define NR_SE 165   */
#define NR_LI 20

/* int i, j, ind_de, ind_para, temp, cont;
char linha [150], barra [5], volt [6], cgMW [6], cgMvar [6], ccMW [6],
	 ccMvar [6], barra1 [5], cfluxoMW [8], cfluxoMvar [8];
*/

typedef struct
{
	int tipo;
	int x;
	int y;
	char origem  [15];
	char destino [15];
	char unidade [5];
	int ckt;
} reg_ana;

reg_ana ponto_ana,*ptr_ana,*ptr_aux_ana;


typedef struct               /* estrutura com dados de Fluxo ativo/reativo */
{                            /* na linha da barra_de para a barra_para     */
	char barra_para [5];
	float fluxo_MW;
	float fluxo_Mvar;
} bar_para;

typedef struct               /* estrutura  com dados de tensao, geracao  */
{                            /* e carga na barra_de                      */
	char barra_de [5];
	int num_bar;
	float tensao;
	float ger_MW;
	float ger_Mvar;
	float carg_MW;
	float carg_Mvar;
	bar_para vet_bar_para [NR_LI];
} bar_de;

// bar_de vet_bar_de [NR_SE];   /* vetor de estruturas */

bar_de far *ptr_arq_flow,*ptr_origem,far *ptr_arq_flow_aux;
long tam_arq_flow,tam_arq_ana;
int starq_flow;
int qtd_reg_flow,qtd_reg_ana,indice_int;
char arq_ana [30];
char str_convert[15],*str_valor;
char far *string;


char far *converte (double valor)
{

	int dec,sinal;

	str_valor = fcvt (valor,6,&dec,&sinal);
	if (sinal == 0)
		str_convert [0] = '+';
	else
		str_convert [0] = '-';
	strncpy (&str_convert[1],str_valor,dec);
	str_convert[dec+1] = '.';
	strncpy (&str_convert[dec+2],str_valor+dec,7-(dec+2));
	if (str_convert [dec+4] >= '5')
	{
		if (str_convert [dec+4] == '9')
		{
			str_convert [dec+3] = '0';
			str_convert [dec+2]++;
		}
		else
			str_convert [dec+3]++;
	}
	str_convert[dec+4] = 0;
//	str_convert[7] = 0;

//	return (str_convert);
}


le_flow ()
{

	starq_flow = open ("ARQENTRA",O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
	tam_arq_flow = filelength (starq_flow);
	qtd_reg_flow = tam_arq_flow / sizeof (bar_de);
	ptr_arq_flow = (bar_de *) farmalloc (tam_arq_flow);
	ptr_arq_flow_aux = ptr_arq_flow;
	read (starq_flow,ptr_arq_flow,tam_arq_flow);
	close (starq_flow);
}

void procura_destino ()
{
	int j;

	indice_int = 0;
	do
	{
		j = strcmp (ptr_aux_ana->destino,ptr_arq_flow_aux->vet_bar_para[indice_int].barra_para);
		indice_int++;
	}
	while ((indice_int < NR_LI) && (j != 0));
	indice_int--;
}

void mostra_ana (char *arquivo)
{
	int k,starq_ana,j,i;
	char c = '.';
	char *pont_nome;
	char ana[] = {"ANA"};
	void *ptr_origem;
	bar_de *ptr_reg;



	strcpy (arq_ana,arquivo);
	pont_nome = strchr (arq_ana,c);
	strcpy ((pont_nome+1),ana);

	starq_ana = open (arq_ana,O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
	if (starq_ana != -1)
	{
		tam_arq_ana = filelength (starq_ana);
		ptr_ana = (reg_ana *) malloc (tam_arq_ana);
		ptr_aux_ana = ptr_ana;
		read (starq_ana,ptr_ana,tam_arq_ana);
		close (starq_ana);
		le_flow ();
		qtd_reg_ana = tam_arq_ana / (sizeof (reg_ana));

		for (i = 0;i < qtd_reg_ana;i++)
		{
			k = 0;
			do
			{
				j = strcmp (ptr_aux_ana->origem,ptr_arq_flow_aux->barra_de);
				if (j != 0)
					ptr_arq_flow_aux += 1;
				k++;
			}
			while ((k < qtd_reg_flow) || (j != 0));

			switch (ptr_aux_ana->tipo)
			{
				case 0:
						settextstyle (SMALL_FONT,HORIZ_DIR,0);
						setcolor (LIGHTBLUE);
						converte (ptr_arq_flow_aux->tensao*138);
						outtextxy (ptr_aux_ana->x,ptr_aux_ana->y,str_convert);
						break;

				case 1:
						settextstyle (SMALL_FONT,HORIZ_DIR,0);
						if (ptr_aux_ana->destino[0] == 0)
						{
//							setcolor (GREEN);
							if (strcmp (ptr_aux_ana->unidade,"MW") == 0)
							{
								converte (ptr_arq_flow_aux->ger_MW);
								setcolor (2);
							}
							else
							{
								converte (ptr_arq_flow_aux->ger_Mvar);
								setcolor (3);
							}
							outtextxy (ptr_aux_ana->x,ptr_aux_ana->y,str_convert);
						}
						else
						{
							procura_destino ();
							if (strcmp(ptr_aux_ana->unidade,"MW") == 0)
							{
								setcolor (12);
								converte (ptr_arq_flow_aux->vet_bar_para[indice_int+ptr_aux_ana->ckt].fluxo_MW);
							}
							else
							{
								setcolor (13);
								converte (ptr_arq_flow_aux->vet_bar_para[indice_int+ptr_aux_ana->ckt].fluxo_Mvar);
							}
							outtextxy (ptr_aux_ana->x,ptr_aux_ana->y,str_convert);
						}
			}
			ptr_aux_ana += 1;
			ptr_arq_flow_aux = ptr_arq_flow;
		}
	}
	settextstyle (DEFAULT_FONT,HORIZ_DIR,1);
	setcolor (RED);
}


