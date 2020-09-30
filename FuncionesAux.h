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

using namespace std;

class errorFichero {
public:
    const char* what() const throw () {
        return "Error: Lectura de fichero";
    }
};

class FuncionesAux {
public:
    void cargaFichero(string nombre, vector<vector<float>> &matriz, long& n, long& m);
    void cargaParametrizacion(string nombreArchivo, vector<string>& archivos, vector<string>& nombreAlg, vector<int>& numSemilla, int& numIntentosSinMov, float& probIntDiv, int& numEvalFuncionObj, int& numMaxVecinosVisit, int& tenenciaTabu);
};

#endif /* FUNCIONESAUX_H */

