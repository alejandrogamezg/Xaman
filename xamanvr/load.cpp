#include "load.h"
#include "ui_load.h"
#include <QTime>
#include <QMovie>

Load::Load(QWidget *parent) :
    QDialog(parent,Qt::FramelessWindowHint),
    ui(new Ui::Load)
{

    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);   
    //close();

    QMovie *movie = new QMovie(":/images/Iconos/XAMAN_1.gif");
 //   QLabel *processLabel = new QLabel(this);
   // processLabel->setFixedSize(560,500);
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label->setMovie(movie);
    movie->start();
    Load::accept();

}

Load::~Load()
{
   /* QTime dieTime= QTime::currentTime().addSecs(2);
    while (QTime::currentTime() < dieTime){
           QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    */

    delete ui;
}
