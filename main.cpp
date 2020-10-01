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
    Set_random(1);


    //Cargamos los datos
    Faux.cargaParametrizacion(registro, archivos, algoritmos, semillas, numIntentosSinMov, probIntDiv, numEvalFuncionObj, numMaxVecinosVisit, tenenciaTabu);


    for (int i = 0; i < archivos.size(); i++) {
        vector<vector<float>> matrizDistancias;
        long n = 0;
        long m = 0;

        //Leemos el fichero de registro y ejecutamos todos los archivos que tenemos y le creamos su matriz de distancias 
        Faux.cargaFichero(archivos[i], matrizDistancias, n, m);

        cout << n << " " << m << endl;

        //        for(int i = 0 ; i < n; i++){
        //            for(int j = 0; j < n; j++){
        //                cout<<matrizDistancias[i][j]<<' ';
        //            }
        //            cout<<endl; 
        //        }

        vector<int> solu;
        vector<float> seleccionados;
        Greedy g(n, m, matrizDistancias, solu);

        seleccionados = g.algoritmoGreedy();

        for (int i = 0; i < m; i++) {
            cout << seleccionados[i] << ' ';

        }


        float coste = 0.0;
        for (int i = 0; i < m-1; i++) {
            for (int j = i+1; j < m; j++) {
                coste += matrizDistancias[seleccionados[i]][seleccionados[j]];
            }
        }

        cout<<endl; 
        cout << "Coste: " << coste <<endl; 


    }








    return 0;
}

