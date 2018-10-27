/* ==========================================================================
 * Universidade Federal de São Carlos - Campus Sorocaba
 * Disciplina: Estruturas de Dados 2
 * Prof. Tiago A. de Almeida
 *
 * Trabalho 01
 *
 * RA: 552585
 * Aluno: João Vitor de Sá Medeiros Santos
 * ========================================================================== */

/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* Tamanho dos campos dos registros */
#define TAM_PRIMARY_KEY	11
#define TAM_NOME 		51
#define TAM_MARCA 		51
#define TAM_DATA 		11
#define TAM_ANO 		3
#define TAM_PRECO 		8
#define TAM_DESCONTO 	4
#define TAM_CATEGORIA 	51


#define TAM_REGISTRO 	192
#define MAX_REGISTROS 	1000
#define MAX_CATEGORIAS 	30
#define TAM_ARQUIVO (MAX_REGISTROS * TAM_REGISTRO + 1)

#define NULL_RRN -1

/* Saídas para o usuario */
#define OPCAO_INVALIDA 				"Opcao invalida!\n"
#define MEMORIA_INSUFICIENTE 		"Memoria insuficiente!"
#define REGISTRO_N_ENCONTRADO 		"Registro(s) nao encontrado!\n"
#define CAMPO_INVALIDO 				"Campo invalido! Informe novamente.\n"
#define ERRO_PK_REPETIDA			"ERRO: Ja existe um registro com a chave primaria: %s.\n"
#define ARQUIVO_VAZIO 				"Arquivo vazio!"
#define INICIO_BUSCA 		 		"**********************BUSCAR**********************\n"
#define INICIO_LISTAGEM  			"**********************LISTAR**********************\n"
#define INICIO_ALTERACAO 			"**********************ALTERAR*********************\n"
#define INICIO_EXCLUSAO  			"**********************EXCLUIR*********************\n"
#define INICIO_ARQUIVO  			"**********************ARQUIVO*********************\n"
#define INICIO_ARQUIVO_SECUNDARIO	"*****************ARQUIVO SECUNDARIO****************\n"
#define SUCESSO  				 	"OPERACAO REALIZADA COM SUCESSO!\n"
#define FALHA 					 	"FALHA AO REALIZAR OPERACAO!\n"



/* Registro do Produto */
typedef struct {
	char pk[TAM_PRIMARY_KEY];
	char nome[TAM_NOME];
	char marca[TAM_MARCA];
	char data[TAM_DATA];	/* DD/MM/AAAA */
	char ano[TAM_ANO];
	char preco[TAM_PRECO];
	char desconto[TAM_DESCONTO];
	char categoria[TAM_CATEGORIA];
} Produto;


/*----- Registros dos Índices -----*/

/* Struct para índice Primário */
typedef struct primary_index{
  char pk[TAM_PRIMARY_KEY];
  int rrn;
} Ip;

/* Struct para índice secundário */
typedef struct secundary_index{
  char pk[TAM_PRIMARY_KEY];
  char string[TAM_NOME];
} Is;

/* Struct para índice secundário de preços */
typedef struct secundary_index_of_final_price{
  float price;
  char pk[TAM_PRIMARY_KEY];
} Isf;

/* Lista ligada para o Índice abaixo*/
typedef struct linked_list{
  char pk[TAM_PRIMARY_KEY];
  struct linked_list *prox;
} ll;

/* Struct para lista invertida */
typedef struct reverse_index{
  char cat[TAM_CATEGORIA];
  ll* lista;
} Ir;

/*----- GLOBAL -----*/
char ARQUIVO[TAM_ARQUIVO];

/* ==========================================================================
 * ========================= PROTÓTIPOS DAS FUNÇÕES =========================
 * ========================================================================== */

// ############################## BUILT-IN ##################################	
/* Recebe do usuário uma string simulando o arquivo completo e retorna o número
 * de registros. */
int carregar_arquivo();

/* Exibe o Produto */
int exibir_registro(int rrn, char com_desconto);

/* Recupera do arquivo o registro com o rrn informado
 *  e retorna os dados na struct Produto */
Produto recuperar_registro(int rrn);

/* (Re)faz o índice respectivo */
void criar_iprimary(Ip *indice_primario, int* nregistros);

/* Realiza os scanfs na struct Produto */
void ler_entrada(char* registro, Produto *novo);

/* Rotina para impressao de indice secundario */
void imprimirSecundario(Is* iproduct, Is* ibrand, Ir* icategory, Isf *iprice, int nregistros, int ncat);

// ############################## ADDED ##################################
void insert_indexes(Produto *new_product, int nregistros, int* ncat, Ip *indice_primario, Ir *icategory, Is *iproduct, Is *ibrand, Isf *iprice);

void insert_iprimary(Ip *indice_primario, Produto* novo, int nregistros);

void sort_iprimary(Ip* indice_primario, int nregistros);

void set_icategory(Ir *icategory, int nregistros, int *ncat);

void insert_icategory(Ir *icategory, Produto* novo, int* ncat);

void sort_icategory(Ir* icategory, int ncat);

Ir* has_category(Ir* icategory, const char* category, int ncat);

void ll_append_ordered(Ir* curr_cat, const char* pk);

int cmpr_input_bycategory(const void* a, const void* b);

int cmpr_bycategory(const void* a, const void* b);

void set_iproduct(Is *iproduct, int nregistros);

void insert_iproduct(Is *iproduct, Produto *novo, int nregistros);

