#ifndef DIALOGOCONECCION_H
#define DIALOGOCONECCION_H

#include <QDialog>

namespace Ui {
class DialogoConeccion;
}

class DialogoConeccion : public QDialog
{
    Q_OBJECT

public:
    explicit DialogoConeccion(QWidget *parent = 0);
    ~DialogoConeccion();

private:
    Ui::DialogoConeccion *ui;
};

#endif // DIALOGOCONECCION_H
