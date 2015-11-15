#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtConcurrent>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dialogoConecccion= new DialogoConeccion(this);
    progresoConnecion=new QProgressDialog(tr("Esperando connecciones"),tr("Cancelar"),0,0,this);
    progresoConnecion->setFixedSize(width()/2,progresoConnecion->height());
    controlador=new Controler(this,this->ui->visualBoard);
    connect(ui->action_nueva_partida,SIGNAL(triggered()),this,SLOT(iniciarPartida()));
    connect(progresoConnecion,&QProgressDialog::canceled,&MessagesSender::cancelarInicioPartida);
    connect(&futureWatcher,SIGNAL(finished()),progresoConnecion,SLOT(cancel()));
}
void MainWindow::unirseAPartida()
{
    dialogoConecccion->exec();
}

void MainWindow::iniciarPartida()
{
    QFuture<void> future=QtConcurrent::run(&MessagesSender::iniciarPartida);
    futureWatcher.setFuture(future);
    progresoConnecion->setModal(Qt::WindowModal);
    progresoConnecion->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
    progresoConnecion->exec();
    futureWatcher.waitForFinished();
    u_int8_t banderaeAEnviar;
    this->ui->visualBoard->waitAndProcessOponentMoves(banderaeAEnviar);
    MessagesSender::enviarRespuestaDeTurno(banderaeAEnviar,0);
}
MainWindow::~MainWindow()
{
    delete ui;
    delete dialogoConecccion;
}
