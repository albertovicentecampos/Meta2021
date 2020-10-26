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

// archivos: GKD-c_1_n500_m50.txt GKD-c_2_n500_m50.txt GKD-c_3_n500_m50.txt MDG-a_21_n2000_m200.txt MDG-a_22_n2000_m200.txt MDG-a_23_n2000_m200.txt SOM-b_11_n300_m90.txt SOM-b_12_n300_m120.txt SOM-b_13_n400_m40.txt
#include "BusquedaLocal.h"

BusquedaLocal::BusquedaLocal(int n, int m, vector<vector<float>> d, int nEvaluaciones) :
tamN(n),
tamM(m),
distancias(d),
numEvaluaciones(nEvaluaciones),
iter(0),
costeActual(0),
mejora(false),
diferencia(0.0),
posIntercambio(0) {
    solActual.resize(tamM, 0);
    vAntiguo.resize(tamM, 0);
    vNuevo.resize(tamM, 0);
    vDistancia.reserve(tamM);
    noSeleccionados.resize(tamN, false);
}

vector<int> BusquedaLocal::algoritmoBusquedaLocal() {
    solucionInicialAleatoria();
    posIntercambio = 0;
    while (iter < numEvaluaciones) {
        rellena();
        ordena();
        ordenaSolActual();
        mejora = false;
        for (int i = 0; i < tamN; i++) {
            if (noSeleccionados[i] == false) {
                cambiarValor(posIntercambio, i);
                diferencia = 0.0;
                diferencia = factorizacion();
                iter++;
                if (diferencia > 0) {
                    mejorar();
                    noSeleccionados[i] = true;
                    break;
                }
            }
        }
        //Comprobamos si no ha mejorado con todos los selecionados
        if (!mejora) {
            //Se pasa al siguiente menor de la lista di
            posIntercambio++;
        }
        //Comprobación para que no se salga del array
        if (posIntercambio == tamM - 1) {
            break;
        }
    }

    return solActual;
    cout << "VECTOR: " << endl;
    for (int i = 0; i < tamM; i++) {
        cout << vDistancia[i].first << ' ';
    }
    cout << endl;
    cout << "Coste:" << costeActual << endl;

}

void BusquedaLocal::solucionInicialAleatoria() {

    //Solucion inicial por Greedy
    Greedy g(tamN, tamM, distancias);
    solActual = g.algoritmoGreedy();
    vAntiguo = solActual;
    vNuevo = solActual;
    noSeleccionados = g.noSeleccionados();




    //Solucion inical aleatoria



    costeActual = Faux.coste(distancias, tamM, solActual);
}

void BusquedaLocal::rellena() {
    vAntiguo = solActual;
    vNuevo = solActual;
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
    return valorDistancia;
}

float BusquedaLocal::consultaMatriz(int i, int j) {
    float valor = 0.0;
    if (i < j) {
        valor = distancias[i][j];
    } else {
        valor = distancias[j][i];
    }

    return valor;
}

void BusquedaLocal::ordena() {
    pair<int, float> temporal;
    for (int i = 0; i < tamM; i++) {
        for (int j = i + 1; j < tamM; j++) {
            if (vDistancia[i].second > vDistancia[j].second) {
                temporal = vDistancia[i];
                vDistancia[i] = vDistancia[j];
                vDistancia[j] = temporal;
            }
        }
    }
}

void BusquedaLocal::ordenaSolActual() {
    for (int i = 0; i < tamM; i++) {
        solActual[i] = vDistancia[i].first;
    }
    vAntiguo = solActual;
    vNuevo = solActual;
}

void BusquedaLocal::cambiarValor(int pos, int valor) {
    vNuevo[pos] = valor;
}

float BusquedaLocal::factorizacion() {
    //Calcular el aporte del coste del que queremos intercambiar con respecto a todos
    float aporteCosteViejo = 0.0;
    float aporteCosteNuevo = 0.0;
    float diferencia = 0.0;

    aporteCosteViejo = calculoAporte(vAntiguo);
    aporteCosteNuevo = calculoAporte(vNuevo);

    diferencia = aporteCosteNuevo - aporteCosteViejo;
    return diferencia;
}

float BusquedaLocal::calculoAporte(vector<int> v) {
    float aporte = 0.0;
    for (int i = 0; i < tamM; i++) {
        aporte += consultaMatriz(v[i], v[posIntercambio]);
    }
    return aporte;
}

void BusquedaLocal::mejorar() {
    mejora = true;
    vAntiguo = vNuevo;
    solActual = vNuevo;
    costeActual += diferencia;
    posIntercambio = 0;
}