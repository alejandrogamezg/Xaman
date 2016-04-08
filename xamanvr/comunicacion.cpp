#include "comunicacion.h"
#include "QDebug"

Comunicacion::Comunicacion(QObject *parent) : QObject(parent)
{

}

bool Comunicacion::Hacerping(QString ip){
    int returnedCode = QProcess::execute("ping -n 1 -w 100 -l 32 " + ip);
    qDebug("ping");
    //bool returnedCode = QProcess::startDetached("ping " + ip);
    if (returnedCode == 0){
        return true;
    }
    return false;
    /*
    QProcess ping;
    QString exec = "ping";
    QStringList params;
    qDebug() << ip;
    params << "ping -n 1 " + ip;
    ping.start(exec, params, QIODevice::ReadOnly);
    ping.waitForFinished(-1);

    QString p_stdout = ping.readAllStandardOutput();
    qDebug() << p_stdout;
    if (!p_stdout.isEmpty()){
        return true;
    }
    return false;
    */
}

