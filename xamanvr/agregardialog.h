#ifndef AGREGARDIALOG_H
#define AGREGARDIALOG_H

#include <QDialog>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

namespace Ui {
class AgregarDialog;
}

class AgregarDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AgregarDialog(QWidget *parent = 0);
    ~AgregarDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AgregarDialog *ui;
    QSqlDatabase db;
};

#endif // AGREGARDIALOG_H
