#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dialogoConecccion= new DialogoConeccion(this);
}
void MainWindow::unirseAPartida()
{
    dialogoConecccion->exec();
}
MainWindow::~MainWindow()
{
    delete ui;
}
