#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QtNetwork>

class TcpSocket : public QObject
{
    Q_OBJECT
public:
    TcpSocket(QTcpSocket*);
    QTcpSocket* socketXaman;
    virtual ~TcpSocket();

public slots:
    void slotReadyRead();
    void slotDisconnected();

signals:
    void dataReady(QTcpSocket *socket, TcpSocket *sckt);
    void lostConnection(QTcpSocket *socket);
};

#endif // TCPSOCKET_H
