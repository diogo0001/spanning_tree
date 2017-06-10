/*
 * grafo.c
 *
 *  Created on: Jul 5, 2016
 *      Author: Renan Augusto Starke
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include "grafo.h"
#include "vertice.h"
#include "../lista_enc/lista_enc.h"
#include "../lista_enc/no.h"
#include "../fila/fila.h"
#include "../arvore/sub_arvore.h"

#include "../pilha/pilha.h"

#define FALSE 0
#define TRUE 1

//#define DEBUG

#define INFINITO INT_MAX

struct grafos
{
    int id;
    lista_enc_t *vertices;
};


void bfs(grafo_t *grafo, vertice_t* inicial)
{
     if (grafo == NULL || inicial == NULL)
    {
        perror("busca_largura: ponteiro invalido.");
        exit(EXIT_FAILURE);
    }

    no_t *no, *no2;
    no = obter_cabeca(grafo->vertices);

    vertice_t *v,*u;
    fila_t *fila, *fila_aux;
    lista_enc_t *lista;
    arestas_t *a;
    fila = cria_fila();

    grafo_t* G = cria_grafo(1);                 // estruturas auxiliares
    fila_aux = cria_fila();

    int grau = 0;
    int grau_count = 0;
    arvore_t *tree;

    while (no)
    {
        v = (vertice_t*)obter_dado(no);
        vertice_set_dist(v,INFINITO);
        vertice_set_pai_nulo(v);
        no = obtem_proximo(no);
    }


    vertice_set_dist(inicial,0);
    enqueue(inicial,fila);

    while(!fila_vazia(fila))
    {
        u = dequeue(fila);
        lista = vertice_get_arestas(u);
        no2 = obter_cabeca(lista);
        printf("Dequeue v: %d\n", vertice_get_id(u));

        while(no2)
        {
            a = (arestas_t*)obter_dado(no2);
            v = aresta_get_adjacente(a);
            if (vertice_get_dist(v) == INFINITO)                // se não foi visitado
            {
                vertice_set_dist(v,vertice_get_dist(u)+1);      // seta o nivel
                vertice_set_pai(v,u);
                enqueue(v,fila);
                printf("Enqueue v: %d\n", vertice_get_id(v));
                grau_count++;
            }

            no2 = obtem_proximo(no2);
        }
        printf("\n");

        if(grau <= grau_count)
            grau = grau_count;

        grau_count = 0;
    }

    //free(lista);
    free(fila);

    printf("\nGrau: %d\n\n",grau);
    tree = cria_arvore(grau,0);

}


void dfs(grafo_t *grafo, vertice_t* inicial)
{
    if (grafo == NULL || inicial == NULL)
    {
        perror("busca_largura: ponteiro invalido.");
        exit(EXIT_FAILURE);
    }

    no_t *no, *no2;
    no = obter_cabeca(grafo->vertices);

    vertice_t *v,*u;
    pilha_t * pilha;
    lista_enc_t *lista;
    arestas_t *a;
    pilha = cria_pilha();

    while (no)
    {
        v = (vertice_t*)obter_dado(no);
        set_visitado(v,FALSE);
        no = obtem_proximo(no);
    }

    push(inicial,pilha);

    while(!pilha_vazia(pilha))
    {
        u = pop(pilha);
        lista = vertice_get_arestas(u);             // pega a lista de arestas de cada vertice
        no2 = obter_cabeca(lista);                  // pega a cabeça da lista de arestas
        printf("Pop u: %d\n", vertice_get_id(u));

        if (get_visitado(u) == FALSE)
        {
            set_visitado(u,TRUE);

            while(no2)
            {
                v = (vertice_t*)obter_dado(no2);
                push(v,pilha);
                no2 = obtem_proximo(no2);
            }
        }

        printf("\n");
    }
    //free(lista);
    free(pilha);

}

//--------------------------------------------------------------------------------------

grafo_t *cria_grafo(int id)
{
    grafo_t *p = NULL;

    p = (grafo_t*) malloc(sizeof(grafo_t));

    if (p == NULL)
    {
        perror("cria_grafo:");
        exit(EXIT_FAILURE);
    }

    p->id = id;
    p->vertices = cria_lista_enc();

    return p;
}

vertice_t* grafo_adicionar_vertice(grafo_t *grafo, int id)
{
    vertice_t *vertice;
    no_t *no;

#ifdef DEBUG
    printf("grafo_adicionar_vertice: %d\n", id);
#endif

    if (grafo == NULL)
    {
        fprintf(stderr,"grafo_adicionar_vertice: grafo invalido!");
        exit(EXIT_FAILURE);
    }

    if (procura_vertice(grafo, id) != NULL)
    {
        fprintf(stderr,"grafo_adicionar_vertice: vertice duplicado!\n");
        exit(EXIT_FAILURE);
    }

    vertice = cria_vertice(id);
    no = cria_no(vertice);

    add_cauda(grafo->vertices, no);

    return vertice;
}

vertice_t* procura_vertice(grafo_t *grafo, int id)
{
    no_t *no_lista;
    vertice_t *vertice;
    int meu_id;

    if (grafo == NULL)
    {
        fprintf(stderr,"procura_vertice: grafo invalido!");
        exit(EXIT_FAILURE);
    }

    if (lista_vazia(grafo->vertices) == TRUE)
        return FALSE;

    no_lista = obter_cabeca(grafo->vertices);

    while (no_lista)
    {
        //obtem o endereco da lista
        vertice = obter_dado(no_lista);

        //obterm o id do vertice
        meu_id = vertice_get_id(vertice);

        if (meu_id == id)
        {
            return vertice;
        }

        no_lista = obtem_proximo(no_lista);
    }

    return NULL;
}

void adiciona_adjacentes(grafo_t *grafo, vertice_t *vertice, int n, ...)
{
    va_list argumentos;
    vertice_t *sucessor;
    arestas_t *aresta;

    int id_sucessor;
    int peso;
    int x;

    /* Initializing arguments to store all values after num */
    va_start (argumentos, n);

    for (x = 0; x < n; x=x+2 )
    {
        id_sucessor = va_arg(argumentos, int);
        peso = va_arg(argumentos, int);

        sucessor = procura_vertice(grafo, id_sucessor);

        if (sucessor == NULL)
        {
            fprintf(stderr, "adiciona_adjacentes: sucessor nao encontrado no grafo\n");
            exit(EXIT_FAILURE);
        }

        aresta = cria_aresta(vertice, sucessor,peso);
        adiciona_aresta(vertice, aresta);

#ifdef DEBUG
        printf("\tvertice: %d\n", vertice_get_id(vertice));
        printf("\tsucessor: %d\n", id_sucessor);
        printf("\tpeso: %d\n", peso);
#endif

    }

    va_end (argumentos);
}

