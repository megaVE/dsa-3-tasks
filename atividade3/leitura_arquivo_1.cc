#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

//std::vector< std::vector<short int> > matrizAdj;
std::vector< std::vector<short int> > matrizOrd;

using namespace std;

#define ITERAR 1000 // Número de Conjunto de Iterações de Busca Largura a serem realizadas

void aloca_solucao(int *x, int *y){
    x[0] = y[0];
    x[1] = y[1];
    x[2] = y[2];
    x[3] = y[3];
    x[4] = y[4];
    x[5] = y[5];
}

int menos(int x, int y){
    if(x - y < 0)
        return 0;
    else
        return x-y;
}

int mais(int x, int y, int l){
    if(x + y > l-1)
        return l-1;
    else
        return x+y;
}

int partition(int low, int high) {
    int pivot = matrizOrd[high][0];
    int i = (low - 1);
    for (int j = low; j < high; j++) {
        if (matrizOrd[j][0] >= pivot) {
        i++;
        int a,b,c;
        a = matrizOrd[i][0];
        b = matrizOrd[i][1];
        c = matrizOrd[i][2];
        matrizOrd[i][0] = matrizOrd[j][0];
        matrizOrd[i][1] = matrizOrd[j][1];
        matrizOrd[i][2] = matrizOrd[j][2];
        matrizOrd[j][0] = a;
        matrizOrd[j][1] = b;
        matrizOrd[j][2] = c;
        }
    }
    int a,b,c;
    a = matrizOrd[i+1][0];
    b = matrizOrd[i+1][1];
    c = matrizOrd[i+1][2];
    matrizOrd[i+1][0] = matrizOrd[high][0];
    matrizOrd[i+1][1] = matrizOrd[high][1];
    matrizOrd[i+1][2] = matrizOrd[high][2];
    matrizOrd[high][0] = a;
    matrizOrd[high][1] = b;
    matrizOrd[high][2] = c;
    return (i + 1);
}

void quickSort(int low, int high) {
  if (low < high) {
    int pi = partition(low, high);
    quickSort(low, pi - 1);
    quickSort(pi + 1, high);
  }
}

int calcula_grafo(int *g, int *x, int *y){
    return sqrt(pow(x[g[1]] - x[g[0]], 2) + pow(y[g[1]] - y[g[0]], 2)) + sqrt(pow(x[g[2]] - x[g[1]], 2) + pow(y[g[2]] - y[g[1]], 2)) + sqrt(pow(x[g[3]] - x[g[2]], 2) + pow(y[g[3]] - y[g[2]], 2)) + sqrt(pow(x[g[4]] - x[g[3]], 2) + pow(y[g[4]] - y[g[3]], 2));
}

void zera_vetor(int i, int *v){
    for(int a = 0; a < i; a++)
        v[a] = 0;
}

