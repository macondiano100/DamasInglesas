#include "visualboard.h"
#include "modelo/connection/utilidadesVariadas.h"
#include "modelo/connectionmanager.h"
#include "visual/mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QtConcurrent>
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
    progressDialogWaitingOponent=new QProgressDialog(tr("Esperando oponente"),"",0,0,this);
    progressDialogWaitingOponent->setCancelButton(nullptr);
    progressDialogWaitingOponent->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
    connect(&futureWatcher,SIGNAL(finished()),progressDialogWaitingOponent,SLOT(cancel()));
    connect(&futureWatcher,&QFutureWatcher<void>::finished,progressDialogWaitingOponent,[](){cout<<"lambda"<<endl;});
    QFile stylesheet(":style/buttonStyle.css");
    stylesheet.open(QIODevice::ReadOnly);
    QTextStream ts(&stylesheet);
    setStyleSheet(ts.readAll());
}
void VisualBoard::manejarMovimiento()
{
    bool validMovement;
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
    {
        VisualBoardSquare *destinySquare=qobject_cast<VisualBoardSquare *>(sender());
        VisualBoardSquare *aux;
        if(forcedMove&&!tablero.
                esMovimientoConSalto(sourceSquare->fila,sourceSquare->columna,
                                     destinySquare->fila,destinySquare->columna))
        {

            estadoActual=WAITING_DESTINY;
        }
        else{
            aux=sourceSquare;
            ResultadoDeMovimiento res=tablero.moverFicha(
                        sourceSquare->fila,sourceSquare->columna,
                        destinySquare->fila,destinySquare->columna
                        );
            switch(res)
            {
            case MOVIMIENTO_INVALIDO:
            case POSICION_OCUPADA:
                if(!forcedMove)
                {
                    estadoActual=WAITING_SOURCE;
                    turnOffSquares();
                    validMovement=false;
                }
                break;
            case ADQUIERE_DOBLE_MOVIMIENTO_LA_FICHA:
            case TURNO_FINALIZADO:
                turnOffSquares();
                forcedMove=false;
                validMovement=true;
                estadoActual=WAITING_OPONENT;
                break;
            case DOBLE_MOV_Y_SIGUE_JUGANDO:
            case SIGUE_MOVIENDO:
                turnOffSquares();
                sourceSquare=destinySquare;
                forcedMove=true;
                highLightSquares();
                validMovement=true;
                break;
            }
            if(validMovement)
            {
                bool sigueMoviendo=(res==DOBLE_MOV_Y_SIGUE_JUGANDO||res==SIGUE_MOVIENDO);
                lastMovements.
                        emplace_back(aux->fila,aux->columna,
                                     destinySquare->fila,destinySquare->columna,
                                     sigueMoviendo);
                if(!sigueMoviendo)
                {
                    MessagesSender::enviarInformacionDeTurno(0,lastMovements,false);
                    lastMovements.clear();
                    waitAndProcessOponentMoves();
                }
            }

        }
    }
    case WAITING_OPONENT:break;
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
ResultadoDeMovimiento VisualBoard::doMovements(vector<Movimiento> movimientos)
{
    ResultadoDeMovimiento res=SIGUE_MOVIENDO;
    for(Movimiento mov:movimientos)
    {
        if(res!=SIGUE_MOVIENDO&&res!=DOBLE_MOV_Y_SIGUE_JUGANDO) break;
        if(!tablero.esMovimientoValido(mov.filaOrigen,mov.colOrigen,mov.filaDestino,mov.colDestino))
            break;
        res=tablero.moverFicha(mov.filaOrigen,mov.colOrigen,mov.filaDestino,mov.colDestino);
        if((res==SIGUE_MOVIENDO||res==DOBLE_MOV_Y_SIGUE_JUGANDO)!=mov.fichaComida)break;
        repaint();
        esperar(700);
    }
    return res;
}

void VisualBoard::turnOffSquares()
{
    for(auto fila:squares)
        for(auto square:fila)
        {
            square->setHighlight(false);
        }
}
void VisualBoard::waitAndProcessOponentMoves()
{
    uint32_t nTurnoRecibido;
    uint8_t banderasRecibidas;
    ResultadoDeMovimiento resultadoOponente;
    auto llamada=bind
            (&MessagesSender::esperarTurnoOponente,
             ref(nTurnoRecibido),
             ref(lastMovements),
             ref(banderasRecibidas));
    QFuture<void> future=QtConcurrent::run(&llamada);
    futureWatcher.setFuture(future);
    progressDialogWaitingOponent->exec();
    futureWatcher.waitForFinished();
    ResultadoDeMovimiento res=doMovements(lastMovements);
    lastMovements.clear();
    estadoActual=WAITING_SOURCE;
}

VisualBoard::~VisualBoard()
{
    for(auto fila:squares)
        for(auto square:fila)
        {
            delete square;
        }
    delete progressDialogWaitingOponent;
}

