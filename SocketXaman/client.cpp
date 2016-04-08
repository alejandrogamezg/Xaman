#include "client.h"
#include "ui_client.h"
#include "windows.h"
#include <QtNetwork>


//INICIALIZACION DEL SOCKET EN EL CONSTRUCTOR DEL CLIENTE
//PASAR EL WITGET PRINCIPAL COMO PADRE / EVITAMOS ELIMINAR SOCKET A CADA RATO
Client::Client(QWidget *parent) :
    QDialog(parent), networkSession(0),
    ui(new Ui::Client)
{
    ui->setupUi(this);

    tcpsocket = new QTcpSocket(this);
    tcpsocket->setSocketOption(QAbstractSocket::KeepAliveOption, QVariant(4));
    //SEÑALES DE QTCPSOCKET 
    connect(tcpsocket, SIGNAL(readyRead()), this, SLOT(read()));//REPRESENTA CUANDO LOS DATOS HAN SIDO RESIVIDOS
    connect(tcpsocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));//CAPTURAR CUALQUIER ERROR DE CONEXION
    connect(tcpsocket, SIGNAL(disconnected()), this, SLOT(reconnect()));

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired){
        QSettings settings(QSettings::UserScope, QLatin1String("Cliente Xaman"));
        settings.beginGroup(QLatin1String("Xaman"));
        const QString id = settings.value(QLatin1String("DefaultNwtworkConfiguration")).toString();
        settings.endGroup();

        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
                config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));

        networkSession->open();
    }

    this->requestNew();
}

Client::~Client()
{
    delete ui;
}

void Client::requestNew()
{
    blockSize = 0;
    tcpsocket->abort();

    tcpsocket->connectToHost("192.168.0.1", 6000);
    //tcpsocket->connectToHost(QHostAddress::LocalHost, 6000);

    if (tcpsocket->waitForConnected()){
        QString ip;
        QList<QHostAddress> ipList = QNetworkInterface::allAddresses();
        for (int i=0; i<ipList.size(); i++){
            if (ipList.at(i) != QHostAddress::LocalHost &&
                    ipList.at(i).toIPv4Address()) {
                ip = ipList.at(i).toString();
                break;
            }
        }
        tcpsocket->write(ip.toStdString().c_str());
        //tcpsocket->write(tr("Hola servidor").toStdString().c_str());
    }
}

void Client::read()
{
    QByteArray Data = tcpsocket->readAll();
    qDebug() << Data;

    QString result(Data.data());

    QStringList list = result.split(",");

    qDebug() << "recibido";

    if (list.at(0) == "Reproducir") {
        QStringList app = list.at(1).split("/");
        if(app.at(1)=="RollerCoaster_1.mp4"){
            openRoller1();
        }
        else if(app.at(1)=="RollerCoaster_2.mp4"){
            openRoller2();
        }
        else if(app.at(1)=="RollerCoaster_3.mp4"){
            openRoller3();
        }
        else{
            reproducirContenido(list.at(1));
        }
    } else if(list.at(0)=="Detener"){
        //´presionar la letra q para salir del titan
                WORD vkCOde=0x51;
                INPUT KeyEvent={0};
                KeyEvent.type=INPUT_KEYBOARD;
                KeyEvent.ki.wScan=MapVirtualKey(vkCOde,MAPVK_VK_TO_VSC);
                //SendInput(1,&KeyEvent,sizeof(INPUT));

                KeyEvent.ki.time=0;
                KeyEvent.ki.dwExtraInfo=0;
                KeyEvent.ki.wVk=vkCOde;
                KeyEvent.ki.dwFlags=0;
                SendInput(1,&KeyEvent,sizeof(INPUT));

        QTime dieTimer1= QTime::currentTime().addSecs(2);
        while (QTime::currentTime() < dieTimer1){
               QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }

        QString commando = "taskKill /IM MaxVR.exe";
        system(commando.toStdString().c_str());

        QTime dieTimer= QTime::currentTime().addSecs(1);
        while (QTime::currentTime() < dieTimer){
               QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }

        QString cmd2 = "taskKill /IM nolimits2app.exe";
        system(cmd2.toStdString().c_str());
    }
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError){
    case QAbstractSocket::RemoteHostClosedError:
        //this->reconnect();
        QTimer::singleShot(3000, this, SLOT(reconnect()));
        qDebug() << "remote host closed";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QTimer::singleShot(3000, this, SLOT(reconnect()));
        //this->reconnect();
        qDebug() << "Connection refused";
        break;
    case QAbstractSocket::HostNotFoundError:
        //this->reconnect();
        QTimer::singleShot(3000, this, SLOT(reconnect()));
        qDebug() << "Host not found";
        break;
    default:
        qDebug() << "error";
    }
}

