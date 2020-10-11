/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BusquedaLocal.cpp
 * Author: Alberto
 * 
 * Created on 11 de octubre de 2020, 11:13
 */

#include "BusquedaLocal.h"

BusquedaLocal::BusquedaLocal(int n, int m, vector<vector<float>> d, int nEvaluaciones) :
tamN(n),
tamM(m),
distancias(d),
numEvaluaciones(nEvaluaciones),
iter(0),
costeActual(0) {
    solActual.resize(tamM, 0);
    vDistancia.reserve(tamM);
}

/*vector<int>*/ void BusquedaLocal::algoritmoBusquedaLocal() {

    solucionInicialAleatoria();
    rellena();
    cout << "VECTOR: ";
    for (int i = 0; i < tamM; i++) {
        cout << vDistancia[i].second << ' ';
    }
    cout << endl;

    //int posIntercambio = minimaDistancia();
    //cout << posIntercambio << endl;

    //    while (iter < numEvaluaciones) {
    //
    //    }

}

void BusquedaLocal::solucionInicialAleatoria() {
    Greedy g(tamN, tamM, distancias);
    solActual = g.algoritmoGreedy();
    costeActual = Faux.coste(distancias, tamM, solActual);
}

void BusquedaLocal::rellena() {
    for (int i = 0; i < tamM; i++) {
        vDistancia[i].first = solActual[i];
        vDistancia[i].second = calculaD(solActual[i]);
    }
}

float BusquedaLocal::calculaD(int numI) {
    float valorDistancia = 0.0;
    for (int i = 0; i < tamM; i++) {
        valorDistancia += consultaMatriz(numI, solActual[i]);
    }
}

float BusquedaLocal::consultaMatriz(int i, int j) {
    float valor = 0.0;
    if (i < j) {
        valor = distancias[i][j];
    } else {
        valor = distancias[j][i];
    }
}

int BusquedaLocal::minimaDistancia() {
    int pos = 0;
    float menor = vDistancia[0].second;

    for (int i = 0; i < tamM; i++) {
        if (menor > vDistancia[i].second) {
            menor = vDistancia[i].second;
            pos = i;
        }
    }

    return pos;
}