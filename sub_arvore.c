#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include "sub_arvore.h"
#include "lista_enc.h"
//#include "../lista_enc/no.h"
//#include "vertice.h"
#include "pilha.h"
//#include "../fila/fila.h"


struct sub_arvore{
    sub_arvore_t *pai;
    lista_enc_t *filhos;          // Optou-se por uma lista encadeada de filhos pois cada vertice do grafo pode ter qtde diferente de arestas,
    vertice_t* dado;              // ponteiro para o dado recebido
    int id;
    int level;
    int pai_n_filhos;             // para  o pai
    sub_arvore_t *boss;           // chefe do grupo  usado para verificar se há ciclos
};

struct arvore{
    sub_arvore_t *raiz;
    lista_enc_t *lista_sub;
    int n_sub;
    int level;
};

//////////////////////////////////////////////////////////////////////////////////////////////
arvore_t *cria_arvore(void)
{
    arvore_t *tree = (arvore_t*)malloc(sizeof(arvore_t));

    if(tree == NULL){
        perror("\ncria arvore");
        exit(1);
    }

    tree->raiz = NULL;
    tree->lista_sub = cria_lista_enc();
    tree->n_sub = 0;
    tree->level = 0;

    return tree;
}

///////////////////////////////////////////////////////////////////////////////////////////////
sub_arvore_t* cria_sub(arvore_t* tree, int id, void* dado){ //era vertice_t* dado

    if(tree == NULL){
        perror("cria_sub");
        exit(1);
    }

    sub_arvore_t* sub = (sub_arvore_t*)malloc(sizeof(sub_arvore_t));

    if(sub == NULL){
        perror("malloc cria_sub");
        exit(1);
    }

    sub->pai = NULL;
    sub->filhos = cria_lista_enc();
    sub->id = id;
    sub->dado = dado;
    sub->level = 0;
    sub->pai_n_filhos = 0;
    sub->boss = NULL;

    no_t *no = cria_no(sub);
    add_cauda(tree->lista_sub,no);    // add a sub_arvore criada à lista de sub_arvores da árvore principal

    return sub;
}

///////////////////////////////////////////////////////////////////////////////////////////////
void add_filho(sub_arvore_t* pai, sub_arvore_t* filho)
{                                                                                   // filho é a estrurura a ser inserida de fato, pai é a referência para a conexão
    if(pai == NULL || filho == NULL){
        perror("add_filho");
        exit(1);
    }

    filho->pai = pai;
    no_t *no = cria_no(filho);        // add o filho à lista de filhos do pai
    add_cauda(pai->filhos,no);
    pai->pai_n_filhos++;

}

