#include "dialogadmin.h"
#include "ui_dialogadmin.h"
#include "agregardialog.h"
#include "comunicacion.h"

DialogAdmin::DialogAdmin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAdmin)
{
    ui->setupUi(this);

    initXaman();
}

DialogAdmin::~DialogAdmin()
{
    delete ui;
}

void DialogAdmin::initXaman()
{
    QString consulta;
    consulta.append("SELECT * FROM xaman");
    QSqlQuery consultar;
    consultar.prepare(consulta);
    if(!consultar.exec()){
        QMessageBox::warning(this, "Error", "No se ha encontrado información acerca de los Xaman.");
    }
    int fila=0;
    int col=0;

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->insertRow(0);
    ui->tableWidget->setRowHeight(0,80);
    while(consultar.next()){
        Comunicacion com;

        QToolButton *button = new QToolButton(ui->scrollArea);
        button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        button->setStyleSheet("background-color: rgba(0,0,0,0%)");

        button->setIconSize(QSize(60,60));
        button->setText(consultar.value(2).toString());

        QMenu *menu= new QMenu();
        QSignalMapper *sigmapper = new QSignalMapper(this);

        if(com.Hacerping(consultar.value(0).toString()))
        {
            QAction *apagar = new QAction("Apagar", button);
            QAction *reiniciar = new QAction("Reiniciar", button);


            menu->addAction(apagar);
            menu->addAction(reiniciar);


            connect(apagar, SIGNAL(triggered(bool)), sigmapper, SLOT(map()));
            connect(reiniciar, SIGNAL(triggered(bool)), sigmapper, SLOT(map()));

            sigmapper->setMapping(apagar, consultar.value(0).toString() + ",Apagar");
            sigmapper->setMapping(reiniciar, consultar.value(0).toString() + ",Reiniciar");

           // button->setStyleSheet("background-color: #e5e5e5");
            button->setIcon(QIcon(":/images/Iconos/XAMon.png"));

        } else
        {
           QAction *encender = new QAction("Encender", button);
           menu->addAction(encender);

           connect(encender, SIGNAL(triggered(bool)), sigmapper, SLOT(map()));

           sigmapper->setMapping(encender, consultar.value(1).toString() + ",Encender");

           button->setIcon(QIcon(":/images/Iconos/XAMoff.png"));



        }

        button->setMenu(menu);
        button->setPopupMode(QToolButton::InstantPopup);

        if(col<8){
            ui->tableWidget->setCellWidget(fila,col,button);

            ui->tableWidget->setColumnWidth(col,ui->tableWidget->width()/7);

            col++;
            if(col==7){
                col=0;
                fila++;
                ui->tableWidget->insertRow(fila);
                ui->tableWidget->setRowHeight(fila,80);

            }
        }
    }
}

void DialogAdmin::openDB()
{
    QString nombre;
    nombre.append("XAMAN.sqlite");
    db= QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(nombre);
    db.open();

    crearTabla();
}

void DialogAdmin::crearTabla()
{
    QString consulta;
    consulta.append("CREATE TABLE IF NOT EXISTS xaman ("
                    "IP varchar(50) PRIMARY KEY UNIQUE ,"
                    "MAC varchar(50) UNIQUE ,"
                    "NOMBRE varchar(50) UNIQUE"
                    ");");
    QSqlQuery crear;
    crear.prepare(consulta);
    if(crear.exec()){
        qDebug ()<<"exito";
    }else{
        qDebug ()<<"Error crear";
    }
}

void DialogAdmin::on_btnadd_clicked()
{
    AgregarDialog w;
    w.exec();
}
