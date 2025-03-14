#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <time.h>
#include <conio2.h>
struct my_tm 
{
    int tm_sec;   // Indica os segundos de 0 a 59
    int tm_min;   // Indica os minutos de 0 a 59
    int tm_hour;  // Indica as horas de 0 a 24
    int tm_mday;  // Indica os dias do mês de 1 a 31
    int tm_mon;   // Indica os meses do ano de 0 a 11
    int tm_year;  // Indica o ano a partir de 1900
    int tm_wday;  // Indica o dia da semana de 0 (domingo) até 6 (sábado)
    int tm_yday;  // Indica o dia do ano de 1 a 365
    int tm_isdst; // Indica o horário de verão se for diferente de zero
};
union TpDados
{
	float valorN;
	char valorD[10];
	char valorL;
	char valorC[50];
	char valorM[50];
 
};

struct pDados{
	union TpDados dados;
	struct pDados *prox;
};
typedef struct pDados pDados;
struct TpCampos{
	
	int dec;
	int width;
	char type;
	char fieldName[15];
	struct TpCampos *prox;
	pDados *pdados;
	pDados *pAtual;
};
typedef struct TpCampos TpCampos;
struct TpStatus{
	
	char status;
	struct TpStatus *prox;
};
typedef struct TpStatus TpStatus;
struct TpArquivo{
	
	struct TpArquivo *ant;
	struct TpArquivo *prox;
	char nomeDBF[25];
	char data[15];
	char hora[15];
	TpStatus *status;
	TpCampos *campos;
};
typedef struct TpArquivo TpArquivo;
struct TpUnidade{
	
	struct TpUnidade *top;
	struct TpUnidade *bottom;
	char und[3];
	TpArquivo *arqs; 
};
typedef struct TpUnidade TpUnidade;

