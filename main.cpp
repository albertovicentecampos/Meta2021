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
#include "FuncionesAux.h"
#include "Greedy.h"
#include <vector>

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
    int numEvalFuncionObj;
    int numMaxVecinosVisit;
    int tenenciaTabu;
    int ncadaIter;
    FuncionesAux Faux;


    Set_random(26532177);

    //Cargamos los datos
    Faux.cargaParametrizacion(registro, archivos, algoritmos, semillas, numIntentosSinMov, probIntDiv, numEvalFuncionObj, numMaxVecinosVisit, tenenciaTabu);

    for (int i = 0; i < archivos.size(); i++) {
        vector<vector<float>> matrizDistancias;
        long n = 0;
        long m = 0;
        float coste = 0.0;

        Faux.cargaFichero(archivos[i], matrizDistancias, n, m);

        for (int j = 0; j < algoritmos.size(); j++) {
            for (int k = 0; k < semillas.size(); k++) {
                Set_random(semillas[k]);
                if (algoritmos[j] == "greedy") {
                    
                    vector<float> seleccionados;

                    Greedy g(n, m, matrizDistancias);
                    seleccionados = g.algoritmoGreedy();

                    Faux.visualizaSeleccionados(seleccionados, m);
                    coste = Faux.coste(matrizDistancias, m, seleccionados);

                    cout << endl;
                    cout << "Coste: " << coste << endl;

                } else if (algoritmos[j] == "blocal") {

                } else if (algoritmos[j] == "btabu") {

                }
            }
        }
    }

    return 0;
}