void sort_iproduct(Is* iproduct, int nregistros);

void set_ibrand(Is *ibrand, int nregistros);

void insert_ibrand(Is  *ibrand, Produto *novo, int nregistros);

void sort_ibrand(Is* ibrand, int nregistros);

void set_iprice(Isf *iprice, int nregistros);

void insert_iprice(Isf  *iprice, Produto *novo, int nregistros);

void sort_iprice(Isf* iprice, int nregistros);

int cmpr_byprice(const void *a, const void *b);

void imprimirDados(int nregistros);

int read_product(Produto* new_product, Ip* indice_primario, int nregistros);

void key_gen(Produto *new);

int append_file(Produto *new, char* FILE);

int recuperar_rrn(Ip* iprimary, const char* pk, int nregistros);

int cmpr_bypk(const void *a, const void *b);

int cmpr_string(const void *a, const void *b);

int cmpr_input_bypk(const void *a, const void *b);

int cmpr_input_string(const void *a, const void *b);

int search_bypk(Ip* iprimary, int nregistros);

int search_byname(Ip* iprimary, Is* iproduct, int nregistros);

void printall_iprice(Ip* iprimary, Isf* iprice, int nregistros);

void printall_brand(Ip* iprimary, Is* ibrand, int nregistros);

void printall_primary(Ip* iprimary, int nregistros);

void printall_category(Ip* iprimary,Ir* icategory, int ncat, int nregistros);

int search_bybrand_and_category(Ip* iprimary, Is* ibrand, int nregistros);

int has_category_inregister(Produto j, char* category);

int alterar(Ip* iprimary, Isf* iprice, int nregistros);

void update_iprice(Isf* iprice, Produto updated, int nregistros);

void overwrite_data(Produto new, int rrn);

int is_desc_pattern(char* desc);

int remover(Ip* iprimary, int nregistros);

int is_tombstone(Produto* p);

void liberar(int* nregistros, int *ncat, Ip* iprimary, Ir* icategory, Is* iproduct, Is* ibrand, Isf* iprice);

void clear_char(char* c, int len);

/* ==========================================================================
 * ============================ FUNÇÃO PRINCIPAL ============================
 * =============================== NÃO ALTERAR ============================== */
int main(){
  /* Arquivo */
	Produto *new_product;
	int carregarArquivo = 0, nregistros = 0, ncat = 0;
	scanf("%d%*c", &carregarArquivo); /* 1 (sim) | 0 (nao) */
	if (carregarArquivo)
		nregistros = carregar_arquivo();

	/* Índice primário */
	Ip *iprimary = (Ip *) malloc (MAX_REGISTROS * sizeof(Ip));
  	if (!iprimary) {
		perror(MEMORIA_INSUFICIENTE);
		exit(1);
	}
	criar_iprimary(iprimary, &nregistros);

	/*Alocar e criar índices secundários*/
	//TODO: verificar corretude das alocações
	//No momento, tudo NULL
	Is *iproduct = (Is *) malloc(MAX_REGISTROS * sizeof(Is)); 
	Is *ibrand = (Is *) malloc(MAX_REGISTROS * sizeof(Is)); 
	Isf *iprice = (Isf *) malloc(MAX_REGISTROS * sizeof(Isf));
	//setar lista ligada
	Ir *icategory = (Ir *) malloc(MAX_CATEGORIAS * sizeof(Ir));

	//provavelmente mais um parametro sera adicionado: nregistros
	set_iproduct(iproduct, nregistros);
	set_ibrand(ibrand, nregistros);
	set_iprice(iprice, nregistros);
	set_icategory(icategory, nregistros, &ncat);

	/* Execução do programa */
	int opcao = 0;
	while(1)
	{
		scanf("%d%*c", &opcao);
		switch(opcao)
		{
			case 1:
				/*cadastro*/
				new_product = (Produto *) malloc(sizeof(Produto));
				// if(nregistros == MAX_REGISTROS){
				// 	printf(MEMORIA_INSUFICIENTE);
				// }
				// else 
				if(read_product(new_product, iprimary, nregistros)){
					insert_indexes(new_product, nregistros, &ncat, iprimary, icategory, iproduct, ibrand, iprice);
					nregistros++;
				}
				free(new_product);

			break;
			case 2:
				/*alterar desconto*/
				printf(INICIO_ALTERACAO);
				
				if(alterar(iprimary, iprice, nregistros))
					printf(SUCESSO);
				else
					printf(FALHA);
				
			break;
			case 3:
				/*excluir produto*/
				printf(INICIO_EXCLUSAO);
				
				if(remover(iprimary, nregistros))
					printf(SUCESSO);
				else
					printf(FALHA);
				
			break;
			case 4:
				/*busca*/
				printf(INICIO_BUSCA );
				scanf("%d%*c", &opcao);
				
				switch(opcao){
					//1 - codigo
					//2 - nome do produto
					//3 - marca + categoria
					case 1:
						search_bypk(iprimary, nregistros);						
						break;
					case 2:
						search_byname(iprimary, iproduct, nregistros);
						break;
					case 3:
						search_bybrand_and_category(iprimary, ibrand, nregistros);
						break;
					default:

						break;

				}

			break;
			case 5:
				/*listagens*/
				printf(INICIO_LISTAGEM);
				scanf("%d%*c", &opcao);
				
				switch(opcao){
					//1 - codigo
					//2 - categoria
					//3 - marca
					//4 - preco c/ desc

					case 1:
						printall_primary(iprimary, nregistros);						
						break;

					case 2:
						printall_category(iprimary, icategory, ncat, nregistros);
						break;

					case 3:
						printall_brand(iprimary, ibrand, nregistros);
						break;

					case 4:
						printall_iprice(iprimary, iprice, nregistros);
						break;

					default:

						break;

				}

			break;

			case 6:
				liberar(&nregistros, &ncat, iprimary, icategory, iproduct, ibrand, iprice);
			break;

			case 7:
				/*imprime o arquivo de dados*/
				imprimirDados(nregistros);				
			break;

			case 8:
				/*imprime os índices secundários*/
				imprimirSecundario(iproduct, ibrand, icategory, iprice, nregistros, ncat);
			break;

			case 9:
	      		/*Liberar memória e finalizar o programa */
				return 0;
			break;

			default:
				printf(OPCAO_INVALIDA);
			break;
		}
	}
	return 0;
}