int main(void){
    string line, nome; // line: auxilia na leitura das informações do arquivo
    int l = 0; // l: armazena o número de vértices do grafo
    int s[6]; // s: armazena os vértices e o peso da solução
    int stemp[6]; // stemp: armazena os vértices e o peso da melhor solução temporária
    int saux[6]; // saux: armazena os vértices e o peso da solução auxiliar
    zera_vetor(6,s);
    zera_vetor(6,stemp);
    zera_vetor(6,saux);
    
    cin >> nome;
    cout << nome << endl;
    ifstream myfile(nome);

    // ***

    if(myfile.is_open()){ // Contagem do Número de Linhas do Arquivo
        while(!myfile.eof()){
            myfile >> line;
            myfile >> line;
            myfile >> line;
            l++;
        }
        /*cout << endl << "Número Total de Vértices: " << l << endl << endl;*/
        myfile.close();
    }
    else
        return 1;

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
            /*cout << "Vértice Nº" << n+1 << ":\t " << x[n] << "\t | " << y[n] << endl;*/
            n++;
        }
        myfile.close();
    }
    else
        return 2;

    // ***

    /*matrizAdj.resize(l); // Criação da Matriz de Adjacência
    for(int a = 0; a < l; a++)
        matrizAdj[a].resize(l);

    for(int a = 0; a < l; a++) // Cálculo das Arestas e Alocação das Distâncias à Matriz
        for(int b = 0; b < l; b++)
            matrizAdj[a][b] = sqrt(pow(x[b] - x[a], 2) + pow(y[b] - y[a], 2));*/

    // ***

    matrizOrd.resize(l*l); // Criação da Matriz Ordenação
	for(int a = 0; a < l*l; a++)
		matrizOrd[a].resize(3);

    for(int a = 0, aux = 0; a < l; a++) // Preenchimento da Matriz Ordenação
        for(int b = 0; b < l; b++, aux++){
            matrizOrd[aux][0] = sqrt(pow(x[b] - x[a], 2) + pow(y[b] - y[a], 2));
            matrizOrd[aux][1] = a;
            matrizOrd[aux][2] = b;
        }

    // ***
    
    /*for(int b = 1; b < l*l; ++b){ // Ordenação da Matriz Ordenação via Quick Sort
        int c1 = matrizOrd[b][0];
        int c2 = matrizOrd[b][1];
        int c3 = matrizOrd[b][2];
        int a;
        for(a = b-1; a >= 0 && matrizOrd[a][0] < c1; --a){
            matrizOrd[a+1][0] = matrizOrd[a][0];
            matrizOrd[a+1][1] = matrizOrd[a][1];
            matrizOrd[a+1][2] = matrizOrd[a][2];
        }
        matrizOrd[a+1][0] = c1;
        matrizOrd[a+1][1] = c2;
        matrizOrd[a+1][2] = c3;
    }*/

    quickSort(0,l*l-1);

    /*cout << endl << "Valores Inversamente Ordenados da Matriz: ";
    for(int a = 0; a < l*l ;a++)
        cout << matrizOrd[a][0] << " ";
    cout << endl;*/

    // ***
    
    s[0] = matrizOrd[0][1]; // Heurística Construtiva: Cria solução com base nos vértices de maior peso
    s[1] = matrizOrd[0][2]; // Dois primeiros vértices

    for(int a = 2; a < l*l; a++){ // Terceiro vértice
        if(((matrizOrd[a][1] == s[0] || matrizOrd[a][1] == s[1]) && matrizOrd[a][2] != s[0] && matrizOrd[a][2] != s[1]) || ((matrizOrd[a][2] == s[0] || matrizOrd[a][2] == s[1]) && matrizOrd[a][1] == s[0] && matrizOrd[a][1] == s[1])){
            if(matrizOrd[a][1] == s[0] || matrizOrd[a][2] == s[0]){
                s[2] = s[1];
                s[1] = s[0];
                if(matrizOrd[a][1] == s[1])
                    s[0] = matrizOrd[a][2];
                else
                    s[0] = matrizOrd[a][1];
            } else{
                if(matrizOrd[a][1] == s[1])
                    s[2] = matrizOrd[a][2];
                else
                    s[2] = matrizOrd[a][1];
            }
            a = l*l;
        }
    }

    for(int a = 4; a < l*l; a++){ // Quarto vértice
        if(((matrizOrd[a][1] == s[0] || matrizOrd[a][1] == s[2]) && matrizOrd[a][2] != s[0] && matrizOrd[a][2] != s[1] && matrizOrd[a][2] != s[2]) || ((matrizOrd[a][2] == s[0] || matrizOrd[a][2] == s[2]) && matrizOrd[a][1] == s[0] && matrizOrd[a][1] == s[1] && matrizOrd[a][1] == s[2])){
            if(matrizOrd[a][1] == s[0] || matrizOrd[a][2] == s[0]){
                s[3] = s[2];
                s[2] = s[1];
                s[1] = s[0];
                if(matrizOrd[a][1] == s[1])
                    s[0] = matrizOrd[a][2];
                else
                    s[0] = matrizOrd[a][1];
            }
            else{
                if(matrizOrd[a][1] == s[2])
                    s[3] = matrizOrd[a][2];
                else
                    s[3] = matrizOrd[a][1];
            }
            a = l*l;
        }
    }

    for(int a = 6; a < l*l; a++){ // Quinto vértice
        if(((matrizOrd[a][1] == s[0] || matrizOrd[a][1] == s[3]) && matrizOrd[a][2] != s[0] && matrizOrd[a][2] != s[1] && matrizOrd[a][2] != s[2] && matrizOrd[a][2] != s[3]) || ((matrizOrd[a][2] == s[0] || matrizOrd[a][2] == s[3]) && matrizOrd[a][1] == s[0] && matrizOrd[a][1] == s[1] && matrizOrd[a][1] == s[2] && matrizOrd[a][1] == s[3])){
            if(matrizOrd[a][1] == s[0] || matrizOrd[a][2] == s[0]){
                s[4] = s[3];
                s[3] = s[2];
                s[2] = s[1];
                s[1] = s[0];
                if(matrizOrd[a][1] == s[1])
                    s[0] = matrizOrd[a][2];
                else
                    s[0] = matrizOrd[a][1];
            }
            else{
                if(matrizOrd[a][1] == s[3])
                    s[4] = matrizOrd[a][2];
                else
                    s[4] = matrizOrd[a][1];
            }
            a = l*l;
        }
    }

    s[5] = calcula_grafo(s,x,y);

    // ***

    for(int a = 1; a < ITERAR+1; a++){ // Aplicação de Busca Largura
        aloca_solucao(saux,s);
        for(int b = 0; b < 5; b++){
            saux[b] = mais(saux[b],a,l);
            saux[5] = calcula_grafo(saux,x,y);
            if(saux[5] > stemp[5])
                aloca_solucao(stemp,saux);
            aloca_solucao(saux,s);
        }
        for(int b = 0; b < 5; b++){
            saux[b] = menos(saux[b],a);
            saux[5] = calcula_grafo(saux,x,y);
            if(saux[5] > stemp[5])
                aloca_solucao(stemp,saux);
            aloca_solucao(saux,s);
        }
    }
    
    // ***

    if(stemp[5] == 0) // Imprime o Resultado
        cout << s[0]+1 << " " << s[1]+1 << " " << s[2]+1 << " " << s[3]+1 << " " << s[4]+1 << endl << s[5] << endl;
    else
        cout << stemp[0]+1 << " " << stemp[1]+1 << " " << stemp[2]+1 << " " << stemp[3]+1 << " " << stemp[4]+1 << endl << stemp[5] << endl;

    // ***

    delete x;
    delete y;

    return 0;
}