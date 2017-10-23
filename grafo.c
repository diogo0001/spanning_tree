
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include "grafo.h"
#include "sub_arvore.h"
#include "vertice.h"
#include "lista_enc.h"
#include "no.h"
#include "fila.h"

#include "pilha.h"

#define FALSE 0
#define TRUE 1

//#define DEBUG
#define DEBUG_K
//#define DEBUG_PRE_FIND

//#define DEBUG_FIND
#define DEBUG_UNION

#define INFINITO INT_MAX

struct grafos
{
    int id;
    int n_vertices;
    lista_enc_t *vertices;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

arvore_t* kruskal(grafo_t *grafo)               
{
    if (grafo == NULL)
    {
        perror("kruskal: ponteiro invalido.");
        exit(EXIT_FAILURE);
    }

    no_t *no, *no2;
    no = obter_cabeca(grafo->vertices);

    vertice_t *u,*v;
    fila_t *fila;
    lista_enc_t *lista;
    arestas_t *a, **a_sort;   //**mst; não utulizada no momento
    fila = cria_fila();

    int counter_arestas = 0;
    int vert_count = 0;
    int grau_count;
    int i;
    arvore_t *spanning_tree = cria_arvore();
    sub_arvore_t *sub_tree, *s_u, *s_v;
    sub_arvore_t *raiz = NULL;

    #ifdef DEBUG_K
    printf("\n\nKRUSKAL *******************************************************************\n");
    #endif

    while (no)                              // varre a lista de vertices do grafo  ----MAKE-SET  (cria as sub árvores)
    {
        v = (vertice_t*)obter_dado(no);     // obtem um vertice, pega a sua lista arestas e marca este como visitado (flag 1)
        vertice_set_visitado(v,1);
        lista = vertice_get_arestas(v);     // obtem uma aresta, e assim até acabar a lista de arestas
        no2 = obter_cabeca(lista);

        sub_tree = cria_sub(spanning_tree,vertice_get_id(v),v);       // adiciona sub_arvore à floresta inicial
        arvore_set_boss(sub_tree,sub_tree);                            // seta como pai de si mesma
        vertice_set_link(v,sub_tree);                                 // aponta o vertice para a sub_arvore para elas ficares mutuamente conectadas

        grau_count = 0;

        #ifdef DEBUG_K
            printf("\n\nVertice id: %d\n", vertice_get_id(v));
        #endif


        while(no2)                                       // colocará todas as arestas em uma fila, sem repeli-las, para a ordenação
        {
            a = (arestas_t*)obter_dado(no2);

            if(get_visitado(aresta_get_adjacente(a)) == 0){   // verifica se a aresta já foi visitada testando a flag "visitado" do  vertice adjacente

                enqueue(a,fila);                         // add na fila para colocar no array, é necessário varrer uma vez para contar o tamanho do array
                counter_arestas++;                               // incrementa contador para o malloc do array.. quantos elementos há na fila (arestas)

                #ifdef DEBUG_K
                printf("\nAresta numero: %d da fila", counter_arestas);
                printf("\nPeso: %d", aresta_get_peso(a));
                printf("\nAdjacente: %d \n", vertice_get_id(aresta_get_adjacente(a)));
                #endif
            }
            grau_count++;
            no2 = obtem_proximo(no2);
        }

        vertice_set_grau(v,grau_count);                 // seta o numero de arestas em cada vertice
        vert_count++;
        no = obtem_proximo(no);
    }

#ifdef DEBUG_K
    printf("\n%ditens na fila\n",counter_arestas);
    printf("%d vertices\n",vert_count);
#endif

    grafo->n_vertices = vert_count;                      // salva no grafo o seu numero de vertices

    a_sort = malloc(sizeof(int*)*(counter_arestas+1));           // cria o array de ponteiros para as arestas


    if(a_sort == NULL ){
        perror("a_sort/mst malloc");
        exit(EXIT_FAILURE);
    }

    for(i=0; i<counter_arestas; i++){               // criado um array para fazer a ordenação de modo mais eficiente
        a_sort[i] = dequeue(fila);          // coloca os elementos da fila no array, mesmo sendo necessário mais uma varredura, a função heap é mais eficiente
        #ifdef DEBUG_K                      // com array de índices, é uma escolha em detrimento de outra
        printf("\n%d", aresta_get_peso(a_sort[i]));
        #endif
    }

    a_sort = heap(a_sort, counter_arestas);         // o heap sort ordena o array

#ifdef DEBUG_K
    printf("\n");
    for(i=0; i<counter_arestas; i++) {
        printf("\n%d", aresta_get_peso(a_sort[i]));
        //enqueue(a_sort[i],fila);
    }
#endif // DEBUG_K

//                                                  Até aqui --------- MAKE SET   E ORDENAÇÃO    
//-----------------------------------------------------------------------------------------------------------------------
    #ifdef DEBUG_K
    printf("\nAte ordenacao OK!\n");
    printf("\n%d vertices\n",vert_count);
    #endif

    i = 0;                                            

    while(i < counter_arestas){                   

        a = a_sort[i];                                // pega a aresta de menor peso
        u = aresta_get_fonte(a);
        v = aresta_get_adjacente(a);                  // pega os vertices da aresta

        #ifdef DEBUG_K
            printf("\n\n\n*****************************************************************************");
            printf("\n***** Id aresta u -- v: %d -- %d  ----------- sub_counter %d  ******",vertice_get_id(u),vertice_get_id(v),i+1);
        #endif

        s_u = vertice_get_link(u);
        s_v = vertice_get_link(v);

        s_u = find(s_u);
        s_v = find(s_v);


        #ifdef DEBUG_K
        printf("\nRetorno find():");
        printf("\ns_u: %2d  ---- boss: %2d ",vertice_get_id(u),sub_arvore_get_id(s_u));
        printf("\ns_v: %2d  ---- boss: %2d \n",vertice_get_id(v),sub_arvore_get_id(s_v));
        #endif // DEBUG_FIND

        if(s_v != s_u){

            raiz = v_union(s_u,s_v,u,v);
            arvore_set_n_sub(spanning_tree,arvore_get_n_sub(spanning_tree)+1);     // seta o numero de sub_arvores a arvore contem.. deve ser igual ao num de vertices do grafo
        }

        #ifdef DEBUG_K
        printf("Level raiz: %d ",get_level(raiz));
        #endif

        i++;
    }

    spanning_tree = set_raiz(spanning_tree ,raiz);
    arvore_set_level(spanning_tree, get_level(raiz));
    arvore_set_n_sub(spanning_tree,arvore_get_n_sub(spanning_tree)+1); 

    //free(lista);
    //free(fila);
    printf("\n\nDados da funcao kruskal:");
    printf("\nId raiz   : %d ",sub_arvore_get_id(raiz));
    printf("\nNivel maximo: %d ",get_level(raiz));
    printf("\nN subs: %d\n",arvore_get_n_sub(spanning_tree));

#ifdef DEBUG_K
printf("\n\nEND_KRUSKAL -----------------------------------------------------------------------\n");
#endif


    return spanning_tree;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  FIND OK!

sub_arvore_t* find(sub_arvore_t* v)
{
    if(v == NULL){
        perror("find");
        exit(1);
    }

#ifdef DEBUG_FIND
printf("\n\nFIND --------------------------------------------------------------------");
#endif

    sub_arvore_t *v0, *boss_v0;
    //int level;
    v0 = v;

    boss_v0 = arvore_get_boss(v0);
    //inc_level(boss_v0);

    #ifdef DEBUG_FIND
    printf("\nLevel: %2d  ---- sub: %2d  ---- boss: %2d ",get_level(v0),sub_arvore_get_id(v0),sub_arvore_get_id(boss_v0));
    #endif // DEBUG_FIND

    if(v0 == boss_v0)
        return v0;

    v0 = boss_v0;

    v0 = find(v0);

    return v0;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Função v_union:  Problemas ao conectar pais e filhos, pois o algoritmo gera uma arvore que não condiz com as conexões do grafo, é uma questão do algoritmo.
//                  Está sendo tentado estabelecer as conexões verdadeiras entre os vertices(sub_arvores)

sub_arvore_t* v_union(sub_arvore_t* u0, sub_arvore_t* v0, vertice_t* u, vertice_t* v)
{
    if(v0 == NULL || u0 == NULL){
        perror("v_union");
        exit(1);
    }

#ifdef DEBUG_UNION
printf("\n\nUNION ---------------------------------------------------------------------");
#endif

    sub_arvore_t* boss;
    sub_arvore_t *link_v0, *link_u0;

    link_u0 = vertice_get_link(u);          // faz o link do ponteiro da sub_arvore cque corresponde ao vértice
    link_v0 = vertice_get_link(v);

    if(get_level(v0) > get_level(u0)){
        boss = v0;
        //set_level(v0,(get_level(v0)+1));
        //add_filho(v0,u0);
        inc_level(v0);
        arvore_set_boss(u0,boss);

            add_filho(link_v0,link_u0);
    }

    else{
        boss = u0;
        //set_level(u0, (get_level(u0)+1));
        //add_filho(u0,v0);
        inc_level(u0);
        arvore_set_boss(v0,boss);
        //add_filho(link_v0,link_u0);
        
            add_filho(link_u0,link_v0);

    }

    #ifdef DEBUG_UNION
        printf("\nId u0  : %2d  ----  Level u0  : %2d  ----  Filhos u0  : %2d",sub_arvore_get_id(u0),get_level(u0),pai_get_n_filhos(u0));
        printf("\nId v0  : %2d  ----  Level v0  : %2d  ----  Filhos v0  : %2d",sub_arvore_get_id(v0),get_level(v0),pai_get_n_filhos(v0));
        printf("\nId boss: %2d  ----  Level boss: %2d  ----  Filhos boss: %2d\n",sub_arvore_get_id(boss), get_level(boss),pai_get_n_filhos(boss));
    #endif

    return boss;
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Funções do heap sort com array

arestas_t** heap(arestas_t **vet,int tm)
{
    if(vet == NULL){
        perror("heap");
        exit(1);
    }

    int i;
    for(i = (tm-1)/2; i>=0; i--)
        vet = heapfy(vet,i,tm-1);

    for(i = tm-1; i>-1; i--){
        vet = swapp(vet,i,0);
        vet = heapfy(vet,0,i-1);
    }
    return vet;

}



arestas_t** heapfy(arestas_t **vet, int ini, int fim)
{
    if(vet == NULL){
        perror("heapfy");
        exit(1);
    }

    arestas_t* aux = vet[ini];
    int j = ini*2 + 1;

    while(j < fim)
    {
        if(aresta_get_peso(vet[j]) < aresta_get_peso(vet[j+1]))
            j++;

        if(aresta_get_peso(aux) < aresta_get_peso(vet[j]))
        {
            vet[ini] = vet[j];
            ini = j;
            j = 2*ini + 1;
        }
        else
            j = fim + 1;
    }
    vet[ini] = aux;

    return vet;
}


arestas_t** swapp(arestas_t** vet, int i, int j)
{
    if(vet == NULL){
        perror("swapp");
        exit(1);
    }

    arestas_t* aux = vet[i];
    vet[i] = vet[j];
    vet[j] = aux;

    return vet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
    p->n_vertices = 0;
    p->vertices = cria_lista_enc();

    return p;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
vertice_t* grafo_adicionar_vertice(grafo_t *grafo, int id)
{
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

    vertice_t *vertice;
    no_t *no;

#ifdef DEBUG
    printf("grafo_adicionar_vertice: %d\n", id);
#endif

    vertice = cria_vertice(id);
    no = cria_no(vertice);

    add_cauda(grafo->vertices, no);

    return vertice;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

    vertice_set_grau(vertice, (n/2));

    va_end (argumentos);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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




///////////////////////////////////////////////////////////////////////////////////////////


void exportar_grafo_csv(const char *filename, grafo_t *grafo)
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
        perror("exportar_graf_csv:");
        exit(EXIT_FAILURE);
    }
    
    fprintf(file, "id_pai,coordX,coordY,id,coordX1,coordY1\n");
    printf("HERE1");
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
            
            fprintf(file, "%d,%d,%d,%d,%d,%d \n",
                    vertice_get_id(vertice),peso,peso,
                    vertice_get_id(adjacente),
                    peso,peso);
            printf("HERE");
            no_arest = obtem_proximo(no_arest);
        }
        no_vert = obtem_proximo(no_vert);
    }
   // fprintf(file, "}\n");
    fclose(file);
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void libera_tudo (grafo_t *grafo, arvore_t* tree)
{
    no_t *no_vert;
    no_t *no_arest;
    no_t *no_liberado;
    vertice_t *vertice;
    arestas_t *aresta;
    lista_enc_t *lista_arestas;


    no_t *no_sub;
    no_t *no_filhos;
    no_t *no_liberado_sub;
    sub_arvore_t* sub;
  
    lista_enc_t *lista_filhos;
    lista_enc_t *sub_arvores;


    #ifdef DEBUG_FREE
    int counter_alloc = 0;
    printf("\n\nLibera tudo ***********");
    #endif



    if (grafo == NULL || tree == NULL)
    {
        fprintf(stderr, "libera_tudo\n");
        exit(EXIT_FAILURE);
    }

    no_sub = arvore_get_cabeca(tree);

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


        sub = obter_dado(no_sub);
        no_filhos = sub_get_cabeca(sub);
        
        while (no_filhos)
        {
            no_liberado_sub = no_filhos;

            no_filhos = obtem_proximo(no_filhos);
            free(no_liberado_sub);


            #ifdef DEBUG_FREE
            counter_alloc++;
            printf("\nno_filhos %d alloc",counter_alloc);
            #endif

        }

        free(lista_filhos);
        no_liberado_sub = no_sub;
        no_sub = obtem_proximo(no_sub);
        free(no_liberado_sub);

        #ifdef DEBUG_FREE
        counter_alloc +=2;
        printf("\nlista e sub %d alloc",counter_alloc);
        #endif

        //libera no da lista
        no_liberado = no_vert;
        no_vert = obtem_proximo(no_vert);
        free(no_liberado);
    }

    //libera grafo e vertice
    free(grafo->vertices);
    free(grafo);

    libera_arv(tree);
}


/*
int grafo_get_tam(grafo_t* g){

     if(g == NULL){
        perror("grafo_get_tam");
        exit(EXIT_FAILURE);
    }

    return g->n_vertices;
}
*/
/*

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
sub_arvore_t* pre_find(arvore_t* tree, vertice_t* v)
{
    if(v == NULL || tree == NULL){
        perror("pre_find");
        exit(1);
    }

#ifdef DEBUG_PRE_FIND
printf("\n\nPRE_FIND -------------------------------------------------------------------------");
#endif

    sub_arvore_t* sub;
    vertice_t* v0;
    no_t* no = arvore_get_cabeca(tree);

    sub = obter_dado(no);
  //  v0 =  sub_get_dado(sub);

    while(vertice_get_id(v) != vertice_get_id(v0)){
        sub = obter_dado(no);
        v0 = sub_get_dado(sub);
        no = obtem_proximo(no);

        #ifdef DEBUG_PRE_FIND
        printf("\nId vertice: %2d /----/ Id sub: %d\n",vertice_get_id(v),sub_arvore_get_id(sub));
        #endif
    }


    #ifdef DEBUG_PRE_FIND
        printf("\nId vertice 0: %2d /----/ Id sub: %d\n",vertice_get_id(v0),sub_arvore_get_id(sub));
    #endif

    sub = find(sub);

    #ifdef DEBUG_PRE_FIND
        printf("\nReturn %d\n",sub_arvore_get_id(sub));
    #endif

    return sub;
}


*/


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Funções não utilizadas
/*
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
    fila_t *fila;
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


    }

    //free(lista);
    free(fila);

    printf("\nGrau: %d",grau);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
    //free(lista);
    free(pilha);

}
*/


/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void prim(grafo_t* grafo)
{
    if (grafo == NULL)
    {
        perror("prim: ponteiro invalido.");
        exit(EXIT_FAILURE);
    }

    no_t *no, *no2;
    no = obter_cabeca(grafo->vertices);

    vertice_t *v;
   // fila_t *fila, *fila_v;
    lista_enc_t *lista;
    arestas_t *a, **a_sort, **mst;
   // fila = cria_fila();
   // fila_v = cria_fila();

    int dest, primeiro, menorpeso;
    int vert_count = 0;
    int grau = 0;
    //int grau_count = 0;
    int i,j;
    //arvore_t *tree;

    #ifdef DEBUG_P
    printf("\n\nPRIM *******************************************************************\n");
    #endif

    while (no)                              // varre a lista de vertices do grafo
    {
        v = (vertice_t*)obter_dado(no);     // obtem um vertice, pega a sua lista arestas e marca este como visitado (flag 1)
       // enqueue(v,fila_v);                  // salva numa fila para passar para funções
        no2 = obter_cabeca(lista);

        #ifdef DEBUG_K
            printf("\n\nVertice id: %d\n", vertice_get_id(v));
        #endif
        vert_count++;
        no = obtem_proximo(no);
    }

    int pai[vert_count];

    for(i=0; i<=vert_count; i++)
        pai[i] = -1;

    pai[0] = 0;

    while(1){

        primeiro = 1;

        for(i=0; i<=vert_count; i++){

            lista = vertice_get_arestas(v);
            no = obter_cabeca(lista);

            if(pai[i] != -1){


                for(j=0; j<vertice_get_grau(v); j++){

                    a = obter_dado(no);

                    if(pai[])

                        if(primeiro){
                            menorpeso =
                    }
                }
            }
        }

        if(primeiro) break;

        pai[dest] = [0];

    }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/
