#ifndef TABLERO_H_INCLUDED
#define TABLERO_H_INCLUDED

#include <iostream>

#include "Ficha.h"
#include "Jugador.h"

using namespace std;

constexpr short TAMANIO_TABLERO=8;

typedef enum{
    MOVIMIENTO_INVALIDO=1,
    SIGUE_MOVIENDO,
    POSICION_OCUPADA,

    NO_HAY_FICHA_QUE_MOVER,
    TURNO_FINALIZADO,
    ADQUIERE_DOBLE_MOVIMIENTO_LA_FICHA,

    DOBLE_MOV_Y_SIGUE_JUGANDO,
    SE_HA_GANADO_LA_PARTIDA,
}ResultadoDeMovimiento;

class Tablero{
private:
    Jugador* primerJugador;
    Jugador* segundoJugador;
    short tamanio;
    short piezasPorJugador;
    void setTamanio(short t);
    void setPiezasPorJugador(short p);
    bool esCasillaDentroDelRango(short fila,short columna);
    bool otorgarFicha(short fila,short columna);
    inline bool esMovimientoSimple(short filaO,short colO,short filaD,short colD);
    inline bool puedeContinuar(short fila,short col,Ficha* ficha);

public:
    Ficha*** fichas;
    Tablero(Tablero &&other);
    Tablero(short tamanio,Jugador* primerJugador,Jugador* segundoJugador);
    Tablero(Jugador* primerJugador,Jugador* segundoJugador);
    Tablero(short tamanio);
    Tablero();
    ~Tablero();
    bool esMovimientoConSalto(short filaO,short colO,short filaD,short colD);
    bool esMovimientoConSalto(Ficha *ficha,short filaO,short colO,short filaD,short colD);
    bool casillaVacia(short fila,short col);
    short getPiezasPorJugador();
    short getTamanio();
    Jugador* getPrimerJugador();
    void setPrimerJugador(Jugador* j);
    Jugador* getSegundoJugador();
    void setSegundoJugador(Jugador* j);
    bool agregarFicha(short fila,short columna,short color);
    inline Ficha* consultar(short fila,short columna){
        return esCasillaDentroDelRango(fila,columna)?fichas[fila][columna]:nullptr;
    }
    bool esMovimientoValido(short filaO,short colO,short filaD,short colD);
    ResultadoDeMovimiento moverFicha(short filaOrig,short columnaOrig,short filaDest,
                                     short columnaDest);
    Tablero& operator=(Tablero&& other);
};


#endif // TABLERO_H_INCLUDED




