#include "tcpsocket.h"

TcpSocket::TcpSocket(QTcpSocket *socket)
{
    this->socketXaman = socket;

    connect(this->socketXaman, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
}

TcpSocket::~TcpSocket()
{

}

void TcpSocket::slotReadyRead()
{
    emit dataReady(this->socketXaman, this);
}

void TcpSocket::slotDisconnected(){

}