//////////////////////////////////////////////////////////////////////////////////////////////////
no_t* arvore_get_cabeca(arvore_t* tree)
{
    if(tree == NULL){
        perror("get_sub");
        exit(1);
    }

    no_t* no = obter_cabeca(tree->lista_sub);
    //sub_arvore_t* sub = obter_dado(no);
    //free(no);

    return no;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void arvore_set_boss(sub_arvore_t* filho, sub_arvore_t *boss)
{
    if (boss == NULL || filho == NULL){
        fprintf(stderr, "arvore_set_pai: vertice invalido\n");
        exit(EXIT_FAILURE);
    }

    filho->boss = boss;
    //boss->level++;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
sub_arvore_t* arvore_get_boss(sub_arvore_t* filho)
{
    if (filho == NULL){
        fprintf(stderr, "arvore_set_pai: vertice invalido\n");
        exit(EXIT_FAILURE);
    }

    return filho->boss;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
sub_arvore_t* arvore_get_pai(sub_arvore_t* filho)
{
    if (filho == NULL){
        fprintf(stderr, "arvore_get_pai: vertice invalido\n");
        exit(EXIT_FAILURE);
    }

    return filho->pai;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
sub_arvore_t* arvore_get_filho_id(sub_arvore_t *pai, int id)
{
    if (pai == NULL){
        fprintf(stderr, "arvore_get_filho: vertice invalido\n");
        exit(EXIT_FAILURE);
    }

    no_t *no = obter_cabeca(pai->filhos);
    sub_arvore_t* filho;

    while(no){

        filho = obter_dado(no);
        if(id == filho->id)
            return filho;

        no = obtem_proximo(no);
    }
    printf("\nNao ha filhos com esse id!\n");
    return NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
no_t* sub_get_cabeca(sub_arvore_t *pai){

    if (pai == NULL){
        fprintf(stderr, "arvore_get_filho: vertice invalido\n");
        exit(EXIT_FAILURE);
    }

    if(!lista_vazia(pai->filhos))
        return  obter_cabeca(pai->filhos);

    else
        printf("\nNao ha filhos!\n");
    return NULL;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
int sub_arvore_get_id(sub_arvore_t* v)
{
    if (v == NULL){
        fprintf(stderr, "sub_arvore_get_id: vertice invalido\n");
        exit(EXIT_FAILURE);
    }

    return v->id;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
sub_arvore_t* sub_set_dado(sub_arvore_t* sub, vertice_t* dado)
{
    if (sub == NULL){
        fprintf(stderr, "sub_set_dado: vertice invalido\n");
        exit(EXIT_FAILURE);
    }

    sub->dado = dado;

    return sub;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
vertice_t* sub_get_dado(sub_arvore_t* sub)
{
    if (sub == NULL){
        fprintf(stderr, "sub_get_dado: vertice invalido\n");
        exit(EXIT_FAILURE);
    }

    return sub->dado;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
sub_arvore_t* get_raiz(arvore_t* a)
{
  if (a == NULL){
        fprintf(stderr, "get_raiz: vertice invalido\n");
        exit(EXIT_FAILURE);
    }

    return a->raiz;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
arvore_t* set_raiz(arvore_t* tree, sub_arvore_t* raiz){

    if(tree == NULL){
        perror("set_raiz");
        exit(1);
    }

        tree->raiz = raiz;           //  a raíz será o filho que está sendo inserido
        //raiz->pai = NULL;            // este não terá nenhum pai, pois é o primeiro

    return tree;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
int get_level(sub_arvore_t* sub)
{
  if (sub == NULL){
        fprintf(stderr, "get_flag: vertice invalido\n");
        exit(EXIT_FAILURE);
    }

    return sub->level;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
int set_level(sub_arvore_t* sub, int level)
{
    if (sub == NULL){
        fprintf(stderr, "set_flag: vertice invalido\n");
        exit(EXIT_FAILURE);
    }

    sub->level = level;

    return sub->level;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
int inc_level(sub_arvore_t* sub)
{
    if (sub == NULL){
        fprintf(stderr, "set_flag: vertice invalido\n");
        exit(EXIT_FAILURE);
    }

    sub->level++;

    return sub->level;
}

int arvore_set_n_sub(arvore_t* tree, int n){

    if (tree == NULL){
        fprintf(stderr, "arvore_get_n_sub: vertice invalido\n");
        exit(EXIT_FAILURE);
    }

    tree->n_sub = n;

    return tree->n_sub;
}

int arvore_get_n_sub(arvore_t* tree){

    if (tree == NULL){
        fprintf(stderr, "arvore_get_n_sub: vertice invalido\n");
        exit(EXIT_FAILURE);
    }

    return tree->n_sub;
}


int arvore_set_level(arvore_t* tree, int level){

    if (tree == NULL){
        fprintf(stderr, "arvore_get_n_sub: vertice invalido\n");
        exit(EXIT_FAILURE);
    }

    tree->level = level;

    return tree->level;
}

int arvore_get_level(arvore_t* tree){

    if (tree == NULL){
        fprintf(stderr, "arvore_get_n_sub: vertice invalido\n");
        exit(EXIT_FAILURE);
    }

    return tree->level;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int pai_get_n_filhos(sub_arvore_t* pai){

    return pai->pai_n_filhos;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void teste_mst(arvore_t* tree){

    if(tree == NULL){
        perror("teste_mst");
        exit(EXIT_FAILURE);
    }

    int i;
    vertice_t* v;
    sub_arvore_t *raiz, *sub, *filho;

    fila_t *fila, *fila2, *fila3;
  //  fila = cria_fila();
 //   fila2 = cria_fila();
  //  fila3 = cria_fila();

    no_t *no, *no_filhos;
    no = arvore_get_cabeca(tree);
 //   sub = obter_dado(no);
 //   v = sub_get_dado(sub);

    raiz = tree->raiz;

    printf("\n****************** Teste da arvore gerada ********************\n");
    printf("\nNumero de sub_arvores: %d",tree->n_sub);
    printf("\nNivel: %d",tree->level);
    printf("\nId raiz: %d",raiz->id);
    printf("\n----------------------------------------------------------------------\n\n");



    while(no){
        sub = obter_dado(no);
        v = sub_get_dado(sub);
        printf("\nId vertice: %2d ---- Id sub: %2d\n",vertice_get_id(v),sub->id);
        mostra_filhos(sub);
        printf("--------------------------------------\n");
        no = obtem_proximo(no);
    }

    printf("\n----------------------------------------------------------------------\n\n");

                                                    // implementar as buscas para testar a arvore

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
fila_t* mostra_filhos(sub_arvore_t* pai){

    if(pai == NULL){
        perror("mostra_filhos");
        exit(1);
    }

    fila_t* fila = cria_fila();
    no_t* no = sub_get_cabeca(pai);

    sub_arvore_t* filho;

    while(no){

        filho = obter_dado(no);
        enqueue(filho,fila);
        printf("\nId da filho: %d", filho->id);
        no = obtem_proximo(no);
    }
    printf("\n");

    return fila;
}

void libera_arv(arvore_t* tree)
{
     if (tree == NULL)
    {
        fprintf(stderr, "libera_arvore: arvore invalida\n");
        exit(EXIT_FAILURE);
    }

    free(tree->lista_sub);
    free(tree);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//sub_arvore_t* busca_pre_ordem(sub_arvore_t* sub){

//    if(sub == NULL)
  //      return sub;
/*
    sub_arvore_t* v;
    no_t *no;
    pilha_t * pilha = cria_pilha();
    push(sub);




    while(!pilha_vazia(pilha)){

        v = pop(pilha);

        printf("\nId da sub_arvore: %d", sub->id);

    }
*/
//}
