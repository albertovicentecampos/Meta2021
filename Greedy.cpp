/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Greedy.cpp
 * Author: Alberto
 * 
 * Created on 24 de septiembre de 2020, 18:21
 */

#include "Greedy.h"

Greedy::Greedy(int n, int m, vector<vector<float>> d) :
tamN(n),
tamM(m),
distancias(d),
maxValor(0.0),
numPos(0),
primerElemento(0) {
    vDistancia.resize(tamN, 0);
    vN.resize(tamN, 0);
    marcados.resize(tamN, false);
}

vector<int> Greedy::algoritmoGreedy() {

    primerElemento = Randint(1, tamN);
    seleccionadosM.push_back(numPos);
    marcados[primerElemento] = true;

    //Calculamos el vector de distancias
    for (int i = 0; i < tamM; i++) {
        calculoDistancias(seleccionadosM[i]);
        //Seleccionamos el mayor del momento
        mayorDistancia();
        //Añadimos a la lista de los seleccionados
        seleccionadosM.push_back(numPos);
        marcados[numPos] = true;
    }
    return seleccionadosM;
}

void Greedy::calculoDistancias(int i) {
    for (int j = 0; j < tamN; j++) {
        if (i < j) {
            vDistancia[j] += distancias[i][j]; 
        } else {
            vDistancia[j] += distancias[j][i];
        }
    }
}

void Greedy::mayorDistancia() {
    maxValor = 0;
    for (int i = 0; i < tamN; i++) {
        bool existe = false;
        if (vDistancia[i] > maxValor) {
            //Comprobamos si está ya elegido
            if (marcados[i] == false) {
                maxValor = vDistancia[i];
                numPos = i; 
            }

        }
    }
}