void Client::sessionOpened()
{
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    if (config.type() == QNetworkConfiguration::UserChoice)
        id = networkSession->sessionProperty(QLatin1String("USerChoiceConfiguration")).toString();
    else
        id = config.identifier();

    QSettings settings(QSettings::UserScope, QLatin1String("Xamanclient"));
    settings.beginGroup(QLatin1String("Xaman"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();
}

void Client::reconnect()
{
    /*
    if (tcpsocket->state() != QAbstractSocket::UnconnectedState){
        tcpsocket->disconnectFromHost();
        tcpsocket->waitForDisconnected();
    }
    */
    qDebug() << tcpsocket->state();
    //tcpsocket->connectToHost(QHostAddress::LocalHost, 6000);
    tcpsocket->connectToHost("192.168.0.1", 6000);
    tcpsocket->waitForConnected();
}

void Client::openRoller1()
{
    QString app="C:/Program Files/NoLimits 2/64bit/nolimits2app.exe";
    QProcess *process=new QProcess();
    process->setProgram(app);
    process->start();

    QTime dieTimer= QTime::currentTime().addSecs(10);
    while (QTime::currentTime() < dieTimer){
           QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    WORD vkCOde=0x0D;
    INPUT KeyEvent={0};
    KeyEvent.type=INPUT_KEYBOARD;
    KeyEvent.ki.wScan=MapVirtualKey(vkCOde,MAPVK_VK_TO_VSC);
    //SendInput(1,&KeyEvent,sizeof(INPUT));

    KeyEvent.ki.time=0;
    KeyEvent.ki.dwExtraInfo=0;
    KeyEvent.ki.wVk=vkCOde;
    KeyEvent.ki.dwFlags=0;
    SendInput(1,&KeyEvent,sizeof(INPUT));

    WORD vkCodeentertup =0x0D;
    INPUT altup={0};
    altup.type = INPUT_KEYBOARD;
    altup.ki.time = 0;
    altup.ki.dwExtraInfo = 0;
    altup.ki.wVk=vkCodeentertup;
    altup.ki.dwFlags =(KEYEVENTF_KEYUP);
    altup.ki.wScan = MapVirtualKey(vkCodeentertup, MAPVK_VK_TO_VSC);

    SendInput(1, &altup, sizeof(INPUT)); // enter up

    QTime dieTimer2= QTime::currentTime().addMSecs(500);
    while (QTime::currentTime() < dieTimer2){
           QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    SendInput(1,&KeyEvent,sizeof(INPUT));
    SendInput(1, &altup, sizeof(INPUT));

    QTime dieTimer3= QTime::currentTime().addMSecs(500);
    while (QTime::currentTime() < dieTimer3){
           QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    SendInput(1,&KeyEvent,sizeof(INPUT));
    SendInput(1, &altup, sizeof(INPUT));

    QTime dieTime= QTime::currentTime().addSecs(2);
    while (QTime::currentTime() < dieTime){
           QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    INPUT input;
    input.type=INPUT_MOUSE;
    input.mi.dx=25576;
    input.mi.dy=42960;
    input.mi.dwFlags=(MOUSE_EVENT|MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_VIRTUALDESK|MOUSEEVENTF_MOVE| MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP);
    input.mi.mouseData=0;
    input.mi.dwExtraInfo=NULL;
    input.mi.time=0;
    SendInput(1,&input,sizeof(INPUT));

    INPUT inputplay;
    inputplay.type=INPUT_MOUSE;
    inputplay.mi.dx=32678;
    inputplay.mi.dy=25576;
    inputplay.mi.dwFlags=(MOUSE_EVENT|MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_VIRTUALDESK|MOUSEEVENTF_MOVE| MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP);
    inputplay.mi.mouseData=0;
    inputplay.mi.dwExtraInfo=NULL;
    inputplay.mi.time=0;
    SendInput(1,&inputplay,sizeof(INPUT));
    SendInput(1,&inputplay,sizeof(INPUT));
}
void Client::openRoller2()
{
    QString app="C:/Program Files/NoLimits 2/64bit/nolimits2app.exe";
    QProcess *process=new QProcess();
    process->setProgram(app);
    process->start();

    QTime dieTimer= QTime::currentTime().addSecs(10);
    while (QTime::currentTime() < dieTimer){
           QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    WORD vkCOde=0x0D;
    INPUT KeyEvent={0};
    KeyEvent.type=INPUT_KEYBOARD;
    KeyEvent.ki.wScan=MapVirtualKey(vkCOde,MAPVK_VK_TO_VSC);
    //SendInput(1,&KeyEvent,sizeof(INPUT));

    KeyEvent.ki.time=0;
    KeyEvent.ki.dwExtraInfo=0;
    KeyEvent.ki.wVk=vkCOde;
    KeyEvent.ki.dwFlags=0;
    SendInput(1,&KeyEvent,sizeof(INPUT));

    WORD vkCodeentertup =0x0D;
    INPUT altup={0};
    altup.type = INPUT_KEYBOARD;
    altup.ki.time = 0;
    altup.ki.dwExtraInfo = 0;
    altup.ki.wVk=vkCodeentertup;
    altup.ki.dwFlags =(KEYEVENTF_KEYUP);
    altup.ki.wScan = MapVirtualKey(vkCodeentertup, MAPVK_VK_TO_VSC);

    SendInput(1, &altup, sizeof(INPUT)); // enter up

    QTime dieTimer2= QTime::currentTime().addMSecs(500);
    while (QTime::currentTime() < dieTimer2){
           QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    SendInput(1,&KeyEvent,sizeof(INPUT));
    SendInput(1, &altup, sizeof(INPUT));

    QTime dieTimer3= QTime::currentTime().addMSecs(500);
    while (QTime::currentTime() < dieTimer3){
           QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    SendInput(1,&KeyEvent,sizeof(INPUT));
    SendInput(1, &altup, sizeof(INPUT));

    QTime dieTime= QTime::currentTime().addSecs(2);
    while (QTime::currentTime() < dieTime){
           QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    INPUT input;
    input.type=INPUT_MOUSE;
    input.mi.dx=25576;
    input.mi.dy=42960;
    input.mi.dwFlags=(MOUSE_EVENT|MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_VIRTUALDESK|MOUSEEVENTF_MOVE| MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP);
    input.mi.mouseData=0;
    input.mi.dwExtraInfo=NULL;
    input.mi.time=0;
    SendInput(1,&input,sizeof(INPUT));

    INPUT inputplay;
    inputplay.type=INPUT_MOUSE;
    inputplay.mi.dx=32678;
    inputplay.mi.dy=29127;
    inputplay.mi.dwFlags=(MOUSE_EVENT|MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_VIRTUALDESK|MOUSEEVENTF_MOVE| MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP);
    inputplay.mi.mouseData=0;
    inputplay.mi.dwExtraInfo=NULL;
    inputplay.mi.time=0;
    SendInput(1,&inputplay,sizeof(INPUT));
    SendInput(1,&inputplay,sizeof(INPUT));


}
void Client::openRoller3()
{

    QString app="C:/Program Files/NoLimits 2/64bit/nolimits2app.exe";
    QProcess *process=new QProcess();
    process->setProgram(app);
    process->start();

    QTime dieTimer= QTime::currentTime().addSecs(10);
    while (QTime::currentTime() < dieTimer){
           QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    WORD vkCOde=0x0D;
    INPUT KeyEvent={0};
    KeyEvent.type=INPUT_KEYBOARD;
    KeyEvent.ki.wScan=MapVirtualKey(vkCOde,MAPVK_VK_TO_VSC);
    //SendInput(1,&KeyEvent,sizeof(INPUT));

    KeyEvent.ki.time=0;
    KeyEvent.ki.dwExtraInfo=0;
    KeyEvent.ki.wVk=vkCOde;
    KeyEvent.ki.dwFlags=0;
    SendInput(1,&KeyEvent,sizeof(INPUT));
    QTime dieTimer2= QTime::currentTime().addMSecs(500);
    while (QTime::currentTime() < dieTimer2){
           QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    SendInput(1,&KeyEvent,sizeof(INPUT));

    QTime dieTime= QTime::currentTime().addSecs(2);
    while (QTime::currentTime() < dieTime){
           QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    INPUT input;
    input.type=INPUT_MOUSE;
    input.mi.dx=25576;
    input.mi.dy=42960;
    input.mi.dwFlags=(MOUSE_EVENT|MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_VIRTUALDESK|MOUSEEVENTF_MOVE| MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP);
    input.mi.mouseData=0;
    input.mi.dwExtraInfo=NULL;
    input.mi.time=0;
    SendInput(1,&input,sizeof(INPUT));

    INPUT inputplay;
    inputplay.type=INPUT_MOUSE;
    inputplay.mi.dx=32678;
    inputplay.mi.dy=32678;
    inputplay.mi.dwFlags=(MOUSE_EVENT|MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_VIRTUALDESK|MOUSEEVENTF_MOVE| MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP);
    inputplay.mi.mouseData=0;
    inputplay.mi.dwExtraInfo=NULL;
    inputplay.mi.time=0;
    SendInput(1,&inputplay,sizeof(INPUT));
    SendInput(1,&inputplay,sizeof(INPUT));
}

void Client::reproducirContenido(QString contenido)
{
    QString location = QDir::currentPath() + "/Content/" + contenido;
    QString comando = "start " + QDir::currentPath() + "/MaxVR/MaxVR.exe " + location;
    system(comando.toStdString().c_str());
}
