#ifndef FICHA_H_INCLUDED
#define FICHA_H_INCLUDED

#include "Coloreable.h"

class Ficha:public Coloreable{
private:
    bool dobleMovimiento;
public:
    Ficha(){
    }
    Ficha(short color,bool dobleMovimiento):Coloreable(color){
        setDobleMovimiento(dobleMovimiento);
    }
    Ficha(short color):Ficha(color,false){
    }
    bool esDobleMovimiento(){
        return dobleMovimiento;
    }
    void setDobleMovimiento(bool d){
        dobleMovimiento=d;
    }
};


#endif // FICHA_H_INCLUDED
