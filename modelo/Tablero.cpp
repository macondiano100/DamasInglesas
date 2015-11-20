#include "Tablero.h"


Jugador *Tablero::getPrimerJugador(){
    return primerJugador;
}

void Tablero::setPrimerJugador(Jugador *j){
    if(getPrimerJugador())delete primerJugador;
    primerJugador=j;
    if(getPrimerJugador())getPrimerJugador()->setFichasActuales(piezasPorJugador);
}

Jugador *Tablero::getSegundoJugador(){
    return segundoJugador;
}


void Tablero::setSegundoJugador(Jugador *j){
    if(getSegundoJugador())delete segundoJugador;
    segundoJugador=j;
    if(getSegundoJugador())getSegundoJugador()->setFichasActuales(piezasPorJugador);
}

bool Tablero::agregarFicha(short fila, short columna, short color){
    if(esCasillaDentroDelRango(fila,columna)){
        fichas[fila][columna]=new Ficha(color);
        return true;
    }
    else{
        return false;
    }
}


bool Tablero::esMovimientoValido(short filaO, short colO, short filaD, short colD){
    if(esCasillaDentroDelRango(filaO,colO)&&esCasillaDentroDelRango(filaD,colD)
            &&!casillaVacia(filaO,colO)&&casillaVacia(filaD,colD))
    {
        return esMovimientoSimple(filaO,colO,filaD,colD)
                ||esMovimientoConSalto(filaO,colO,filaD,colD);
    }
    return false;
}
bool Tablero::esMovimientoConSalto(Ficha *ficha, short filaO, short colO, short filaD, short colD)
{
    bool movimientoValido;
    if(consultar(filaD,colD)) return false;
    if((filaD==filaO-2)&&(colD==colO+2||colD==colO-2))
        movimientoValido=(ficha->getColor()==COLOR_ROJO||
                (ficha->getColor()==COLOR_BLANCO&&ficha->esDobleMovimiento()));
    else if((filaD==filaO+2)&&(colD==colO+2||colD==colO-2))
        movimientoValido=(ficha->getColor()==COLOR_BLANCO||
                (ficha->getColor()==COLOR_ROJO&&ficha->esDobleMovimiento()));
    else return false;
    Ficha *fichaAComer=consultar((filaO+filaD)/2,(colO+colD)/2);
    return movimientoValido&&
            fichaAComer!=nullptr&&
            (fichaAComer->getColor()!=ficha->getColor());
}

bool Tablero::casillaVacia(short fila, short col)
{
    return !consultar(fila,col);
}
bool Tablero::esMovimientoConSalto(short filaO, short colO, short filaD, short colD)
{
    Ficha *ficha=consultar(filaO,colO);
    return ficha!=nullptr&&esMovimientoConSalto(ficha,filaO,colO,filaD,colD);
}
ResultadoDeMovimiento Tablero::moverFicha(short filaOrig, short columnaOrig, short filaDest, short columnaDest){
    Ficha* ficha=consultar(filaOrig,columnaOrig);
    ResultadoDeMovimiento res;
    bool movimientoValido;
    if(ficha!=nullptr)
    {
        if(esCasillaDentroDelRango(filaDest,columnaDest)){
            if(casillaVacia(filaDest,columnaDest)){
                if(esMovimientoSimple(filaOrig,columnaOrig,filaDest,columnaDest)){
                movimientoValido=true;
                res=TURNO_FINALIZADO;
                }
                else if(esMovimientoConSalto(filaOrig,columnaOrig,filaDest,columnaDest)){
                    movimientoValido=true;
                    otorgarFicha((filaOrig+filaDest)/2,(columnaOrig+columnaDest)/2);
                    res=puedeContinuar(filaDest,columnaDest,ficha)?
                               SIGUE_MOVIENDO:TURNO_FINALIZADO;
                }
                else
                {
                    res=MOVIMIENTO_INVALIDO;
                    movimientoValido=false;
                }
                if(movimientoValido)
                {
                    fichas[filaOrig][columnaOrig]=nullptr;
                    fichas[filaDest][columnaDest]=ficha;
                    if( (ficha->getColor()==COLOR_BLANCO&&filaDest==TAMANIO_TABLERO-1) ||
                            (ficha->getColor()==COLOR_ROJO&&filaDest==0))
                    {
                        ficha->setDobleMovimiento(true);
                        res=(puedeContinuar(filaDest,columnaDest,ficha)?
                                 DOBLE_MOV_Y_SIGUE_JUGANDO:
                                 ADQUIERE_DOBLE_MOVIMIENTO_LA_FICHA);
                    }
                    cout<<"Piezas 1 "<<primerJugador->getFichasActuales()<<endl;
                    cout<<"Piezas 2 "<<segundoJugador->getFichasActuales()<<endl;
                    if(primerJugador->getFichasActuales()==0||segundoJugador->getFichasActuales()==0)
                        res=SE_HA_GANADO_LA_PARTIDA;
                }
            }
            else{
                res=POSICION_OCUPADA;
            }
        }
        else
        {
            res=MOVIMIENTO_INVALIDO;
        }
        return res;
    }
    else
    {
        return NO_HAY_FICHA_QUE_MOVER;
    }

}

