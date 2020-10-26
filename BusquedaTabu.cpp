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

        //        for (int i = 0; i < tamN; i++) {
        //            cout << noSeleccionados[i] << ' ';
        //        }
        //        cout << endl;

        cout << "Ya ordenado: ";
        Faux.visualizaSeleccionados(solActual, tamM);
        cout << endl;

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
                //cout<<"Entraaaaaaaaaaaaaaaaaaaaaaaa"<<endl;
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
                        //                                                cout<<"Intercambio en la posicion: "<<posIntercambio<<" el valor de: "<<i<<endl;
                        //                                                Faux.visualizaSeleccionados(vAntiguo,tamM);
                        //                                                cout<<endl;
                        //                                                Faux.visualizaSeleccionados(vNuevo,tamM);

                        //FACTORIZACION
//                        float costeA = costeActual;
//                                                float diferencia = 0.0;
//                                                diferencia = factorizacion();
//                                                //cout<<"Diferencia: "<<diferencia<<endl;
//                                                
//                                                costeA += diferencia;                   

                        //NO FACTORIZACION

                        costeActual = Faux.coste(distancias, tamM, vNuevo);

                        //iter++;


                        //                        cout << "Mejora Coste vecindario: " << costeMejorVecindario << " es mejor que Coste actual: " << costeActual << endl;
                        if (costeMejorVecindario < costeActual) {
                            iter++;
                            costeMejorVecindario = costeActual;
                            solMejorVecinos = vNuevo;
                            //Lo marcamos true en los no seleccionados? 
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

        cout << "itercacion: " << iter << endl;

        if (!esta) {
            listaTabu.push_back(elementoLT);
            listaTabu.pop_front();
        }


//                cout << "LitaT: ";
//                list<int>::iterator it;
//                for (it = listaTabu.begin(); it != listaTabu.end(); it++) {
//                    cout << *it << "->";
//                }
//                cout << endl;

        //Actualizamos la memoria a largo plazo 
        for (int i = 0; i < tamM; i++) {
            memLargoPlazo[solMejorVecinos[i]]++;
        }

        //Sustituimos solVecindario por solActual (Para la siguiente iteracion
        solActual = solMejorVecinos;

        //Comprobamos que la mejor solucion de nuestros 10 vecinos es mejor que la global
        cout << "Mejora Coste vecindario: " << costeMejorVecindario << "y  Coste Global: " << costeGlobal << endl;
        if (costeMejorVecindario > costeGlobal) {
            cout << "Mejora Coste vecindario: " << costeMejorVecindario << " es mejor que Coste Global: " << costeGlobal << endl;
            solGlobal = solMejorVecinos;
            costeGlobal = costeMejorVecindario;
            reinicializacion = 0;
        } else {
            reinicializacion++;
        }

        costeMejorVecindario = 0;

        if (reinicializacion == numIntentosSinMov) {
            //Reinicio oscilacion estratégica
            cout << "REINCICIALIZACION" << endl;
            reinicializacion = 0;
            float num = Randfloat(0, 1);

            //iter++;
            cout << "Reini: iter: " << iter << endl;
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
                cout << "Diversificamos" << endl;
                //Diversificamos (Cogemos los del principio hasta tamM pq son los mas pequeños)
                //                cout << "Intensificamos" << endl;
                for (int i = 0; i < tamM; i++) {
                    solActual[i] = MLPlazo[i].first;
                    //solActual[i] = memAux[i];
                }
                
                                                for (int i = 0; i < tamM; i++) {
                                                    cout << solActual[i] << ' ';
                                
                                                }
                                                cout << endl;
            } else {
                cout << "Intensificamos" << endl;
                //Intensificamos (Cogemos los del final hasta tamM pq son los mas grandes)
                //                cout << "Diversificamos" << endl;
                int tam = tamM;
                for (int i = 0; i < tamM; i++) {
                    solActual[i] = MLPlazo[i].first;
                    //solActual[i] = memAux[tam];
                    tam--;
                }

                                                for (int i = 0; i < tamM; i++) {
                                                    cout << solActual[i] << ' ';
                                
                                                }
                                                cout << endl;
            }


            //memLargoPlazo.resize(tamN,0);
        }

        //iter++;

    }//While

    return solGlobal;

}

void BusquedaTabu::solucionInicialAleatoria() {
    Greedy g(tamN, tamM, distancias);
    solActual = g.algoritmoGreedy();
    solMejorVecinos = solActual;
    solGlobal = solActual;


    vAntiguo = solActual;
    vNuevo = solActual;

    //noSeleccionados = g.noSeleccionados();
    for (int i = 0; i < tamM; i++) {
        noSeleccionados[solActual[i]] = true;
    }

    //    for (int i = 0; i < tamN; i++) {
    //        cout << noSeleccionados[i] << ' ';
    //    }
    //    cout << endl;

    costeActual = Faux.coste(distancias, tamM, solActual);
    costeGlobal = costeActual;
    //costeMomento = Faux.coste(distancias, tamM, solMomento);
}

void BusquedaTabu::rellena() {
    vAntiguo = solActual;
    vNuevo = solActual;
    noSeleccionados.resize(tamN, false);
    for (int i = 0; i < tamM; i++) {
        noSeleccionados[solActual[i]] = true;
    }
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