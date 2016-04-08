#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QObject>
#include <QToolButton>
#include <QTcpSocket>
#include <QAction>
#include <QMenu>

class ToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit ToolButton(QWidget *parent = 0);
    ~ToolButton();

signals:
    void changeStateEvent();

public slots:
    void receive_data();
    void socket_disconnect();
    void turnOn();
    void turnOff();
    void reboot();
    void sendMessage(QString);

public:
    QString getIp();
    void setIp(QString);
    bool getState();
    void setState(bool);
    void update();
    void setSocket(QTcpSocket *socket);
    void setMac(QString);
    QString getMac();

private:
    QString ip;
    QString mac;
    QTcpSocket *socket;
    bool state = false;
    QAction* encender;
    QAction* reiniciar;
    QAction* apagar;
    QMenu* menu;

};

#endif // TOOLBUTTON_H