/* Exibe o Produto */
int exibir_registro(int rrn, char com_desconto)
{
	if(rrn<0)
		return 0;
	float preco;
	int desconto;
	Produto j = recuperar_registro(rrn);
  	char *cat, categorias[TAM_CATEGORIA];
	printf("%s\n", j.pk);
	printf("%s\n", j.nome);
	printf("%s\n", j.marca);
	printf("%s\n", j.data);
	if(!com_desconto)
	{
		printf("%s\n", j.preco);
		printf("%s\n", j.desconto);
	}
	else
	{
		sscanf(j.desconto,"%d",&desconto);
		sscanf(j.preco,"%f",&preco);
		preco = preco *  (100-desconto);
		preco = ((int) preco)/ (float) 100 ;
		printf("%07.2f\n",  preco);

	}
	strcpy(categorias, j.categoria);

	cat = strtok (categorias, "|");

	while(cat != NULL){
		printf("%s", cat);
		cat = strtok (NULL, "|");
		if(cat != NULL){
			printf(", ");
		}
	}

	printf("\n");

	return 1;
}


int carregar_arquivo()
{
	scanf("%[^\n]\n", ARQUIVO);
	return strlen(ARQUIVO) / TAM_REGISTRO;
}


/* Recupera do arquivo o registro com o rrn
 * informado e retorna os dados na struct Produto */
Produto recuperar_registro(int rrn)
{
	char temp[193], *p;
	strncpy(temp, ARQUIVO + ((rrn)*192), 192);
	temp[192] = '\0';
	Produto j;
	p = strtok(temp,"@");
	strcpy(j.nome,p);
	p = strtok(NULL,"@");
	strcpy(j.marca,p);
	p = strtok(NULL,"@");
	strcpy(j.data,p);
	p = strtok(NULL,"@");
	strcpy(j.ano,p);
	p = strtok(NULL,"@");
	strcpy(j.preco,p);
	p = strtok(NULL,"@");
	strcpy(j.desconto,p);
	p = strtok(NULL,"@");
	strcpy(j.categoria,p);
	key_gen(&j);
	return j;
}


/* Imprimir indices secundarios */
void imprimirSecundario(Is* iproduct, Is* ibrand, Ir* icategory, Isf *iprice, int nregistros, int ncat){
	int opPrint = 0;
	ll *aux;
	printf(INICIO_ARQUIVO_SECUNDARIO);
	scanf("%d", &opPrint);
	if(!nregistros)
		printf(ARQUIVO_VAZIO);
	switch (opPrint) {
		case 1:
			for(int i = 0; i < nregistros; i++){
				printf("%s %s\n",iproduct[i].pk, iproduct[i].string);
			}
		break;
		case 2:
			for(int i = 0; i < nregistros; i++){
				printf("%s %s\n",ibrand[i].pk, ibrand[i].string);
			}
		break;
		case 3:
			for(int i = 0; i < ncat; i++){
				printf("%s", icategory[i].cat);
				aux =  icategory[i].lista;
				while(aux != NULL){
					printf(" %s", aux->pk);
					aux = aux->prox;
				}
				printf("\n");
			}
		break;

		case 4:
		for(int i = 0; i < nregistros; i++){
			printf("%s %.2f\n",iprice[i].pk, iprice[i].price);
		}
		break;
	}
}

/* ==========================================================================
 * ======================= IMPLEMENTAÇÕES DAS FUNÇÕES =======================
 * ========================================================================== */


// ############################## ÍNDICES ##################################	

void insert_indexes(Produto *new_product, int nregistros, int* ncat, Ip *indice_primario, Ir *icategory, Is *iproduct, Is *ibrand, Isf *iprice){
	//Controla inserção em todos arquivos de índice
	insert_iprimary(indice_primario, new_product, nregistros);
	insert_iproduct(iproduct, new_product, nregistros);
	insert_ibrand(ibrand, new_product, nregistros);
	insert_iprice(iprice, new_product, nregistros);
	insert_icategory(icategory, new_product, ncat);
}

void criar_iprimary(Ip *indice_primario, int* nregistros){
	int i, j;
	Ip *atual = indice_primario;

	//rotina de inicialização
	for( i = 0; i < MAX_REGISTROS; i++){
		//limpa PK
		for(j = 0; j < TAM_PRIMARY_KEY; j++)
			*((atual+i)->pk + j) = '\0';
		//limpa rrn
		(atual+i)->rrn = NULL_RRN; //convencionado
	}
	
	*nregistros  = 0;

	return;
}

