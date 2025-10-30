void descompac (FILE *arq)
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
				fread (&registro,3,1,arq);
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