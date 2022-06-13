#include<stdio.h>
#include<stdlib.h>

int tempo = 0;
float gasto = 0;


typedef struct Lote{
    int tipoProduto;        // 1-Coxinha, 2-Peixe, 3-Almôndega
    float custoLote;
    int instanteDet;        // Instante de deteriorização
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


// Funções Base

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

    No* prim = cabecaFila->primeiro;
    while (prim != NULL) {
        printf("%d\n", prim->produto.tipoProduto);
        prim = prim->prox;
    }
    
}

Lote geraLote() // Gera e retorna um lote aleatório dadas as probabilidades de cada produto (Coxinha - 50%, Peixe - 30%, Almôndega - 20%)
{
    int numero = rand() % 10;
    Lote aux; 
    if (numero >= 0 && numero <= 4){    // (50%)    
        aux.tipoProduto = 1;     // 1- Coxinha
        aux.custoLote = 0.8;
        aux.instanteDet = 0;
        gasto += aux.custoLote;
        return aux;
    }

    if (numero >= 5 && numero <=7){    // (30%) 
        aux.tipoProduto = 2;     // 1- Coxinha
        aux.custoLote = 0.7;
        aux.instanteDet = 0;
        gasto += aux.custoLote;
        return aux;
    }

    aux.tipoProduto = 3; // 3-  Almôndega (20%)
    aux.custoLote = 0.4;
    aux.instanteDet = 0;
    gasto += aux.custoLote;
    return aux;
}

No *alocaNo(Lote lote)
{
    No *aux = (No *)malloc(sizeof(No));
    if (aux == NULL) return NULL;

    aux->produto = lote;
    aux->prox = NULL;
}

void insereFila(Fila **cabecaFila, Lote lote)
{
    No *novo = alocaNo(lote);

    if ((*cabecaFila)->primeiro == NULL){
        (*cabecaFila)->primeiro = novo;
        (*cabecaFila)->ultimo = novo;
        (*cabecaFila)->qtde++;
        return;
    }

    (*cabecaFila)->ultimo->prox = novo;
    (*cabecaFila)->ultimo = novo;
    (*cabecaFila)->qtde++;
}

// Funções Auxiliares 

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

Fila *verificaMenorFila(ListaMaquinas *compativel)
{
    // Se houver apenas uma máquina compatível, retorne a fila dessa máquina
    if (compativel->prox == NULL){
        return compativel->maquina.cabecaFila;
    }

    int menor;
    menor = compativel->maquina.cabecaFila->qtde;
    
    // Percorre a lista pela 1ª vez para achar a menor fila
    ListaMaquinas *aux = compativel->prox;
    while (aux != NULL) {
        // printf("maquina atual: %d\n", aux->maquina.modelo);
        // printf("menor: %d\n", menor);
        if (aux->maquina.cabecaFila->qtde <= menor){
            menor = aux->maquina.cabecaFila->qtde;
        }
        aux = aux->prox;
    }

    aux = compativel;
    // Percorre a lista pela 2ª vez para retornar a menor fila
    while (aux != NULL) {
        // printf("maquina atual (inserir): %d\n", aux->maquina.modelo);
        if(aux->maquina.cabecaFila->qtde == menor){
            printf("foi pra maquina %d\n", aux->maquina.modelo);
            return aux->maquina.cabecaFila;  
        } 
        aux = aux->prox;
    }
    

}

// Funções Principais:

int compraMaquina(ListaMaquinas **prim, int tipoMaq, int qtde)  // Realiza as alocações e retorna o custo da máquina comparada
{
    Fila *cabecaFila = criaFila();
    Maquina maquina = inicializaMaquina(tipoMaq, cabecaFila); 

    insereListaMaquinas(&*prim, maquina);

    switch (tipoMaq) {
        case 1: case 2: return qtde * 100000; break;
        case 3: return qtde * 150000; break;
        case 4: return qtde * 60000; break;
        case 5: return qtde * 50000; break;
        case 6: return qtde * 30000; break;
    }
}

void insereLoteFila(ListaMaquinas **prim, Lote lote)
{
    ListaMaquinas *varredor = *prim;    // Varrer a ListaMaquinas e econtrar a(s) máquina(s) compatível(is)

    ListaMaquinas *compativel = NULL;   
    // VERIFICAR QUAIS SÃO AS MAQUINAS COMPATÍVEIS
    while (varredor != NULL) {
        if (lote.tipoProduto == 1 && varredor->maquina.modelo != 1) {    // A coxinha (lote = 1) é compatível com todas as máquinas, menos com a FishPak (modelo = 1)
            insereListaMaquinas(&compativel, varredor->maquina);  
        } else if (lote.tipoProduto == 2 && varredor->maquina.modelo != 2) {    // O peixe (lote = 2) é compatível com todas as máquinas, menos com a chickenPak (modelo = 2) 
            insereListaMaquinas(&compativel, varredor->maquina);
        } else if (lote.tipoProduto == 3 && varredor->maquina.modelo != 1 && varredor->maquina.modelo != 2){ // A almôndega (lote = 3) é compatível com todas as máquinas, menos com modelo = 1 e modelo = 2
            insereListaMaquinas(&compativel, varredor->maquina);
        }

        varredor = varredor->prox;
    }
    printf("Lista de Maquinas compativeis para o lote %d:\n", lote.tipoProduto);
    imprimeListaMaquinas(compativel);
 
    if (compativel){
        Fila *fila = verificaMenorFila(compativel);
        insereFila(&fila, lote);
        printf("Fila\n");
        imprimeFila(fila);
    } 
}

int main() {
    ListaMaquinas *prim = NULL;
    int tipoMaq;
    int qtde;
    do{    
        printf("Qual maquina deseja adicionar?\n");     
        printf("\n1-FishPak\t2-ChickenPak\t3-AllPak\t4-Plastific\t5-EnSacAll\t6-Universal\n0-Ja adicionei todas\n\n")   ;
        scanf("%d", &tipoMaq);

        switch (tipoMaq) {
            case 0: break;
            case 1: case 2: case 3: case 4: case 5: case 6:{
                printf("Quantas?\n");
                scanf("%d", &qtde);
                for (int i = 0; i < qtde; i++) gasto += compraMaquina(&prim, tipoMaq, qtde);
            }break;
            
            default:{
                printf("Opcao invalida!\n");
            }break;
        }

    }while (tipoMaq != 0);

    // printf("Lista Maquinas na main:\n");
    // imprimeListaMaquinas(prim);

    // Começo da simulação

    int i;
    Lote lote;
    for (i = 0; i < 7; i++){
        lote = geraLote();
        insereLoteFila(&prim, lote);  
    }  

    // Lote lote, lote2, lote3;
    // lote.custoLote = 0.8;
    // lote.instanteDet = 0;
    // lote.tipoProduto = 1;

    // lote2.custoLote = 0.7;
    // lote2.instanteDet = 0;
    // lote2.tipoProduto = 2;

    // lote3.custoLote = 0.4;
    // lote3.instanteDet = 0;
    // lote3.tipoProduto = 3;




    printf("Gasto: %.4f", gasto);
    return 0;

}