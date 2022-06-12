#include<stdio.h>
#include<stdlib.h>

int tempo = 0;

typedef struct Lote{
    int tipoProduto;        // 1-Coxinha, 2-Peixe, 3-Almôndega
    float custoLote;
    int tempoDet;
}Lote;

typedef struct No{
    Lote produto;
    struct No *prox;
}No;

typedef struct Fila{
    No *primeiro;
    No *ultimo;
    int qtde;
}Fila;

typedef struct Maquina{
    int modelo;             // 1-FishPak, 2-ChickenPak, 3-AllPak, 4-Plastific, 5-EnSacAll, 6-Universal 
    int tempoEmpacotamento;
    Fila *cabecaFila;
}Maquina;

typedef struct ListaMaquinas{
    Maquina maquina;
    struct ListaMaquinas *prox;
}ListaMaquinas;


// Funções Auxiliares 

Fila *criaFila()    // Inicializa a Fila da Maquina
{
    Fila *aux = (Fila *)malloc(sizeof(Fila));
    if(aux == NULL){
        printf("Erro ao alocar Fila");
        return NULL;
    }

    aux->primeiro = NULL;
    aux->ultimo = NULL;
    aux->qtde = 0;
    return aux;
}

void insereListaMaquinas(ListaMaquinas **prim, Maquina maquina) // Insere uma máquina no inicio da Lista de Maquinas que o usuário escolheu
{
    ListaMaquinas *novo = (ListaMaquinas *)malloc(sizeof(ListaMaquinas));
    novo->maquina = maquina;
    novo->prox = NULL;
    if(*prim == NULL){
        *prim = novo;
        return;
    }

    novo->prox = *prim;
    *prim = novo;
}

Maquina inicializaMaquina(int modelo, Fila *cabecaFila) // Guarda as informações das Maquinas (Modelo, Fila e Tempo de Empacotamento)
{
    Maquina aux;
    aux.modelo = modelo;
    aux.cabecaFila = cabecaFila;
    switch (modelo) {
        case 1: aux.tempoEmpacotamento = tempo + 10; break;
        case 2: aux.tempoEmpacotamento = tempo + 16; break;
        case 3: aux.tempoEmpacotamento = tempo + 0; break;      // Está como 0 porque depende do produto.   
        case 4: aux.tempoEmpacotamento = tempo + 25; break;
        case 5: aux.tempoEmpacotamento = tempo + 30; break;
        case 6: aux.tempoEmpacotamento = tempo + 35; break;
    }

    return aux;
}

void imprimeListaMaquinas(ListaMaquinas *prim)  // Imprime a Lista de Maquinas para se verificar está devidamente alocada
{
    if(prim == NULL){
        printf("Lista vazia!\n");
        return;
    }

    while (prim != NULL) {
        printf("Modelo: %d, tempo: %d\n", prim->maquina.modelo, prim->maquina.tempoEmpacotamento);
        prim = prim->prox;
    }
}

void imprimeFila(Fila *cabecaFila)
{
    if (cabecaFila->primeiro == NULL){
        printf("Fila vazia!\n");
        return;
    }
}

int geraLote() // Gera e retorna um lote aleatório dadas as probabilidades de cada produto (Coxinha - 50%, Peixe - 30%, Almôndega - 20%)
{
    int aux = rand() % 10;
    if (aux >= 0 && aux <= 4){    
        return 1;     // 1- Coxinha
    }

    if (aux >= 5 && aux <=7){
        return 2;    // 2- Peixe
    }

    return 3;   // 3- Almôndega
    
}

// Funções Principais:

int compraMaquina(ListaMaquinas **prim, int tipoMaq, int qtde)  // Realiza as alocações e retorna o custo da máquina comparada
{
    Fila *cabecaFila = criaFila();
    Maquina maquina = inicializaMaquina(tipoMaq, cabecaFila); 

    insereListaMaquinas(&*prim, maquina);

    switch (tipoMaq) {
        case 1: return qtde * 100000; break;
        case 2: return qtde * 100000; break;
        case 3: return qtde * 150000; break;
        case 4: return qtde * 60000; break;
        case 5: return qtde * 50000; break;
        case 6: return qtde * 30000; break;
    }
}

void insereLoteFila(ListaMaquinas **prim, int lote)
{
    ListaMaquinas *compativel = NULL;   // Criarei outra ListaMaquinas, para armazenar as maquina(s) compatível(is)
    ListaMaquinas *varredor = *prim;    // Varrer a ListaMaquinas e econtrar a(s) máquina(s) compatível(is)
    while (varredor != NULL) {
        if(lote == 1 && varredor->maquina.modelo != 1) {    // A coxinha (lote = 1) é compatível com todas as máquinas, menos com a FishPak (modelo = 1);
            insereListaMaquinas(&compativel, varredor->maquina);  
            printf("Lista de Maquinas compativeis:\n");
            imprimeListaMaquinas(compativel);
        }
        varredor = varredor->prox;
    }
    

    // Fila *fila = verificaMelhorFila();
}

int main() {
    ListaMaquinas *prim = NULL;
    int tipoMaq;
    int gasto = 0;
    do{     // Menu de opções (menos verboso que um Switch)

        printf("Qual maquina deseja adicionar?\n");     
        printf("\n1-FishPak\t2-ChickenPak\t3-AllPak\t4-Plastific\t5-EnSacAll\t6-Universal\n0-Ja adicionei todas\n\n")   ;
        scanf("%d", &tipoMaq);

        if(tipoMaq != 0){
            int qtde;
            printf("Quantas?\n");
            scanf("%d", &qtde);

            for (int i = 0; i < qtde; i++) gasto += compraMaquina(&prim, tipoMaq, qtde);
        } 

    }while (tipoMaq != 0);

    printf("Lista Maquinas na main:\n");
    imprimeListaMaquinas(prim);

    // Começo da simulação
    int lote = geraLote();
    // printf("Lote: %d\n", lote);
    insereLoteFila(&prim, lote);




    // printf("Gasto: %d", gasto);
    return 0;

}