/*insere novo regitro no indice primario*/
void insert_iprimary(Ip *indice_primario, Produto* novo, int nregistros){
	// typedef struct primary_index{
	//   char pk[TAM_PRIMARY_KEY];
	//   int rrn;
	// } Ip;

	//ajusta o ponteiro para a posição vazia do indice
	Ip* eof = indice_primario + (nregistros);

	strncpy(eof->pk, novo->pk, strlen(novo->pk));
	eof->rrn = nregistros;
	sort_iprimary(indice_primario, nregistros+1);
}


/*organiza indice primario*/
void sort_iprimary(Ip* indice_primario, int nregistros){
	//void qsort(void *base, size_t nel, size_t width, int (*compar)(const void *, const void *));
	qsort(indice_primario, nregistros, sizeof(Ip), cmpr_bypk);

	// int c, d;
	// Ip swap;
	// for (c = 0 ; c < ( nregistros - 1 ); c++){
	//     for (d = 0 ; d < nregistros - c - 1; d++){
	// 		if (strcmp((indice_primario+d)->pk,(indice_primario+d+1)->pk) > 0 ){
	// 		swap = *(indice_primario+d);
	// 			*(indice_primario+d) = *(indice_primario+d+1);
	// 		*(indice_primario+d) = swap;
	// 		}
	//     }
 //  	}
}

void set_icategory(Ir *icategory, int nregistros, int *ncat){
	/* Struct para lista invertida */
	// typedef struct reverse_index{
	//   char cat[TAM_CATEGORIA];
	//   ll* lista;
	// } Ir;

	Ir *atual = icategory;

	if(!nregistros){
		*ncat = 0;
		//rotina de inicialização
		for(int i = 0; i < MAX_CATEGORIAS; i++){
			//limpa categoria
			for(int j = 0; j < TAM_CATEGORIA; j++)
				*((atual+i)->cat + j) = '\0';
			//limpa string
			(atual+i)->lista = NULL;
		}
	} else {
		//populate by FILE
	}
}

void insert_icategory(Ir *icategory, Produto* novo, int* ncat){
	//insert new category if doesnt exists

	/*	
		pos = search
		if(search == null){
			insert
			sort
			pos = search
		}
		append(pos)
	*/	

	Ir* pos;
	char* cat; /*purr*/
	cat = strtok (novo->categoria, "|");
	while(cat != NULL){
		pos = has_category(icategory, cat, *ncat);
		if(pos == NULL){
	 		//maximum category reached
		 	if(*ncat == MAX_CATEGORIAS)
			 	return;
			//otherwise insert
			strncpy((icategory+*ncat)->cat, cat, strlen(cat));
			*ncat = *ncat + 1;
			sort_icategory(icategory, *ncat);
			pos = has_category(icategory, cat, *ncat);


		}
		//append linked list on "pos"
		ll_append_ordered(pos, novo->pk);
		//tries to retrieve more categories	from strtok buffer
		cat = strtok (NULL, "|");
	}
}	

/*qsorts category array*/
void sort_icategory(Ir* icategory, int ncat){
	//quicksort 
	qsort(icategory, ncat, sizeof(Ir), cmpr_bycategory);
}

void set_iproduct(Is *iproduct, int nregistros){
	int i, j;
	Is *atual = iproduct;

	if(!nregistros){
		//rotina de inicialização
		for( i = 0; i < MAX_REGISTROS; i++){
			//limpa PK
			for(j = 0; j < TAM_PRIMARY_KEY; j++)
				*((atual+i)->pk + j) = '\0';
			//limpa string
			for(j = 0; j < TAM_NOME; j++)
				*((atual+i)->string + j) = '\0'; 
		}
	} else {

	}
	return;
}


/*Insere ao final do indice secundario*/
void insert_iproduct(Is  *iproduct, Produto *novo, int nregistros){
	int i;
	Is* eof = iproduct + nregistros;
	strncpy(eof->pk, novo->pk, strlen(novo->pk));
	strncpy(eof->string, novo->nome, strlen(novo->nome));
	sort_iproduct(iproduct, nregistros+1); //risky +1
}
 
void sort_iproduct(Is* iproduct, int nregistros){
	//quicksort 
	qsort(iproduct, nregistros, sizeof(Is), cmpr_bypk);
}

void set_ibrand(Is *ibrand, int nregistros){
	int i, j;
	Is *atual = ibrand;

	if(!nregistros){
		//rotina de inicialização
		for( i = 0; i < MAX_REGISTROS; i++){
			//limpa PK
			for(j = 0; j < TAM_PRIMARY_KEY; j++)
				*((atual+i)->pk + j) = '\0';
			//limpa string
			for(j = 0; j < TAM_NOME; j++)
				*((atual+i)->string + j) = '\0'; 
		}
	} else {
		//rbuild from file
	}
	return;
}

/*Insere ao final do indice secundario*/
void insert_ibrand(Is  *ibrand, Produto *novo, int nregistros){
	Is* eof = ibrand + nregistros;
	//check repeated brands?
	strncpy(eof->pk, novo->pk, strlen(novo->pk));
	strncpy(eof->string, novo->marca, strlen(novo->marca));
	sort_ibrand(ibrand, nregistros+1); //risky +1
}

void sort_ibrand(Is* ibrand, int nregistros){
	//quicksort 
	qsort(ibrand, nregistros, sizeof(Is), cmpr_string);

}

