#include <iostream>
#include <fstream>
#include <vector>

std::vector< std::vector<int> > matrizArs; // matrizArs: armazena as arestas do grafo
std::vector< std::vector<int> > matrizDist; // matrizDist: armazena a distância mínima de cada ponto no grafo

typedef struct no_{ // no: elemento da lista de elementos intermediários
    int p; // p: elemento intermediário entre a distância de dois vértices
    struct no_ *proximo; // proximo: aponta para o próximo elemento intermediário da lista ou nulo, se não houver
}no;

typedef struct id_{ // id: índice da lista de elementos intermediários
    int a; // a: primeiro vértice
    int b; // b: último vértice
    no *lista; // lista: vetor que aponta para a lista de elementos intermediários entre o caminho dos vértices 'a' e 'b'
    struct id_ *proximoid; // proximoid: aponta para o próximo cabeçalho de lista de elementos intermediários
}id;

using namespace std;

void adiciona_lista(id *topo, int pa, int pb, int x){ // Adiciona um Vértice à Lista de Elementos Intermediários de um Determinado Caminho
    for(id *auxid = topo; auxid != NULL; auxid = auxid->proximoid)
        if((pa == auxid->a && pb == auxid->b) || (pa == auxid->b && pb == auxid->a)){ // Busca os Índices Corretos
            no *auxno;
            if(auxid->lista == NULL){ // Se for o primeiro elemento da Lista, insere ali
                auxid->lista = new(no);
                auxno = auxid->lista;
            }
            else{ // Se não for o primeira, percorre a Lista até um espaço livre
                auxno = auxid->lista;
                for(; auxno->proximo != NULL; auxno = auxno->proximo);
                if(auxno->p != x){
                    auxno->proximo = new(no);
                    auxno = auxno->proximo;
                    auxno->p = x;
                    auxno->proximo = NULL;
                }
            }
        }
}

void inicia_lista(id *topo, int l){ // Inicializa os Índices da Lista de Elementos Intermediários
    topo->a = 0;
    topo->b = 0;
    topo->lista = NULL;
    topo->proximoid = NULL;
    id *aux = topo;
    for(int a = 0; a < l; a++) // Cria um Vetor-Matriz com l * l Índices
        for(int b = 0; b < l; b++){
            topo->proximoid = new(id);
            topo = topo->proximoid;
            topo->a = a;
            topo->b = b;
            topo->lista = NULL;
            topo->proximoid;
        }
}

int main(void){
    id *inicio = new(id); // inicio: nó que aponta ao primeiro elemento da lista de elementos intermediários
    string line; // line: variável auxiliar na leitura do arquivo
    string nome; // name: variável auxiliar que armazena o nome do arquivo
    int l = 0; // l: armazena o número de vértices do grafo
    int r = 0; // r: armazena o número de arestas do grafo
    int diametro[3] = {0,-1,-1}; // diametro: armazena o valor do diâmetro do grafo

    /* LEITURA DO ARQUIVO */

    cin >> nome;
    for(int a = 0; a < nome.length()-4; a++)
        cout << nome[a];
    ifstream myfile(nome);

    if(myfile.is_open()){ // Contagem do Número de Linhas do Arquivo para Calculo do Número de Vértices e Arestas
        myfile >> l;
        while(!myfile.eof()){
            myfile >> line;
            myfile >> line;
            myfile >> line;
            r++;
        }
        r--; // O contador foi decrementado, pois os arquivos continham uma linha vazia no final
        myfile.close();
    }
    else
        return 1;

    myfile.open(nome);
    if(myfile.is_open()){
        matrizArs.resize(r); // Alocação da Matriz de Arestas com Vértices e Distâncias
        for(int a = 0; a < r; a++)
            matrizArs[a].resize(3);

        matrizDist.resize(l); // Alocação da Matriz de Distâncias
        for(int a = 0; a < l; a++)
            matrizDist[a].resize(l);

        myfile >> line;
        for(int a = 0; a < r && !myfile.eof(); a++){ // Preenchimento da Matriz de Arestas
            myfile >> matrizArs[a][0];
            myfile >> matrizArs[a][1];
            myfile >> matrizArs[a][2];
        }
        myfile.close();
    }
    else
        return 2;

    /* INICIALIZAÇÃO DA MATRIZ DE DISTÂNCIAS E DA LISTA DE INTERMEDIÁRIOS */

    for(int a = 0; a < l; a++) // Inicialização da Matriz de Distâncias
        for(int b = 0; b < l; b++)
            matrizDist[a][b] = -1; // Quando os vértices não forem adjacentes, o valor será definido inicialmente como "-1"

    for(int a = 0; a < r; a++){ // Quando forem os vértices adjacentes, o valor será definido como sua própria distância
        matrizDist[matrizArs[a][1]][matrizArs[a][2]] = matrizArs[a][0];
        matrizDist[matrizArs[a][2]][matrizArs[a][1]] = matrizArs[a][0];
    }

    inicia_lista(inicio, l);

    /* APLICAÇÃO DO ALGORITMO DE FLOYD-WARSHALL */

    for(int x = 0; x < l; x++) // Aplicação do Algoritmo de Floyd-Warshall
        for(int y = 0; y < l; y++)
            for(int z = 0; z < l; z++)
                if((x != y && y != z) && ((matrizDist[y][z] > matrizDist[y][x] + matrizDist[x][z] && matrizDist[y][z] > 0 && matrizDist[y][x] > 0 && matrizDist[x][z] > 0) || (matrizDist[y][z] == -1 && matrizDist[y][x] != -1 && matrizDist[x][z] != -1))){ // Se o caminho YZ for menor se passando por um intermediário X ou se o caminho direto YZ não existir, o caminho YXZ é tomado como ligante destes vértices
                    cout << "lol" << endl;
                    matrizDist[y][z] = matrizDist[y][x] + matrizDist[x][z];
                    adiciona_lista(inicio, y, z, x); // Adiciona-se o Vértice Intermediário X à Lista de Intermediários
                }

    /* BUSCA DO MAIOR DIÂMETRO DO GRAFO E DO CAMINHO PERCORRIDO */

    for(int x = 0; x < l; x++) // Busca do Maior Diâmetro Encontrado
        for(int y = 0; y < l; y++)
            if(matrizDist[x][y] > diametro[0]){
                diametro[0] = matrizDist[x][y];
                diametro[1] = x;
                diametro[2] = y;
            }
    
    cout << endl << diametro[1] << " ";

    id *leituraid = inicio;
    for(; leituraid->a != diametro[1] && leituraid->b != diametro[2]; leituraid = leituraid->proximoid); // Busca da Lista de Elementos Intermediários
    for(no *leitura = leituraid->lista; leitura != NULL; leitura = leitura->proximo)
        cout << leitura->p << " ";
    cout << diametro[2];
    
    cout << endl << diametro[0];

    /* DESALOCAÇÃO DE MEMÓRIA */

    for(int a; a < r; a++)
        matrizArs[a].resize(0);
    matrizArs.resize(0);

    for(int a; a < l; a++)
        matrizDist[a].resize(0);
    matrizDist.resize(0);

    return 0;
}