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
    seleccionadosM.resize(tamM, 0);

}

Greedy::Greedy(const Greedy& orig) {
}

Greedy::~Greedy() {
}

vector<float> Greedy::algoritmoGreedy() {

    //    int primerElemento = Randint(1, tamN);
    //    seleccionadosM[0] = primerElemento;
    //
    //    for (int i = 0; i < tamN; i++) {
    //        vDistancia[i] = 0;
    //
    //        for (int j = 0; j < tamM; j++) {
    //            if (distancias[seleccionadosM[j]][i] == 0) {
    //                vDistancia[i] = distancias[i][seleccionadosM[j]];
    //            }
    //            vDistancia[i] = distancias[seleccionadosM[j]][i];
    //        }
    //    }


    //-----------------------------------------
    int primerElemento = Randint(1, tamN);
    seleccionadosM[0] = primerElemento;

    float maxValor = 0;
    int numPos = 0;

    //Calculamos el vector de distancias
    for (int i = 0; i < tamM; i++) {
        for (int j = 0; j < tamN; j++) {

            if (distancias[seleccionadosM[i]][j] == 0) {
                vDistancia[j] += distancias[j][seleccionadosM[i]];
            } else {
                vDistancia[j] += distancias[seleccionadosM[i]][j];
            }
        }

        //Seleccionamos el mayor del momento

        maxValor = vDistancia[0];

        for (int i = 1; i < tamN; i++) {
            if (vDistancia[i] > maxValor) {
                numPos = i;
                maxValor = vDistancia[i];
            }
        }


        //Añadimos a la lista de los seleccionados
        seleccionadosM[i + 1] = numPos;
    }

    return seleccionadosM;

    //Seleccionamos el mayor



    //Añadimos a la lista de los seleccionados



    //
    //    //-----------------------------------------
    //    for (int i = 0; i < tamN; i++) {
    //        vDistancia[i] = 0;
    //        for (int j = 0; j < tamN; j++) {
    //            vDistancia[i] += distancias[i][j];
    //        }
    //    }

}
