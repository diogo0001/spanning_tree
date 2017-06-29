//
//  In_Out_file.h
//  MinSpanTree
//
//  Created by Jose Nicolau Varela on 09/06/17.
//  Copyright (c) 2017 JOSÉ NICOLAU VARELA. All rights reserved.
//

#ifndef __MinSpanTree__In_Out_file__
#define __MinSpanTree__In_Out_file__

#include <stdio.h>

#include "/grafo/grafo.h"
#include "/lista_enc/lista_enc.h"
#include "/lista_enc/no.h"

typedef struct struc_arquivo struc_arquivo_t;

lista_enc_t* ler_arquivo(char *nomeArquivo, int *tamanho);
void calcula_peso(struc_arquivo_t* coord1);
struc_arquivo_t* cria_coord (int pai_id,int x, int y,int ide, int x2,int y2, float peso);
int obtem_pai_id_dado_arquivo(struc_arquivo_t* dado_arquivo);
int obtem_x_dado_arquivo(struc_arquivo_t* dado_arquivo);
int obtem_y_dado_arquivo(struc_arquivo_t* dado_arquivo);
int obtem_peso_dado_arquivo(struc_arquivo_t* dado_arquivo);
int obtem_id_dado_arquivo(struc_arquivo_t* dado_arquivo);
int obtem_x2_dado_arquivo(struc_arquivo_t* dado_arquivo);
int obtem_y2_dado_arquivo(struc_arquivo_t* dado_arquivo);
void exportar_arvore_cvs(const char *filename, arvore_t *MST,lista_enc_t* lista_coord);

#endif /* defined(__MinSpanTree__In_Out_file__) */
