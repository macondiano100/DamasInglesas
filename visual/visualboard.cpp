#include "visualboard.h"
#include "modelo/connection/utilidadesVariadas.h"
#include "modelo/connectionmanager.h"
#include "visual/mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QtConcurrent>
#include <QString>
#include <iostream>
constexpr int NTURNO=0;
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
            if(inverted)gridLayout->addWidget(square,
                                              BOARD_SIZE-fila-1,
                                              BOARD_SIZE-columna-1);
            else gridLayout->addWidget(square,fila,columna);
        }
    progressDialogWaitingOponent=new QProgressDialog(
                tr("Esperando Movimiento del oponente"),"",0,0,this
                );
    progressDialogWaitingOponent->setCancelButton(nullptr);
    progressDialogWaitingOponent->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
    QFile stylesheet(":style/buttonStyle.css");
    stylesheet.open(QIODevice::ReadOnly);
    QTextStream ts(&stylesheet);
    setStyleSheet(ts.readAll());
}
void VisualBoard::connectControler(Controler *controler)
{
    connect(&(controler->futureWatcherEspearaOponente),SIGNAL(finished()),
            progressDialogWaitingOponent,SLOT(cancel()));
    for(auto fila:squares)
        for(auto square:fila)
            connect(square,SIGNAL(clicked()),controler,
                SLOT(casillaClicked()));
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

void VisualBoard::highLightSquares(bool forcedMove,VisualBoardSquare* sourceSquare)
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
    bool fichaComida;
    for(Movimiento mov:movimientos)
    {
        fichaComida=tablero.esMovimientoConSalto(mov.filaOrigen,mov.colOrigen,mov.filaDestino,mov.colDestino);
        if(res!=SIGUE_MOVIENDO&&res!=DOBLE_MOV_Y_SIGUE_JUGANDO) break;
        if(!tablero.esMovimientoValido(mov.filaOrigen,mov.colOrigen,mov.filaDestino,mov.colDestino))
        {
            break;
        }
        res=tablero.moverFicha(mov.filaOrigen,mov.colOrigen,mov.filaDestino,mov.colDestino);
        if(fichaComida!=mov.fichaComida)
        {
            break;
        }
        cout<<"Lammando repaint"<<endl;
        repaint();
        esperar(300);
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

VisualBoard::~VisualBoard()
{
    for(auto fila:squares)
        for(auto square:fila)
        {
            delete square;
        }
    delete progressDialogWaitingOponent;
}

