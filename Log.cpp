/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Log.cpp
 * Author: Luis
 * 
 * Created on 23 de octubre de 2019, 19:16
 */

#include "Log.h"

Log::Log(string _nArchivo,int _semilla,string _nAlg): 
nArchivo(_nArchivo), semilla(_semilla), nAlg(_nAlg)
{

}

Log::Log(const Log& orig) {
}

Log::~Log() {
}

void Log::crearArchivo(){
    log_file.assign(nArchivo+"-"+nAlg+"-"+to_string(semilla)+".txt");
    log.open(log_file.c_str()); 
}

void Log::escribirEnArchivo(string p){
        log<<p<<endl;
}

void Log::escribirEnArchivoVector(string p){
    log<<p;
}

void Log::saltoLinea(){
    log<<endl;
}

void Log::cerrarArchivo(){
    log.close();
}