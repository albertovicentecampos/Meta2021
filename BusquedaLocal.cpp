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
        ordena();
        ordenaSolActual();

        //        cout<<"VECTOR: "<<endl;
        //        for(int  i = 0; i < tamM; i++){
        //            cout<<vDistancia[i].second<< ' ';
        //        }

        bool mejora = false;

        for (int i = 0; i < tamN; i++) {
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
//                    cout << "INTERCAMBIAMOS LA POS: " << posIntercambio << "El valor de " << i << endl;
//                    cout << "Vector Antiguo: ";
//                    for (int i = 0; i < tamM; i++) {
//                        cout << vAntiguo[i] << ' ';
//                    }
//                    cout<<"Coste antiguo: "<<costeActual<<endl;
//                    cout<<endl; 
//                    cout << "Vector Nuevo: ";
//
//                    for (int i = 0; i < tamM; i++) {
//                        cout << vNuevo[i] << ' ';
//                    }
//                    
//                    cout<<"Coste nuevo: "<<costeActual+diferencia<<endl;

                    mejora = true;
                    vAntiguo = vNuevo;
                    solActual = vNuevo;
                    costeActual += diferencia;

                   // noSeleccionados[posIntercambio] = false;

                    noSeleccionados[i] = true;
                    posIntercambio = 0;
                    //cout << "Coste:" << costeActual << endl;
                    break;

                }
            }
        } //FOR


        //Comprobamos si no ha mejorado con todos los selecionados
        //cout << "Diferencia: " << diferencia << endl;

        if (!mejora) {
            //cout<<"Entra"<<endl; 
            //Se pasa al siguiente menor de la lista di
            //noSeleccionados[valorP] = true;
            posIntercambio++;

        }

        //Se detiene la búsqueda si no mejora ningun vecino 
        //Para que no se salga del array
        if (posIntercambio == tamM - 1) {
            break;
        }


        //cout << "PosIntercambio: " << posIntercambio << endl;
    }

    cout << "VECTOR: " << endl;
    for (int i = 0; i < tamM; i++) {
        cout << vDistancia[i].first << ' ';
    }
    cout<<endl;
    cout << "Coste:" << costeActual << endl;
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
    vAntiguo = solActual;
    vNuevo = solActual;
    for (int i = 0; i < tamM; i++) {
        vDistancia[i].first = solActual[i];
        vDistancia[i].second = calculaD(solActual[i]);
    }
}

void BusquedaLocal::ordena() {
    //float temporal;
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

void BusquedaLocal::ordenaSolActual(){
    for(int i = 0; i<tamM; i++){
        solActual[i] = vDistancia[i].first;
    }
    
    vAntiguo = solActual;
    vNuevo = solActual; 
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