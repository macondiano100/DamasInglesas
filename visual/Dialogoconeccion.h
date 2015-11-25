#ifndef DIALOGOCONECCION_H
#define DIALOGOCONECCION_H
#include <iostream>
using namespace std;
#include <QDialog>

namespace Ui {
class DialogoConeccion;
}

class DialogoConeccion : public QDialog
{
    Q_OBJECT

public:
    explicit DialogoConeccion(QWidget *parent = 0);
    void setNombreUsuario(std::string nombre);
    ~DialogoConeccion();
public slots:
void unirse_a_partida();
private:
    string nombreUsuario;
    Ui::DialogoConeccion *ui;
};

#endif // DIALOGOCONECCION_H
