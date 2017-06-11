/*
 * grafo.c
 *
 *  Created on: Nov 18, 2016
 *      Author: xtarke
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "grafo.h"
#include "vertice.h"

//#define DEBUG
#define DEBUG_PRIM

struct vertices
{
    int id;
    int grau;
    /* mais informacoes, se necessario */
};

struct arestas
{
    int peso_adj;
    //int peso;
    /* mais informacoes, se necessario */
};

struct grafos
{
    int n_vertices;
    vertice_t *vertices;
    aresta_t **matriz_adj;	/* Matriz de adjacencia */
};


grafo_t *cria_grafo(int vertices)
{
    int i;
    aresta_t **matriz_adj;
    grafo_t *g = malloc(sizeof(grafo_t));
    
    if (g == NULL)
    {
        perror("cria_grafo (g)");
        exit(EXIT_FAILURE);
    }
    
    g->n_vertices = vertices;
    g->vertices = malloc(vertices * sizeof(vertice_t));
    
    if (g->vertices == NULL)
    {
        perror("cria_grafo (vertices)");
        exit(EXIT_FAILURE);
    }
    
    memset(g->vertices, 0, vertices * sizeof(vertice_t));
    
    matriz_adj = malloc(vertices * sizeof(aresta_t *));
    
    if (matriz_adj == NULL)
    {
        perror("cria_grafo (matriz_adj)");
        exit(EXIT_FAILURE);
    }
    
#ifdef DEBUG
    printf("\nCRIA GRAFO  -------------------------------------------------------------");
#endif
    
    for ( i = 0; i < vertices; i++ )
    {
        matriz_adj[i] = malloc(vertices * sizeof(vertice_t));
        
        if (matriz_adj[i] == NULL)
        {
            perror("cria_grafo (matriz_adj[i])");
            exit(EXIT_FAILURE);
        }
        
        g->vertices[i].grau = 0;
        g->vertices[i].id = i+1;
#ifdef DEBUG
        printf("\nCriado vertice %d",vertice_get_ide(g,i));
#endif
    }
    
    g->matriz_adj = matriz_adj;
    
    //nenhuma adjacencia
    memset(g->matriz_adj,0,sizeof(vertices * sizeof(vertice_t)));
    
#ifdef DEBUG
    printf("\nCRIA GRAFO END ----------------------------------------------------------\n");
#endif
    
    return g;
}


///////////////////////////////////////////////////////////////////////////////////////////
int cria_adjacencia(grafo_t *g, int u, int v, int peso)
{
    if (g == NULL)
        return FALSE;
    
    if (u > g->n_vertices || v > g->n_vertices )
        return FALSE;
    
   //  g->matriz_adj[u][v].adj = TRUE;
    g->matriz_adj[u][v].peso_adj = peso;                // a adjacência será diretamente definida pelo peso.. se não houver adjacência FALSE == -1
    g->matriz_adj[v-1][u-1].peso_adj = peso;
    g->vertices[v-1].grau++;
    g->vertices[u-1].grau++;
    
#ifdef DEBUG
    printf("\nCriada aresta %d - %d  ----   peso: %d  ",u,v,peso);
#endif
    
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////
int vertice_set_id(grafo_t* g, int id, int v)
{
    if (g == NULL || v > g->n_vertices)
        return FALSE;
    
    g->vertices[v].id = id;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////
int vertice_get_ide(grafo_t* g, int v)
{
    if (g == NULL || v > g->n_vertices)
        return FALSE;
    
    return g->vertices[v].id;
}


/////////////////////////////////////////////////////////////////////////////////////
int vertice_get_grau(grafo_t* g, int v)
{
    if (g == NULL || v > g->n_vertices)
        return FALSE;
    
    return g->vertices[v].grau;
}

///////////////////////////////////////////////////////////////////////////////////////
int aresta_get_pesos(grafo_t* g, int u, int v)
{
    if (g == NULL)
        return FALSE;
    
    if (u > g->n_vertices || v > g->n_vertices )
        return FALSE;
    
    return g->matriz_adj[u][v].peso_adj;
}


/////////////////////////////////////////////////////////////////////////////////////
int rem_adjacencia(grafo_t *g, int u, int v)
{
    if (g == NULL)
    {
        return FALSE;
    }
    
    if (u > g->n_vertices || v > g->n_vertices)
        return FALSE;
    
    g->matriz_adj[u][v].peso_adj = FALSE;
    
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////
int adjacente(grafo_t *g, int u, int v)
{
    
    if (u > g->n_vertices || v > g->n_vertices)
        return FALSE;
    
    return (g->matriz_adj[u][v].peso_adj);
}


//////////////////////////////////////////////////////////////////////////////////////////
void libera_grafo (grafo_t *g)
{
    int i;
    
    if (g == NULL)
    {
        perror("libera_grafo");
        exit(EXIT_FAILURE);
    }
    
    for (i=0; i < g->n_vertices; i++)
        free(g->matriz_adj[i]);
    
    free(g->matriz_adj);
    free(g->vertices);
    free(g);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void exportar_grafo_dot(const char *filename, grafo_t *grafo, int tamanho)
{
    FILE *file;
    int v;
    int u;
    
   
    
    if (filename == NULL || grafo == NULL){
        fprintf(stderr, "exportar_grafp_dot: ponteiros invalidos\n");
        exit(EXIT_FAILURE);
    }
    
    file = fopen(filename, "w");
    
    if (file == NULL){
        perror("exportar_grafp_dot:");
        exit(EXIT_FAILURE);
    }
    
    fprintf(file, "graph {\n");
    
    //obtem todos os nos da lista
    
    for ( u = 0 ;u< tamanho -1; u++) {
        
       for ( v = u +1 ;v< tamanho; v++){
           
           
    
            fprintf(file, "\t%d -- %d [label = %d];\n",
                    vertice_get_ide(grafo,u),
                   vertice_get_ide(grafo,v),
            
                  aresta_get_pesos(grafo,  vertice_get_ide(grafo,u), vertice_get_ide(grafo,v)) );
           }
        
    }
    
    
       

    
    fprintf(file, "}\n");
    fclose(file);
}

