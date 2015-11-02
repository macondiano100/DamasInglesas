#ifndef UTILIDADESVARIADAS_H_INCLUDED
#define UTILIDADESVARIADAS_H_INCLUDED

#include <sstream>
using namespace std;

void esperar(unsigned long long milisegundos);

#ifdef _WIN32
    #include <windows.h>
    void esperar(unsigned long long milisegundos){
        Sleep(milisegundos);
    }
#else
    #include <unistd.h>
    inline void esperar(unsigned long long milisegundos){
        usleep(milisegundos*1000);
    }
#endif
#endif // UTILIDADESVARIADAS_H_INCLUDED