void set_iprice(Isf *iprice, int nregistros){
	int i, j;
	Isf *atual = iprice;

	if(!nregistros){
		//rotina de inicialização
		for( i = 0; i < MAX_REGISTROS; i++){
			//limpa PK
			for(j = 0; j < TAM_PRIMARY_KEY; j++)
				*((atual+i)->pk + j) = '\0';
			(atual+i)->price = 0.0;
		}
	} else {

	}
}

/*Insere ao final do indice secundario*/
void insert_iprice(Isf  *iprice, Produto *novo, int nregistros){
	float preco;
//	char spreco[TAM_PRECO];
	int desconto;
	Isf* eof = iprice + nregistros;
	
	// for (int i = 0; i < TAM_PRECO; i++)
	// {
	// 	spreco[i] = '\0';
	// }

	sscanf(novo->desconto,"%d",&desconto);
	sscanf(novo->preco,"%f",&preco);
	preco = (preco *  (100-desconto))/100.0;
	preco = preco * 100;
	preco = ((int) preco)/ (float) 100 ;
	// sprintf(spreco, "%07.2f", preco);

	// sscanf(spreco,"%f",eof->price);

	eof->price = preco;

	strncpy(eof->pk, novo->pk, strlen(novo->pk));
	sort_iprice(iprice, nregistros+1); //risky +1
}

void update_iprice(Isf* iprice, Produto updated, int nregistros){
	Isf* idx;
	idx = iprice;
	float preco;
	int desconto;
	//since iprice is price ordered, is more expensive to sort by pk
	//bsearch the pk and then order by price again
	//so we run through whole index
	for(int i = 0 ; i < nregistros; i++){
		if (strcmp(updated.pk, (idx+i)->pk) == 0)
		{
			sscanf(updated.desconto,"%d",&desconto);
			sscanf(updated.preco,"%f",&preco);
			preco = (preco *  (100-desconto))/100.0;
			preco = preco * 100;
			preco = ((int) preco)/ (float) 100 ;
			(idx+i)->price = preco;

			sort_iprice(iprice, nregistros);
		}
	}
}

void sort_iprice(Isf* iprice, int nregistros){
	//quicksort 
	qsort(iprice, nregistros, sizeof(Isf), cmpr_byprice);
}

/*Dada uma certa chave, retorna o rrn dela consultando o ip*/
int recuperar_rrn(Ip* iprimary, const char* pk, int nregistros){
	Ip* idx_encontrado;
	idx_encontrado = bsearch(pk, iprimary, nregistros, sizeof(Ip), cmpr_input_bypk);
	if(idx_encontrado == NULL)
		return NULL_RRN;
	return idx_encontrado->rrn;
}

// ############################## OUTPUTS ##################################	
void imprimirDados(int nregistros){
	if(!nregistros)
		printf(ARQUIVO_VAZIO);
	else{
		printf(INICIO_ARQUIVO);
		printf("%s\n", ARQUIVO);	
	}
}

void printall_iprice(Ip* iprimary, Isf* iprice, int nregistros){
	int rrn;
	for(int i =0; i < nregistros; i++){
		//printf("nrrn %d\n", (iprimary+i)->rrn);
		rrn = recuperar_rrn(iprimary,(iprice+i)->pk, nregistros);
		exibir_registro(rrn, 1);
		
		if(i != nregistros-1 && rrn != NULL_RRN){
			printf("\n");
		}
	} 
}

void printall_category(Ip* iprimary,Ir* icategory, int ncat, int nregistros){
	ll* aux;
	char categoria[TAM_CATEGORIA];
	int num_keys = 0;
	int rrn_vector[MAX_REGISTROS];
	int rrn;

	//clear categoria
	for(int i = 0; i< TAM_CATEGORIA; i++){
		categoria[i] = '\0';
	}

	scanf("%[^\n]\n", categoria);

	for(int i = 0;i < ncat; i++){
		if(strcmp((icategory+i)->cat, categoria) == 0 ){
			aux =  (icategory+i)->lista;
			while(aux != NULL){
				rrn = recuperar_rrn(iprimary, aux->pk, nregistros);
				exibir_registro(rrn ,0);
				aux = aux->prox;
				if(aux != NULL && rrn != NULL_RRN)
					printf("\n");
			}
			return ;
		}
	}

	printf(REGISTRO_N_ENCONTRADO);

}

void printall_primary(Ip* iprimary, int nregistros){
	for(int i =0; i < nregistros; i++){
		//printf("nrrn %d\n", (iprimary+i)->rrn);
		exibir_registro((iprimary+i)->rrn, 0);
		
		if(i != nregistros-1 && (iprimary+i)->rrn != NULL_RRN){
			printf("\n");
		}
	}
}

void printall_brand(Ip* iprimary, Is* ibrand, int nregistros){
	int rrn;
	for(int i =0; i < nregistros; i++){
		//printf("nrrn %d\n", (iprimary+i)->rrn);
		rrn = recuperar_rrn(iprimary, (ibrand+i)->pk, nregistros);
		exibir_registro(rrn, 0);
		
		if(i != nregistros-1 && rrn != NULL_RRN){
			printf("\n");
		}
	}
}

// ############################### INPUTS ##################################	

