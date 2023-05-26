#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

std::vector< std::vector<int> > matrizAdj;
std::vector< std::vector<int> > matrizOrd;
std::vector< std::vector<int> > arvoreMin;

struct fila{
    int va;
    int vb;
    int d; 
};

using namespace std;

void zera_vetor(int i, int *v){
    for(int a = 0; a < i; a++)
        v[a] = 0;
}

int busca_ciclo(int pai, int l, int *pai_atual, int *pai_anterior, int *fila){ // Retorna 1 caso haja ciclo no grafo ou 0, caso não
    int soma_fila = 0; // soma_fila: verifica se a fila está vazia
    pai_atual[pai] = 1; // O Vértice Pai se torna o Primeiro Vértice Cinza
    for(int a = 0; a < l-1; a++){ // Adição dos Primeiros Vértices Adjacentes à Fila
        if(arvoreMin[a][1] == pai)
            fila[arvoreMin[a][2]] = 1;
        if(arvoreMin[a][2] == pai)
            fila[arvoreMin[a][1]] = 1;
    }
    for(int a = 0; a < l; a++){
        soma_fila = soma_fila + fila[a];
        pai_atual[a] = fila[a]; // Vértices da Fila se transformam em Cinzas
    }
    pai_anterior[pai] = 1; // O Primeiro Vértice Cinza se transforma em Preto
    while(soma_fila > 0){
        soma_fila = 0;
        zera_vetor(l, fila); // A Fila é Esvaziada
        for(int a = 0; a < l-1; a++){ // Percorre a Matriz da Árvore Mínima
            for(int b = 0; b > l; b++){
                if(pai_atual[b] == 1){ // Percorre Todos os Elementos Cinzas
                    if(arvoreMin[a][1] == b){
                        if(pai_atual[arvoreMin[a][2]] == 1) // Se o Elemento Adjacente for Cinza, então existe ciclo
                            return 1;
                        if(!pai_anterior[arvoreMin[a][2] == 1]) // Se o Elemento Adjacente for Branco, ele é adicionado à fila
                            fila[arvoreMin[a][2]] = 1;
                    }
                    if(arvoreMin[a][2] == b){
                        if(pai_atual[arvoreMin[a][1]] == 1)
                            return 1;
                        if(!pai_anterior[arvoreMin[a][1] == 1])
                            fila[arvoreMin[a][1]] = 1;
                    }
                }
            }
        }
        for(int a = 0; a < l; a++){
            soma_fila = soma_fila + fila[a];
            if(pai_atual[a] == 1)
                pai_anterior[a] = pai_atual[a];
            pai_atual[a] = fila[a];
        }
    }
    return 0;
}