Tablero &Tablero::operator=(Tablero&& other)
{
    if(primerJugador!=nullptr)delete primerJugador;
    if(segundoJugador!=nullptr)delete segundoJugador;
    for(short i=0,j=getTamanio(),l;i<j;i++){
        for(l=0;l<j;l++){
            delete fichas[i][l];
        }
        delete fichas[i];
    }
    delete fichas;
    fichas=other.fichas;
    other.fichas=nullptr;
    primerJugador=other.primerJugador;
    segundoJugador=other.segundoJugador;
    tamanio=other.tamanio;
    piezasPorJugador=other.piezasPorJugador;
    return *this;
}

void Tablero::setTamanio(short t){
    tamanio=t;
}

void Tablero::setPiezasPorJugador(short p){
    piezasPorJugador=p;
}

bool Tablero::esCasillaDentroDelRango(short fila, short columna){
    short tamanio=getTamanio();
    return fila>=0 && fila<tamanio && columna>=0 && columna<tamanio;
}

bool Tablero::otorgarFicha(short fila, short columna){
    Ficha *ficha=consultar(fila,columna);
    if(ficha!=nullptr){
        (ficha->getColor()==COLOR_ROJO?getPrimerJugador():getSegundoJugador())->decrementaFichasActuales();
        fichas[fila][columna]=nullptr;
        delete ficha;
        return true;
    }
    else{
        return false;
    }
}

bool Tablero::esMovimientoSimple
(short filaO, short colO, short filaD, short colD)
{
    Ficha *ficha=consultar(filaO,colO);
    if((filaD==filaO-1)&&(colD==colO+1||colD==colO-1))
        return (ficha->getColor()==COLOR_ROJO||
                (ficha->getColor()==COLOR_BLANCO&&ficha->esDobleMovimiento()));
    else if((filaD==filaO+1)&&(colD==colO+1||colD==colO-1))
        return (ficha->getColor()==COLOR_BLANCO||
                (ficha->getColor()==COLOR_ROJO&&ficha->esDobleMovimiento()));
    else
            return false;
}


Tablero::Tablero(short tamanio, Jugador *primerJugador, Jugador *segundoJugador){
    short i,j,piezasPorJugador,piezasAgregadas,preJ;
    this->primerJugador=primerJugador;
    this->segundoJugador=segundoJugador;
    setTamanio(tamanio);
    setPiezasPorJugador(piezasPorJugador=12);
    if(primerJugador!=nullptr && segundoJugador!=nullptr){
        getPrimerJugador()->setFichasActuales(piezasPorJugador);
        getSegundoJugador()->setFichasActuales(piezasPorJugador);
    }
    fichas=new Ficha**[tamanio];
    for(i=0;i<tamanio;i++){
        fichas[i]=new Ficha*[tamanio];
        for(j=0;j<tamanio;j++){
            fichas[i][j]=NULL;
        }
    }
    //Todas las fichas van en las cuadros negros, primer cuadro negra fila=0, col=1
    /*Agregamos las fichas del primer jugador
            es decir, fichas rojas en casillas negras, 0..2...n
        */
    if(primerJugador)
    {
        for(i=piezasAgregadas=0,j=1;piezasAgregadas<piezasPorJugador;i++){
            for(preJ=j;j<tamanio;j+=2,piezasAgregadas++){
                agregarFicha(i,j,COLOR_BLANCO);
            }
            j=preJ==0?1:0;
        }

    }
    /*Agregamos las fichas del segundo jugador
            es decir, fichas blanca en casillas rojas, 1..3...n
        */
    if(segundoJugador)
    {
        for(piezasAgregadas=0,i=tamanio-1,j=0;piezasAgregadas<piezasPorJugador;i--){
            for(preJ=j;j<tamanio;j+=2,piezasAgregadas++){
                agregarFicha(i,j,COLOR_ROJO);
            }
            j=preJ==0?1:0;

        }

    }
}

Tablero::Tablero(Jugador *primerJugador, Jugador *segundoJugador):Tablero(TAMANIO_TABLERO,primerJugador,
                                                                          segundoJugador){
}

Tablero::Tablero(short tamanio):Tablero(tamanio,NULL,NULL){
}

Tablero::Tablero():Tablero(TAMANIO_TABLERO){
}

Tablero::~Tablero(){
    if(fichas!=nullptr)
    {
     for(short i=0,j=getTamanio(),l;i<j;i++){
            for(l=0;l<j;l++){
                delete fichas[i][l];
            }
            delete fichas[i];
        }
        delete fichas;
    }
}

short Tablero::getPiezasPorJugador(){
    return piezasPorJugador;
}

short Tablero::getTamanio(){
    return tamanio;
}

bool Tablero::puedeContinuar(short fila, short col,Ficha* ficha){
    int filaD,colD;
    filaD=fila+2;
    colD=col+2;
    if(esCasillaDentroDelRango(filaD,colD)&&esMovimientoConSalto(ficha,fila,col,filaD,colD)){
          return true;
    }
    colD=col-2;
    if(esCasillaDentroDelRango(filaD,colD)&&esMovimientoConSalto(ficha,fila,col,filaD,colD)){
        return true;
    }
    filaD=fila-2;
    colD=col+2;
    if(esCasillaDentroDelRango(filaD,colD)&&esMovimientoConSalto(ficha,fila,col,filaD,colD))
    {
            return true;
        }
    colD=col-2;
    if(esCasillaDentroDelRango(filaD,colD)&&esMovimientoConSalto(ficha,fila,col,filaD,colD)){
         return true;
    }
    return false;
}

Tablero::Tablero(Tablero &&other):primerJugador(other.primerJugador),
    segundoJugador(other.segundoJugador),
    tamanio(other.tamanio),piezasPorJugador(other.piezasPorJugador),fichas(other.fichas){}
