#ifndef COLOREABLE_H_INCLUDED
#define COLOREABLE_H_INCLUDED


enum{
    COLOR_ROJO=1,
    COLOR_BLANCO
};

class Coloreable{
private:
    short color;
public:
    Coloreable(){
    }
    Coloreable(short color){
        setColor(color);
    }
    short getColor(){
        return color;
    }
    bool setColor(short c){
        if(c==COLOR_ROJO || c==COLOR_BLANCO){
            color=c;
            return true;
        }
        else{
            return false;
        }
    }
};
#endif // COLOREABLE_H_INCLUDED
