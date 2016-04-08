#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QTcpSocket>
#include <QDebug>

QT_BEGIN_NAMESPACE
class QTcpSocket;
class QNetworkSession;
QT_END_NAMESPACE

namespace Ui {
class Client;
}

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0);
    ~Client();

private slots:
    void requestNew();
    void read();
    void displayError(QAbstractSocket::SocketError socketError);
    void sessionOpened();
    void reconnect();
    void reproducirContenido(QString);
    void openRoller1();
    void openRoller2();
    void openRoller3();

private:
    QTcpSocket *tcpsocket;
    quint16 blockSize;
    QNetworkSession *networkSession;
    Ui::Client *ui;
};

#endif // CLIENT_H
