#include "controler.h"
#include <QObject>
#include <QtConcurrent>
#include <QMessageBox>
enum Estado
{
    WAITING_SOURCE,
    WAITING_DESTINY,
    WAITING_OPONENT
};
Controler::Controler(MainWindow *mainWindow, VisualBoard *visualBoard):mainWindow(mainWindow),
    visualBoard(visualBoard)
{
    estadoActual=WAITING_OPONENT;
    forcedMove=false;
}


void Controler::casillaClicked()
{
    bool validMovement;
    switch(estadoActual)
    {
    case WAITING_SOURCE:
        sourceSquare=qobject_cast<VisualBoardSquare *>(sender());
        if(!tablero.casillaVacia(sourceSquare->fila,sourceSquare->columna)&&
                                 tablero.consultar(sourceSquare->fila,sourceSquare->columna)->getColor()==
                                 getJugadorUsuario()->getColor())
        {
            visualBoard->highLightSquares(forcedMove,sourceSquare);
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
            case NO_HAY_FICHA_QUE_MOVER:
            case MOVIMIENTO_INVALIDO:
            case POSICION_OCUPADA:
                if(!forcedMove)
                {
                    estadoActual=WAITING_SOURCE;
                    visualBoard->turnOffSquares();
                    validMovement=false;
                }
                break;
            case SE_HA_GANADO_LA_PARTIDA:
            case ADQUIERE_DOBLE_MOVIMIENTO_LA_FICHA:
            case TURNO_FINALIZADO:
                visualBoard->turnOffSquares();
                forcedMove=false;
                validMovement=true;
                estadoActual=WAITING_OPONENT;
                cout<<"After double"<<endl;
                break;
            case DOBLE_MOV_Y_SIGUE_JUGANDO:
            case SIGUE_MOVIENDO:
                visualBoard->turnOffSquares();
                sourceSquare=destinySquare;
                forcedMove=true;
                visualBoard->highLightSquares(forcedMove,sourceSquare);
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
                visualBoard->repaint();
                if(!sigueMoviendo)
                {
                    bool ok,partidaGanada=res==SE_HA_GANADO_LA_PARTIDA;
                    mainWindow->actualizarNumeroFichas();
                    MessagesSender::pararRespuestasKeepAlive();
                    MessagesSender::enviarInformacionDeTurno(NTURNO,lastMovements,partidaGanada);
                    lastMovements.clear();
                    ok=waitAndProcessAnswer(partidaGanada);
                    NTURNO++;
                    if(!ok)
                    {
                        cierreForsozo();
                        return;
                    }
                    else if(partidaGanada)
                    {
                        seGanoLaPartida(getJugadorUsuario());
                        return;
                    }
                    u_int8_t banderasAenviar;
                    ok=waitAndProcessOponentMoves(banderasAenviar);
                    mainWindow->actualizarNumeroFichas();
                    MessagesSender::enviarRespuestaDeTurno(banderasAenviar,NTURNO);
                    if(!ok)
                    {
                        cierreForsozo();
                        return;
                    }
                    NTURNO++;
                    partidaGanada=(banderasAenviar&PARTIDA_GANADA);
                    if(partidaGanada)
                    {
                        seGanoLaPartida(getJugadorOponente());
                        return;
                    }
                    QtConcurrent::run(MessagesSender::responderKeepAlives);
                    estadoActual=WAITING_SOURCE;
               }

            }
        }
    }
    case WAITING_OPONENT:break;

    }
}
bool Controler::waitAndProcessAnswer(bool partidaGanada)
{
    u_int8_t banderasRecibidas=0;
    u_int32_t nTurnoRecibido=-1;
    MessagesSender::esperaRespuestaDeTurno(banderasRecibidas,nTurnoRecibido);
    if(nTurnoRecibido!=NTURNO||(banderasRecibidas&CIERRE_FORZOSO))
    {
        cout<<"Error de envio de mensaje"<<endl;
        return false;
    }
    if(partidaGanada&&(banderasRecibidas&PARTIDA_GANADA))
    {
        return true;
    }
    else if(partidaGanada==(banderasRecibidas&PARTIDA_GANADA))
    {
        return true;
    }
    else
    {
        cout<<"Error de comunicacion"<<endl;
        return false;
    }

}
bool Controler::waitAndProcessOponentMoves(u_int8_t &banderasRespuesta)
{
    uint32_t nTurnoRecibido;
    u_int8_t banderasRecibidas;
    auto llamada=bind
            (&MessagesSender::esperarTurnoOponente,
             ref(nTurnoRecibido),
             ref(lastMovements),
             ref(banderasRecibidas));
    QFuture<void> future=QtConcurrent::run(&llamada);
    futureWatcherEspearaOponente.setFuture(future);
    visualBoard->progressDialogWaitingOponent->exec();
    futureWatcherEspearaOponente.waitForFinished();
    if(banderasRecibidas&CIERRE_FORZOSO)
    {
        return false;
    }
    if(nTurnoRecibido!=NTURNO)
    {
      banderasRespuesta=banderasRespuesta|CIERRE_FORZOSO;
      return false;
    }
    ResultadoDeMovimiento resultadoOponente=visualBoard->doMovements(lastMovements);
    banderasRespuesta=RESPUESTA_DE_TURNO;

    switch (resultadoOponente) {
        case TURNO_FINALIZADO:
            if(banderasRecibidas&PARTIDA_GANADA) banderasRespuesta
                    =banderasRespuesta|CIERRE_FORZOSO;
            break;
        case ADQUIERE_DOBLE_MOVIMIENTO_LA_FICHA:
        break;
        case MOVIMIENTO_INVALIDO:
        case SIGUE_MOVIENDO:
        case POSICION_OCUPADA:
        case NO_HAY_FICHA_QUE_MOVER:
        case DOBLE_MOV_Y_SIGUE_JUGANDO:
            banderasRespuesta=banderasRespuesta|CIERRE_FORZOSO;
        break;
        case SE_HA_GANADO_LA_PARTIDA:
             if(banderasRecibidas&PARTIDA_GANADA)
             {
                banderasRespuesta=banderasRecibidas|PARTIDA_GANADA;
             }
             else
                banderasRespuesta=banderasRespuesta|CIERRE_FORZOSO;
        break;

    }
    lastMovements.clear();
    if(banderasRespuesta&CIERRE_FORZOSO)
    {
       return false;
    }
    return true;
}

