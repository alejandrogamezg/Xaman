#ifndef DIALOGADMIN_H
#define DIALOGADMIN_H

#include <QDialog>

#include <QDebug>
#include <QDir>
#include <QIcon>
#include <QLayoutItem>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>
#include <QSignalMapper>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QTreeWidget>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidgetAction>
#include <QWindow>
#include <QtXml>

namespace Ui {
class DialogAdmin;
}

class DialogAdmin : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAdmin(QWidget *parent = 0);
    ~DialogAdmin();
    void initXaman();
    void openDB();
    void crearTabla();

private slots:
    void on_btnadd_clicked();

private:
    Ui::DialogAdmin *ui;
    QSqlDatabase db;
};

#endif // DIALOGADMIN_H
