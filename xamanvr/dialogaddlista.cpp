#include "dialogaddlista.h"
#include "ui_dialogaddlista.h"
#include <QtXml>
#include <QtCore>
#include <QTextStream>
#include "xaman.h"
#include "ui_xaman.h"

DialogAddLista::DialogAddLista(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddLista)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

DialogAddLista::~DialogAddLista()
{
    delete ui;
}

void DialogAddLista::on_buttonBox_accepted()
{
    //Abrir las listas de reproduccion
    QDomDocument list;
    QFile file (QDir::currentPath()+"/PlayList/ListasdeReproduccion.xml");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        //return -1;
    }else{
        if(!list.setContent(&file)){
          //return -1;
        }
        file.close();
    }
    QDomElement node = list.createElement("List");
    QDomElement root = list.firstChildElement();

    root.appendChild(node);
    //agregar el elemento a la lista
    node.setAttribute("name",ui->lineEdit->text());
    root.appendChild(node);

    //Guardar las listas
     if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
         //return -1;
     }else{
         QTextStream stream(&file);
         stream << list.toString();
         file.close();
     }
}
