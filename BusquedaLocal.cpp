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
    vAntiguo.resize(tamM, 0);
    vNuevo.resize(tamM, 0);
    vDistancia.reserve(tamM);
    noSeleccionados.resize(tamN, false);
}

/*vector<int>*/ void BusquedaLocal::algoritmoBusquedaLocal() {

    solucionInicialAleatoria();

    int posIntercambio = 0;

    while (iter < numEvaluaciones) {

        rellena();
        posIntercambio = minimaDistancia();

        for(int i = 0 ; i < tamN ; i++){
            if (noSeleccionados[i] == false) {    
                cambiarValor(posIntercambio, i);

                //Calcular el aporte del coste del que queremos intercambiar con respecto a todos
                float aporteCosteViejo = 0.0;
                aporteCosteViejo = calculoAporteAntiguo(posIntercambio);
                //cout << "Aporte coste Viejo : " << aporteCosteViejo <<endl;

                //Calcular el aporte del coste del nuevo no Seleccionado con respecto a los demás 
                float aporteCosteNuevo = 0.0;
                aporteCosteNuevo = calculoAporteNuevo(posIntercambio);
                //cout << "Aporte coste Nuevo : " << aporteCosteNuevo <<endl;
                iter++;

                //Calculamos la diferencia 
                float diferencia = 0.0;
                diferencia = aporteCosteNuevo - aporteCosteViejo;
                
                if (diferencia > 0) {

                    vAntiguo = vNuevo;
                    solActual = vAntiguo;
                    costeActual += diferencia;
                    
                    cout << "Coste:" << costeActual << endl;

                    noSeleccionados[i] = true;
                    break;

                } 
            } 
        }
    }
}

void BusquedaLocal::solucionInicialAleatoria() {
    Greedy g(tamN, tamM, distancias);
    solActual = g.algoritmoGreedy();
    vAntiguo = solActual;
    vNuevo = solActual;
    noSeleccionados = g.noSeleccionados();
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

    for (int i = 1; i < tamM; i++) {
        if (menor > vDistancia[i].second) {
            menor = vDistancia[i].second;
            pos = i;
        }
    }

    return pos;
}

float BusquedaLocal::calculoAporteAntiguo(int pos) {
    float aporte = 0.0;
    for (int i = 0; i < tamM; i++) {
        aporte += consultaMatriz(vAntiguo[i], vAntiguo[pos]);
    }
    return aporte;
}

float BusquedaLocal::calculoAporteNuevo(int pos) {
    float aporte = 0.0;
    for (int i = 0; i < tamM; i++) {
        aporte += consultaMatriz(vNuevo[i], vNuevo[pos]);
    }
    return aporte;
}

void BusquedaLocal::cambiarValor(int pos, int valor) {
    vNuevo[pos] = valor;
}