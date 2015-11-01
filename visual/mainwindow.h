#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Dialogoconeccion.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
void unirseAPartida();
public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private:
    DialogoConeccion *dialogoConecccion;
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