int main(void){
    string line, nome; // line: auxilia na leitura das informações do arquivo; nome: armazena o nome do arquivo a ser lido
    int aux;
    float aux2; // aux(n): variáveis auxiliares
    int l = 0; // l: armazena o número de vértices do grafo
    int areaMaior[4] = {0, 0, 0, 0}; // areaMaior: armazena as coordenadas dos vértices do maior grafo
    cin >> nome;
    ifstream myfile(nome);

    // ***

    if(myfile.is_open()){ // Contagem do Número de Linhas do Arquivo
        while(!myfile.eof()){
            myfile >> line;
            myfile >> line;
            myfile >> line;
            l++;
        }
        myfile.close();
    }
    else
        cout << "Deu pau pra ler." << endl;

    int *x = new int[l]; // x: armazena a posição (x,_) de cada vértice
    int *y = new int[l]; // y: armazena a posição (_,y) de cada vértice

    // ***

    myfile.open(nome);
    if(myfile.is_open()){ // Leitura e Alocação da Coordenada dos Vértices
        int n = 0;
        while(!myfile.eof()){
            myfile >> line;
            myfile >> x[n];
            myfile >> y[n];
            n++;
        }
        myfile.close();
    }
    else
        cout << "Deu pau pra ler." << endl;

    // ***

    matrizAdj.resize(l); // Criação da Matriz de Adjacência
    for(int a = 0; a < l; a++)
        matrizAdj[a].resize(l);

    for(int a = 0; a < l; a++) // Cálculo das Arestas e Alocação das Distâncias à Matriz
        for(int b = 0; b < l; b++)
            matrizAdj[a][b] = sqrt(pow(x[b] - x[a], 2) + pow(y[b] - y[a], 2));

    // ***

    matrizOrd.resize(l*l); // Criação da Matriz Ordenação
	for(int a = 0; a < l*l; a++)
		matrizOrd[a].resize(3);

    for(int a = 0, aux = 0; a < l; a++) // Preenchimento da Matriz Ordenação
        for(int b = 0; b < l; b++, aux++){
            matrizOrd[aux][0] = matrizAdj[a][b];
            matrizOrd[aux][1] = a;
            matrizOrd[aux][2] = b;
        }

    // ***
    
    for(int b = 1; b < l*l; ++b){ // Ordenação da Matriz Ordenação via Quick Sort
        int c1 = matrizOrd[b][0];
        int c2 = matrizOrd[b][1];
        int c3 = matrizOrd[b][2];
        int a;
        for(a = b-1; a >= 0 && matrizOrd[a][0] > c1; --a){
            matrizOrd[a+1][0] = matrizOrd[a][0];
            matrizOrd[a+1][1] = matrizOrd[a][1];
            matrizOrd[a+1][2] = matrizOrd[a][2];
        }
        matrizOrd[a+1][0] = c1;
        matrizOrd[a+1][1] = c2;
        matrizOrd[a+1][2] = c3;
    }

    int *pai_atual = new int[l]; // Vetor de Nós Cinzas da Busca Largura
    int *pai_anterior = new int[l]; // Vetor de Nós Pretos da Busca Largura
    int *fila = new int[l]; // Fila da Busca Largura
    int *registro = new int[l]; // Vetor de Pontos Adicionados à Árvore Mínima
    zera_vetor(l, registro);

    arvoreMin.resize(l-1); // Criação da Matriz Árvore Mínima
	for(int a = 0; a < l-1; a++)
		arvoreMin[a].resize(3);

    for(int a = 0; a < l-1; a++){ // Inicialização da Matriz Árvore Mínima
        arvoreMin[a][0] = -1;
        arvoreMin[a][1] = -1;
        arvoreMin[a][2] = -1;
    }

    for(aux = 0; matrizOrd[aux][0] <= 0; aux++); // Pulando as Arestas Zeradas (que representavam a menor distância do vértice até ele mesmo)

    for(int a = 0; a < l-1;){ // Preenchimento da Matriz Árvore Mínima
        int tem_ciclo = 0;
        int tempa = registro[matrizOrd[aux][1]];
        int tempb = registro[matrizOrd[aux][2]];
        arvoreMin[a][0] = matrizOrd[aux][0];
        arvoreMin[a][1] = matrizOrd[aux][1];
        arvoreMin[a][2] = matrizOrd[aux][2];
        registro[matrizOrd[aux][1]] = 1;
        registro[matrizOrd[aux][2]] = 1;
        for(int b = 0; b < l-1 && tem_ciclo == 0; b++) // Remove a Aresta se for dupliacada
            if(a != b && (arvoreMin[a][1] == arvoreMin[b][1] && arvoreMin[a][2] == arvoreMin[b][2] || arvoreMin[a][1] == arvoreMin[b][2] && arvoreMin[a][2] == arvoreMin[b][1]))
                tem_ciclo = 1;
        for(int b = 0; b < l && tem_ciclo == 0; b++) // Verifica, por busca largura de cada vértice, se há ciclo no grafo
            if(registro[b] == 1 && tem_ciclo == 0){
                zera_vetor(l, pai_atual);
                zera_vetor(l, pai_anterior);
                zera_vetor(l, fila);
                tem_ciclo = busca_ciclo(b, l, pai_atual, pai_anterior, fila);
            }
        if(tem_ciclo == 0) // Se não há ciclo = prossegue-se a preencher a árvore
            a++;
        else{ // Se há ciclo = a aresta é apagada e a modificação do registro desfeita
            arvoreMin[a][0] = -1;
            arvoreMin[a][1] = -1;
            arvoreMin[a][2] = -1;
            registro[matrizOrd[aux][1]] = tempa;
            registro[matrizOrd[aux][2]] = tempb;
        }
        aux++;
    }

    // ***

    aux = 0;
    aux2 = 0;
    for(int a = 0; a < l; a++, aux++){ // Encontra o Maior Subgrafo
        for(int b = 0+aux; b < l; b++){
            for(int c = 0+aux; c < l; c++){
                aux2 = ((x[a] * y[b] + y[a] * x[c] + x[b] * y[c] - y[b] * x[c] - x[a] * y[c] - y[a] * x[b]) / 2); // Cálculo da Área Demarcada por 3 Vértices Selecionados
                    aux2 = -1 * aux2;
                if(aux2 > areaMaior[0]){
                    areaMaior[0] = aux2;
                    areaMaior[1] = a;
                    areaMaior[2] = b;
                    areaMaior[3] = c;
                }
            }
        }
    }

    cout << areaMaior[1] << " " << areaMaior[2] << " " << areaMaior [3] << endl << areaMaior[0];

    // ***

    delete x;
    delete y;

    return 0;
}