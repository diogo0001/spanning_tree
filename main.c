//
//  main.c
//  MinSpanTree
//
//  Created by Jose Nicolau Varela on 09/06/17.
//  Copyright (c) 2017 JOSÉ NICOLAU VARELA. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "In_Out_file.h"
#include "lista_enc.h"
#include "no.h"
#include"fila.h"
#include "pilha.h"

#include "grafo.h"

int main(void) {
    
    grafo_t *g;
    
     struc_arquivo_t* dado_arquivo;
    fila_t* fila_dado_arq;
    fila_dado_arq = cria_fila();
    pilha_t* pilha = NULL;
    pilha = cria_pilha();
   
    int pai_id = 0 ;
    int id = 0 ;
    int pai_aux= -1;
   
    int peso = 0;
    int tamanho = 0 ;
  
  //  int matriz_adj;
    
    lista_enc_t *lista_arquivo = NULL;
    
    lista_arquivo =  ler_arquivo("/Users/alvarogubert/Documents/Engenharia 2017-1/programacao II/ProgII projeto final/MinSpannTree/teste1.csv",& tamanho);
    
    printf("tamanho = %d   ", tamanho);
    g = cria_grafo(tamanho);
    
    
    no_t* no_arquivo = obter_cabeca(lista_arquivo);
    
    
    printf("IMPRIMINDO FILA FINAL\n");
    while (no_arquivo!= NULL) {
        dado_arquivo = obter_dado(no_arquivo);
        
        calcula_peso(dado_arquivo);
        
        pai_id = obtem_pai_id_dado_arquivo(dado_arquivo);
        id = obtem_id_dado_arquivo(dado_arquivo);
        peso = obtem_peso_dado_arquivo(dado_arquivo);
        
        
        printf("pai = %d  ", pai_id);
        printf("id = %d ", id);
        printf("peso = %d \n ",peso);
        
              push(dado_arquivo, pilha);
        
        
        
           enqueue(dado_arquivo, fila_dado_arq);
            
        
        no_arquivo = obtem_proximo(no_arquivo);
        
        
    }
  
    
 //   cria_adjacencia(g,1,2,10);
   // cria_adjacencia(g,1,5,15);
  //  cria_adjacencia(g,1,3,5);
  //  cria_adjacencia(g,2,5,7);
  //  cria_adjacencia(g,3,5,3);
  //  cria_adjacencia(g,3,4,1);
  //  cria_adjacencia(g,4,5,2);
    
    
    
    while (!fila_vazia(fila_dado_arq)) {
        
        
     struc_arquivo_t* dado =   dequeue(fila_dado_arq);
        
        pai_id = obtem_pai_id_dado_arquivo(dado);
        id = obtem_id_dado_arquivo(dado);
        peso = obtem_peso_dado_arquivo(dado);
        
        if (pai_aux != id) {
        
            cria_adjacencia(g, pai_id, id  ,peso);
        
            pai_aux = pai_id;}
        
        printf("pai = %d  ", pai_id);
        printf("id = %d ", id);
        printf("peso = %d \n ",peso);
        }
    
    
    int i = 0;
    
   while(i < tamanho){
        
       printf("\nVertice: %d  ----  Grau: %d", vertice_get_ide(g,i),vertice_get_grau(g,i));
       i++;
    }
    
   exportar_grafo_dot("/Users/alvarogubert/Documents/Engenharia 2017-1/programacao II/ProgII projeto final/teste1.dot", g,tamanho);
    
    /*    Adicionar arestas    *
     *------------------------ *
     *                         *
     *                         *
     *                         *
     *                         *
     *                         *
     ***************************/
    
    libera_grafo(g);
    libera_fila(fila_dado_arq);
    
    return EXIT_SUCCESS;
}
