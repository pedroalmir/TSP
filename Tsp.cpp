/* 
 * File:   Tsp.cpp
 * Author: david
 * 
 * Created on February 8, 2013, 1:40 PM
 */

#include "Tsp.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <time.h>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ifstream;
using std::stringstream;

Tsp::Tsp() {
    srand(time(0));
}
Tsp::Tsp(char _arq[]) {
    lerGrDoArq(_arq);
}

/**
 * 
 * @param _arq: nome do arquivo, que contem a relacao de vertices do grafo, a ser aberto.
 * @return verdadeiro, caso a matriz tenha sido carregada com sucesso. Falso caso contrario.
 */
bool Tsp::lerGrDoArq(const char _arq[]) {
    string linha;
    ifstream inst_tsp(_arq);

    int lstVert[52][2], **tMat;
    int tamLstVert = 52,
            i, j, k = 0;
    
    bool ret;

    if (inst_tsp.is_open() == false) {
        cout << "Arquivo inacessivel ou nao existente.";
        return false;
    } else {
        while (inst_tsp.good()) {
            getline(inst_tsp, linha);

            // nao e um digito, portanto faz parte de um cabecalho.
            if (isdigit(linha[0]) == false) {
                cout << linha << endl;
            } else {
                i = 1;
                while (linha[i] != ' ') i++; // percorre ate o x: |123 x23 123|
                ++i;
                j = 0;
                
                while (linha[i + j] != ' ') j++; // decodifica coordenada X
                stringstream x(linha.substr(i, j - 2));
                
                i = i + j + 1;
                j = 0;
                while (linha[i + j] != '.' && linha[i + j]) j++; // decodifica coordenada Y
                stringstream y(linha.substr(i, j));
                
                if (!(x >> lstVert[k][0]) || !(y >> lstVert[k][1])) {
                    cout << "Erro de conversao!";
                    return false;
                }
                k++;
                
                cout << linha << endl;
            }
        }
        inst_tsp.close();
        
        // criando matriz de adj.
        tMat = new int*[tamLstVert];
        for (i = 0; i < tamLstVert; i++) {
            tMat[i] = new int[tamLstVert];
            for (j = 0; j < tamLstVert; j++)
                tMat[i][j] = 0;
        }
        
        /*
         * Obtem a distancia entre cada uma das cidades a partir do valores lidos do arquivo.
         * Estas mesmas sao calculadas pela da equacao de Pitagoras: x**2 +y**2 = z**2
         * Onde:
         * x = pos X da cidade A - pos X da cidade B
         * x = pos Y da cidade A - pos Y da cidade B
         * z = distancia entre as cidades A e B.
         */
        for (i = 0; i < tamLstVert; i++) {
            for (j = i +1; j < tamLstVert; j++) { // distancia euclidiana
                int dx = pow(lstVert[i][0] -lstVert[j][0], 2);
                int dy = pow(lstVert[i][1] -lstVert[j][1], 2);
                
                tMat[j][i] = tMat[i][j] = sqrt(dx +dy);
            }
        }
        
        return ret = g.carregarMatExist(tMat, tamLstVert);
    }
}
bool Tsp::gerarGrRand(int _numVert) {
    bool ret;

    ret = g.gerarMatRand(_numVert); // gera matri de adj. randomica
    //if (ret == true) g.exibirMat(); // exibe grafo gerado.
    return ret;
}

/**
 * Invoca o algoritmo twiceAround para resolver o TSP e exibe a resposta.
 */
int Tsp::resTwiAroundOrig() {
    int custCirc = 0;
    if (g.getValido() == false)
        cout << "Grafo invalido, nao ha nada para resolver." << endl;
    else {
        custCirc = g.twiceAround();
        //g.exibirMat(2); // exibir circuito hamiltoniano
    }
    
    return custCirc;
}
/**
 * Invoca o algoritmo twiceAround para resolver o TSP e exibe a resposta.
 * Este algoritmo utilizara o algoritmo de dijkstra como entrada.
 */
int Tsp::resTwiAroundDijk() {
    int custCirc = 0;
    if (g.getValido() == false)
        cout << "Grafo invalido, nao ha nada para resolver." << endl;
    else {
        custCirc = g.twiceAroundComDijkstra();
        //g.exibirMat(2); // exibir circuito hamiltoniano
    }
    
    return custCirc;
}
/**
 * Compara o metodo original ao twice-around que utiliza a arvores de caminhos minimos como entrada
 * @_numInter vezes, em relacao ao custo do circuito obtido (quando menor, melhor).
 * @param _numInter numero de vezes que os circuitos sao comparados.
 * @param _intervTamMat intervalo que o tamanho da matriz de adj criada pode assumir
 */
void Tsp::cmpEntreTwiceArounds(const int _numInter, const int _intervTamMat) {
    int tCustOrig, tCustDijk, tContOrig, tContDijk, tTam;
    
    tContOrig = tContDijk = 0;
    
    if (_numInter < 1)
        cout << "Erro: numero de interacoes invalido." << endl;
    else {
        for (int i = 0; i < _numInter; i++) {
            tTam = 0; // acha um tamanho randomico para a matriz de adj.
            while (tTam == 0) tTam = rand() %_intervTamMat;
            gerarGrRand(tTam);

            tCustOrig = resTwiAroundOrig(); // calc. com o custo pelo twice-around original
            tCustDijk = resTwiAroundDijk(); // calc. com o custo pelo twice-around modificado
            
            //cout << "Custo (tw-ar original): " << tCustOrig << ".\n";
            //cout << "Custo (tw-ar dijkstra): " << tCustDijk << ".\n\n";
            
            if (tCustOrig < tCustDijk)
                tContOrig++;
            else if (tCustOrig > tCustDijk)
                tContDijk++;
        }
        
        cout << "N. de vezes que o alg orig foi melhor: " << tContOrig << endl
             << "N. de vezes que o alg dijk foi melhor: " << tContDijk << endl;
    }
}