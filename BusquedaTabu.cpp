/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BusquedaTabu.cpp
 * Author: Alberto
 * 
 * Created on 23 de octubre de 2020, 10:14
 */

#include "BusquedaTabu.h"

BusquedaTabu::BusquedaTabu(int n, int m, vector<vector<float>> d, int ten, int tamV, int numEval, int numIntSinMov, float probDivInt) :
tamN(n),
tamM(m),
distancias(d),
tenencia(ten),
tamVecinos(tamV),
costeActual(0.0),
costeMejorVecindario(0.0),
iter(0),
numEvaluaciones(numEval),
posIntercambio(0),
numIntentosSinMov(numIntSinMov),
probabilidadDivInt(probDivInt) {

    solGlobal.resize(tamM, 0);
    solActual.resize(tamM, 0);
    solMejorVecinos.resize(tamM, 0);

    listaTabu.resize(tenencia, -1);
    memLargoPlazo.resize(n, 0);

    noSeleccionados.resize(tamN, false);

    vNuevo.resize(tamM, 0);
    vAntiguo.resize(tamM, 0);
}

vector<int> BusquedaTabu::algoritmoBusquedaTabu() {

    solucionInicialAleatoria();
    posIntercambio = 0;
    int reinicializacion = 0;

    while (iter < numEvaluaciones) {

        rellena();
        ordena();
        ordenaSolActual();

        int vecino = 0;
        int elementoLT = 0;

        for (int i = 0; i < tamN; i++) {
            if (noSeleccionados[i] == false) {
                //Comprobamos que no este en la lista tabu
                bool esTabu = false;
                list<int>::iterator it;
                for (it = listaTabu.begin(); it != listaTabu.end(); it++) {
                    if (i == *it) {
                        esTabu = true;
                        break;
                    }
                }
                if (!esTabu) {
                    //Comprobamos que el elemento que cogemos de los no seleccionados no está en la lista Tabu 
                    if (vecino < tamVecinos) {
                        cambiarValor(posIntercambio, i);
                        float diferencia = 0.0;
                        diferencia = factorizacion();
                        iter++;
                        costeActual += diferencia;
                        if (costeMejorVecindario < costeActual) {
                            costeMejorVecindario = costeActual;
                            solMejorVecinos = vNuevo;
                            //Lo marcamos true en los no seleccionados? 
                        }
                        elementoLT = vAntiguo[posIntercambio];
                        vecino++;

                        //                    if (diferencia > 0) {
                        //                        mejorar(diferencia);
                        //                        noSeleccionados[i] = true;
                        //                    }




                        //                    float costeAux = 0.0;
                        //                    elementoLT = solActual[posIntercambio];
                        //                    solActual[posIntercambio] = i;
                        //                    costeAux = Faux.coste(distancias,tamM,solMomento);
                        //                    
                        //                    if(costeMomento < costeAux ){                     
                        //                        solMomento = solActual; 
                        //                        costeMomento = costeAux; 
                        //                    }


                    } else {
                        //Si lleva 10 vecinos paramos
                        break;
                    }
                }
            }
        }

        //Añadimos a la lista Tabu 

        listaTabu.push_back(elementoLT);
        listaTabu.pop_front();

        //Actualizamos la memoria a largo plazo 

        for (int i = 0; i < tamM; i++) {
            memLargoPlazo[solMejorVecinos[i]]++;
        }


        //Sustituimos solVecindario por solActual (Para la siguiente iteracion
        solActual = solMejorVecinos;

        //Comprobamos que la mejor solucion de nuestros 10 vecinos es mejor que la global

        if (costeMejorVecindario > costeGlobal) {
            solGlobal = solMejorVecinos;
            costeGlobal = costeMejorVecindario;
            reinicializacion = 0;
        } else {
            reinicializacion++;
        }


        if (reinicializacion == numIntentosSinMov) {
            //Reinicio oscilacion estratégica
            reinicializacion = 0;
            float num = Randfloat(0, 1);

            //Reordenamos el vector

            vector<pair<int, int>> MLPlazo;
            MLPlazo.reserve(tamN);

            for (int i = 0; i < tamN; i++) {
                MLPlazo[i].first = i;
                MLPlazo[i].second = memLargoPlazo[i];
            }

            //Ordenamos la memoria a largo plazo de menor a mayor

            pair<int, int> temporal;
            for (int i = 0; i < tamN; i++) {
                for (int j = i + 1; j < tamN; j++) {
                    if (MLPlazo[i].second > MLPlazo[j].second) {
                        temporal = MLPlazo[i];
                        MLPlazo[i] = MLPlazo[j];
                        MLPlazo[j] = temporal;
                    }
                }
            }

            //Metemos los indices ordenados de la memoria a largo plazo en un vector auxiliar         
            vector<int> memAux;
            memAux.resize(tamN, 0);

            for (int i = 0; i < tamN; i++) {
                memAux[i] = MLPlazo[i].first;
            }

            if (num <= probabilidadDivInt) {
                //Diversificamos (Cogemos los del principio hasta tamM pq son los mas pequeños)
                for (int i = 0; i < tamM; i++) {
                    solActual[i] = memAux[i];
                }
                
            } else {
                //Intensificamos (Cogemos los del final hasta tamM pq son los mas grandes)
                int tam = tamM;
                for (int i = 0; i < tamM; i++) {
                    solActual[i] = memAux[tam];
                    tam--;
                } 
                
            }
        }

    }

}

