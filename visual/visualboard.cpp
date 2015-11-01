#include "visualboard.h"
#include <QFile>
#include <QTextStream>
#include <QString>
#include <iostream>
VisualBoard::VisualBoard(QWidget *parent, int inverted) : QWidget(parent),forcedMove(false)
{
    gridLayout=new QGridLayout(this);
    VisualBoardSquare *square;
    estadoActual=WAITING_SOURCE;
    for(int fila=0;fila<BOARD_SIZE;fila++)
        for(int columna=0;columna<BOARD_SIZE;columna++)
        {
            square=
                    squares[fila][columna]=new(std::nothrow) VisualBoardSquare(fila,columna);
            QString color;
            ((fila%2==0&&columna%2==0)||(fila%2!=0&&columna%2!=0))?
                        color="red":color="black";
            square->setProperty("color",color);
            connect(square,SIGNAL(clicked()),
                    SLOT(manejarMovimiento()));
            if(inverted)gridLayout->addWidget(square,
                                              BOARD_SIZE-fila-1,
                                              BOARD_SIZE-columna-1);
            else gridLayout->addWidget(square,fila,columna);
        }
    QFile stylesheet(":style/buttonStyle.css");
    stylesheet.open(QIODevice::ReadOnly);
    QTextStream ts(&stylesheet);
    setStyleSheet(ts.readAll());
}
void VisualBoard::manejarMovimiento()
{
    switch(estadoActual)
    {
    case WAITING_SOURCE:
        sourceSquare=qobject_cast<VisualBoardSquare *>(sender());
        if(!tablero.casillaVacia(sourceSquare->fila,sourceSquare->columna))
        {
            highLightSquares();
            estadoActual=WAITING_DESTINY;
        }
        break;
    case WAITING_DESTINY:
        VisualBoardSquare *destinySquare=qobject_cast<VisualBoardSquare *>(sender());
        if(forcedMove&&!tablero.
                esMovimientoConSalto(sourceSquare->fila,sourceSquare->columna,
                                     destinySquare->fila,destinySquare->columna))
        {

            estadoActual=WAITING_DESTINY;
        }
        else{
            ResultadoDeMovimiento res=tablero.moverFicha(
                        sourceSquare->fila,sourceSquare->columna,
                        destinySquare->fila,destinySquare->columna
                        );
            switch(res)
            {
            case ADQUIERE_DOBLE_MOVIMIENTO_LA_FICHA:
            case TURNO_FINALIZADO:
                estadoActual=WAITING_SOURCE;
                turnOffSquares();
                forcedMove=false;
                break;
            case MOVIMIENTO_INVALIDO:
            case POSICION_OCUPADA:
                if(!forcedMove)
                {
                    estadoActual=WAITING_SOURCE;
                    turnOffSquares();
                }
                break;
            case DOBLE_MOV_Y_SIGUE_JUGANDO:
            case SIGUE_MOVIENDO:
                turnOffSquares();
                sourceSquare=squares[destinySquare->fila][destinySquare->columna];
                forcedMove=true;
                highLightSquares();
                break;
            }

        }
        break;
    }
}

void VisualBoard::invertirTablero()
{
    array<array<QLayoutItem*,BOARD_SIZE>,BOARD_SIZE> oldArrange;
    QLayoutItem* aux;
    for(int fila=0;fila<BOARD_SIZE;fila++)
    {
        for(int columna=0;columna<BOARD_SIZE;columna++)
        {
            aux=gridLayout->itemAtPosition(fila,columna);
            gridLayout->removeItem(aux);
            oldArrange[fila][columna]=aux;
        }
    }
    for(int fila=0;fila<BOARD_SIZE;fila++)
    {
        for(int columna=0;columna<BOARD_SIZE;columna++)
        {
            gridLayout->addItem
                    (oldArrange[BOARD_SIZE-fila-1][BOARD_SIZE-columna-1],fila,columna);
        }
    }
    repaint();
}
#include <functional>
void VisualBoard::highLightSquares()
{
    if(forcedMove)
    {
        for(auto fila:squares)
            for(auto square:fila)
            {
                if(tablero.esMovimientoConSalto(sourceSquare->fila,sourceSquare->columna,
                                              square->fila,square->columna))
                {
                    square->setHighlight(true);
                }
            }
    }
    else{
        sourceSquare->setHighlight(true);
        for(auto fila:squares)
            for(auto square:fila)
            {
                if(tablero.esMovimientoValido(sourceSquare->fila,sourceSquare->columna,
                                              square->fila,square->columna))
                {
                    square->setHighlight(true);
                }
            }
    }

}
void VisualBoard::turnOffSquares()
{
    for(auto fila:squares)
        for(auto square:fila)
        {
            square->setHighlight(false);
        }
}
VisualBoard::~VisualBoard()
{
    for(auto fila:squares)
        for(auto square:fila)
        {
            delete square;
        }
}