int read_product(Produto* new_product, Ip* indice_primario, int nregistros){

//TODO: fazer função de gerar PK
//TODO: validações
//TODO: gravar no ARQUIVO
// 	/* Registro do Produto */
// typedef struct {
// 	char pk[TAM_PRIMARY_KEY];
// 	char nome[TAM_NOME];
// 	char marca[TAM_MARCA];
// 	char data[TAM_DATA];	/* DD/MM/AAAA */
// 	char ano[TAM_ANO];
// 	char preco[TAM_PRECO];
// 	char desconto[TAM_DESCONTO];
// 	char categoria[TAM_CATEGORIA];
// } Produto;

	
	int rtn = 0;
	scanf("%[^\n]\n", new_product->nome);
	scanf("%[^\n]\n", new_product->marca);
	scanf("%[^\n]\n", new_product->data);
	scanf("%[^\n]\n", new_product->ano);
	scanf("%[^\n]\n", new_product->preco);
	scanf("%[^\n]\n", new_product->desconto);
	scanf("%[^\n]\n", new_product->categoria);

	key_gen(new_product);
	
	if(recuperar_rrn(indice_primario, new_product->pk, nregistros) != NULL_RRN){
		printf(ERRO_PK_REPETIDA, new_product->pk);
		return 0;
	}
	rtn = append_file(new_product, ARQUIVO);
	return rtn;

} 

// ############################### CONTROLS ##################################	
void liberar(int* nregistros, int *ncat, Ip* iprimary, Ir* icategory, Is* iproduct, Is* ibrand, Isf* iprice){
	Produto p;
	int novo_nregistros = 0, novo_ncat = 0;
	char ARQUIVO_2_TURNO[TAM_ARQUIVO];
	
	free(iprimary);
	free(icategory);
	free(iproduct);
	free(ibrand);
	free(iprice);

	iprimary = (Ip *) malloc (MAX_REGISTROS * sizeof(Ip));
  	if (!iprimary) {
		perror(MEMORIA_INSUFICIENTE);
		exit(1);
	}
	criar_iprimary(iprimary, &novo_nregistros);
	/*Alocar e criar índices secundários*/ 

	iproduct = (Is *) malloc(MAX_REGISTROS * sizeof(Is)); 
	ibrand = (Is *) malloc(MAX_REGISTROS * sizeof(Is)); 
	iprice = (Isf *) malloc(MAX_REGISTROS * sizeof(Isf));
	icategory = (Ir *) malloc(MAX_CATEGORIAS * sizeof(Ir));

	set_iproduct(iproduct, novo_nregistros);
	set_ibrand(ibrand, novo_nregistros);
	set_iprice(iprice, novo_nregistros);
	set_icategory(icategory, novo_nregistros, &novo_ncat);


	for (int i = 0; i < *nregistros; i++){
		p = recuperar_registro(i);
		if(!is_tombstone(&p)){
			append_file(&p, ARQUIVO_2_TURNO);
			insert_indexes(&p, novo_nregistros, &novo_ncat, iprimary, icategory, iproduct, ibrand, iprice);
			novo_nregistros++;	
		}
	}

	clear_char(ARQUIVO, strlen(ARQUIVO));
	strncpy(ARQUIVO, ARQUIVO_2_TURNO, strlen(ARQUIVO_2_TURNO));
	*nregistros = novo_nregistros;
	*ncat = novo_ncat;
	ARQUIVO_2_TURNO[0] = '\0';
}
void clear_char(char* c, int len){
	for (int i = 0; i < len; ++i)
	{
		*(c+i) = '\0';
	}
}

int is_tombstone(Produto* p){
	if(*(p->nome) == '*' && *(p->nome+1) == '|')
		return 1;
	return 0;
}


int remover(Ip* iprimary, int nregistros){
	char pk[TAM_PRIMARY_KEY];
	int rrn;
	Produto p;
	Ip* idx_encontrado;

	scanf("%[^\n]\n", pk);
	rrn = recuperar_rrn(iprimary, pk, nregistros);
	if(rrn == NULL_RRN){
		printf(REGISTRO_N_ENCONTRADO);
		return 0;
	}
	p = recuperar_registro(rrn);
	*(p.nome) = '*';
	*(p.nome+1) = '|';
	overwrite_data(p, rrn);
	
	idx_encontrado = bsearch(pk, iprimary, nregistros, sizeof(Ip), cmpr_input_bypk);
	idx_encontrado->rrn = NULL_RRN;
	return 1;
}

int alterar(Ip* iprimary, Isf* iprice, int nregistros){
	char pk[TAM_PRIMARY_KEY], desc[TAM_DESCONTO];
	int rrn;
	Produto p;

	scanf("%[^\n]\n", pk);
	rrn = recuperar_rrn(iprimary, pk, nregistros);
	if( rrn == NULL_RRN){
		printf(REGISTRO_N_ENCONTRADO);
		return 0;
	}
	
	scanf("%[^\n]\n", desc);
	if(!is_desc_pattern(desc)){
		printf(CAMPO_INVALIDO);
		return 0;
	
	}else{
		p = recuperar_registro(rrn);
		strncpy(p.desconto, desc, strlen(desc));
		overwrite_data(p, rrn);
		/*update iprice*/
		update_iprice(iprice, p, nregistros);
		return 1;
	}
}

