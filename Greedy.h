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
    Greedy(long n, long m, vector<vector<float>> d);
    Greedy(const Greedy& orig);
    vector<float> algoritmoGreedy();
    
    void calculoDistancias(int i, int j);
    void mayorDistancia();
    virtual ~Greedy();
private:
    int tamM;
    int tamN;   
    int primerElemento;
    float maxValor; 
    int numPos;
    
    vector<float> vDistancia;
    vector<vector<float>> distancias;
    vector<float> seleccionadosM;
    
    FuncionesAux aux; 
};

#endif /* GREEDY_H */

