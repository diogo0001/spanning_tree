# spanning_tree
Minimum cost routes for telecom.

Minimum cost routes for telecom. Programa codificado em linguagem C e Processing.

Procedimento para instalação:

Crie uma pasta "lowCostMST2".

Clique em download para baixar os arquivos renomeie a pasta para "lowCostMST2".

Na IDE do processing crie um novo skecth e cole o codigo que esta no arquivo CodigoProcessing.txt, salve dentro de "lowCostMST2"
com o nome "MSTprocessing".

Copie o arquivo .png para dentro da pasta do processing (figura do mapa)

Os arquivos de entrada e saida de dados serão automaticamente criados na execução dos programas.
Poderá ocorrer erro se o caminho dos arquivos, não estiverem corretos. Ajuste para sua plataforma.

Monte o projeto em C em uma IDE apropriada.

Execução do Projeto para gerar Uma Minimum Spanning Tree, a partir de grafos:



1- Execute o programa no processing para fazer os testes.

Procedimento:

2- Clique na tela a posiçao desejada para os vertices.

3- Dois modos para inserir as arestas:

    a)  De todos os vertices para todos os vertices:
        Tecle "e" para criar as arestas automaticamente.
        Tecle "s "para salvar.
        
        Rode o programa em C para calcular MST.
        Volte para o processing e tecle r para carregar o MST.
        Tecle " c "para limpar a tela, se desejar.
 
    b) Escolher os adjacentes manualmente:
    
    Use as teclas UP ou DOUN para escolher o vertice. Observe no lado esquerdo da tela o id do vertice.
    Para inserir uma aresta, tecle o seu id e em seguida tecle "a".
    Feche os ciclos.
     Tecle "s "para salvar.
    
    
    Rode o programa em C para calcular MST.
    Volte para o processing e tecle r para carregar o MST.
    Tecle " c "para limpar a tela, se desejar.
    
    Para inserir uma figura de fundo de tela digite "i ".
    Voce poderá criar um grafo sobre o mapa e xecutar testes reais.
  
  
  #Testes
    
    Para verificar se o algoritmo está funcionando corretamente, foi utilizado o Processing para gerar o grafo e o arquivo que será lido pelo programa em C, e este criará um novo arquivo que será, por sua vez, lido no processing e sobreposto ao grafo original. Como o processing gera os pontos por coordenadas, é possível aferir os valores e as distâncias das arestas. Pode-se, a partir de um grafo conhecido, e também sua MST, conferir se o algoritmo gerou a árvore correta de forma também visual além dos valores. Conforme os testes feitos, todos apresentaram sucesso.
    
   #Benchmarks
  
  O algoritmo utilizado (Kruskal), é mais eficiente para grafos esparsos, mas funciona perfeitamente para grafos densos. No processing foi criada a função de inserir arestas onde desejado (cria-se um grafo esparso), e também a opção de conectar todos os vertices a todos com um único clique (E) (grafo denso). 
  A verificação do tempo de execução não apresentou muita diferença para os dois casos, porém foram feitos testes com números relativamente pequenos de vértices. Fazendo com grafos densos de 30 vértices (pior caso), os desempenhos obtidos na execução do programa (não consideranto overhead da parte do processing, apenas a execução do código) em duas máquinas diferentes foram:
    
    Máquina:
    Processador Intel(R) Core(TM)2Duo CPU T6400 @ 2.00 GHz
    Ram: 3,00 GB
    - Output file is bin\Debug\MST.exe with size 79.42 KB
    
    Para grafo denso (todos os vértices):
    - 1,04 s para grafo denso;
    
    Para um grafo esparso (Até 3 arestas média):
    -0,280 s;
    
    

   
