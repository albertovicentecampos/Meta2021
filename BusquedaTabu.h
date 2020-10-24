/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BusquedaTabu.h
 * Author: Alberto
 *
 * Created on 23 de octubre de 2020, 10:14
 */

#ifndef BUSQUEDATABU_H
#define BUSQUEDATABU_H

#include <vector>
#include <list>
#include "Greedy.h"
#include "FuncionesAux.h"
#include <utility>

using namespace std; 

class BusquedaTabu {
public:
    BusquedaTabu(int n, int m, vector<vector<float>> d, int ten, int tamV, int numEval, int numIntSinMov, float probDivInt);
    vector<int> algoritmoBusquedaTabu();
    void solucionInicialAleatoria();
    void rellena();
    float calculaD(int numI);
    float consultaMatriz(int i, int j);
    void ordena();
    void ordenaSolActual();
    void cambiarValor(int pos, int valor);
    float factorizacion();
    float calculoAporte(vector<int> v);
    void mejorar(float diferencia);
    
private:
    
    int tamN;
    int tamM;
    int tamVecinos; 
    vector<vector<float>> distancias; 
    int tenencia; 
    list<int> listaTabu;
    vector<int> memLargoPlazo;
    
    FuncionesAux Faux; 
    
    vector<int> solGlobal;
    float costeGlobal;
    
    vector<int> solActual; 
    float costeActual;
    
    vector<int> solMejorVecinos; 
    float costeMejorVecindario;
    
    vector<bool> noSeleccionados; 
    
    int iter;
    int numEvaluaciones; 
    int numIntentosSinMov;
    
    int posIntercambio;
    
    vector<pair<int,float>> vDistancia;
    vector<int> vAntiguo;
    vector<int> vNuevo;

    float probabilidadDivInt; 
};

#endif /* BUSQUEDATABU_H */