void ll_append_ordered(Ir* curr_cat, const char* pk){
	/*
			typedef struct linked_list{
		  char pk[TAM_PRIMARY_KEY];
		  struct linked_list *prox;
		} ll;
	*/
	ll *curr_node, *new;

	//instance of ll
	new = (ll*) malloc(sizeof(ll));
	strncpy(new->pk, pk, strlen(pk));
	new->prox = NULL;

	//if empty inserts OR inserts on first pos if less
	//<does not bug when NULL->pk since if NULL, just accepts the 1st condition
	if(curr_cat->lista == NULL || strcmp((curr_cat->lista)->pk, pk) > 0){
			new->prox = curr_cat->lista;
			curr_cat->lista = new;	
	} else {
		curr_node = curr_cat->lista;
		while(curr_node->prox != NULL && 
			strcmp((curr_node->prox)->pk, pk) < 0){
			curr_node = curr_node->prox;
		}
		//inserrts after
		new->prox = curr_node->prox;
		curr_node->prox = new;
	}
}

Ir* has_category(Ir* icategory, const char* category, int ncat){

	return bsearch(category, icategory, ncat, sizeof(Ir), cmpr_input_bycategory);	
}

/*usa bsearch para encontrar nome do produto correspondente*/
int search_byname(Ip* iprimary, Is* iproduct, int nregistros){
	char name[TAM_NOME];
	Is* idx;
	int i = 0;

	scanf("%[^\n]\n", name);
	idx = bsearch(name, iproduct, nregistros, sizeof(Is), cmpr_input_string );
	if (idx == NULL)
	{
		printf(REGISTRO_N_ENCONTRADO);
		return 0;
	}

	//must rewind for first name ocurrence, since there are repeated names
	while((idx-1) != NULL && strcmp ((idx-1)->string, name) == 0)
		idx--;

	while(strcmp ((idx+i)->string, name) == 0){
		//certainly the record exists, so its only needed to retrieve the RRN by
		//ths secondary index PK
		exibir_registro(recuperar_rrn(iprimary, (idx+i)->pk, nregistros), 0);
		if (strcmp ((idx+i+1)->string, name) == 0)
		{
			printf("\n");
		}
		i++;
	}
	return 1;

}

/*usa bsearch para encontrar nome do produto correspondente*/
int search_bybrand_and_category(Ip* iprimary, Is* ibrand, int nregistros){
	char brand[TAM_MARCA], cat[TAM_CATEGORIA];
	int rrn_vector[MAX_REGISTROS];

	Produto j;
	Is* idx;
	int rrn, vec_size = 0, printed = 0, i = 0;

	scanf("%[^\n]\n", brand);
	scanf("%[^\n]\n", cat);
	idx = bsearch(brand, ibrand, nregistros, sizeof(Is), cmpr_input_string );

	if (idx == NULL)
	{
		printf(REGISTRO_N_ENCONTRADO);
		return 0;
	}

	//must rewind for first name ocurrence, since there are repeated brands
	while((idx-1) != NULL && strcmp ((idx-1)->string, brand) == 0)
		idx--;

	while((idx+i) != NULL && strcmp ((idx+i)->string, brand) == 0){
		//certainly the record exists, so its only needed to retrieve the RRN by
		//ths secondary index PK
		rrn = recuperar_rrn(iprimary, (idx+i)->pk, nregistros);
		j = recuperar_registro(rrn);
		if (has_category_inregister(j, cat)){
			//exibir_registro(rrn,0);
			//printed = 1;
			rrn_vector[vec_size] = rrn;
			vec_size++;
		}

		i++;
	}

	for(i = 0; i < vec_size; i++){
		exibir_registro(rrn_vector[i], 0);
		if (i < vec_size-1)
			printf("\n");
	}

	//if we have the brand but could not find the desired category
	if(vec_size == 0){
		printf(REGISTRO_N_ENCONTRADO);
		return 0;
	}

	return 1;

}


int has_category_inregister(Produto j, char* category){
	char* tok;
	int has = 0;

	tok = strtok (j.categoria, "|");
	while(tok != NULL){
		if (strcmp(tok, category) == 0)
			has = 1;
		tok = strtok (NULL, "|");
	}
	return has;
}


/*receives pk from user and exibits messeges*/
int search_bypk(Ip* iprimary, int nregistros){
	char pk[TAM_PRIMARY_KEY];
	int rrn = NULL_RRN;

	scanf("%[^\n]\n", pk);
	rrn = recuperar_rrn(iprimary, pk, nregistros);	
	if(rrn == NULL_RRN){
		printf(REGISTRO_N_ENCONTRADO);
		return 0;
	}

	return exibir_registro(rrn, 0);

}


/*função padrão alterada para comparar no qsort*/
int cmpr_bypk(const void *a, const void *b){
	Ip* ptr_b = (Ip*) b;
	Ip* ptr_a = (Ip*) a;
	return strcmp(ptr_a->pk, ptr_b->pk);
}

/*função padrão alterada para comparar no bsearch*/
int cmpr_input_bypk(const void *a, const void *b){
	Ip* ptr_b = (Ip*) b;
	return strcmp((const char *) a , ptr_b->pk);
}

/*função de comparação entre nome de produto p/  e qsort*/
int cmpr_string(const void *a, const void *b){
	Is* ptr_a = (Is*) a;
	Is* ptr_b = (Is*) b;
	int unequal;

	unequal = strcmp(ptr_a->string, ptr_b->string);
	if (!unequal)
	{

		//tiebreaker criteria
		//pk is never equal
		unequal = cmpr_bypk(a, b);
	}

	return unequal;	
}

