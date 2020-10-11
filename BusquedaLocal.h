/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BusquedaLocal.h
 * Author: Alberto
 *
 * Created on 11 de octubre de 2020, 11:13
 */

#ifndef BUSQUEDALOCAL_H
#define BUSQUEDALOCAL_H

#include <vector>
#include "FuncionesAux.h"
#include "Greedy.h"
#include <utility>

using namespace std; 

class BusquedaLocal {
public:
    BusquedaLocal(int n, int m, vector<vector<float>> d, int nEvaluaciones);
    /*vector<int>*/void algoritmoBusquedaLocal();
    void solucionInicialAleatoria();
    void rellena();
    
    float calculaD(int numI);
    float consultaMatriz(int i, int j);
    
    int minimaDistancia();
    
private:
    int tamN;
    int tamM;
    vector<vector<float>> distancias;
    
    vector<pair<int, float>> vDistancia; 
    
    int numEvaluaciones; 
    int iter; 
    vector<int> solActual; 
    float costeActual; 
    
    FuncionesAux Faux; 
    
    
    
};

#endif /* BUSQUEDALOCAL_H */

