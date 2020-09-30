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

Greedy::Greedy(long n, long m, vector<vector<float>> d, vector<int> sol) :
tamN(n),
tamM(m),
distancias(d),
solucion(sol),
mayorDistancia(0) {
    vDistancia.resize(tamN, 0);
}

Greedy::Greedy(const Greedy& orig) {
}

Greedy::~Greedy() {
}

void Greedy::algoritmoGreedy() {

    for (int i = 0; i < tamN; i++) {
        vDistancia[i] = 0;
        for (int j = 0; j < tamN; j++) {
            vDistancia[i] += distancias[i][j];
        }
    }

}
