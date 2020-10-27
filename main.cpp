/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Alberto
 *
 * Created on 23 de septiembre de 2020, 11:40
 */

#include <cstdlib>
#include <vector>

#include "FuncionesAux.h"
#include "Greedy.h"
#include "BusquedaLocal.h"
#include "BusquedaTabu.h"
#include "random.h"
#include "timer.h"
#include "Log.h"


using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    string registro = "registro.txt";
    vector<string> archivos;
    vector<string> algoritmos;
    vector<int> semillas;
    int numIntentosSinMov;
    float probIntDiv;
    int numEval;
    int numVecinosVisit;
    int tenenciaTabu;
    int ncadaIter;
    FuncionesAux Faux;


    //Cargamos los datos
    Faux.cargaParametrizacion(registro, archivos, algoritmos, semillas, numIntentosSinMov, probIntDiv, numEval, numVecinosVisit, tenenciaTabu);

    for (int i = 0; i < archivos.size(); i++) {

        vector<vector<float>> matrizDistancias;
        int n;
        int m;
        Faux.cargaFichero(archivos[i], matrizDistancias, n, m);

        for (int j = 0; j < algoritmos.size(); j++) {
            for (int k = 0; k < semillas.size(); k++) {
                Set_random(semillas[k]);
                if (algoritmos[j] == "greedy") {
                    Log *l = new Log(archivos[i], semillas[k], algoritmos[j]);
                    l->crearArchivo();
                    l->escribirEnArchivo("ARCHIVO " + archivos[i] + " CON SEMILLA " + to_string(semillas[k]) + " UTILIZANDO EL ALGORITMO " + algoritmos[j]);
                    l->saltoLinea();
                    float coste = 0.0;
                    vector<int> seleccionados;
                    seleccionados.resize(m, 0);

                    Greedy* g = new Greedy(n, m, matrizDistancias, l);
                    start_timers();
                    seleccionados = g->algoritmoGreedy();
                    elapsed_time();

                    //Faux.visualizaSeleccionados(seleccionados, m);
                    coste = Faux.coste(matrizDistancias, m, seleccionados);

                    cout << "Tiempo en realizar e algoritmo: " + to_string(elapsed_time()) + " segundos" << endl;
                    l->escribirEnArchivo("COSTE GREEDY: " + to_string(coste));
                    l->saltoLinea();
                    l->escribirEnArchivo("Tiempo en realizar el algoritmo: " + to_string(elapsed_time()) + " segundos");
                    l->cerrarArchivo();
                    //                    cout << endl;
                    //                    cout << "Coste: " << coste << endl;

                } else if (algoritmos[j] == "blocal") {
                    Log *l2 = new Log(archivos[i], semillas[k], algoritmos[j]);
                    l2->crearArchivo();
                    l2->escribirEnArchivo("ARCHIVO " + archivos[i] + " CON SEMILLA " + to_string(semillas[k]) + " UTILIZANDO EL ALGORITMO " + algoritmos[j]);
                    l2->saltoLinea();

                    float coste = 0.0;
                    vector<int> seleccionados;
                    seleccionados.resize(m, 0);

                    BusquedaLocal* bl = new BusquedaLocal(n, m, matrizDistancias, numEval, l2);
                    start_timers();
                    seleccionados = bl->algoritmoBusquedaLocal();
                    elapsed_time();

                    Faux.visualizaSeleccionados(seleccionados, m);
                    coste = Faux.coste(matrizDistancias, m, seleccionados);

                    cout << "Tiempo en realizar e algoritmo: " + to_string(elapsed_time()) + " segundos" << endl;
                    l2->escribirEnArchivo("COSTE BUSQUEDA LOCAL: " + to_string(coste));
                    l2->saltoLinea();
                    l2->escribirEnArchivo("Tiempo en realizar el algoritmo: " + to_string(elapsed_time()) + " segundos");
                    l2->cerrarArchivo();
                    cout << endl;
                    cout << "Coste: " << coste << endl;

                } else if (algoritmos[j] == "btabu") {

                    Log *l3 = new Log(archivos[i], semillas[k], algoritmos[j]);
                    l3->crearArchivo();
                    l3->escribirEnArchivo("ARCHIVO " + archivos[i] + " CON SEMILLA " + to_string(semillas[k]) + " UTILIZANDO EL ALGORITMO " + algoritmos[j]);
                    l3->saltoLinea();


                    float coste = 0.0;
                    vector<int> seleccionados;
                    seleccionados.resize(m, 0);

                    BusquedaTabu* bt = new BusquedaTabu(n, m, matrizDistancias, tenenciaTabu, numVecinosVisit, numEval, numIntentosSinMov, probIntDiv, l3);
                    start_timers();
                    seleccionados = bt->algoritmoBusquedaTabu();
                    elapsed_time();

                    Faux.visualizaSeleccionados(seleccionados, m);
                    coste = Faux.coste(matrizDistancias, m, seleccionados);

                    cout << "Tiempo en realizar e algoritmo: " + to_string(elapsed_time()) + " segundos" << endl;
                    l3->escribirEnArchivo("COSTE BUSQUEDA TABU: " + to_string(coste));
                    l3->saltoLinea();
                    l3->escribirEnArchivo("Tiempo en realizar el algoritmo: " + to_string(elapsed_time()) + " segundos");
                    l3->cerrarArchivo();
                    cout << endl;
                    cout << "Coste: " << coste << endl;

                }
            }
        }
    }

    return 0;
}

