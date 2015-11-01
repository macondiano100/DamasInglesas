#ifndef JUGADOR_H_INCLUDED
#define JUGADOR_H_INCLUDED

#include <iostream>

#include "Coloreable.h"

using namespace std;


class Jugador:public Coloreable{
private:
    string nombre;
    string host;
    short fichasActuales;
public:
    Jugador(){
        setFichasActuales(0);
    }
    Jugador(short color){
        setColor(color);
    }
    Jugador(short color,string nombre,string host):Coloreable(color){
        setNombre(nombre);
        setHost(host);
        setFichasActuales(0);
    }
    string getNombre(){
        return nombre;
    }
    bool setNombre(string n){
        nombre=n;
        return true;
    }
    string getHost(){
        return host;
    }
    bool setHost(string h){
        host=h;
        return true;
    }
    int getFichasActuales(){
        return fichasActuales;
    }
    bool setFichasActuales(short f){
        if(f>=0){
            fichasActuales=f;
            return true;
        }
        return false;
    }
    short incrementaFichasActuales(){
        return setFichasActuales(getFichasActuales()+1)?getFichasActuales():-1;
    }
    short decrementaFichasActuales(){
        return setFichasActuales(getFichasActuales()-1)?getFichasActuales():-1;
    }



};

#endif // JUGADOR_H_INCLUDED
