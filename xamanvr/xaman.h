#ifndef XAMAN_H
#define XAMAN_H

#include "comunicacion.h"
#include "dialogaddlista.h"
#include "logindialog.h"
#include "sender.h"
#include "toolbutton.h"
#include "tcpsocket.h"

#include <QDebug>
#include <QDir>
#include <QIcon>
#include <QLayoutItem>
#include <QList>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>
#include <QSignalMapper>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidgetAction>
#include <QWindow>
#include <QtXml>

namespace Ui {
class Xaman;
}

class Xaman : public QMainWindow
{
    Q_OBJECT
public:
    explicit Xaman(QWidget *parent = 0);
    ~Xaman();
    void actualizarList();
    void addRoot (QDomElement element, QString attribute);
    void addChild(QTreeWidgetItem *parent, QString name);
    void contenido();
    void creartabla();
    void init();
    void initXamanButton ();
    void initPlayList();
    void listElement(QDomElement root, QString tagname,QString attribute);
    void openDB();
    void setContenido(QTreeWidget*);
    int c=0;

private slots:
    void addToList(QString);
    void addXaman();
    void listToPlaylist(int);
    void menu(const QPoint&);
    void menuMusica(const QPoint&);
    void menuImagenes(const QPoint&);
    void menuList(const QPoint&);
    void on_btn_reiniciar_todo_clicked();
    void reproducirContenido(QString);
    void setValue(int);
    //void toIp(QString);
    void toAll(QString);
    void EliminarPlaylist(int);
    void on_btn_addPlaylist_clicked();
    //Seccion del server
    void server_Error();
    void server_Connected();
    void receive_client(QTcpSocket *socket, TcpSocket *tcpsocket);
    void remotePlay();
    void remoteStop();
    void remotePause();
    void remoteNext();
    void remotePrevious();
    void playlistPositionChanged(int currentItem);
    void test(int);

private:
    Ui::Xaman *ui;
    QSqlDatabase db;
    Sender sender;
    QList<ToolButton*> buttons;
    //Seccion del server
    QTcpServer *server;
};

#endif // XAMAN_H
