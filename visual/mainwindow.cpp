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
    ui->visualBoard->connectControler(controlador);
    connect(ui->action_nueva_partida,SIGNAL(triggered()),this,SLOT(iniciarPartida()));
    connect(progresoConnecion,&QProgressDialog::canceled,&MessagesSender::cancelarInicioPartida);
    connect(&futureWatcher,SIGNAL(finished()),progresoConnecion,SLOT(cancel()));
}
void MainWindow::unirseAPartida()
{
    dialogoConecccion->exec();
    inicializarNumeroFichas();
    controlador->iniciaJuegoComoOponente();
    repaint();
}

void MainWindow::iniciarPartida()
{
    QFuture<void> future=QtConcurrent::run(&MessagesSender::iniciarPartida);
    futureWatcher.setFuture(future);
    progresoConnecion->setModal(Qt::WindowModal);
    progresoConnecion->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
    progresoConnecion->exec();
    futureWatcher.waitForFinished();
    inicializarNumeroFichas();
    controlador->iniciaJuegoComoHost();
    repaint();
}

void MainWindow::actualizarNumeroFichas()
{
    ui->labelNumeroFichasRojas->setText(QString::number(tablero.getPrimerJugador()->getFichasActuales()));
    ui->labelNumeroFichasBlancas->setText(QString::number(tablero.getSegundoJugador()->getFichasActuales()));
}

void MainWindow::inicializarNumeroFichas()
{
    ui->groupBOxJugadorRojas->setTitle(QString(tablero.getPrimerJugador()->getNombre().c_str()));
    ui->groupBoxJugadorBlancas->setTitle(QString(tablero.getSegundoJugador()->getNombre().c_str()));
    ui->labelNumeroFichasRojas->setText(QString::number(tablero.getPrimerJugador()->getFichasActuales()));
    ui->labelNumeroFichasBlancas->setText(QString::number(tablero.getSegundoJugador()->getFichasActuales()));
}
MainWindow::~MainWindow()
{
    delete ui;
    delete dialogoConecccion;
}