TpCampos* CriaNoCampos(int dec, int width, char fieldName[], char type)
{
	TpCampos *NC;
	NC = (TpCampos*)malloc(sizeof(TpCampos));
	NC->dec = dec;
	NC->width = width;
	NC->type = type;
	strcpy(NC->fieldName, fieldName);
	NC->prox = NULL;
	NC->pdados = NULL;
	NC->pAtual = NULL;
	
	return NC;
	
}
TpStatus* CriaNoStatus(char status)
{
	TpStatus *NC;
	NC = (TpStatus*)malloc(sizeof(TpStatus));
	NC->prox = NULL;
	NC->status = status;
	
	return NC;
}
TpArquivo* CriaNoArquivo(char nome[], char data[], char hora[])
{
	TpArquivo *NC;
	NC = (TpArquivo*)malloc(sizeof(TpArquivo));
	strcpy(NC->data, data);
	strcpy(NC->nomeDBF, nome);
	strcpy(NC->hora, hora);
	NC->ant = NULL;
	NC->prox = NULL;
	NC->status = NULL;
	NC->campos = NULL;	
	
	return NC;
}
TpUnidade* CriaNoUnidade(char und[])
{
	TpUnidade *NC;
	NC = (TpUnidade*)malloc(sizeof(TpUnidade));
	NC->arqs = NULL;
	NC->bottom = NULL;
	NC->top = NULL;
	return NC;
}
void Moldura(int CI, int LI, int CF, int LF, int CorTxt, int CorFundo)
{
	int a;
	textcolor(CorTxt); // 0...15
	textbackground(CorFundo); // 0..7
	gotoxy(CI, LI); printf("%c", 201);
	gotoxy(CI, LF); printf("%c", 200);
	gotoxy(CF, LI); printf("%c", 187);
	gotoxy(CF, LF); printf("%c", 188);
	
	for(a=CI+1; a<CF; a++)
	{
		gotoxy(a, LI);	printf("%c", 205);
		gotoxy(a, LF);	printf("%c", 205);
	}
	
	for(a=LI+1; a<LF; a++)
	{
		gotoxy(CI, a);	printf("%c", 186);
		gotoxy(CF, a);	printf("%c", 186);
	}
	
	textcolor(7);
	textbackground(0);
}
void TelaCreate(TpUnidade *atual, TpArquivo *arq)
{
	Moldura(1,1, 120,30, 7, 7);
	gotoxy(40,3), printf("\033[1m====================================\033[0m");
	gotoxy(50,5), textcolor(7),printf ("\033[1;4mdBase Dinamico\033[0m");
	gotoxy(40,7), printf("\033[1m====================================\033[0m");
	gotoxy(6,10),printf ("Field Name");
	gotoxy(20,10), printf ("Type");
	gotoxy(34,10), printf("Width");
	gotoxy(46,10), printf ("Dec");
	gotoxy(6,12), printf ("===========================================");
	
	gotoxy(35,26), printf("ENTER - SAIR");
	gotoxy(6,27), textcolor(7),printf("-----------------------------------------");
	gotoxy(6,28), textcolor(7),printf ("CREATE\t\t||<%s>||%s", atual->und, arq->nomeDBF);
	gotoxy(6,29), textcolor(7),printf("-----------------------------------------");
}
void TelaDir(TpUnidade *atual)
{
	Moldura(1,1, 120,30, 7, 7);
	gotoxy(40,3), printf("\033[1m====================================\033[0m");
	gotoxy(50,5), textcolor(7),printf ("\033[1;4mdBase Dinamico\033[0m");
	gotoxy(40,7), printf("\033[1m====================================\033[0m");
	gotoxy(6,10),printf (". DIR");
	gotoxy(6,12), printf ("Database Files");
	gotoxy(24,12), printf("# Records");
	gotoxy(38,12), printf ("Last Update");
	
	gotoxy(35,26), printf("ENTER - SAIR");
	gotoxy(6,27), textcolor(7),printf("-----------------------------------------");
	gotoxy(6,28), textcolor(7),printf ("DIR\t\t||<%s>||", atual->und);
	gotoxy(6,29), textcolor(7),printf("-----------------------------------------");
}
void TelaListStructure(TpUnidade *atual,TpArquivo *arqs)
{
	Moldura(1,1, 120,30, 7, 7);
	gotoxy(40,3), printf("\033[1m====================================\033[0m");
	gotoxy(50,5), textcolor(7),printf ("\033[1;4mdBase Dinamico\033[0m");
	gotoxy(40,7), printf("\033[1m====================================\033[0m");
	gotoxy(6,8),printf (". LIST STRUCTURE");
	gotoxy(6,10), printf ("Structure for database: %s%s", atual->und, arqs->nomeDBF);
	if (arqs->status == NULL)
		gotoxy(6,12), textcolor(7),printf ("Number of data records: 0");
	else
	{
		TpStatus *status = arqs->status; 
		int cont = 0;
		while(status != NULL)
		{
			cont++;
			status = status->prox;
		}
		gotoxy(6,12), textcolor(7),printf ("Number of data records: %d", cont);
		
	}
	gotoxy(6,14), printf ("Date of Last Update: %s", arqs->data);
	gotoxy(6, 16), printf ("Field");
	gotoxy(22, 16), printf ("Field Name");
	gotoxy(36, 16), printf ("Type");
	gotoxy(50, 16), printf ("Width");
	gotoxy(64, 16), printf ("Dec");
	
	gotoxy(70,26), printf("ENTER - SAIR");
	gotoxy(6,27), textcolor(7),printf("-----------------------------------------------------------------------------");
	if (arqs->status == NULL)
		gotoxy(6,28), textcolor(7),printf ("LIST STRUCTURE\t\t||<%s>|| %s\t\t||Rec: None", atual->und, arqs->nomeDBF);
	else
	{
		TpStatus *status = arqs->status; 
		int cont = 0;
		while(status != NULL)
		{
			cont++;
			status = status->prox;
		}
		gotoxy(6,28), textcolor(7),printf ("LIST STRUCTURE\t\t||<%s>|| %s\t\t||Rec: %d", atual->und, arqs->nomeDBF, cont);
		
	}
	gotoxy(6,29), textcolor(7),printf("-----------------------------------------------------------------------------");
	
	
	
}
void TelaAppend(TpUnidade *L, TpArquivo *arqs)
{
	Moldura(1,1, 120,30, 7, 7);
	gotoxy(40,3), printf("\033[1m====================================\033[0m");
	gotoxy(50,5), textcolor(7),printf ("\033[1;4mdBase Dinamico\033[0m");
	gotoxy(40,7), printf("\033[1m====================================\033[0m");
	int i = 0;
	TpCampos *auxCampos;
	auxCampos = arqs->campos;
	while(auxCampos != NULL)
	{
		gotoxy(6, 10+i), printf ("%s", auxCampos->fieldName);
		
		auxCampos = auxCampos->prox;
		i++;
	}
	
	
	gotoxy(35,26), printf("ENTER - SAIR");
	gotoxy(6,27), textcolor(7),printf("-----------------------------------------");
	gotoxy(6,28), textcolor(7),printf ("APPEND\t\t||<%s>||%s", L->und, arqs->nomeDBF);
	gotoxy(6,29), textcolor(7),printf("-----------------------------------------");
}
void TelaList(TpUnidade *L, TpArquivo *arqs)
{
	Moldura(1,1, 120,30, 7, 7);
	gotoxy(40,3), printf("\033[1m====================================\033[0m");
	gotoxy(50,5), textcolor(7),printf ("\033[1;4mdBase Dinamico\033[0m");
	gotoxy(40,7), printf("\033[1m====================================\033[0m");
	gotoxy(6, 8), printf (". LIST");
	gotoxy(6,10), printf("Record#");
	int i = 0;
	TpCampos *auxCampos;
	auxCampos = arqs->campos;
	while(auxCampos != NULL)
	{
		gotoxy(20+i, 10), printf ("%s", auxCampos->fieldName);
		
		auxCampos = auxCampos->prox;
		i = i + 14;
	}
	
	
	gotoxy(59,26), printf("ENTER - SAIR");
	gotoxy(6,27), textcolor(7),printf("-----------------------------------------------------------------");
	if (arqs->campos == NULL)
		gotoxy(6,28), textcolor(7),printf ("LIST\t\t||<%s>||%s\t\t||Rec: None", L->und, arqs->nomeDBF);
	else
	{
		int cont = 0;
		TpStatus *status;
		status = arqs->status;
		while(status != NULL)
		{
			status = status->prox;
			cont++;
		}
		gotoxy(6,28), textcolor(7),printf ("LIST\t\t||<%s>||%s\t\t||Rec: EOF/%d", L->und, arqs->nomeDBF, cont);
	}
	gotoxy(6,29), textcolor(7),printf("-----------------------------------------------------------------");
	
}
void TelaListFor(TpUnidade *L, TpArquivo *arqs, char campo[], char registro[])
{
	Moldura(1,1, 120,30, 7, 7);
	gotoxy(40,3), printf("\033[1m====================================\033[0m");
	gotoxy(50,5), textcolor(7),printf ("\033[1;4mdBase Dinamico\033[0m");
	gotoxy(40,7), printf("\033[1m====================================\033[0m");
	gotoxy(6, 8), printf (". LIST FOR %s = \"%s\"", campo, registro);
	gotoxy(6,10), printf("Record#");
	int i = 0;
	TpCampos *auxCampos;
	auxCampos = arqs->campos;
	while(auxCampos != NULL)
	{
		gotoxy(20+i, 10), printf ("%s", auxCampos->fieldName);
		
		auxCampos = auxCampos->prox;
		i = i + 14;
	}
	
	
	gotoxy(59,26), printf("ENTER - SAIR");
	gotoxy(6,27), textcolor(7),printf("-----------------------------------------------------------------");
	if (arqs->campos == NULL)
		gotoxy(6,28), textcolor(7),printf ("LIST FOR\t\t||<%s>||%s\t\t||Rec: None", L->und, arqs->nomeDBF);
	else
	{
		int cont = 0;
		TpStatus *status;
		status = arqs->status;
		while(status != NULL)
		{
			status = status->prox;
			cont++;
		}
		gotoxy(6,28), textcolor(7),printf ("LIST FOR\t\t||<%s>||%s\t\t||Rec: EOF/%d", L->und, arqs->nomeDBF, cont);
	}
	gotoxy(6,29), textcolor(7),printf("-----------------------------------------------------------------");
	
	
	
}
void TelaLocate(TpUnidade *L, TpArquivo *arqs,char campo[], char registro[], int pos)
{
	Moldura(1,1, 120,30, 7, 7);
	gotoxy(40,3), printf("\033[1m====================================\033[0m");
	gotoxy(50,5), textcolor(7),printf ("\033[1;4mdBase Dinamico\033[0m");
	gotoxy(40,7), printf("\033[1m====================================\033[0m");
	gotoxy(6, 8), printf (". LOCATE FOR %s = \"%s\"", campo, registro);
	gotoxy(6,10), printf("Record =\t\t%d", pos);

	
	
	gotoxy(59,26), printf("ENTER - SAIR");
	gotoxy(6,27), textcolor(7),printf("-----------------------------------------------------------------");
	if (arqs->campos == NULL)
		gotoxy(6,28), textcolor(7),printf ("LOCATE FOR\t\t||<%s>||%s\t\t||Rec: None", L->und, arqs->nomeDBF);
	else
	{
		int cont = 0;
		TpStatus *status;
		status = arqs->status;
		while(status != NULL)
		{
			status = status->prox;
			cont++;
		}
		gotoxy(6,28), textcolor(7),printf ("LOCATE FOR\t\t||<%s>||%s\t\t||Rec: %d/%d", L->und, arqs->nomeDBF, pos,cont);
	}
	gotoxy(6,29), textcolor(7),printf("-----------------------------------------------------------------");
	
	
}
void TelaGoTo(TpUnidade *L, TpArquivo *arqs,int pos)
{
	Moldura(1,1, 120,30, 7, 7);
	gotoxy(40,3), printf("\033[1m====================================\033[0m");
	gotoxy(50,5), textcolor(7),printf ("\033[1;4mdBase Dinamico\033[0m");
	gotoxy(40,7), printf("\033[1m====================================\033[0m");
	gotoxy(6, 8), printf (". GOTO %d", pos);
	gotoxy(64,26), printf("ENTER - SAIR");
	gotoxy(6,27), textcolor(7),printf("----------------------------------------------------------------------");

	int cont = 0;
	TpStatus *status;
	status = arqs->status;
	while(status != NULL)
	{
		status = status->prox;
		cont++;
	}
	gotoxy(6,28), textcolor(7),printf ("GOTO\t\t||<%s>||%s\t\t||Rec: %d/%d", L->und, arqs->nomeDBF, pos,cont);
	gotoxy(6,29), textcolor(7), printf("----------------------------------------------------------------------");

}
void TelaDisplay(TpUnidade *L, TpArquivo *arqs, int pos)
{
	Moldura(1,1, 120,30, 7, 7);
	gotoxy(40,3), printf("\033[1m====================================\033[0m");
	gotoxy(50,5), textcolor(7),printf ("\033[1;4mdBase Dinamico\033[0m");
	gotoxy(40,7), printf("\033[1m====================================\033[0m");
	gotoxy(6, 8), printf (". DISPLAY");
	gotoxy(6,10), printf("Record#");
	int i = 0;
	TpCampos *auxCampos;
	auxCampos = arqs->campos;
	while(auxCampos != NULL)
	{
		gotoxy(20+i, 10), printf ("%s", auxCampos->fieldName);
		
		auxCampos = auxCampos->prox;
		i = i + 14;
	}
	
	
	gotoxy(59,26), printf("ENTER - SAIR");
	gotoxy(6,27), textcolor(7),printf("-----------------------------------------------------------------");
	if (arqs->campos == NULL)
		gotoxy(6,28), textcolor(7),printf ("DISPLAY\t\t||<%s>||%s\t\t||Rec: None", L->und, arqs->nomeDBF);
	else
	{
		int cont = 0;
		TpStatus *status;
		status = arqs->status;
		while(status != NULL)
		{
			status = status->prox;
			cont++;
		}
		gotoxy(6,28), textcolor(7),printf ("DISPLAY\t\t||<%s>||%s\t\t||Rec: %d/%d", L->und, arqs->nomeDBF, pos,cont);
	}
	gotoxy(6,29), textcolor(7),printf("-----------------------------------------------------------------");
}
void TelaEdit(TpUnidade *L, TpArquivo *arqs, int pos)
{
	Moldura(1,1, 120,30, 7, 7);
	gotoxy(40,3), printf("\033[1m====================================\033[0m");
	gotoxy(50,5), textcolor(7),printf ("\033[1;4mdBase Dinamico\033[0m");
	gotoxy(40,7), printf("\033[1m====================================\033[0m");
	int i = 0, j =0;
	TpCampos *auxCampos;
	auxCampos = arqs->campos;
	pDados *p;
	auxCampos = arqs->campos;
	while(auxCampos != NULL)
	{
		gotoxy(6, 10+j), printf ("%s", auxCampos->fieldName);
		p = auxCampos->pAtual;
		i = 0;
		while(p != NULL && i < pos - 1)
		{
			p = p->prox;
			i++;
		
		}
		if (p != NULL) // pos do registro
		{
			if (auxCampos->type == 'C')
				gotoxy(20,10+j),printf ("%s", p->dados.valorC);
			else
			if(auxCampos->type == 'D')
				gotoxy(20,10+j),printf ("%s", p->dados.valorD);
			else
			if(auxCampos->type == 'L')
				gotoxy(20,10+j),printf ("%c", p->dados.valorL);
			else
			if(auxCampos->type == 'M')
				gotoxy(20,10+j),printf ("%s", p->dados.valorM);
			else
			gotoxy(20,10+j),printf ("%.0f", p->dados.valorN);
			
			
		}
		j++;
		auxCampos = auxCampos->prox;
		
	}
	
	gotoxy(59,26), printf("ENTER - SAIR");
	gotoxy(6,27), textcolor(7),printf("-----------------------------------------------------------------");
	int cont = 0;
	TpStatus *status;
	status = arqs->status;
	while(status != NULL)
	{
		status = status->prox;
		cont++;
	}
	gotoxy(6,28), textcolor(7),printf ("EDIT\t\t||<%s>||%s\t\t||Rec: %d/%d", L->und, arqs->nomeDBF, pos,cont);
	gotoxy(6,29), textcolor(7),printf("-----------------------------------------------------------------");
	
}
void TelaDelete(TpUnidade *L, TpArquivo *arqs, int pos)
{
	Moldura(1,1, 120,30, 7, 7);
	gotoxy(40,3), printf("\033[1m====================================\033[0m");
	gotoxy(50,5), textcolor(7),printf ("\033[1;4mdBase Dinamico\033[0m");
	gotoxy(40,7), printf("\033[1m====================================\033[0m");
	gotoxy(6, 8), printf (". DELETE");
	gotoxy(12,9), printf ("1 record deleted");
	gotoxy(64,26), printf("ENTER - SAIR");
	gotoxy(6,27), textcolor(7),printf("----------------------------------------------------------------------");

	int cont = 0;
	TpStatus *status;
	status = arqs->status;
	while(status != NULL)
	{
		status = status->prox;
		cont++;
	}
	gotoxy(6,28), textcolor(7),printf ("DELETE\t\t||<%s>||%s\t\t||Rec: %d/%d", L->und, arqs->nomeDBF, pos,cont);
	gotoxy(6,29), textcolor(7), printf("----------------------------------------------------------------------");
}
void TelaDeleteALL(TpUnidade *L, TpArquivo *arqs, int deletados)
{
	Moldura(1,1, 120,30, 7, 7);
	gotoxy(40,3), printf("\033[1m====================================\033[0m");
	gotoxy(50,5), textcolor(7),printf ("\033[1;4mdBase Dinamico\033[0m");
	gotoxy(40,7), printf("\033[1m====================================\033[0m");
	gotoxy(6, 8), printf (". DELETE ALL");
	gotoxy(12,9), printf ("%d records deleted", deletados);
	gotoxy(64,26), printf("ENTER - SAIR");
	gotoxy(6,27), textcolor(7),printf("----------------------------------------------------------------------");

	int cont = 0;
	TpStatus *status;
	status = arqs->status;
	while(status != NULL)
	{
		status = status->prox;
		cont++;
	}
	gotoxy(6,28), textcolor(7),printf ("DELETE ALL\t\t||<%s>||%s\t\t||Rec: EOF/%d", L->und, arqs->nomeDBF, cont);
	gotoxy(6,29), textcolor(7), printf("----------------------------------------------------------------------");
}
void TelaRecall(TpUnidade *L, TpArquivo *arqs, int pos)
{
	Moldura(1,1, 120,30, 7, 7);
	gotoxy(40,3), printf("\033[1m====================================\033[0m");
	gotoxy(50,5), textcolor(7),printf ("\033[1;4mdBase Dinamico\033[0m");
	gotoxy(40,7), printf("\033[1m====================================\033[0m");
	gotoxy(6, 8), printf (". RECALL");
	gotoxy(12,9), printf ("1 records recalled");
	gotoxy(64,26), printf("ENTER - SAIR");
	gotoxy(6,27), textcolor(7),printf("----------------------------------------------------------------------");

	int cont = 0;
	TpStatus *status;
	status = arqs->status;
	while(status != NULL)
	{
		status = status->prox;
		cont++;
	}
	gotoxy(6,28), textcolor(7),printf ("RECALL\t\t||<%s>||%s\t\t||Rec: %d/%d", L->und, arqs->nomeDBF, pos,cont);
	gotoxy(6,29), textcolor(7), printf("----------------------------------------------------------------------");
	
	
	
}
void TelaRecallALL(TpUnidade *L, TpArquivo *arqs, int ativados)
{
	Moldura(1,1, 120,30, 7, 7);
	gotoxy(40,3), printf("\033[1m====================================\033[0m");
	gotoxy(50,5), textcolor(7),printf ("\033[1;4mdBase Dinamico\033[0m");
	gotoxy(40,7), printf("\033[1m====================================\033[0m");
	gotoxy(6, 8), printf (". RECALL ALL");
	gotoxy(12,9), printf ("%d records recalled", ativados);
	gotoxy(64,26), printf("ENTER - SAIR");
	gotoxy(6,27), textcolor(7),printf("----------------------------------------------------------------------");

	int cont = 0;
	TpStatus *status;
	status = arqs->status;
	while(status != NULL)
	{
		status = status->prox;
		cont++;
	}
	gotoxy(6,28), textcolor(7),printf ("RECALL ALL\t\t||<%s>||%s\t\t||Rec: EOF/%d", L->und, arqs->nomeDBF,cont);
	gotoxy(6,29), textcolor(7), printf("----------------------------------------------------------------------");
	
	
	
}
void alimentarUnidade(TpUnidade **L)
{
	TpUnidade *D, *C;
	
	D = (TpUnidade*)malloc(sizeof(TpUnidade));
	C = (TpUnidade*)malloc(sizeof(TpUnidade));
	
	D->arqs = NULL;
	strcpy(D->und, "D:");
	D->top = NULL;
	D->bottom = C;
	
	
	C->arqs = NULL;
	strcpy(C->und, "C:");
	C->top = D;
	C->bottom = NULL; 
	
	*L = D;
	
}
void setDefaultTo(TpUnidade *L, char unidade[], TpUnidade **atual, TpArquivo **arqs)
{
	
	if (strcmp(unidade, "D:") == 0)
	{
		if (strcmp(L->und, "D:") == 0)
		{
			*atual = L;
		}
		else
		{
			while(L != NULL && strcmp(L->und, "D:") != 0)
				L = L->bottom;
			if (L != NULL)
				*atual = L;
		}
		gotoxy(4,10),printf ("Unidade Alterada com Sucesso");
		if (*arqs != NULL) // caso apontando para um arquivo de uma unidade, caso eu mude, preciso trocar.  
		{
			TpArquivo *aux;
			aux = L->arqs;
			while(aux != NULL && strcmp(aux->nomeDBF, (*arqs)->nomeDBF))
				aux = aux->prox;
			if (aux != NULL) // encontrou o arquivo na unidade
				*arqs = aux;
			else
				*arqs = NULL;
			
		}
		
	}
	else
	if(strcmp(unidade,"C:") == 0)
	{
		if (strcmp(L->und, "C:") == 0)
		{
			*atual = L;
		}
		else
		{
			while(L != NULL && strcmp(L->und, "C:") != 0)
				L = L->bottom;
			if (L != NULL)
				*atual = L;
		}
		gotoxy(4,10),printf ("Unidade Alterada com Sucesso");
		if (*arqs != NULL) // caso esteja em um arquivo de uma unidade, caso eu mude, preciso trocar.  
		{
			TpArquivo *aux;
			aux = L->arqs;
			while(aux != NULL && strcmp(aux->nomeDBF, (*arqs)->nomeDBF))
				aux = aux->prox;
			if (aux != NULL) // encontrou o arquivo na unidade
				*arqs = aux;
			else
				*arqs = NULL;
			
		}
	}
	else
	{
		gotoxy(4,10), printf ("Unidade Invalida");
		*atual = NULL;
	}
	getch();
	
}
void create(TpUnidade **L, char nomeDBF[50])
{
	
	char fieldname[15], type;
	int width, dec;
	TpArquivo *NC, *aux, *ant;
	TpCampos *NCCampo, *auxCampos, *antCampos;
	aux = (*L)->arqs;
	while(aux != NULL && strcmp(aux->nomeDBF, nomeDBF) != 0)
		aux = aux->prox;
	if (aux == NULL)
	{
		struct tm *data;
		time_t seconds;
		time(&seconds);
		data = localtime(&seconds);
		
		char horario[15], dataFormatada[15];
		sprintf(horario, "%d:%d", data->tm_hour, data->tm_min);
		sprintf (dataFormatada, "%d/%d/%d", data->tm_mday, data->tm_mon + 1, data->tm_year + 1900);
	
		
		NC = CriaNoArquivo(nomeDBF, dataFormatada, horario);
		if ((*L)->arqs == NULL)
			{
				(*L)->arqs = NC;
			}
		else
		{
			aux = (*L)->arqs;
			while(aux != NULL) // final da unidade
			{
				ant = aux;
				aux = aux->prox;
			}
			ant->prox = NC;
			NC->ant = ant;
			
		}
		aux = (*L)->arqs;
		
		while(aux != NULL && strcmp(aux->nomeDBF, nomeDBF))
			aux = aux->prox;
		if (aux != NULL) // unidade correta do arquivo
		{
			int i = 0, j;
			clrscr();
			TelaCreate(*L, aux);
			
			gotoxy(4, 14 + i), printf("%d", i + 1);
			fflush(stdin);
			gotoxy(6, 14), gets(fieldname);
			while(strcmp(fieldname, "") != 0)
			{
				
				fflush(stdin);
				gotoxy(20, 14+i), scanf ("%c", &type);
				gotoxy(34, 14+i), scanf ("%d", &width);
				gotoxy(46, 14+i), scanf("%d", &dec);		
				
				j = 0;
				while(fieldname[j] != '\0')
				{
					fieldname[j] = toupper(fieldname[j]);	
					j++;
				}
				fieldname[j] = '\0';
				type = toupper(type);
				
				auxCampos = aux->campos;
				while(auxCampos != NULL && strcmp(auxCampos->fieldName, fieldname) != 0)
					auxCampos = auxCampos->prox;
				if (auxCampos == NULL)
				{
					
					NCCampo = CriaNoCampos(dec, width, fieldname, type);
					if (aux->campos == NULL)
					{
						aux->campos = NCCampo;
					}
					else
					{
						auxCampos = aux->campos;
						while(auxCampos != NULL) // insere no final
						{
							antCampos = auxCampos;
							auxCampos = auxCampos->prox;
							
						}
						antCampos->prox = NCCampo;
						
					}
					
					
				}
				
				i++;
				
				gotoxy(4, 14+ i), printf("%d", i + 1);
				fflush(stdin);
				gotoxy(6, 14+i), gets(fieldname);
			}
			
			
		}
		
	
	}
	else
	{
		gotoxy(4,10),printf ("Arquivo Ja Existente\n");
		getch();
	}
		
	

}
void dir(TpUnidade *L)
{

	clrscr();
	TelaDir(L);
	TpArquivo *arqs;
	TpStatus *status;
	int i = 0, cont= 0;
	arqs = L->arqs;
	while(arqs != NULL)
	{
		gotoxy(6, 14+i),printf ("%s", arqs->nomeDBF);
		gotoxy(38, 14+i), printf ("%s", arqs->data);
		
		if (arqs->status == NULL)
			gotoxy(24, 14+i),printf ("0");
		else
		{
			status = arqs->status; 
			while(status != NULL)
			{
				cont++;
				status = status->prox;
			}
			gotoxy(24,14+i),printf ("%d", cont);
		}
		arqs = arqs->prox;
		i++;
	}
	getch();
}
void use (TpUnidade *L, char arquivo[], TpArquivo **atual)
{

	TpArquivo *aux;
	aux = L->arqs;
	while(aux != NULL && strcmp(aux->nomeDBF, arquivo) != 0)
		aux = aux->prox;
	if (aux != NULL) // achou
		{
			*atual = aux;
			gotoxy(6,10), printf ("Arquivo Selecionado");
		}
		
	else
	{
		*atual = NULL;
		gotoxy(6,10), printf ("Arquivo Nao Encontrado");
	
	}
	getch();
	
	
	
}
void listStructure(TpUnidade *L,TpArquivo *arqs)
{
	TpCampos *campos;
	int i = 0, total = 0;
	clrscr();
	TelaListStructure(L, arqs);
	campos = arqs->campos;
	while(campos != NULL)
	{
		gotoxy(6, 18+i), printf ("%d", i+1);
		gotoxy(22, 18+i), printf ("%s", campos->fieldName);
		gotoxy(36, 18+i), printf ("%c", campos->type);
		gotoxy(50, 18+i), printf ("%d", campos->width);
		gotoxy(64, 18+i), printf ("%d", campos->dec);
		total = total + campos->width;
		i++;
		campos = campos->prox;
	}
	gotoxy(6, 18+i), printf("** Total **");
	gotoxy(50, 18+i), printf("%d", total);
	getch();
}	
void append(TpUnidade *L, TpArquivo **arqs)
{
	clrscr();
	TelaAppend(L, *arqs);
	TpArquivo *auxArquivo;
	TpCampos *auxCampos, *auxCampos2;
	TpStatus *status, *NCStatus;
	union TpDados dados;
	pDados *NCDados, *ant, *p;
	char character[50], logical;
	float numerical;
	int i = 0, k;
	auxCampos = (*arqs)->campos;

	while(auxCampos != NULL)
	{
	
		if (auxCampos->type == 'C')	// character	
		{
			fflush(stdin);
			gotoxy(16, 10+i), gets(character);
			k = 0;
			while(character[k] != '\0')
			{
				character[k] = toupper(character[k]);	
				k++;
			}
			character[k] = '\0';
			strcpy(dados.valorC, character);
			NCDados = (pDados*)malloc(sizeof(pDados));
			NCDados->dados = dados;
			NCDados->prox = NULL;
			if(auxCampos->pAtual == NULL)
			{
				auxCampos->pdados = NCDados;
				auxCampos->pAtual = NCDados;
			}	
			else
			{
				p = auxCampos->pAtual;
				while(p != NULL)
				{
					ant = p;
					p = p->prox;
				}
					
				ant->prox = NCDados;
			}	
		}
		else // numerical
		if (auxCampos->type == 'N')
		{
			gotoxy(16, 10+i), scanf("%f", &numerical);
			
			dados.valorN = numerical;
			NCDados = (pDados*)malloc(sizeof(pDados));
			NCDados->dados = dados;
			NCDados->prox = NULL;
			if(auxCampos->pAtual == NULL)
			{
				auxCampos->pAtual = NCDados;
				auxCampos->pdados = NCDados;
			}
			else
			{
				p = auxCampos->pAtual;
				while(p != NULL)
				{
					ant = p;
					p = p->prox;
				}
					
				ant->prox = NCDados;
			}	
		}
		else
		if(auxCampos->type == 'M') //memo;
		{
			
			fflush(stdin);
			gotoxy(16, 10+i), gets(character);
			k = 0;
			while(character[k] != '\0')
			{
				character[k] = toupper(character[k]);	
				k++;
			}
			character[k] = '\0';
			strcpy(dados.valorM, character);
			NCDados = (pDados*)malloc(sizeof(pDados));
			NCDados->dados = dados;
			NCDados->prox = NULL;
			if(auxCampos->pAtual == NULL)
			{
				auxCampos->pAtual = NCDados;
				auxCampos->pdados = NCDados;
			}
			else
			{
				p = auxCampos->pAtual;
				while(p != NULL)
				{
					ant = p;
					p = p->prox;
				}
					
				ant->prox = NCDados;
			}	
		}
		else
		if(auxCampos->type == 'D') // date
		{
			fflush(stdin);
			gotoxy(16, 10+i), gets(character);
			k = 0;
			while(character[k] != '\0')
			{
				character[k] = toupper(character[k]);	
				k++;
			}
			character[k] = '\0';
			strcpy(dados.valorD, character);
			NCDados = (pDados*)malloc(sizeof(pDados));
			NCDados->dados = dados;
			NCDados->prox = NULL;
			if(auxCampos->pAtual == NULL)
			{
				auxCampos->pAtual = NCDados;
				auxCampos->pdados = NCDados;
			}
			else
			{
				p = auxCampos->pAtual;
				while(p != NULL)
				{
					ant = p;
					p = p->prox;
				}
					
				ant->prox = NCDados;
			}	
		}
		else // logical
		{
			fflush(stdin);
			gotoxy(16, 10+i), scanf("%c", &logical);
			logical = toupper(logical);
			dados.valorL = logical; 
			NCDados = (pDados*)malloc(sizeof(pDados));
			NCDados->dados = dados;
			NCDados->prox = NULL;
			if(auxCampos->pAtual == NULL)
			{
				auxCampos->pAtual = NCDados;
				auxCampos->pdados = NCDados;
			}
			else
			{
				p = auxCampos->pAtual;
				while(p != NULL)
				{
					ant = p;
					p = p->prox;
				}
					
				ant->prox = NCDados;
			}	
		}
		
		
		i++;
		auxCampos = auxCampos->prox;
	}
	
	NCStatus = CriaNoStatus(1);
	if((*arqs)->status == NULL)
		 (*arqs)->status = NCStatus;
	else
	{
		status = (*arqs)->status;
		while(status->prox != NULL) // inserir no final
			status = status->prox;
		status->prox = NCStatus;
	}
	
	
}
void list(TpUnidade *L, TpArquivo *arqs, char set)
{
	clrscr();
	TelaList(L, arqs);
	TpCampos *campos;
	TpStatus *status;
	pDados *p;
	campos = arqs->campos;
	status = arqs->status;
	int i = 0, j = 0, k = 0, pos = 0;
	while(status != NULL)
	{
		if (status->status == 1 || set == 0) // true
		{
			campos = arqs->campos;
			while(campos != NULL)
			{
				p = campos->pAtual;
				pos = 0; 
				while(p != NULL && pos < k)
				{
					p = p->prox;
					pos++;
				}
				if (p != NULL) // pos correta
				{
					
					gotoxy(6, 11+i), printf("%d", k+1); //pos do registro
					if (campos->type == 'C')
						gotoxy(20+j,11+i),printf ("%s", p->dados.valorC);
					else
					if(campos->type == 'D')
						gotoxy(20+j,11+i),printf ("%s", p->dados.valorD);
					else
					if(campos->type == 'L')
						gotoxy(20+j,11+i),printf ("%c", p->dados.valorL);
					else
					if(campos->type == 'M')
						gotoxy(20+j,11+i),printf ("%s", p->dados.valorM);
					else
						gotoxy(20+j,11+i),printf ("%.0f", p->dados.valorN);
						
				
				}
				j = j + 14;
				campos = campos->prox;
			}
		
			i++; // pos vertical
						
		}
		status = status->prox;	
		j=0;
		k++; // pos do registro

	}
		
		
		
	
	getch();
}
 void listfor(TpUnidade *L, TpArquivo *arqs, char campo[], char registro[], char set)
{

	TpCampos *auxCampos, *campos;
	TpStatus *status;
	pDados *p, *pAux;
	auxCampos = arqs->campos;
	int pos = 0, i = 0, j = 0, k = 0, m = 0,n=0; // pos do reg
	char flag = 0, aux[50];
	while(auxCampos != NULL && strcmp(auxCampos->fieldName, campo) != 0)
		auxCampos = auxCampos->prox;
	if (auxCampos != NULL) // achou
	{
		clrscr();
		TelaListFor(L, arqs, campo, registro);
		status = arqs->status;
		p = auxCampos->pAtual;
		while(status != NULL && p != NULL)
		{
			if (status->status == 1 || set == 0)
			{
				
				if(auxCampos->type == 'C')
				{
					
					flag = 1;
					m = 0;
					while(registro[m] != '\0')
					{
						if(p->dados.valorC[m] != registro[m])
							flag = 0;
						m++;
					}
					
				}
				else
				if(auxCampos->type == 'D')
				{
					flag = 1;
					m = 0;
					while(registro[m] != '\0')
					{
						if(p->dados.valorD[m] != registro[m])
							flag = 0;
						m++;
					}
					
				}
				else
				if(auxCampos->type == 'L')
				{
					char c = registro[0];
					if (p->dados.valorL == c)
						flag = 1;
				}
				else
				if(auxCampos->type == 'M')
				{
					flag = 1;
					m = 0;
					while(registro[m] != '\0')
					{
						if(p->dados.valorM[m] != registro[m])
							flag = 0;
						m++;
					}
					
				}
				else
				{
					char aux[15]; 
					sprintf(aux, "%.0f", p->dados.valorN); // float para string
					flag = 1;
					m = 0;
					while(registro[m] != '\0')
					{
						if(aux[m] != registro[m])
							flag = 0;
						m++;
					}
				}
				if (flag == 1)
				{
					
					campos = arqs->campos;
					while(campos != NULL)
					{
						pAux = campos->pAtual;
						pos = 0; 
						while(pAux != NULL && pos < k)
						{
							pAux = pAux->prox;
							pos++;
						}
						if (pAux != NULL) // pos correta
						{
							
							gotoxy(6, 11+i), printf("%d", k+1); //pos do registro
							if (campos->type == 'C')
								gotoxy(20+j,11+i),printf ("%s", pAux->dados.valorC);
							else
							if(campos->type == 'D')
								gotoxy(20+j,11+i),printf ("%s", pAux->dados.valorD);
							else
							if(campos->type == 'L')
								gotoxy(20+j,11+i),printf ("%c", pAux->dados.valorL);
							else
							if(campos->type == 'M')
								gotoxy(20+j,11+i),printf ("%s", pAux->dados.valorM);
							else
								gotoxy(20+j,11+i),printf ("%.0f", pAux->dados.valorN);
								
						
						}
						j = j + 14;
						campos = campos->prox;
							
							
							
					}
					i++;// pos vertical
					flag = 0;
				}
				
				
			}
			k++;
			j = 0;
			status = status->prox;
			p = p->prox;
			
		}
		getch();
	}
	else
	{
		gotoxy(6,10),printf ("Campo Informado Nao Encontrado");
		getch();
	}
	
}
void locate(TpUnidade *L, TpArquivo *arqs, char campo[], char registro[], char set)
{
	TpCampos *campos;
	TpStatus *status;
	pDados *p;
	campos = arqs->campos;
	int pos, k = 0;
	char flag = 0;
	while(campos != NULL && strcmp(campos->fieldName, campo) != 0)
		campos = campos->prox;
	if (campos != NULL) // achou
	{
		p = campos->pAtual;
		status = arqs->status;
		while(status != NULL && flag != 1)
		{
			
			if (status->status == 1 || set == 0)
			{
				
				if(campos->type == 'C')
				{
					if(strcmp(p->dados.valorC, registro) == 0)
						flag = 1;
					
				}
				else
				if(campos->type == 'D')
				{
					if(strcmp(p->dados.valorD, registro) == 0)
						flag = 1;
				}
				else
				if(campos->type == 'L')
				{
					char c = registro[0];
					if(p->dados.valorL == c)
						flag = 1;
				}
				else
				if(campos->type == 'M')
				{
					if(strcmp(p->dados.valorM, registro) == 0)
						flag = 1;
				}
				else
				{
					float valor = atof(registro);
					if(valor == p->dados.valorN)
						flag = 1;
				}
				
			
				
			}
			k++;
			p = p->prox;
			status = status->prox;
		}
		if (flag == 1)
		{
			clrscr();
			TelaLocate(L, arqs, campo, registro,k);
			getch();
		}
		else
		{
			gotoxy(6,10), printf("Registro Informado Nao Encontrado");
			getch();
		}
		
		
		
		
	}	
	else
	{
		gotoxy(6,10),printf("Campo Informado Nao Encontrado"); 
		getch();
	}


}
void goTo(TpUnidade *L, TpArquivo *arqs, char aux[], int *reg)
{
	int pos, i;
	pos = atoi(aux);
	TpStatus *status;
	pDados *p;
	char flag = 0;
	i = 0;
	status = arqs->status;
	while(status != NULL && i < pos - 1)
	{
		i++;
		status = status->prox;
	}
	if (status != NULL)
	{
		*reg = pos;
		clrscr();
		TelaGoTo(L, arqs, *reg);
		getch();
	}
	else
	{
		*reg = 0;
		gotoxy(6,10), printf("Registro Informado Nao Encontrado");
		getch();
	}
}
void display(TpUnidade *L,TpArquivo *arqs, int pos, char set)
{
	
	TpStatus *status;
	TpCampos *campos;
	pDados *p;
	int i =0, j = 0;
	status = arqs->status;
	while(status != NULL && i < pos - 1)
	{
		i++;
		status = status->prox;
		
	}
	if (status != NULL && (status->status == 1 || set == 0))
	{
		clrscr();
		TelaDisplay(L, arqs, pos);
		campos = arqs->campos;
		gotoxy(6, 11), printf("%d", pos); //pos do registro
		while(campos != NULL)
		{
			i = 0;
			p = campos->pAtual;
			while(p != NULL && i < pos-1)
			{
				i++;
				p = p->prox;
			}	
			if (p != NULL)
			{
				if (campos->type == 'C')
					gotoxy(20+j,11),printf ("%s", p->dados.valorC);
				else
				if(campos->type == 'D')
					gotoxy(20+j,11),printf ("%s", p->dados.valorD);
				else
				if(campos->type == 'L')
					gotoxy(20+j,11),printf ("%c", p->dados.valorL);
				else
				if(campos->type == 'M')
					gotoxy(20+j,11),printf ("%s", p->dados.valorM);
				else
				gotoxy(20+j,11),printf ("%.0f", p->dados.valorN);
			}
			campos = campos->prox;
			j = j + 14;
		}
			
		
	}
	else
		gotoxy(6,10), printf("Registro Desativado");

	getch();
	
}
void edit(TpUnidade *L, TpArquivo *arqs, int reg)
{
	
	TpCampos *campos;
	TpStatus *status;
	pDados *p;
	union TpDados dados;
	char character[50], logical;
	float numerical;
	int i, j = 0, k;
	campos = arqs->campos;
	status = arqs->status;
	i = 0;
	while(status != NULL && i < reg - 1)
	{
		status = status->prox;
		i++;
	}
	if (status != NULL && status->status == 1)
	{
		clrscr();
		TelaEdit(L, arqs, reg);
		while(campos != NULL)
		{
			p = campos->pAtual;
			i = 0;
			while(p != NULL && i < reg - 1)
			{
				p = p->prox;
				i++;
			
			}
			if (p != NULL) // pos do registro
			{
				
				if (campos->type == 'C')
				{
					fflush(stdin);
					gotoxy(34,10+j),gets(character);
					
					
					if (strcmp(character, "") != 0)
					{
						k = 0;
						while(character[k] != '\0')
						{
							character[k] = toupper(character[k]);	
							k++;
						}
						strcpy(p->dados.valorC, character);
					}
					
				}	
				else
				if(campos->type == 'D')
				{
					fflush(stdin);
					gotoxy(34,10+j),gets(character);
					if (strcmp(character, "") != 0)
					{
						k = 0;
						while(character[k] != '\0')
						{
							character[k] = toupper(character[k]);	
							k++;
						}
						strcpy(p->dados.valorD, character);
					}
				}
				else
				if(campos->type == 'L')
				{
					fflush(stdin);
					gotoxy(34,10+j),gets(character);
					if (strcmp(character, "") != 0)
					{
						logical = toupper(character[0]);
						p->dados.valorL = logical;
					}
				}
				else
				if(campos->type == 'M')
				{
					fflush(stdin);
					gotoxy(34,10+j),gets(character);
					if (strcmp(character, "") != 0)
					{
						k = 0;
						while(character[k] != '\0')
						{
							character[k] = toupper(character[k]);	
							k++;
						}
						strcpy(p->dados.valorM, character);
					}
				}
				else
				{
					
					gotoxy(34, 10+j), gets(character);
					if (strcmp(character, "") != 0)
					{
						numerical = atof(character);
						p->dados.valorN = numerical;
					}
				}
				
				
			}
			j++;
			campos = campos->prox;
			
		}
	}
	else
	{
		gotoxy(6,10), printf("Registro Desativado");
		getch();
	}	

}
void deleteR(TpUnidade *L, TpArquivo *arqs, int *pos)
{
	
	TpStatus *status;
	int i = 0;
	status = arqs->status;
	while(status != NULL && i < *pos - 1)
	{
		status = status->prox;
		i++;
	}
	if (status != NULL && status->status == 1)
	{
		clrscr();
		TelaDelete(L, arqs, *pos);
		status->status = 0;
		*pos = 0;
		
	}
	else
	{
		gotoxy(6,10), printf ("Registro Ja Desativado");
	}
	getch();
	
}
void deleteALL(TpUnidade *L, TpArquivo *arqs, int *pos)
{
	
	TpStatus *status;
	status = arqs->status;
	int deletados = 0;
	while(status != NULL)
	{
		if (status->status == 1)
		{
			status->status = 0;
			deletados++;
		}
		
		status = status->prox;
		
	}
	*pos = 0;
	clrscr();
	TelaDeleteALL(L, arqs, deletados);
	getch();
	
}
void recall(TpUnidade *L, TpArquivo *arqs, int pos)
{
	
	TpStatus *status;
	int i = 0;
	status = arqs->status;
	while(status != NULL && i < pos - 1)
	{
		status = status->prox;
		i++;
	}
	if (status != NULL && status->status == 0)
	{
		clrscr();
		TelaRecall(L, arqs, pos);
		status->status = 1;
		
	}
	else
	{
		gotoxy(6,10), printf ("Registro Ja Ativado");
	}
	getch();
	
	
}
void recallALL(TpUnidade *L, TpArquivo *arqs, int *pos)
{
	
	TpStatus *status;
	status = arqs->status;
	int ativados = 0;
	while(status != NULL)
	{
		if (status->status == 0)
		{
			status->status = 1;
			ativados++;
		}
		
		status = status->prox;
		
	}
	*pos = 0;
	clrscr();
	TelaRecallALL(L, arqs, ativados);
	getch();
	
}
void interpretarString(char frase[50], TpUnidade **Lista, TpUnidade **atual, char *op, TpArquivo **arqs, int *reg, char *set)
{
	int i=0 , j = 0;
	char aux[15], flag, campo[15], registro[15];
	while(frase[i] != '\0')
	{
		frase[i] = toupper(frase[i]);	
		i++;
	}
	frase[i] = '\0';
	i = 0;
	while(frase[i] != '\0' && flag != 1)
	{
	
		aux[j] = frase[i];
		aux[j+1] = '\0';
		if (strcmp(aux, "SET DEFAULT TO") == 0)
		{
			i++;
			if (frase[i] != '\0' && frase[i] == ' ')
			{
				aux[0] = '\0';
				j=0;
				i++;
				while(frase[i] != '\0')
				{
					aux[j] = frase[i];
					j++;
					i++;
				
					
				}
				aux[j] = '\0';
				setDefaultTo(*Lista, aux, atual, arqs);
				flag = 1;
			}
			else
			{
				flag = 0;
	
			}
			
		}
		else
		if(strcmp(aux, "CREATE") == 0)
		{
			i++;
			if(frase[i] != '\0' && frase[i] == ' ')
			{
				aux[0] = '\0';
				j=0;
				i++;
				if (frase[i] != '.')
				{
					
					while(frase[i] != '\0' && frase[i] != '.')
					{
	
						aux[j] = frase[i];
						i++;
						j++;	
											
					}
					if (frase[i] != '\0' && frase[i-1] != ' ') 
					{
						aux[j] = frase[i];
						i++;
						j++;
						if(frase[i] == 'D')
						{
							aux[j] = frase[i];
							i++;
							j++;
							if(frase[i] == 'B')
							{
								aux[j] = frase[i];
								i++;
								j++;
								if (frase[i] == 'F' && frase[i+1] == '\0')
								{
									aux[j] = frase[i];
									i++;
									j++;
									flag = 1;
								}
							}
						}
						
						
						
					}
					else
						flag = 0;
					
					if (flag == 1)
					{
						aux[j] = '\0';
						create(atual, aux);
					}
						
				}
				
				
			}
			else
			{
				flag = 0;
			}
			
			
		}
		else
		if(strcmp(aux, "QUIT") == 0) //quit
		{
			i++;
			if (frase[i] == '\0')
			{
				*op = 27;
				flag = 1; 
			}
		}
		else
		if(strcmp(aux, "DIR") == 0)
		{
			i++;
			if(frase[i] == '\0')
			{
				if ((*atual)->arqs != NULL)
					dir(*atual);
				else
					{
						gotoxy(6,10), printf ("Unidade Vazia");
						getch();
					}
					
				flag = 1;
			}
		}
		else
		if(strcmp(aux, "USE") == 0)
		{
			
				i++;
				if (frase[i] != '\0' && frase[i] == ' ')
				{
					aux[0] = '\0';
					j=0;
					i++;
					while(frase[i] != '\0' && frase[i] != '.')
					{
	
						aux[j] = frase[i];
						i++;
						j++;	
											
					}
					if (frase[i] != '\0') 
					{
						aux[j] = frase[i];
						i++;
						j++;
						if(frase[i] == 'D')
						{
							aux[j] = frase[i];
							i++;
							j++;
							if(frase[i] == 'B')
							{
								aux[j] = frase[i];
								i++;
								j++;
								if (frase[i] == 'F' && frase[i+1] == '\0')
								{
									aux[j] = frase[i];
									i++;
									j++;
									flag = 1;
									
								}
							}
						}
						
						
						
					}
					
					if (flag == 1)
					{
						aux[j] = '\0';
						use(*atual, aux, arqs);
						*reg = 0;
					}
					
				}
				else
				{
					flag = 0;
				}
		
			
			
				
				
		}
		else
		if(strcmp(aux, "LIST STRUCTURE") == 0)
		{
		
			i++;
			if(frase[i] == '\0')
			{
				if (*arqs != NULL)
					listStructure(*atual,*arqs);
				else
					{
						gotoxy(6,10), printf ("Nenhum Arquivo Selecionado");
						getch();
					}
					
				flag = 1;
				
			}
			
		}
		else
		if(strcmp(aux, "APPEND") == 0)
		{
			i++;
			if (frase[i] == '\0')
			{
				if (*arqs != NULL && (*arqs)->campos != NULL)
					append(*atual,arqs);
				else
				{
					gotoxy(6,10), printf ("Nenhum Arquivo Selecionado e/ou Nenhum Campo Inserido");
					getch();
				}
			
				flag = 1;	
				
			}
			
		
		}
		else
		if(strcmp(aux, "LIST") == 0 && frase[i+1] == '\0')
		{
			
			if (*arqs != NULL)
				list(*atual,*arqs, *set);
			else
				{
					gotoxy(6,10), printf ("Nenhum Arquivo Selecionado");
					getch();
				}
				
			flag = 1;
			
		}
		else
		if(strcmp(aux, "LIST FOR") == 0)
		{
			i++;
			if (frase[i] == ' ' && frase[i] != '\0')
			{
				i++;
				j = 0;
				while(frase[i] != ' ' && frase[i] != '\0')
				{
					campo[j] = frase[i];
					j++;
					i++;
				}
				if (frase[i] != '\0')
				{
					campo[j] = '\0'; 
					i++;
					if (frase[i] == '=')
					{
						i = i + 2;
						if(frase[i] == '"')
						{
							i++;
							j=0;
							while(frase[i] != '"' && frase[i] != '\0')
							{
								registro[j] = frase[i];
								j++;
								i++;
								
							}
							if (frase[i] != '\0')
							{
								registro[j] = '\0';
								flag = 1;
								if (strcmp(registro, "") != 0 && strcmp(campo, "") != 0)
								{
									if (*arqs != NULL) 
										listfor(*atual, *arqs, campo, registro, *set);
									else
									{
										gotoxy(6,10), printf ("Nenhum Arquivo Selecionado");
										getch();
									}
								}
								else
								{
									gotoxy(6,10), printf ("Campo e/ou Registro Invalido");
									getch();
								}
								
								
							}
						}
					
					}
				}
				
				
			}
			
		}
		else
		if(strcmp(aux, "LOCATE FOR") == 0)
		{
		
			i++;
			if (frase[i] == ' ' && frase[i] != '\0')
			{
				i++;
				j = 0;
				while(frase[i] != ' ' && frase[i] != '\0')
				{
					campo[j] = frase[i];
					j++;
					i++;
				}
				if (frase[i] != '\0')
				{
					campo[j] = '\0'; 
					i++;
					if (frase[i] == '=')
					{
						i = i + 2;
						if(frase[i] == '"')
						{
							i++;
							j=0;
							while(frase[i] != '"' && frase[i] != '\0')
							{
								registro[j] = frase[i];
								j++;
								i++;
								
							}
							if (frase[i] != '\0')
							{
								registro[j] = '\0';
								flag = 1;
								if (strcmp(registro, "") != 0 && strcmp(campo, "") != 0)
								{
									if (*arqs != NULL) 
										locate(*atual, *arqs, campo, registro, *set);
									else
									{
										gotoxy(6,10), printf ("Nenhum Arquivo Selecionado");
										getch();
									}
								}
								else
								{
									gotoxy(6,10), printf ("Campo e/ou Registro Invalido");
									getch();
								}
								
								
							}
						}
						
					}
				}
				
				
			}
		}
		else
		if(strcmp(aux, "GOTO") == 0)
		{
			i++;
			if (frase[i] == ' ' && frase[i] != '\0')
			{
				i++;
				j = 0;
				aux[0] = '\0';
				while(frase[i] != '\0' && frase[i] != ' ')
				{
					aux[j] = frase[i];
					j++;
					i++;
				}
				if (frase[i] != ' ')
				{
					flag = 1;
					if (*arqs != NULL)
					{
						aux[j] = '\0';
						goTo(*atual, *arqs, aux, reg);
					}
					else
					{
						gotoxy(6,10), printf ("Nenhum Arquivo Selecionado");
						getch();
					}
					
				}
				
				
			}
		}
		else
		if(strcmp(aux, "DISPLAY") == 0)
		{
			i++;
			if (frase[i] == '\0')
			{
				if (*reg != 0)
				display(*atual,*arqs, *reg, *set);
				else
					{
						gotoxy(6,10), printf ("Nenhum Registro Selecionado e/ou Arquivo Selecionado");
						getch();
					}
					
				flag = 1;
			}
			
		}
		else
		if(strcmp(aux, "EDIT") == 0)
		{
			i++;
			if (frase[i] == '\0')
			{
				if (*reg != 0)
					edit(*atual,*arqs, *reg);
				else
					{
						gotoxy(6,10), printf ("Nenhum Registro Selecionado e/ou Arquivo Selecionado");
						getch();
					}
					
				flag = 1;
			}
			
		}
		else
		if(strcmp(aux, "DELETE") == 0 && frase[i+1] == '\0')
		{
			i++;
			if (frase[i] == '\0')
			{
				if (*reg != 0)
					deleteR(*atual,*arqs, reg);
				else
					{
						gotoxy(6,10), printf ("Nenhum Registro Selecionado e/ou Arquivo Selecionado");
						getch();
					}
					
				flag = 1;
			}
			
			
		}
		else
		if(strcmp(aux, "DELETE ALL") == 0)
		{
			i++;
			if (frase[i] == '\0')
			{
				if (*arqs != NULL)
					deleteALL(*atual,*arqs, reg);
				else
					{
						gotoxy(6,10), printf ("Nenhum Arquivo Selecionado");
						getch();
					}
					
				flag = 1;
			}
		}
		else
		if(strcmp(aux, "RECALL") == 0 && frase[i + 1] == '\0')
		{
			i++;
			if (frase[i] == '\0')
			{
				if (*reg != 0)
					recall(*atual,*arqs, *reg);
				else
					{
						gotoxy(6,10), printf ("Nenhum Registro Selecionado e/ou Arquivo Selecionado");
						getch();
					}
					
				flag = 1;
			}
			
		}
		else
		if(strcmp(aux, "RECALL ALL") == 0)
		{
			i++;
			if (frase[i] == '\0')
			{
				if (*set == 0 && (*arqs) != NULL)
					recallALL(*atual,*arqs, reg);
				else
					{
						gotoxy(6,10), printf ("Illegal Command");
						getch();
					}
					
				flag = 1;
			}
			
			
		}
		else
		if(strcmp(aux, "SET DELETED OFF") == 0)
		{
			i++;
			if (frase[i] == '\0')
			{
				if(*set != 0)
				{
					gotoxy(6,10), printf("Desativado");
					*set = 0;
				}
				else
				{
					gotoxy(6,10), printf("Ja Desativado");
				}
				getch();
				flag = 1;
			}
			
		}
		else
		if(strcmp(aux,"SET DELETED ON") == 0)
		{
			i++;
			if (frase[i] == '\0')
			{
				if(*set != 1)
				{
					gotoxy(6,10), printf("Ativado");
					*set = 1;
				}
				else
				{
					gotoxy(6,10), printf("Ja Ativado");
					
				}
				getch();
				flag = 1;
			}
			
		}
		else
		{
		 	flag =0;
		}
		
		
		j++;
		i++;
	
	}
	
	if (flag == 0)
	{
		gotoxy(6,10),printf ("Comando Invalido");
		getch();
	}

}

