
#ifndef GRAFO_H_
#define GRAFO_H_


#define MAX_VERTICES 50

#define TRUE 1
#define FALSE -1

typedef struct vertices vertice_t;
typedef struct arestas aresta_t;
typedef struct grafos grafo_t;

grafo_t *cria_grafo(int vertices);
void libera_grafo (grafo_t *g);
int cria_adjacencia(grafo_t *g, int u, int v, int peso);
int rem_adjacencia(grafo_t *g, int u, int v);
int adjacente(grafo_t *g, int u, int v);
int aresta_get_pesos(grafo_t* g, int u, int v);
int vertice_get_grau(grafo_t* g, int v);
int vertice_set_id(grafo_t* g, int id, int v);
int vertice_get_ide(grafo_t* g, int v);
void exportar_grafo_dot(const char *filename, grafo_t *grafo,int tamanho);


#endif /* GRAFO_H_ */