void BusquedaTabu::solucionInicialAleatoria() {
    Greedy g(tamN, tamM, distancias);
    solActual = g.algoritmoGreedy();
    solMejorVecinos = solActual;
    solGlobal = solActual;


    vAntiguo = solActual;
    vNuevo = solActual;

    noSeleccionados = g.noSeleccionados();

    costeActual = Faux.coste(distancias, tamM, solActual);
    costeGlobal = costeActual;
    //costeMomento = Faux.coste(distancias, tamM, solMomento);
}

void BusquedaTabu::rellena() {
    vAntiguo = solActual;
    vNuevo = solActual;
    for (int i = 0; i < tamM; i++) {
        vDistancia[i].first = solActual[i];
        vDistancia[i].second = calculaD(solActual[i]);
    }
}

float BusquedaTabu::calculaD(int numI) {
    float valorDistancia = 0.0;
    for (int i = 0; i < tamM; i++) {
        valorDistancia += consultaMatriz(numI, solActual[i]);
    }
    return valorDistancia;
}

float BusquedaTabu::consultaMatriz(int i, int j) {
    float valor = 0.0;
    if (i < j) {
        valor = distancias[i][j];
    } else {
        valor = distancias[j][i];
    }

    return valor;
}

void BusquedaTabu::ordena() {
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

void BusquedaTabu::ordenaSolActual() {
    for (int i = 0; i < tamM; i++) {
        solActual[i] = vDistancia[i].first;
    }
    vAntiguo = solActual;
    vNuevo = solActual;
    solMejorVecinos = solActual;
    //solGlobal = solActual; 
}

void BusquedaTabu::cambiarValor(int pos, int valor) {
    vNuevo[pos] = valor;
}

float BusquedaTabu::factorizacion() {
    //Calcular el aporte del coste del que queremos intercambiar con respecto a todos
    float aporteCosteViejo = 0.0;
    float aporteCosteNuevo = 0.0;
    float diferencia = 0.0;

    aporteCosteViejo = calculoAporte(vAntiguo);
    aporteCosteViejo = calculoAporte(vNuevo);

    diferencia = aporteCosteNuevo - aporteCosteViejo;
    return diferencia;
}

float BusquedaTabu::calculoAporte(vector<int> v) {
    float aporte = 0.0;
    for (int i = 0; i < tamM; i++) {
        aporte += consultaMatriz(v[i], v[posIntercambio]);
    }
    return aporte;
}

void BusquedaTabu::mejorar(float diferencia) {
    //mejora = true;
    vAntiguo = vNuevo;
    //solActual = vNuevo;
    //costeActual += diferencia;
    solMejorVecinos = vNuevo;
    costeMejorVecindario += diferencia;

    posIntercambio = 0;
}