#ifndef SENDER_H
#define SENDER_H

#include <QWidget>
#include <QtNetwork>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

class QUdpSocket;

class Sender : public QWidget
{
    Q_OBJECT
public:
    explicit Sender(QWidget *parent = 0);

signals:

public slots:
    void sendDatagram(QString message);
    void sendDatagramtoIp(QString ip, QString message);
    void wakeOnLan(QString);
    void wakeOnLanAll();

private:
    QUdpSocket *udpSocket;
    QSqlDatabase db;
};

#endif // SENDER_H
