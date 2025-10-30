
/**************************************************************/
/*                        L E A R Q 1                         */
/*  Programa para leitura de um arquivo de saida do Load Flow */
/*                                                            */
/* Autores:  Charles de Capdeville                            */
/*           Antonio Lisboa Salles Neto                       */
/*           Marcio Egydio da Silva Rondon                    */
/**************************************************************/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys\stat.h>
#include <fcntl.h>
#include <io.h>

#define NR_SE 165
#define NR_LI 20

int i, j, ind_de, ind_para, temp, cont;
char linha [150], barra [5], volt [6], cgMW [6], cgMvar [6], ccMW [6],
	 ccMvar [6], barra1 [5], cfluxoMW [8], cfluxoMvar [8];

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

bar_de vet_bar_de [NR_SE];   /* vetor de estruturas */

div_t x;

FILE *arq, *arq1;


/********************************/
/*           ROTINA             */
/* GERA ARQUIVO PARA O DIAGRAMA */
/********************************/

void gerarq ()
{
	int starq_flow;
//	arq1 = fopen ("arqentra", "w");
//	if (arq1 != NULL)    /* testa se houve erro na abertura do arquivo */
//	{
//		for (ind_de = 0; ind_de < NR_SE; ind_de++)
//		{
//			printf ("%d \n",ind_de);
//			fprintf (arq1, "%s %d %5.3f %5.1f %5.1f %5.1f %5.1f ",
/*							vet_bar_de [ind_de].barra_de,
							vet_bar_de [ind_de].num_bar,
							vet_bar_de [ind_de].tensao,
							vet_bar_de [ind_de].ger_MW,
							vet_bar_de [ind_de].ger_Mvar,
							vet_bar_de [ind_de].carg_MW,
							vet_bar_de [ind_de].carg_Mvar);
			for (ind_para = 0; ind_para < NR_LI; ind_para++)
			{
				fprintf (arq1, "%s %5.1f %5.1f ",
								vet_bar_de [ind_de].vet_bar_para [ind_para].barra_para,
								vet_bar_de [ind_de].vet_bar_para [ind_para].fluxo_MW,
								vet_bar_de [ind_de].vet_bar_para [ind_para].fluxo_Mvar);
			}
		}
	}
	else
		printf ("ERRO NA ABERTURA DO ARQUIVO !!! \n");
	fclose (arq1);  */

	starq_flow = open ("ARQENTRA",O_RDWR|O_BINARY|O_CREAT,S_IREAD|S_IWRITE);
	write (starq_flow,&vet_bar_de,(sizeof (bar_de)*NR_SE));
	close (starq_flow);
}

