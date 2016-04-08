#include "toolbutton.h"
#include "sender.h"

#include <QProcess>

ToolButton::ToolButton(QWidget *parent)
    : QToolButton(parent)
{
    Q_INIT_RESOURCE(resource);

    this->menu = new QMenu();
    this->encender = new QAction("Encender", this);
    connect(this->encender, SIGNAL(triggered(bool)), this, SLOT(turnOn()));

    this->reiniciar = new QAction("Reiniciar", this);
    connect(this->reiniciar, SIGNAL(triggered(bool)), this, SLOT(reboot()));

    this->apagar = new QAction("Apagar", this);
    connect(this->apagar, SIGNAL(triggered(bool)), this, SLOT(turnOff()));

    menu->addAction(this->encender);
    menu->addAction(this->reiniciar);
    menu->addAction(this->apagar);

    this->setMenu(menu);
    this->setPopupMode(QToolButton::InstantPopup);
}

ToolButton::~ToolButton()
{

}

void ToolButton::sendMessage(QString msg)
{
    this->socket->write(msg.toStdString().c_str());
    this->socket->waitForBytesWritten();
}

void ToolButton::setMac(QString mac)
{
    this->mac = mac;
}

QString ToolButton::getMac()
{
    return this->mac;
}

void ToolButton::receive_data()
{
    QByteArray Data = this->socket->readAll();
    qDebug() << Data;
}

void ToolButton::socket_disconnect()
{
    qDebug() << "Cliente desconectado";
    this->state = false;
    this->update();
}

void ToolButton::turnOn()
{
    qDebug() << "Encender " << this->mac;
    Sender sender;
    sender.wakeOnLan(this->mac);
}

void ToolButton::turnOff()
{
    qDebug() << "Apagar " << this->ip;
    QString cmd = "shutdown -m " + this->ip + " -s -t 1";
    QProcess::execute(cmd.toStdString().c_str());
}

void ToolButton::reboot()
{
    qDebug() << "Reiniciar " << this->ip;
    QString cmd = "shutdown -m " + this->ip + " -r -t 1";
    QProcess::execute(cmd.toStdString().c_str());
}

QString ToolButton::getIp()
{
    return this->ip;
}

void ToolButton::setIp(QString ip)
{
    this->ip = ip;
}

bool ToolButton::getState()
{
    return this->state;
}

void ToolButton::setState(bool state)
{
    this->state = state;
}

void ToolButton::update()
{
    this->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    this->setStyleSheet("background-color: rgba(0,0,0,0%)");
    this->setIconSize(QSize(60,60));
    if (this->state){
        this->encender->setEnabled(false);
        this->apagar->setEnabled(true);
        this->reiniciar->setEnabled(true);
        this->setIcon(QIcon(":/images/Iconos/XAMon.png"));
    } else {
        this->encender->setEnabled(true);
        this->apagar->setEnabled(false);
        this->reiniciar->setEnabled(false);
        this->setIcon(QIcon(":/images/Iconos/XAMoff.png"));
    }
}

void ToolButton::setSocket(QTcpSocket *socket)
{
    this->socket = socket;
    connect(this->socket, SIGNAL(readyRead()), this, SLOT(receive_data()));
    connect(this->socket, SIGNAL(disconnected()), this, SLOT(socket_disconnect()));
}
