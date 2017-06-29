#ifndef SUB_ARVORE_H_INCLUDED
#define SUB_ARVORE_H_INCLUDED

#include "vertice.h"
#include "no.h"
#include "fila.h"



typedef struct arvore arvore_t;
typedef struct sub_arvore sub_arvore_t;


arvore_t *cria_arvore(void);

sub_arvore_t* cria_sub(arvore_t* tree, int id, void* dado);

void add_filho(sub_arvore_t* pai, sub_arvore_t* filho);

no_t* arvore_get_cabeca(arvore_t* tree);

void arvore_set_boss(sub_arvore_t* filho, sub_arvore_t *boss);

sub_arvore_t* arvore_get_boss(sub_arvore_t* filho);

sub_arvore_t* arvore_get_pai(sub_arvore_t* filho);

sub_arvore_t* arvore_get_filho_id(sub_arvore_t *pai, int id);

no_t* sub_get_cabeca(sub_arvore_t *pai);

int sub_arvore_get_id(sub_arvore_t* v);

vertice_t* sub_get_dado(sub_arvore_t* sub);

sub_arvore_t* sub_set_dado(sub_arvore_t* sub, vertice_t* dado);

sub_arvore_t* get_raiz(arvore_t* a);

int get_level(sub_arvore_t* sub);

int set_level(sub_arvore_t* sub, int level);

int inc_level(sub_arvore_t* sub);

arvore_t* set_raiz(arvore_t* tree, sub_arvore_t* raiz);

int arvore_set_n_sub(arvore_t* tree, int n);

int arvore_get_n_sub(arvore_t* tree);

int arvore_get_level(arvore_t* tree);

int arvore_set_level(arvore_t* tree, int level);

int pai_get_n_filhos(sub_arvore_t* pai);

void teste_mst(arvore_t* tree);

sub_arvore_t* busca_pre_ordem(sub_arvore_t* sub);

fila_t* mostra_filhos(sub_arvore_t* pai);

void libera_arv(arvore_t* tree);

#endif // SUB_ARVORE_H_INCLUDED
