#ifndef DIALOGADDLISTA_H
#define DIALOGADDLISTA_H

#include <QDialog>

namespace Ui {
class DialogAddLista;
}

class DialogAddLista : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddLista(QWidget *parent = 0);
    ~DialogAddLista();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogAddLista *ui;
};

#endif // DIALOGADDLISTA_H
