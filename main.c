//
///  main.c
///  LowCostMST
//
///  Created by Jose Nicolau Varela e Diogo Tavares on 19/06/17.

//

#include <stdio.h>
#include <stdlib.h>
#include "In_Out_file.h"
#include "lista_enc.h"
#include "no.h"
#include "fila.h"
#include "grafo.h"
#include "sub_arvore.h"
#include <time.h>
#include <limits.h>
#include <windows.h>

int main(int argc, const char * argv[]) {


float tmili = 0;
float soma = 0;
float media;
time_t t_inicial, t_final;
int inicio, finaliza;

inicio = GetTickCount();   ///Obtem tempo inicial
///-----------------------------------------------------------------------------------------------------
    grafo_t *g;
    g = cria_grafo(1);
    vertice_t* v = NULL;
    vertice_t* u = NULL;

     sub_arvore_t* sub;
     arvore_t* MST;

    struc_arquivo_t* dado_arquivo;       //para guardar dados do arquivo

    int pai_id = 0 ;
    int id = 0 ;


    int peso = 0;
    int tamanho = 0 ;

    lista_enc_t *lista_arquivo = NULL;  // cria uma lista para retorno dos dados lidos

  //  lista_arquivo =  ler_arquivo("/Users/alvarogubert/Documents/Engenharia 2017-1/programacao II/ProgII projeto final/MinSpannTree/lowCostMST/lowCostMST2/lowCostMST2/centroFpolis.csv",& tamanho);

    lista_arquivo =  ler_arquivo("MSTprocessing/teste_entrada.csv",& tamanho);


    no_t* no_arquivo = obter_cabeca(lista_arquivo);

    while (no_arquivo!= NULL) {
        dado_arquivo = obter_dado(no_arquivo);

        calcula_peso(dado_arquivo);

        pai_id = obtem_pai_id_dado_arquivo(dado_arquivo);
        id = obtem_id_dado_arquivo(dado_arquivo);
        peso = obtem_peso_dado_arquivo(dado_arquivo);


      //  printf("pai = %d  ", pai_id);
      //  printf("id = %d ", id);
      //  printf("peso = %d \n ",peso);

        if(procura_vertice(g, pai_id)== NULL){

            v = grafo_adicionar_vertice(g, pai_id);}
        else{
            v = procura_vertice(g, pai_id);
        }

        if(procura_vertice(g, id)== NULL) {

            u = grafo_adicionar_vertice(g, id);}
        else{
           u = procura_vertice(g, id);
        }

        adiciona_adjacentes(g, v, 2,id,peso);
        adiciona_adjacentes(g, u, 2,pai_id,peso);


        no_arquivo = obtem_proximo(no_arquivo);

inicio = GetTickCount();
    }

    MST = kruskal(g);        // kruskal retorna a arvore criada


    exportar_arvore_cvs("MSTprocessing/testeSaida2.csv", MST,lista_arquivo);

finaliza = GetTickCount();
tmili = finaliza - inicio;
printf (" tempo aproximado  de execução em C = %f",tmili);

  libera_tudo (g, MST);
    
return 0;
}