/*função de comparação entre nome de produto p/ bsearch e qsort*/
int cmpr_input_string(const void *a, const void *b){
	Is* ptr_b = (Is*) b;
	//int unequal;

	return strcmp((const char*) a, ptr_b->string);
	// if (!unequal)
	// 	unequal = -1;

	// return unequal;	
}

/*função de comparação entre nome de categoria p/ bsearch*/

int cmpr_input_bycategory(const void* a, const void* b){
	Ir* idx = (Ir*) b;

	//can be used in qsort since no repeated category is allowed
	return strcmp((const char*) a, idx->cat);
}


/*função de comparação entre nome de categoria p/ qsort*/
int cmpr_bycategory(const void* a, const void* b){
	Ir* idxb = (Ir*) b;
	Ir* idxa = (Ir*) a;
	//can be used in qsort since no repeated category is allowed
	return strcmp(idxa->cat, idxb->cat);
}

int cmpr_byprice(const void *a, const void *b){
	Isf* ptr_a = (Isf*) a;
	Isf* ptr_b = (Isf*) b;
	float res;

	res = ptr_a->price - ptr_b->price ;
	if(res == 0.0)
		return strcmp(ptr_a->pk, ptr_b->pk);
	return (int)res;

}

void key_gen(Produto *new){
	char *pk;
	pk = (char *) malloc(TAM_PRIMARY_KEY * sizeof(char));

	*(pk+0) = *(new->nome);
	*(pk+1) = *(new->nome+1);
	*(pk+2) = *(new->marca);
	*(pk+3) = *(new->marca+1);
	*(pk+4) = *(new->data);
	*(pk+5) = *(new->data+1);
	*(pk+6) = *(new->data+3);
	*(pk+7) = *(new->data+4);
	*(pk+8) = *(new->ano);
	*(pk+9) = *(new->ano+1);
	*(pk+10) = '\0';

	strcpy(new->pk, pk);
	free(pk);
}

void overwrite_data(Produto new, int rrn){
	char *eof = ARQUIVO + (rrn * 192);
	int len;
	//conta o tamanho do Produto new
	int size = strlen( new.nome ) + strlen( new.marca) + strlen( new.data) +
		strlen( new.ano) + strlen( new.preco) +
		 strlen( new.desconto) + strlen( new.categoria);

	//nome
	strncpy(eof, new.nome, len = strlen(new.nome));
	eof = eof + len;
	strncpy(eof, "@", 1);
	eof++;
	//desenvolvedora
	strncpy(eof, new.marca, len = strlen(new.marca));
	eof = eof + len;
	strncpy(eof, "@", 1);
	eof++;
	//data
	strncpy(eof, new.data, len = strlen(new.data));
	eof = eof + len;
	strncpy(eof, "@", 1);
	eof++;
	//classificacao
	strncpy(eof, new.ano, len = strlen(new.ano));
	eof = eof + len;
	strncpy(eof, "@", 1);
	eof++;
	//preco
	strncpy(eof,new.preco, len = strlen(new.preco));
	eof = eof + len;
	strncpy(eof, "@", 1);
	eof++;
	//desconto
	strncpy(eof, new.desconto, len = strlen(new.desconto));
	eof = eof + len;
	strncpy(eof, "@", 1);
	eof++;
	//categoria
	strncpy(eof, new.categoria, len = strlen(new.categoria));
	eof = eof + len;
	strncpy(eof, "@", 1);
	eof++;

	//fill with ####
	size += 7; //@'s
	while(size < 192 ){
		strncpy(eof, "#", 1);
		eof++;
		size++;
	}
}


/*escreve no final do pseudo arquivo*/
int  append_file(Produto *new, char* FILE){
	int size = 0;
	int len = strlen(FILE);
	char *eof = FILE + len;

	//conta o tamanho do Produto new
	size = strlen( new->nome ) + strlen( new->marca) + strlen( new->data) +
		strlen( new->ano) + strlen( new-> preco) +
		 strlen( new-> desconto) + strlen( new->categoria);

	//nome
	strncpy(eof, new->nome, len = strlen(new->nome));
	eof = eof + len;
	strncpy(eof, "@", 1);
	eof++;
	//desenvolvedora
	strncpy(eof, new->marca, len = strlen(new->marca));
	eof = eof + len;
	strncpy(eof, "@", 1);
	eof++;
	//data
	strncpy(eof, new->data, len = strlen(new->data));
	eof = eof + len;
	strncpy(eof, "@", 1);
	eof++;
	//classificacao
	strncpy(eof, new->ano, len = strlen(new->ano));
	eof = eof + len;
	strncpy(eof, "@", 1);
	eof++;
	//preco
	strncpy(eof,new->preco, len = strlen(new->preco));
	eof = eof + len;
	strncpy(eof, "@", 1);
	eof++;
	//desconto
	strncpy(eof, new->desconto, len = strlen(new->desconto));
	eof = eof + len;
	strncpy(eof, "@", 1);
	eof++;
	//categoria
	strncpy(eof, new->categoria, len = strlen(new->categoria));
	eof = eof + len;
	strncpy(eof, "@", 1);
	eof++;

	//fill with ####
	size += 7; //@'s
	while(size < 192 ){
		strncpy(eof, "#", 1);
		eof++;
		size++;
	}

	return 1;
}

int is_desc_pattern(char* desc){
	int value;

	sscanf(desc, "%d", &value);
	if(value > 100 || value < 0 || strlen(desc) > 3)
		return 0;
	return 1;
}