void exportar_grafo_dot(const char *filename, grafo_t *grafo)
{
    FILE *file;

    no_t *no_vert;
    no_t *no_arest;
    vertice_t *vertice;
    vertice_t *adjacente;
    arestas_t *aresta;
    arestas_t *contra_aresta;
    lista_enc_t *lista_arestas;

    int peso;

    if (filename == NULL || grafo == NULL)
    {
        fprintf(stderr, "exportar_grafp_dot: ponteiros invalidos\n");
        exit(EXIT_FAILURE);
    }

    file = fopen(filename, "w");

    if (file == NULL)
    {
        perror("exportar_grafp_dot:");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "graph {\n");

    //obtem todos os nos da lista
    no_vert = obter_cabeca(grafo->vertices);
    while (no_vert)
    {
        vertice = obter_dado(no_vert);

        //obtem todos as arestas
        lista_arestas = vertice_get_arestas(vertice);

        no_arest = obter_cabeca(lista_arestas);
        while (no_arest)
        {
            aresta = obter_dado(no_arest);

            //ignora caso já exportada
            if (aresta_get_status(aresta) == EXPORTADA)
            {
                no_arest = obtem_proximo(no_arest);
                continue;
            }

            //marca como exportada esta aresta
            aresta_set_status(aresta, EXPORTADA);
            adjacente = aresta_get_adjacente(aresta);

            //marca contra-aresta também como exporta no caso de grafo não direcionados
            contra_aresta = procurar_adjacente(adjacente, vertice);
            aresta_set_status(contra_aresta, EXPORTADA);

            //obtem peso
            peso = aresta_get_peso(aresta);

            fprintf(file, "\t%d -- %d [label = %d];\n",
                    vertice_get_id(vertice),
                    vertice_get_id(adjacente),
                    peso);

            no_arest = obtem_proximo(no_arest);
        }
        no_vert = obtem_proximo(no_vert);
    }
    fprintf(file, "}\n");
    fclose(file);
}


void libera_grafo (grafo_t *grafo)
{
    no_t *no_vert;
    no_t *no_arest;
    no_t *no_liberado;
    vertice_t *vertice;
    arestas_t *aresta;
    lista_enc_t *lista_arestas;

    if (grafo == NULL)
    {
        fprintf(stderr, "libera_grafo: grafo invalido\n");
        exit(EXIT_FAILURE);
    }

    //varre todos os vertices
    no_vert = obter_cabeca(grafo->vertices);
    while (no_vert)
    {
        vertice = obter_dado(no_vert);

        //libera todas as arestas
        lista_arestas = vertice_get_arestas(vertice);
        no_arest = obter_cabeca(lista_arestas);
        while (no_arest)
        {
            aresta = obter_dado(no_arest);

            //libera aresta
            free(aresta);

            //libera no da lsita
            no_liberado = no_arest;
            no_arest = obtem_proximo(no_arest);
            free(no_liberado);
        }

        //libera lista de arestas e vertice
        free(lista_arestas);
        free(vertice);

        //libera no da lista
        no_liberado = no_vert;
        no_vert = obtem_proximo(no_vert);
        free(no_liberado);
    }

    //libera grafo e vertice
    free(grafo->vertices);
    free(grafo);
}



