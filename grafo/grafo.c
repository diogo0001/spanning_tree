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

#define DEBUG
#define DEBUG_K

#define INFINITO INT_MAX

struct grafos
{
    int id;
    lista_enc_t *vertices;
};

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
        printf("\tpeso: %d\n\n", peso);
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





arvore_t* kruskal(grafo_t *grafo)
{
    if (grafo == NULL)
    {
        perror("kruskal: ponteiro invalido.");
        exit(EXIT_FAILURE);
    }

    no_t *no, *no2;
    no = obter_cabeca(grafo->vertices);

    vertice_t *v,*u;
    fila_t *fila;
    lista_enc_t *lista;
    arestas_t *a, **a_sort;
    fila = cria_fila();

    int counter = 0;
    int grau = 0;
    int grau_count = 0;
    int i;
    arvore_t *tree;

    while (no)                              // varre a lista de vertices do grafo
    {
        v = (vertice_t*)obter_dado(no);     // obtem um vertice, pega a sua lista arestas e marca este como visitado (flag 1)
        vertice_set_visitado(v,1);          
        lista = vertice_get_arestas(v);     // obtem uma aresta, e assim até acabar a lista de arestas
        no2 = obter_cabeca(lista);

        #ifdef DEBUG_K
            printf("\n\nVertice id: %d\n", vertice_get_id(v));
        #endif

        while(no2)                                       // colocará todas as arestas em uma fila, sem repeli-las
        {
            a = (arestas_t*)obter_dado(no2);

            if(get_visitado(aresta_get_adjacente(a)) == 0){   // verifica se a aresta já foi visitada testando a flag "visitado" do  vertice adjacente                                                
               
                enqueue(a,fila);                         // add na fila
                counter++;                               // incrementa contador para debug
                
                #ifdef DEBUG_K
                printf("\nAresta numero: %d da fila", counter);
                printf("\nPeso: %d", aresta_get_peso(a));
                printf("\nAdjacente: %d \n", vertice_get_id(aresta_get_adjacente(a)));
                #endif
            }
            no2 = obtem_proximo(no2);
        }
        no = obtem_proximo(no);
    }

    fila = fila_de_prioridade(fila);            // criada a fila de arestas!! ...  aresta de menor peso na cabeça...
                                                // para cada iteração será necessário chamar a fila de prioridades para colocar o menor no topo.
                                                // pode não ser a opção mais eficiente, talvez uma única ordenação (heap) seja melhor, nesse caso
                                                // utilizar um array com índices poderia ser mais interessante (array de ponteiros duplos arestas_t **array) 
/*                      
    while(!fila_vazia(fila)){

        a = dequeue(fila);
        fila = fila_de_prioridade(fila);


    }
*/


    //free(lista);
    //free(fila);

    //printf("\nGrau: %d",grau);
    tree = cria_arvore(grau,0);

    return tree;
}


fila_t* fila_de_prioridade(fila_t* f)
{
    if(f == NULL)
    {
        perror("fila_de_prioridade");
        exit(1);
    }

    arestas_t *p1;
    arestas_t *p2;
    arestas_t *temp;
    arestas_t *minimo;
    fila_t *faux = cria_fila();

    p1 = dequeue(f);
    minimo = p1;                                   // apenas para inicializar com algum dado, no while ele sera atualizado

    while(!fila_vazia(f))                       // while para encontrar o maior
    {
        p2 = dequeue(f);

        if(aresta_get_peso(p1) < aresta_get_peso(p2))               // pega o maior da comparação entre os 2 consecutivos
            temp = p1;
        else
            temp = p2;

        if(aresta_get_peso(temp) < aresta_get_peso(minimo))            // compara o maior dos consecutivos com o maior geral
            minimo = temp;

        #ifdef DEBUG
            printf("\nPeso atual do topo: %d",aresta_get_peso(minimo));
        #endif //

        enqueue(p1,faux);                       // coloca na fila auxiliar na mesma ordem
        p1 = p2;
    }
    enqueue(p1,faux);                       // coloca o ultimo na fila

    enqueue(minimo,f);                         // coloca o mais velho na cabeça da fila

    #ifdef DEBUG
        printf("\n\nPeso do topo: %d",aresta_get_peso(minimo));
    #endif

    while(!fila_vazia(faux))
    {
        p1 = dequeue(faux);

        if(p1 != minimo){
            enqueue(p1,f);                      // compara os ponteiros, pois o minimo sera igual a algum elemento da fila, terá o mesmo endereço
            #ifdef DEBUG
            printf("\nEnqueue fila_de_prioridade: %d",aresta_get_peso(p1));
            #endif
        }
    }

    libera_fila(faux);

    return f;
}


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

    //free(lista);           //necessário adaptar caso necessáio utilizar
    free(fila);
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
        vertice_set_visitado(v,FALSE);
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
            vertice_set_visitado(u,TRUE);

            while(no2)
            {
                v = (vertice_t*)obter_dado(no2);
                push(v,pilha);
                no2 = obtem_proximo(no2);
            }
        }

        printf("\n\n");
    }
    //free(lista);                      //necessário adaptar caso necessáio utilizar
    free(pilha);   
}