/**********************************************/
void main ()
{
	for (ind_de=0; ind_de<NR_SE; ind_de++)
	{
		vet_bar_de [ind_de].barra_de [0] = '\0';
		vet_bar_de [ind_de].num_bar = 0;
		vet_bar_de [ind_de].tensao = 0;
		vet_bar_de [ind_de].ger_MW = 0;
		vet_bar_de [ind_de].ger_Mvar = 0;
		vet_bar_de [ind_de].carg_MW = 0;
		vet_bar_de [ind_de].carg_Mvar = 0;
		for (ind_para=0; ind_para<NR_LI; ind_para++)
		{
			vet_bar_de [ind_de].vet_bar_para [ind_para].barra_para [0] = '\0';
			vet_bar_de [ind_de].vet_bar_para [ind_para].fluxo_MW = 0;
			vet_bar_de [ind_de].vet_bar_para [ind_para].fluxo_Mvar = 0;
		}
	}
	ind_de = 0; ind_para = 0; temp =0; i = 0; j = 0; cont = 0; /* inicia variaveis */
	arq = fopen ("saida1.lis","r");          /* abre arquivo para leitura */
	while (strstr (linha,"B U S") == NULL)   /* acha o cabe‡alho */
		fgets (linha,140,arq);
	for (i=0; i<3; i++)                     /* pula cabe‡alho */
		fgets (linha,140,arq);
	i = 0;
	do
	{
		if (strstr (linha,"CENTRAIS ELETRICAS BRASILEIRAS") == NULL)
		{
			fgets (linha,140,arq);
			if ((linha [0] == 0x20) && ((linha [4] >= '0' && linha [4] <= '9') || (linha [4] == 0x20)))
			{
			if (((linha [37] != 0x2D) && (linha [38] != 0x2D)) && (linha [21] != 0x41))
			{
				if (linha [4] != 0x20)
				{
					temp++;
					ind_de = temp - 1;
					ind_para = 0;
				}
				for (j=0; j<116; j++)
				{    	                                   /*   pesquisa     */
					if (linha [j] != 0x20)                 /* dados de interesse */
					{                                      /* na linha e armazena */
						x = div (j,10);                    /* nas variaveis */
						switch (x.quot)
						{
							case 0:
//								puts ("case 0");
								if (j < 5)
								{
									barra [i] = linha [j];
									i++;
									if (j == 4)
									{
										barra [i] = '\0';
										i = 0;
										strcpy (vet_bar_de [ind_de].barra_de, barra);
									}
								}
								break;

							case 2:
//								puts ("case 2");
								if (x.rem < 5)
								{
									volt [i] = linha [j];
									i++;
									if (j == 24)
									{
										volt [i] = '\0';
										i = 0;
										vet_bar_de [ind_de].tensao = atof (volt);
									}
								}
								break;

							case 3:
//								puts ("case 3");
								if (x.rem > 2)
								{
									cgMW [i] = linha [j];
									i++;
								}
								break;

							case 4:
//								puts ("case 4");
								if (j == 40)
								{
									cgMW [i] = linha [j];
									i++;
									cgMW [i] = '\0';
									i = 0;
									vet_bar_de [ind_de].ger_MW = atof (cgMW);
								}
								else
								{
									cgMvar [i] = linha [j];
									i++;
									if (j == 49)
									{
										cgMvar [i] = '\0';
										i = 0;
										vet_bar_de [ind_de].ger_Mvar = atof (cgMvar);
									}
								}
								break;

							case 5:
//								puts ("case 5");
								if (j > 51)
								{
									ccMW [i] = linha [j];
									i++;
									if (j == 58)
									{
										ccMW [i] = '\0';
										i = 0;
										vet_bar_de [ind_de].carg_MW = atof (ccMW);
									}
								}
								break;

							case 6:
//								puts ("case 6");
								ccMvar [i] = linha [j];
								i++;
								if (j == 67)
								{
									ccMvar [i] = '\0';
									i = 0;
									vet_bar_de [ind_de].carg_Mvar = atof (ccMvar);
								}
								break;

							case 8:
//								puts ("case 8");
								if ((linha [4] == 0x20) && (j < 85))
								{
									barra1 [i] = linha [j];
									i++;
									if (j==84)
									{
										barra1 [i] = '\0';
										i = 0;
										vet_bar_de [ind_de].num_bar++;
										strcpy (vet_bar_de [ind_de].vet_bar_para [ind_para].barra_para, barra1);
									}
								}
								break;

							case 9:
//								puts ("case 9");
								if ((linha [4] == 0x20) && (j > 98))
								{
									cfluxoMW [i] = linha [j];
									i++;
								}
								break;

							case 10:
//								puts ("case 10");
								if (linha [4] == 0x20)
								{
									if (j < 108)
									{
										cfluxoMW [i] = linha [j];
										i++;
										if (j == 106)
										{
											cfluxoMW [i] = '\0';
											i = 0;
											vet_bar_de [ind_de].vet_bar_para [ind_para].fluxo_MW = atof (cfluxoMW);
										}
									}
									else
									{
										cfluxoMvar [i] = linha [j];
										i++;
									}
								}
								break;

							case 11:
//								puts ("case 11");
								if (linha [4] == 0x20)
								{
									cfluxoMvar [i] = linha [j];
									i++;
									if (j == 115)
									{
										cfluxoMvar [i] = '\0';
										i = 0;
										vet_bar_de [ind_de].vet_bar_para [ind_para].fluxo_Mvar = atof (cfluxoMvar);
									}
								}
								break;

							default:
								break;

						}
					}
				}
			if (linha [4] == 0x20)
				ind_para++;
			}
			}
		}
		else
		{
			for (j=0; j<8; j++)                     /* pula cabe‡alho */
				fgets (linha,140,arq);
		}
//	puts ("trabalhando");
	cont++;
	printf ("%d \n",cont);
	}
	while (strstr (linha,"SOLUTION TIME") == NULL);
	fclose (arq);
	gerarq ();
	puts ("gerarq");
}
/**********************************************************/

