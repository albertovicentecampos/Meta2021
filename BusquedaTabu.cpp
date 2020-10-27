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

BusquedaTabu::BusquedaTabu(int n, int m, vector<vector<float>> d, int ten, int tamV, int numEval, int numIntSinMov, float probDivInt, Log *log) :
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
costeActualNuevo(0.0),
l(log) {

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

    l->escribirEnArchivo("Solucion inicial aleatoria: ");
    for (int i = 0; i < tamM; i++) {
        l->escribirEnArchivoVector(to_string(solActual[i]) + ' ');
    }
    l->saltoLinea();
    l->escribirEnArchivo("Coste solucion: " + to_string(costeActual));

    posIntercambio = 0;
    int reinicializacion = 0;

    //Faux.visualizaSeleccionados(solActual, tamM);

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

                list<int>::iterator it1;
                l->escribirEnArchivo("LISTA TABU: ");
                for (it1 = listaTabu.begin(); it1 != listaTabu.end(); it1++) {
                    l->escribirEnArchivoVector(to_string(*it1) + " < - ");
                }
                l->saltoLinea();

                if (!esTabu) {
                    //Comprobamos que el elemento que cogemos de los no seleccionados no está en la lista Tabu                        
                    l->escribirEnArchivo("El elemento " + to_string(i) + " no esta en la lista tabu ");
                    if (vecino < tamVecinos) {
                        cambiarValor(posIntercambio, i);
                        //FACTORIZACION
                        costeActualNuevo = costeActual;
                        float diferencia = 0.0;
                        diferencia = factorizacion();
                        costeActualNuevo += diferencia;
                        l->escribirEnArchivo("VECINO: " + to_string(vecino + 1) + " - COSTE: " + to_string(costeActualNuevo));
                        //cout << "Mejora Coste vecindario: " << costeMejorVecindario << " es mejor que Coste actual: " << costeActual << endl;
                        if (costeMejorVecindario < costeActualNuevo) {
                            iter++;
                            costeMejorVecindario = costeActualNuevo;
                            solMejorVecinos = vNuevo;
                        }
                        elementoLT = vAntiguo[posIntercambio];
                        vecino++;
                    } else {

                        l->escribirEnArchivo("EL MEJOR VECINO DE LOS 10 VISITADOS TIENE UN COSTE DE: " + to_string(costeMejorVecindario));
                        l->escribirEnArchivo("EL MEJOR VECINO DE LOS 10 VISITADOS TIENE UNA SOLUCION DE: ");

                        for (int i = 0; i < tamM; i++) {
                            l->escribirEnArchivoVector(to_string(solMejorVecinos[i]) + ' ');
                        }

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

            list<int>::iterator it3;
            l->escribirEnArchivo("ACTUALIZAMOS LISTA TABU: ");
            for (it3 = listaTabu.begin(); it3 != listaTabu.end(); it3++) {
                l->escribirEnArchivoVector(to_string(*it3) + " < - ");
            }
            l->saltoLinea();

        }

        //cout << "iteracion: " << iter << endl;

        //Actualizamos la memoria a largo plazo 
        for (int i = 0; i < tamM; i++) {
            memLargoPlazo[solMejorVecinos[i]]++;
        }

        l->escribirEnArchivo("----- ACTUALIZAMOS MEMORIA A LARGO PLAZO ----- ");
        for (int i = 0; i < tamN; i++) {
            l->escribirEnArchivoVector(to_string(memLargoPlazo[i]) + ' ');
        }
        l->saltoLinea();


        //Comprobamos que la mejor solucion de nuestros 10 vecinos es mejor que la global
        if (costeMejorVecindario > costeGlobal) {

            l->escribirEnArchivo("COSTE MEJOR VECINO: " + to_string(costeMejorVecindario) + " ES MEJOR QUE COSTE GLOBAL: " + to_string(costeGlobal));

            cout << "Mejora Coste vecindario: " << costeMejorVecindario << " es mejor que Coste Global: " << costeGlobal << endl;
            solGlobal = solMejorVecinos;
            costeGlobal = costeMejorVecindario;

            l->escribirEnArchivo("ACTUALIZAMOS COSTE GLOBAL: " + to_string(costeGlobal));

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
            l->escribirEnArchivo("******************REINICIALIZACION DE LA BUSQUEDA******************");
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

            l->escribirEnArchivo("MEMORIA A LARGO PLAZO (valor i, valorMemLargoPlazo de i): ");
            for (int i = 0; i < tamM; i++) {
                l->escribirEnArchivoVector("(" + to_string(MLPlazo[i].first) + ',' + to_string(MLPlazo[i].second) + ") -- ");
            }
            l->saltoLinea();

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

            l->escribirEnArchivo("MEMORIA A LARGO PLAZO ORDENADA (valor i, valorMemLargoPlazo de i): ");
            for (int i = 0; i < tamM; i++) {
                l->escribirEnArchivoVector("(" + to_string(MLPlazo[i].first) + ',' + to_string(MLPlazo[i].second) + ") -- ");
            }
            l->saltoLinea();


            if (num <= probabilidadDivInt) {
                l->escribirEnArchivo("DIVERSIFICACION -> probabilidad: " + to_string(num));
                //cout << "Diversificamos" << endl;
                for (int i = 0; i < tamM; i++) {
                    solActual[i] = MLPlazo[i].first;

                }
                costeActual = Faux.coste(distancias, tamM, solActual);

            } else {
                l->escribirEnArchivo("INTENSIFICACION -> probabilidad: " + to_string(num));
                //cout << "Intensificamos" << endl;
                int tam = tamM;
                for (int i = 0; i < tamM; i++) {
                    solActual[i] = MLPlazo[i].first;
                    tam--;
                }
                costeActual = Faux.coste(distancias, tamM, solActual);
            }

            l->escribirEnArchivo("SOLUCION ACTUAL PASA A SER: ");
            for (int i = 0; i < tamM; i++) {
                l->escribirEnArchivoVector(to_string(solActual[i]) + ' ');
            }
            l->saltoLinea();
        }
    }//While


    l->escribirEnArchivo("MEJOR SOLUCION BUSQUEDA TABU: ");
    for (int j = 0; j < tamM; j++) {
        l->escribirEnArchivoVector(to_string(solGlobal[j]) + ' ');
    }
    
    l->escribirEnArchivo("COSTE MEJOR SOLUCION BUSQUEDA TABU: "+to_string(costeGlobal));
    return solGlobal;

}

void BusquedaTabu::solucionInicialAleatoria() {
    Greedy g(tamN, tamM, distancias, l);
    solActual = g.algoritmoGreedy();
    solMejorVecinos = solActual;
    solGlobal = solActual;

    costeActual = Faux.coste(distancias, tamM, solActual);
    costeGlobal = costeActual;

}

void BusquedaTabu::rellena() {
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
