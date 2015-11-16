#include "visual/mainwindow.h"
#include "modelo/Tablero.h"
#include <QApplication>
Tablero tablero;

int main(int argc, char *argv[])
{
    tablero.setPrimerJugador(new Jugador());
    tablero.setSegundoJugador(new Jugador());
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
