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

Greedy::Greedy(long n, long m, vector<vector<float>> d) :
tamN(n),
tamM(m),
distancias(d),
maxValor(0.0),
numPos(0),
primerElemento(0) {
    vDistancia.resize(tamN, 0);
    seleccionadosM.resize(tamM, 0);
}

Greedy::Greedy(const Greedy& orig) {
}

Greedy::~Greedy() {
}

vector<float> Greedy::algoritmoGreedy() {
    primerElemento = Randint(1, tamN);
    seleccionadosM[0] = primerElemento;

    //Calculamos el vector de distancias
    for (int i = 0; i < tamM; i++) {
        for (int j = 0; j < tamN; j++) {
            calculoDistancias(i, j);
        }
        //Seleccionamos el mayor del momento
        mayorDistancia();

        //Añadimos a la lista de los seleccionados
        seleccionadosM[i + 1] = numPos;
    }
    return seleccionadosM;
}

void Greedy::calculoDistancias(int i, int j) {
    //COMPROBAMOS FILA-COLUMNA Y COLUMNA FILA (ESTO SI TENEMOS LA TRIANGULAR SUPERIOR)
    if (distancias[seleccionadosM[i]][j] == 0) {
        vDistancia[j] += distancias[j][seleccionadosM[i]];
    } else {
        vDistancia[j] += distancias[seleccionadosM[i]][j];
    }

    //ESTO SI TENEMOS TODA LA MATRIZ RELLENA 
    //vDistancia[j] += distancias[seleccionadosM[i]][j]; 
}

void Greedy::mayorDistancia() {
    maxValor = vDistancia[0];
    for (int k = 1; k < tamN; k++) {
        bool existe = false;
        if (vDistancia[k] > maxValor) {
            //Comprobamos si está ya elegido
            for (int z = 0; z < tamM; z++) {
                if (k == seleccionadosM[z]) {
                    //No seleccionar
                    existe = true;
                }
            }
            if (!existe) {
                maxValor = vDistancia[k];
                numPos = k;
            }
        }
    }
}