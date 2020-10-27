/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Log.h
 * Author: Luis
 *
 * Created on 23 de octubre de 2019, 19:16
 */

#ifndef LOG_H
#define LOG_H
#include <string>
#include <fstream>

using namespace std;

class Log {
public:
    Log(string _nArchivo,int _semilla,string _nAlg);
    Log(const Log& orig);
    virtual ~Log();
    void crearArchivo();
    void escribirEnArchivo(string p);
    void escribirEnArchivoVector(string p);
    void saltoLinea();
    void cerrarArchivo();
    
private:
    ofstream log;
    string log_file;
    
    string nArchivo;
    int semilla;
    string nAlg;
};

#endif /* LOG_H */

