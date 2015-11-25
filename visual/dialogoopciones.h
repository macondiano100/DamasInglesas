#ifndef DIALOGOOPCIONES_H
#define DIALOGOOPCIONES_H

#include <QDialog>
#include <iostream>
using namespace std;
namespace Ui {
class DialogoOpciones;
}

class DialogoOpciones : public QDialog
{
    Q_OBJECT

public:
    explicit DialogoOpciones(QWidget *parent = 0);
    string getNameFieldContent();
    void setNameFieldContent(string);
    string getPortFieldContent();
    void setPortFieldContent(string);
    ~DialogoOpciones();
    bool wasOkPressed();
private:
    bool okPressed;
    Ui::DialogoOpciones *ui;
    void showEvent(QShowEvent *);
};

#endif // DIALOGOOPCIONES_H
