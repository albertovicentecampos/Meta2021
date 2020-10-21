/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FuncionesAux.h
 * Author: Alberto
 *
 * Created on 23 de septiembre de 2020, 11:44
 */

#ifndef FUNCIONESAUX_H
#define FUNCIONESAUX_H

#include <string>
#include <vector>
#include <fstream>
#include <iosfwd>
#include <sstream>
#include <iostream>

#include <set>
using namespace std;

class errorFichero {
public:
    const char* what() const throw () {
        return "Error: Lectura de fichero";
    }
};

class FuncionesAux {
public:
    void cargaFichero(string nombre, vector<vector<float>> &matriz, int& n, int& m);
    void cargaParametrizacion(string nombreArchivo, vector<string>& archivos, vector<string>& nombreAlg, vector<int>& numSemilla, int& numIntentosSinMov, float& probIntDiv, int& numEvalFuncionObj, int& numMaxVecinosVisit, int& tenenciaTabu);
    float coste(vector<vector<float>> matrizDistancias, int m, vector<int> seleccionados);
    void visualizaMatriz(vector<vector<float>> matrizDistancias, int n);
    void visualizaSeleccionados(vector<int> seleccionados, int m);
    void rellena(vector<vector<float>> matrizDistancias, int n, set<int> &N);
    
    float calculoAporte(vector<vector<float> > distancias, vector<int> v,int tam, int pos);
    float consultaMatriz(vector<vector<float>> distancias,int i, int j);
    void cambiarValor(vector<int> v, int pos, int valor);
    
    
    
    
};

#endif /* FUNCIONESAUX_H */