void Controler::iniciaJuegoComoHost()
{
    getJugadorUsuario=[](){return tablero.getPrimerJugador();};
    getJugadorOponente=[](){return tablero.getSegundoJugador();};
    estadoActual=WAITING_SOURCE;
    QtConcurrent::run(MessagesSender::responderKeepAlives);
}

void Controler::iniciaJuegoComoOponente()
{
    estadoActual=WAITING_OPONENT;
    getJugadorUsuario=[](){return tablero.getSegundoJugador();};
    getJugadorOponente=[](){return tablero.getPrimerJugador();};
    u_int8_t banderaeAEnviar;
    bool ok=waitAndProcessOponentMoves(banderaeAEnviar);
    MessagesSender::enviarRespuestaDeTurno(banderaeAEnviar,NTURNO);
    if(!ok)
    {
        cierreForsozo();
        return;
    }
    NTURNO++;
    estadoActual=WAITING_SOURCE;
    QtConcurrent::run(MessagesSender::responderKeepAlives);
}
void Controler::cierreForsozo()
{
    MessagesSender::closeConnection();
    QMessageBox messageBox(mainWindow);
    messageBox.setText(tr("Ha habido un error de comunicación."));
    messageBox.exec();
    estadoActual=WAITING_OPONENT;
    NTURNO=0;
}
void Controler::seGanoLaPartida(Jugador* ganador)
{
    QMessageBox messageBox(mainWindow);
    QString mensaje;
    MessagesSender::closeConnection();
    mensaje=(ganador==getJugadorUsuario()?tr("Haz Ganado la partida."):
                                          ("El jugador "+ganador->getNombre()+" ha ganado la partida.").c_str());
    messageBox.setText(mensaje);
    messageBox.exec();
}
