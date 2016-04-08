#ifndef COMUNICACION_H
#define COMUNICACION_H

#include <QObject>
#include <QProcess>

class Comunicacion : public QObject
{
    Q_OBJECT

public:
    explicit Comunicacion(QObject *parent = 0);
    bool Hacerping(QString ip);
signals:


public slots:
};

#endif // COMUNICACION_H
