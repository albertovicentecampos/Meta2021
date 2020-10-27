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

Greedy::Greedy(int n, int m, vector<vector<float>> d, Log* log) :
tamN(n),
tamM(m),
distancias(d),
maxValor(0.0),
numPos(0),
primerElemento(0),
l(log){
    vDistancia.resize(tamN, 0);
    vN.resize(tamN, 0);
    marcados.resize(tamN, false);
    distMayor.reserve(tamN);
}

vector<int> Greedy::algoritmoGreedy() {

    primerElemento = Randint(1, tamN);
    seleccionadosM.push_back(primerElemento);
    marcados[primerElemento] = true;


    //Calculamos el vector de distancias
    for (int i = 0; i < tamM; i++) {
        l->escribirEnArchivo("*CALCULAMOS SELECCIONADOS: ");
        for (int j = 0; j < tamM; j++) {
            l->escribirEnArchivoVector(to_string(seleccionadosM[j]) + ' ');
        }
        l->saltoLinea();

        calculoDistancias(seleccionadosM[i]);
        //Seleccionamos el mayor del momento
        mayorDistancia();
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
    for (int i = 0; i < tamN; i++) {
        distMayor[i].first = i;
        distMayor[i].second = vDistancia[i];
    }


    pair<int, float> temporal;
    for (int i = 0; i < tamN; i++) {
        for (int j = i + 1; j < tamN; j++) {
            if (distMayor[i].second < distMayor[j].second) {
                temporal = distMayor[i];
                distMayor[i] = distMayor[j];
                distMayor[j] = temporal;
            }
        }
    }

    
    for (int i = 0; i < tamN; i++) {
        if (marcados[distMayor[i].first] == false) {
            l->escribirEnArchivo("Elemento: " + to_string(distMayor[i].first) + " con un valor de " + to_string(distMayor[i].second));
            l->saltoLinea();
            marcados[distMayor[i].first] = true;
            l->escribirEnArchivo("Seleccionamos el elemento " + to_string(distMayor[i].first));
            l->saltoLinea();
            seleccionadosM.push_back(distMayor[i].first);
            break;
        }
    }
}

vector<bool> Greedy::noSeleccionados() {
    return marcados;
}