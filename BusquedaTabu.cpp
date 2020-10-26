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
probabilidadDivInt(probDivInt),
costeActualNuevo(0.0) {

    vDistancia.reserve(tamM);

    solGlobal.resize(tamM, 0);
    solActual.resize(tamM, 0);
    solMejorVecinos.resize(tamM, 0);

    listaTabu.resize(tenencia, -1);
    memLargoPlazo.resize(tamN, 0);

    noSeleccionados.resize(tamN, false);

    vNuevo.resize(tamM, 0);
    vAntiguo.resize(tamM, 0);
}

vector<int> BusquedaTabu::algoritmoBusquedaTabu() {

    solucionInicialAleatoria();
    posIntercambio = 0;
    int reinicializacion = 0;
    Faux.visualizaSeleccionados(solActual, tamM);

    while (iter < numEvaluaciones) {

        rellena();
        ordena();
        ordenaSolActual();
        int vecino = 0;
        int elementoLT = 0;

        vAntiguo = solActual;
        vNuevo = solActual;

        for (int i = 0; i < tamN; i++) {
            noSeleccionados[i] = false;
        }

        for (int i = 0; i < tamM; i++) {
            noSeleccionados[solActual[i]] = true;
        }


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
                        //FACTORIZACION
                        costeActualNuevo = costeActual;
                        float diferencia = 0.0;
                        diferencia = factorizacion();
                        costeActualNuevo += diferencia;

                        //cout << "Mejora Coste vecindario: " << costeMejorVecindario << " es mejor que Coste actual: " << costeActual << endl;
                        if (costeMejorVecindario < costeActualNuevo) {
                            iter++;
                            costeMejorVecindario = costeActualNuevo;
                            solMejorVecinos = vNuevo;
                        }
                        elementoLT = vAntiguo[posIntercambio];
                        vecino++;
                    } else {
                        //Si lleva 10 vecinos paramos
                        break;
                    }
                }
            }
        }//for

        //Añadimos a la lista Tabu 
        bool esta = false;
        list<int>::iterator it2;
        for (it2 = listaTabu.begin(); it2 != listaTabu.end(); it2++) {
            if (elementoLT == *it2) {
                esta = true;
            }
        }
        if (!esta) {
            listaTabu.push_back(elementoLT);
            listaTabu.pop_front();
        }

        cout << "iteracion: " << iter << endl;

        //Actualizamos la memoria a largo plazo 
        for (int i = 0; i < tamM; i++) {
            memLargoPlazo[solMejorVecinos[i]]++;
        }

        //Comprobamos que la mejor solucion de nuestros 10 vecinos es mejor que la global
        if (costeMejorVecindario > costeGlobal) {
            cout << "Mejora Coste vecindario: " << costeMejorVecindario << " es mejor que Coste Global: " << costeGlobal << endl;
            solGlobal = solMejorVecinos;
            costeGlobal = costeMejorVecindario;
            reinicializacion = 0;
        } else {
            reinicializacion++;
        }

        //Sustituimos solMejorVecindario por solActual (Para la siguiente iteracion)
        //Lo sustituimos aunque nos empeore la solucionActual
        solActual = solMejorVecinos;
        costeActual = costeMejorVecindario;
        costeMejorVecindario = 0;

        
        if (reinicializacion == numIntentosSinMov) {
            //cout << "REINCICIALIZACION DE LA BUSQUEDA" << endl;
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

            if (num <= probabilidadDivInt) {
                //cout << "Diversificamos" << endl;
                for (int i = 0; i < tamM; i++) {
                    solActual[i] = MLPlazo[i].first;
                    
                }
                costeActual = Faux.coste(distancias, tamM, solActual);

            } else {
                //cout << "Intensificamos" << endl;
                int tam = tamM;
                for (int i = 0; i < tamM; i++) {
                    solActual[i] = MLPlazo[i].first;
                    tam--;
                }
                costeActual = Faux.coste(distancias, tamM, solActual);
            }
        }
    }//While

    return solGlobal;

}

void BusquedaTabu::solucionInicialAleatoria() {
    Greedy g(tamN, tamM, distancias);
    solActual = g.algoritmoGreedy();
    solMejorVecinos = solActual;
    solGlobal = solActual;

    //
    //    vAntiguo = solActual;
    //    vNuevo = solActual;

    //noSeleccionados = g.noSeleccionados();
    //    for (int i = 0; i < tamM; i++) {
    //        noSeleccionados[solActual[i]] = true;
    //    }

    //    for (int i = 0; i < tamN; i++) {
    //        cout << noSeleccionados[i] << ' ';
    //    }
    //    cout << endl;

    costeActual = Faux.coste(distancias, tamM, solActual);
    costeGlobal = costeActual;
    //costeMomento = Faux.coste(distancias, tamM, solMomento);
}

void BusquedaTabu::rellena() {
    //    vAntiguo = solActual;
    //    vNuevo = solActual;
    //    noSeleccionados.resize(tamN, false);
    //    for (int i = 0; i < tamM; i++) {
    //        noSeleccionados[solActual[i]] = true;
    //    }
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
    //    vAntiguo = solActual;
    //    vNuevo = solActual;
    //solMejorVecinos = solActual;
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
    aporteCosteNuevo = calculoAporte(vNuevo);

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