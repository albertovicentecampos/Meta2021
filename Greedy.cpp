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
    seleccionadosM.resize(tamM, -1);
    vN.resize(tamN, 0);
}

vector<float> Greedy::algoritmoGreedy() {

    primerElemento = Randint(1, tamN);
    seleccionadosM[0] = primerElemento;

    vector<bool> marcados;
    marcados.resize(tamN, false);
    marcados[primerElemento] = true;


    //Calculamos el vector de distancias
    for (int i = 0; i < tamM; i++) {
        calculoDistancias(seleccionadosM[i]);
        //Seleccionamos el mayor del momento
        mayorDistancia();
        //Añadimos a la lista de los seleccionados
        seleccionadosM[i + 1] = numPos;
    }
    return seleccionadosM;
}

//void Greedy::maxDistancia(){
//    float maxDis = 0.0;
//    int pos = -1; 
//    for(int i = 0; i<tamN; i++){
//        float disn = calcularDistancia(i);
//        if(disn>maxDis){
//            maxDis = disn;
//            pos = i;
//        }
//    }
//    
//}
//
//float Greedy:: calcularDistancia(int iN){
//    float distancia = 0.0; 
//    for(int i = 0; i<tamM; i++){
//        if(iN<i){
//            distancia += distancias[iN][i];
//        }else{
//            distancia += distancias[i][iN];
//        }
//    }
//    return distancia; 
//}

void Greedy::calculoDistancias(int i) {
    //    COMPROBAMOS FILA-COLUMNA Y COLUMNA FILA (ESTO SI TENEMOS LA TRIANGULAR SUPERIOR)
    //        if (distancias[seleccionadosM[i]][j] == -1) {
    //            vDistancia[j] += distancias[j][seleccionadosM[i]];
    //        } else {
    //            vDistancia[j] += distancias[seleccionadosM[i]][j];
    //        }

    //ESTO SI TENEMOS TODA LA MATRIZ RELLENA 
    //    vDistancia[j] += distancias[seleccionadosM[i]][j]; 

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
            //            Comprobamos si está ya elegido
            for (int z = 0; z < tamM; z++) {
                if (i == seleccionadosM[z]) {
                    //No seleccionar
                    existe = true;
                }
            }
            if (!existe) {
                maxValor = vDistancia[i];
                numPos = i;
            }
        }
    }
}