#include "sender.h"

Sender::Sender(QWidget *parent) : QWidget(parent)
{
    udpSocket = new QUdpSocket(this);
}

void Sender::sendDatagram(QString message)
{
    QByteArray datagram = message.toStdString().c_str();
    udpSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, 1234);

}

void Sender::sendDatagramtoIp(QString ip, QString object){
    QHostAddress host(ip);
    QByteArray datagram = object.toStdString().c_str();
    udpSocket->writeDatagram(datagram.data(), datagram.size(), host, 1234);
}

void Sender::wakeOnLan(QString MAC)
{
    char MACAddr [6];
    char MagicPacket [102];

    int j = sscanf (MAC.toLatin1().data(), "%2x:%2x:%2x:%2x:%2x:%2x",
                    & MACAddr [0], & MACAddr [1], & MACAddr [2], & MACAddr [3], & MACAddr [4], & MACAddr [5]);

    memset (MagicPacket, 0xff, 6);

    int packetsize = 6;

    for (int i = 0; i <16; i++)
    {
        memcpy (MagicPacket + packetsize, MACAddr, 6);
        packetsize += 6;
    }

    udpSocket->writeDatagram(MagicPacket, 102, QHostAddress::Broadcast, 1235);
}

void Sender::wakeOnLanAll()
{
    QString nombre;
    nombre.append("XAMAN.sqlite");
    db= QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(nombre);
    db.open();

    QString consulta;
    consulta.append("SELECT * FROM xaman");
    QSqlQuery consultar;
    consultar.prepare(consulta);
    if(consultar.exec()){
        qDebug ()<<"exito";

    }else{
        qDebug ()<<"Error tabla";
    }
    consultar.first();
    while(consultar.next()){

        char MACAddr [6];
        char MagicPacket [102];

        int j = sscanf (consultar.value(1).toString().toLatin1().data(), "%2x:%2x:%2x:%2x:%2x:%2x",
                        & MACAddr [0], & MACAddr [1], & MACAddr [2], & MACAddr [3], & MACAddr [4], & MACAddr [5]);
        memset (MagicPacket, 0xff, 6);

        int packetsize = 6;

        for (int i = 0; i <16; i++)
        {
            memcpy (MagicPacket + packetsize, MACAddr, 6);
            packetsize += 6;
        }
        udpSocket->writeDatagram(MagicPacket, 102, QHostAddress::Broadcast, 1235);
    }

}

