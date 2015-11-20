#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressDialog>
#include <QFuture>
#include <QFutureWatcher>
#include "Dialogoconeccion.h"
class Controler;
#include "Controlador/controler.h"
#include "modelo/connectionmanager.h"
namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
void unirseAPartida();
void iniciarPartida();
public:
    explicit MainWindow(QWidget *parent = 0);
    void actualizarNumeroFichas();
    void inicializarNumeroFichas();
    void closeEvent(QCloseEvent *)override;
    ~MainWindow();

private:
    DialogoConeccion *dialogoConecccion;
    QProgressDialog *progresoConnecion;
    Ui::MainWindow *ui;
    QFutureWatcher<void> futureWatcher;
    Controler* controlador;

};

#endif // MAINWINDOW_H
