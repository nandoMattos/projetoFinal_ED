#include<stdio.h>
#include<stdlib.h>

int instante = 0;   // Variável para simular o tempo
float gasto = 0;
float ganhos = 0;
int lucro = 0;

typedef struct Lote{
    int tipoProduto;        // 1-Coxinha, 2-Peixe, 3-Almôndega
    float custoLote;
    float precoVenda;
    int instanteDet;        // Tempo que o produto demora para deteriorizar
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
    int instanteEmpacotamento;
    float consumo;
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
        case 1:{
            aux.instanteEmpacotamento = instante + 10; 
            aux.consumo = 20;
        }break
        ;
        case 2: {
            aux.instanteEmpacotamento = instante + 16; 
            aux.consumo = 20;
        }break;

        case 3: {
            aux.instanteEmpacotamento = instante + 0;       // Está como 0 porque depende do produto.   
            aux.consumo = 22;
        }break;

        case 4: {
            aux.instanteEmpacotamento = instante + 25; 
            aux.consumo = 35;
        }break;

        case 5: {
            aux.instanteEmpacotamento = instante + 30; 
            aux.consumo = 40;
        }break;

        case 6: {
            aux.instanteEmpacotamento = instante + 35;
            aux.consumo = 35;
        }break;
    }

    return aux;
}

void imprimeListaMaquinas(ListaMaquinas *prim)  // Imprime a Lista de Maquinas para verificar se está devidamente alocada
{
    if(prim == NULL){
        printf("Lista vazia!\n");
        return;
    }

    while (prim != NULL) {
        printf("Modelo: %d, tempo: %d\n", prim->maquina.modelo, prim->maquina.instanteEmpacotamento);
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
    int numero = rand() % 9;
    Lote aux; 
    if (numero >= 0 && numero <= 4){    // (50%)    
        aux.tipoProduto = 1;     // 1- Coxinha
        aux.custoLote = 0.8;
        aux.precoVenda = 1.45;
        aux.instanteDet = 0;
        gasto += aux.custoLote;
        return aux;
    }

    if (numero >= 5 && numero <=7){    // (30%) 
        aux.tipoProduto = 2;     // 1- Coxinha
        aux.custoLote = 0.7;
        aux.precoVenda = 2;
        aux.instanteDet = 0;
        gasto += aux.custoLote;
        return aux;
    }

    aux.tipoProduto = 3; // 3-  Almôndega (20%)
    aux.custoLote = 0.4;
    aux.precoVenda = 0.8;
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

void insereFila(Fila **cabecaFila, Lote *lote)
{
    No *novo = alocaNo(*lote);
    if (novo == NULL) return;

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

void removeDaFila(Fila **cabecaFila)
{
    No *removedor = (*cabecaFila)->primeiro;
    if (removedor == NULL) {
        printf("Lista Vazia!");
        return;
    };

    if (removedor->prox == NULL) {
        (*cabecaFila)->primeiro = NULL;
        (*cabecaFila)->ultimo = NULL;
        free(removedor);
        return;
    }

    (*cabecaFila)->primeiro = removedor->prox;
    free(removedor);

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

void consumoPorDia(ListaMaquinas *prim)
{
    if (prim == NULL){
        printf("Lista vazia!");
        return;
    }

    while (prim != NULL){
        gasto += prim->maquina.consumo * 24;
        prim = prim->prox;
    }
} 

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

void insereLoteFila(ListaMaquinas **prim, Lote *lote)
{
    ListaMaquinas *varredor = *prim;    // Varrer a ListaMaquinas e econtrar a(s) máquina(s) compatível(is)

    ListaMaquinas *compativel = NULL;   // Criar uma Lista de Maquinas compatíveis

    // VERIFICAR QUAIS SÃO AS MAQUINAS COMPATÍVEIS
    while (varredor != NULL) {
        if ((*lote).tipoProduto == 1 && varredor->maquina.modelo != 1) {    // A coxinha (lote = 1) é compatível com todas as máquinas, menos com a FishPak (modelo = 1)
            insereListaMaquinas(&compativel, varredor->maquina);  
        } else if ((*lote).tipoProduto == 2 && varredor->maquina.modelo != 2) {    // O peixe (lote = 2) é compatível com todas as máquinas, menos com a chickenPak (modelo = 2) 
            insereListaMaquinas(&compativel, varredor->maquina);
        } else if ((*lote).tipoProduto == 3 && varredor->maquina.modelo != 1 && varredor->maquina.modelo != 2){ // A almôndega (lote = 3) é compatível com todas as máquinas, menos com modelo = 1 e modelo = 2
            insereListaMaquinas(&compativel, varredor->maquina);
        }

        //Aproveitar o laço para setar o tempo de empacotamento da maquina 3, já que depende do tipo do produto
        if (varredor->maquina.modelo == 3){ 
            switch ((*lote).tipoProduto) {
                case 1: varredor->maquina.instanteEmpacotamento = instante + 18; break;
                case 2: case 3: varredor->maquina.instanteEmpacotamento = instante + 12; break;
            }
        }
        varredor = varredor->prox;
    }
    printf("Lista de Maquinas compativeis para o lote %d:\n", lote->tipoProduto);
    imprimeListaMaquinas(compativel);
 
    if (compativel){
        switch ((*lote).tipoProduto) {
            case 1: (*lote).instanteDet = instante + 50; break;
            case 2: (*lote).instanteDet = instante + 20; break;
            case 3: (*lote).instanteDet = instante + 90; break;
        }
        Fila *fila = verificaMenorFila(compativel);
        insereFila(&fila, lote);
        printf("Fila\n");
        imprimeFila(fila);
    } 
}

void verificaEmpacotamento(ListaMaquinas **prim)
{
    ListaMaquinas *varredor = *prim;
    Fila *fila = NULL;
    
    int cont = 0;
    while (varredor != NULL)  { 
        if(varredor->maquina.cabecaFila->primeiro != NULL){
            if (varredor->maquina.cabecaFila->primeiro->produto.instanteDet >= instante) {
                if(varredor->maquina.instanteEmpacotamento == instante){
                    removeDaFila(&fila);
                    ganhos += varredor->maquina.cabecaFila->primeiro->produto.precoVenda;
                    imprimeFila(varredor->maquina.cabecaFila);
                }
            } else {
                fila = varredor->maquina.cabecaFila;
                removeDaFila(&fila);   
            }   
        }
        varredor = varredor->prox;
    }

    
}

// Main

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
                printf("Opcao invalida!\n\n");
            }break;
        }

    }while (tipoMaq != 0);

    // Começo da simulação

    Lote lote;
    while (instante < 40 || lucro >= 0) {
        if (instante % 2 == 0) {
            lote = geraLote();
            insereLoteFila(&prim, &lote);
        }
        // verificaEmpacotamento(&prim);
        lucro = ganhos - gasto;
        instante++;
    }

    printf("Gasto: %.4f\n", gasto);
    printf("Ganhos: %.4f\n", ganhos);
    printf("Lucro: %.4f\n", lucro);
    return 0;

}