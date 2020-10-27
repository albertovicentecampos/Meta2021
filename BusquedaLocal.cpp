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

BusquedaLocal::BusquedaLocal(int n, int m, vector<vector<float>> d, int nEvaluaciones, Log *log) :
tamN(n),
tamM(m),
distancias(d),
numEvaluaciones(nEvaluaciones),
iter(0),
costeActual(0),
mejora(false),
diferencia(0.0),
posIntercambio(0),
l(log) {
    solActual.resize(tamM, 0);
    vAntiguo.resize(tamM, 0);
    vNuevo.resize(tamM, 0);
    vDistancia.reserve(tamM);
    noSeleccionados.resize(tamN, false);
}

vector<int> BusquedaLocal::algoritmoBusquedaLocal() {
    solucionInicialAleatoria();

    l->escribirEnArchivo("Solucion inicial aleatoria: ");
    for (int i = 0; i < tamM; i++) {
        l->escribirEnArchivoVector(to_string(solActual[i]) + ' ');
    }
    l->saltoLinea();

    posIntercambio = 0;

    while (iter < numEvaluaciones) {

        l->escribirEnArchivo("ITERACION: " + to_string(iter));
        l->saltoLinea();
        rellena();
        ordena();
        ordenaSolActual();

        l->escribirEnArchivo("REORDENAMOS Solucion en funcion de las distancias: ");
        for (int i = 0; i < tamM; i++) {
            l->escribirEnArchivoVector(to_string(solActual[i]) + ' ');
        }
        l->saltoLinea();

        mejora = false;

        for (int i = 0; i < tamN; i++) {
            if (noSeleccionados[i] == false) {
                l->escribirEnArchivo("Probamos intercambio en la pos " + to_string(posIntercambio) + " el valor de " + to_string(i));
                l->saltoLinea();
                cambiarValor(posIntercambio, i);
                diferencia = 0.0;
                diferencia = factorizacion();
                iter++;
                if (diferencia > 0) {
                    l->escribirEnArchivo("Diferencia positiva -> mejorar");
                    l->saltoLinea();
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
            l->escribirEnArchivo("No nos ha mejorado con los seleccionados. Pasamos al siguiente de solActual: " + to_string(vAntiguo[posIntercambio]));

        }
        //Comprobación para que no se salga del array
        if (posIntercambio == tamM - 1) {
            break;
        }
    }

    l->escribirEnArchivo("MEJOR SOLUCION BUSQUEDA LOCAL: ");
    for (int j = 0; j < tamM; j++) {
        l->escribirEnArchivoVector(to_string(solActual[j]) + ' ');
    }
//    l->saltoLinea();
//    l->escribirEnArchivo("MEJOR COSTE DE LA SOLUCION DE BUSQUEDA LOCAL: " + to_string(costeActual));
   
    return solActual;

}

void BusquedaLocal::solucionInicialAleatoria() {

    //Solucion inicial por Greedy
    Greedy g(tamN, tamM, distancias, l);
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
    l->escribirEnArchivo("Aporte coste viejo: " + to_string(aporteCosteViejo));
    aporteCosteNuevo = calculoAporte(vNuevo);
    l->escribirEnArchivo("Aporte coste nuevo: " + to_string(aporteCosteNuevo));
    l->saltoLinea();
    diferencia = aporteCosteNuevo - aporteCosteViejo;
    l->escribirEnArchivo("DIFERENCIA: " + to_string(diferencia));
    l->saltoLinea();
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

    l->escribirEnArchivo("MEJORAMOS SOLUCION ACTUAL: ");
    for (int i = 0; i < tamM; i++) {
        l->escribirEnArchivoVector(to_string(solActual[i]) + ' ');
    }
    l->saltoLinea();
    l->escribirEnArchivo("CUYO COSTE ES DE : " + to_string(costeActual));
    l->saltoLinea();

}