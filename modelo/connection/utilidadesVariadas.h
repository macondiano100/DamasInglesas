#ifndef UTILIDADESVARIADAS_H_INCLUDED
#define UTILIDADESVARIADAS_H_INCLUDED

#include <sstream>
using namespace std;

void esperar(unsigned long long milisegundos);
string toStr(double x);
int toInt(string s);

#ifdef _WIN32
    #include <windows.h>
    void esperar(unsigned long long milisegundos){
        Sleep(milisegundos);
    }
#else
    #include <unistd.h>
    void esperar(unsigned long long milisegundos){
        usleep(milisegundos*1000);
    }
#endif
string toStr(double x){
    stringstream str;
    str<<x;
    return str.str();
}
int toInt(string s){
    int x;
    stringstream str;
    str<<s;
    str>>x;
    return x;
}
#endif // UTILIDADESVARIADAS_H_INCLUDED
