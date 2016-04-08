#include "agregardialog.h"
#include "ui_agregardialog.h"
#include <QtXml>
#include <QtCore>
#include <QTextStream>
AgregarDialog::AgregarDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AgregarDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    disconnect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
}

AgregarDialog::~AgregarDialog()
{
    delete ui;
}

void AgregarDialog::on_buttonBox_accepted()
{
    //Agregar un "Xaman" a la plataforma
    if(ui->InputIp && ui->InputMac && ui->InputNombre){
        QString consulta;
        //Se hace una insercion en la base de datos
        consulta.append("INSERT INTO xaman ("
                        "IP,"
                        "MAC,"
                        "NOMBRE)"
                        "VALUES("
                        "'"+ui->InputIp->text()+"',"
                        "'"+ui->InputMac->text()+"',"
                        "'"+ui->InputNombre->text()+"'"
                        ");");
        QSqlQuery insertar;
        insertar.prepare(consulta);
        if(insertar.exec()){
            qDebug ()<<"exito";
        }else{
            qDebug ()<<"Error tabla";
        }
        AgregarDialog::accept();
    } else {
    }
}