void executar(void)
{
	char op, frase[50], set = 1;
	int reg = 0; 
	TpArquivo *arqs = NULL;
	TpUnidade *Lista, *atual;
	Lista = NULL;
	alimentarUnidade(&Lista);
	atual = Lista;
	do
	{	
		clrscr();
		Moldura(1,1, 120,30, 7, 7);
		
		//Moldura(45, 4, 67, 6, 15, 7);
		gotoxy(40,3), printf("\033[1m====================================\033[0m");
		gotoxy(50,5), textcolor(7),printf ("\033[1;4mdBase Dinamico\033[0m");
		gotoxy(40,7), printf("\033[1m====================================\033[0m");
		if (atual == NULL)
		{
			atual = Lista;
		}
		gotoxy(6,27), textcolor(7), printf("----------------------------------------------");
			gotoxy(6,27), textcolor(7), printf("----------------------------------------------------------------------");
		if (arqs != NULL)
		{
			
			if(arqs->status != NULL)
			{
				int cont = 0;
				TpStatus *status;
				status = arqs->status;
				while(status != NULL)
				{
		
					cont++;
					status = status->prox;
				}
				gotoxy(6,28), textcolor(7),printf ("Command Line\t\t||<%s>||%s\t\t||Rec: %d/%d", atual->und, arqs->nomeDBF, reg,cont);
			}
			else
			{
				gotoxy(6,28), textcolor(7),printf ("Command Line\t\t||<%s>||%s\t\t||Rec: None", atual->und, arqs->nomeDBF);
			}
		}
		else
			gotoxy(6,28), textcolor(7),printf ("Command Line\t\t||<%s>||NULL\t\t||Rec: None", atual->und);
		gotoxy(6,29), textcolor(7), printf("----------------------------------------------------------------------");
		fflush(stdin);
		gotoxy(6,26),gets(frase);
		interpretarString(frase, &Lista, &atual, &op, &arqs, &reg, &set);
	
	}while(op != 27);
}

int main(void)
{

	executar();
	return 0;
}
