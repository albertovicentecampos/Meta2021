/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Greedy.h
 * Author: Alberto
 *
 * Created on 24 de septiembre de 2020, 18:21
 */

#ifndef GREEDY_H
#define GREEDY_H

#include <vector>
#include "FuncionesAux.h"
#include "random.h"

using namespace std; 

class Greedy {
public:
    Greedy(int n, int m, vector<vector<float>> d);
    vector<float> algoritmoGreedy();
    
    void calculoDistancias(int i);
    void mayorDistancia();
    
//    void maxDistancia();
//    void calcularDistancia();
    
private:
    int tamM;
    int tamN;   
    int primerElemento;
    float maxValor; 
    int numPos;
    
    vector<float> vDistancia;
    vector<vector<float>> distancias;
    vector<float> seleccionadosM;
    
    vector<int> vN;
    
    FuncionesAux aux; 
};

#endif /* GREEDY_H */

