#include "xaman.h"
#include <QApplication>
#include <QCoreApplication>
#include <QTime>
#include "load.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Load s;
    s.show();

   QTime dieTime= QTime::currentTime().addSecs(5);
        while (QTime::currentTime() < dieTime){
               QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
    s.close();
    Xaman w;
    w.show();



    return a.exec();
}
