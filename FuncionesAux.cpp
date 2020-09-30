/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FuncionesAux.cpp
 * Author: Alberto
 * 
 * Created on 23 de septiembre de 2020, 11:44
 */

#include "FuncionesAux.h"

void FuncionesAux::cargaFichero(string nombre, vector<vector<float>> &matriz, long& n, long& m) {
    string linea;

    fstream fe(nombre.c_str(), ios::in);
    if (!fe) {
        throw errorFichero();
    }

    fe>>n;
    fe>>m;
    
    getline(fe, linea);
    matriz.resize(n, vector<float>(n));

    while (!fe.eof()) {
        float num1 = 0;
        float num2 = 0;
       
        fe>>num1;
        fe>>num2;
        fe >> matriz[num1][num2];

        getline(fe, linea);
    }

    fe.close();
}

void FuncionesAux::cargaParametrizacion(string nombreArchivo, vector<string>& archivos, vector<string>& nombreAlg, vector<int>& numSemilla, int& numIntentosSinMov, float& probIntDiv, int& numEvalFuncionObj, int& numMaxVecinosVisit, int& tenenciaTabu) {

    ifstream archivo_entrada;
    string linea;
    int i = 0;
    int numCampos = 0;

    archivo_entrada.open(nombreArchivo);
    while (getline(archivo_entrada, linea)) {
        numCampos++;
    };
    string* datos = new string [numCampos];

    archivo_entrada.clear();
    archivo_entrada.seekg(0, archivo_entrada.beg);

    while (getline(archivo_entrada, linea)) {
        datos[i] = linea;
        i++;
    };

    string arch = datos[0];
    string alg = datos[1];
    string sem = datos[2];
    string numIntentos = datos[3];
    string prob = datos[4];
    string numEval = datos[5];
    string numMaxVecinos = datos[6];
    string tenencia = datos[7];

    //ARCHIVOS
    istringstream isstream1(arch);
    string palabra1;
    while (isstream1 >> palabra1) {
        archivos.push_back(palabra1);
    }
    archivos.erase(archivos.begin());


    //ALGORITMOS
    istringstream isstream2(alg);
    string palabra2;
    while (isstream2 >> palabra2) {
        nombreAlg.push_back(palabra2);
    }
    nombreAlg.erase(nombreAlg.begin());


    //SEMILLAS
    istringstream isstream3(sem);
    string palabra3;
    while (isstream3 >> palabra3) {
        numSemilla.push_back(atoi(palabra3.c_str()));
    }
    numSemilla.erase(numSemilla.begin());


    //NUMERO DE INTENTOS SIN MOVIMIENTO
    vector<string> nIntentos;
    istringstream isstream4(numIntentos);
    string palabra4;
    while (isstream4 >> palabra4) {
        nIntentos.push_back(palabra4);
    }
    nIntentos.erase(nIntentos.begin());
    numIntentosSinMov = stof(nIntentos[0].c_str());


    //PROBABILIDAD DE INTENSIFICACION-DIVERSIFICACION
    vector<string> pr;
    istringstream isstream5(prob);
    string palabra5;
    while (isstream5 >> palabra5) {
        pr.push_back(palabra5);
    }
    pr.erase(pr.begin());
    probIntDiv = stof(pr[0].c_str());


    //NUMERO DE EVALUACIONES DE LA FUNCIÓN OBJETIVO
    vector<string> nEval;
    istringstream isstream6(numEval);
    string palabra6;
    while (isstream6 >> palabra6) {
        nEval.push_back(palabra6);
    }
    nEval.erase(nEval.begin());
    numEvalFuncionObj = stof(nEval[0].c_str());


    //NUMERO MAXIMO DE VECINOS VISITADOS
    vector<string> nV;
    istringstream isstream7(numMaxVecinos);
    string palabra7;
    while (isstream7 >> palabra7) {
        nV.push_back(palabra7);
    }
    nV.erase(nV.begin());
    numMaxVecinosVisit = stof(nV[0].c_str());


    //TENENCIA TABU
    vector<string> ten;
    istringstream isstream8(tenencia);
    string palabra8;
    while (isstream8 >> palabra8) {
        ten.push_back(palabra8);
    }
    ten.erase(ten.begin());
    tenenciaTabu = stof(ten[0].c_str());


}
