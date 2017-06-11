//
//  In_Out_file.c
//  MinSpanTree
//
//  Created by Jose Nicolau Varela on 09/06/17.
//  Copyright (c) 2017 JOSÉ NICOLAU VARELA. All rights reserved.
//
#include <stdlib.h>
#include <stdio.h>
#include "math.h"
#include "In_Out_file.h"
#include "lista_enc.h"
#include "no.h"


struct struc_arquivo {
    int tamanho;
    int pai_id;
    int coordX;
    int coordY;
    int id;
    int coordX2;
    int coordY2;
    int peso;
    int mark;
    
};

struc_arquivo_t* cria_coord (int pai_id,int x, int y,int ide, int x2,int y2, float peso){
    
    struc_arquivo_t* dado = NULL;
    
    dado = (struc_arquivo_t*) malloc(sizeof(struc_arquivo_t));
    dado->tamanho = 0;
    dado->pai_id = pai_id;
    dado->id = ide;
    dado->coordX = x;
    dado->coordY = y;
    dado->coordX2 = x2;
    dado->coordY2 = y2;
    dado->peso = 0;
    dado->mark = -1;
    
    return dado;
}




lista_enc_t* ler_arquivo(char *nomeArquivo, int *tamanho){
    lista_enc_t  *lista = NULL;
    int i;
    char buffer[80];
    int tam = 0;
    
    FILE *fp = fopen(nomeArquivo, "r");
    
    if (fp == NULL){
        
        perror("Erro em main: fopen");
        exit(EXIT_FAILURE);
    }
    fgets(buffer,80,fp);
    
    while (1){
        
        
        fgets(buffer,80,fp);
        if (feof(fp)) break;
        
        tam++;
       
    }
    printf("tamanho = %d\n",tam);
    rewind(fp);
    fgets(buffer,80,fp);
 
    int pai_id;
    int x2;
    int y2;
    int x;
    int y;
    int id ;
    int peso = 0 ;
    
    
    
    lista =   cria_lista_enc();
    
    
    if (lista == NULL){
        
        perror("Erro em main: lista");
        exit(EXIT_FAILURE);}
    
    for (i = 0 ; i<tam ; i++){
        
        fgets(buffer,80,fp);
        
        sscanf (buffer,"%d,%d,%d,%d,%d,%d",&pai_id,&x,&y,&id,&x2,&y2);
        
        
        
        struc_arquivo_t* dado = cria_coord(pai_id,x,y,id,x2,y2,peso);
 
        printf("pai_id = %d  x = %d  y = %d  id = %d  x2 = %d  y2 = %d  peso = %d\n",dado->pai_id,dado->coordX,dado->coordY,dado->id, dado->coordX2, dado->coordY2, dado->peso);
        
        no_t* no = cria_no(dado);
        
        add_cauda(lista,no);
        
        //   no = obtem_proximo(no);
         *tamanho = dado->id;
    }
    
    fclose(fp);
   
    
    return lista;
    
    
    
    
    
    
}

void calcula_peso(struc_arquivo_t* coord1){
    
    int x = coord1->coordX;
    int y = coord1->coordY;
    int x2 = coord1->coordX2;
    int y2 = coord1->coordY2;
    
    
    
    
    int peso = sqrtf(((x-x2)*(x-x2))+((y-y2)*(y-y2)));
    
    
    
    coord1->peso = peso;
    
    
    
}

int obtem_pai_id_dado_arquivo(struc_arquivo_t* dado_arquivo){
    
    int pai_id = dado_arquivo->pai_id;
    //   printf ("dentro da funcao %d", ide);
    return pai_id;
    
}

int obtem_x_dado_arquivo(struc_arquivo_t* dado_arquivo){
    
    int x = dado_arquivo->coordX;
    return x;
    
}

int obtem_y_dado_arquivo(struc_arquivo_t* dado_arquivo){
    
    int y = dado_arquivo->coordY;
    return y;
    
}

int obtem_peso_dado_arquivo(struc_arquivo_t* dado_arquivo){
    
    int peso = dado_arquivo->peso;
    return peso;
    
    
}

int obtem_id_dado_arquivo(struc_arquivo_t* dado_arquivo){
    
    
    return dado_arquivo->id;
}

