#include "dialogoopciones.h"
#include "ui_dialogoopciones.h"
#include <QPushButton>
DialogoOpciones::DialogoOpciones(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogoOpciones)
{
    ui->setupUi(this);
    okPressed=false;
    QPushButton *button=ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(button,&QPushButton::clicked,[this](){okPressed=true;});
}
void DialogoOpciones::showEvent(QShowEvent *)
{
    okPressed=false;
}

string DialogoOpciones::getNameFieldContent()
{
    return ui->fieldNombre->text().toStdString();
}
bool DialogoOpciones::wasOkPressed()
{
    return okPressed;
}

void DialogoOpciones::setNameFieldContent(string s)
{
    ui->fieldNombre->setText(QString::fromStdString(s));
}

string DialogoOpciones::getPortFieldContent()
{
    return ui->fieldPuerto->text().toStdString();
}

void DialogoOpciones::setPortFieldContent(string s)
{
    return ui->fieldPuerto->setText(QString::fromStdString(s));
}

DialogoOpciones::~DialogoOpciones()
{
    delete ui;
}
