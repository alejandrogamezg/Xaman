#include "logindialog.h"
#include "ui_logindialog.h"
#include "agregardialog.h"
#include "dialogadmin.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    disconnect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_buttonBox_accepted()
{
    //Standar de los datos necesarios para credenciales
    QString user = "Hypnogic";
    QString password = "123456";

    //Obtencion de los datos ingresados por el usuario
    QString iuser = ui->inputuser->text();
    QString ipass = ui->inputpassword->text();

    //Validacion de datos
    if(user==iuser)
    {
        if(password == ipass)
        {
            //Aprobacion de los datos y emicion de la señal de aceptacion
            LoginDialog::accept();
            DialogAdmin w;
            w.exec();
        } else {
            ui->label_badpass->setText("Constraseña Incorrecta");
        }
    } else {
        ui->label_baduser->setText("Usuario Incorrecto");
    }
}
