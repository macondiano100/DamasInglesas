#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "modelo/connectionmanager.h"
#include <QtConcurrent>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dialogoConecccion= new DialogoConeccion(this);
    progresoConnecion=new QProgressDialog(tr("Esperando connecciones"),tr("Cancelar"),0,0,this);
    progresoConnecion->setFixedSize(width()/2,progresoConnecion->height());
    connect(ui->action_nueva_partida,SIGNAL(triggered()),this,SLOT(iniciarPartida()));
    connect(progresoConnecion,&QProgressDialog::canceled,&connection::cancelarInicioPartida);
}
void MainWindow::unirseAPartida()
{
    dialogoConecccion->exec();
}

void MainWindow::iniciarPartida()
{
    QFuture<void> future=QtConcurrent::run(connection::iniciarPartida);
    futureWatcher.setFuture(future);
    progresoConnecion->setModal(Qt::WindowModal);
    progresoConnecion->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
    progresoConnecion->exec();
    futureWatcher.waitForFinished();
}
MainWindow::~MainWindow()
{
    delete ui;
    delete dialogoConecccion;
}
