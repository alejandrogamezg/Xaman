#include "xaman.h"
#include "ui_xaman.h"
#include "load.h"
#include "tcpsocket.h"
#include "windows.h"

//Programa principal de la interfaz y configuraciones

Xaman::Xaman(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Xaman)
{
    ui->setupUi(this);

    Q_INIT_RESOURCE(resource);
    init();

    this->server = new QTcpServer(this);
    connect(this->server, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(server_Error()));
    connect(this->server, SIGNAL(newConnection()), this, SLOT(server_Connected()));

    //if(server->listen(QHostAddress::LocalHost, 6000)){
    if(server->listen(QHostAddress::Any, 6000)){
        qDebug() << "Escuchando";
    }

    //BOTONES DE LA INTERFAZ
    connect(ui->widget->controls, SIGNAL(changeVolume(int)), this, SLOT(test(int)));
    connect(ui->widget->controls, SIGNAL(play()), this, SLOT(remotePlay()));
    connect(ui->widget->controls, SIGNAL(stop()), this, SLOT(remoteStop()));
    connect(ui->widget->controls, SIGNAL(pause()), this, SLOT(remotePause()));
    connect(ui->widget->controls, SIGNAL(next()), this, SLOT(remoteNext()));
    connect(ui->widget->controls, SIGNAL(previous()), this, SLOT(remotePrevious())); 
    
    
    //CREACION DE EL ESTILO DE LA INTERFAZ
    ui->tabWidget->setStyleSheet("QTabWidget::pane { border: transparent;}"
                                 "QTabWidget::tab-bar {left: 0px;"
                                 "}"
                                 "QTabBar::tab {"
                                     "background: #000000;"
                                     "border: transparent;"
                                     "border-top: 5px solid #D8FF00;"
                                     "color: white;"
                                     "min-width: 8ex;"
                                     "padding: 2px;"
                                     "width: 495px;"
                                     "height: 23px"
                                     "}"
                                     "QTabBar::tab:selected, QTabBar::tab:hover {"

                                      "color: black;"
                                      "background: url(':/images/Iconos/pestania_principal.png') #000000 ;}"
                                      "QTabBar::tab:!selected {margin-top: 0px;}");

    ui->Contenido->setStyleSheet(  "QTabWidget::tab-bar {left: 14px;"
                                   "}"
                                   "QTabBar::tab {"
                                   "background: #000000;"
                                   "border: transparent;"
                                   "border-top: 1px solid #D8FF00;"
                                   "color: white;"
                                   "min-width: 8ex;"
                                   "padding: 2px;"
                                   "width: 320px;"
                                   "height: 29px"
                                   "}"
                                   "QTabBar::tab:selected, QTabBar::tab:hover {"

                                    "color: black;"
                                    "background: url(':/images/Iconos/pestania_libreria.png') no-repeat #000000 ;}"
                                    "QTabBar::tab:!selected {margin-top: 0px;}" );


    ui->Content_Shows->setStyleSheet("QHeaderView::section {background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #505050, stop: 0.2 #404040, stop: 0.4 #303030 , stop: 0.6 #202020 , stop: 0.8 #101010, stop: 1 #080808 );border: transparent; "
                                     "height: 38px;"
                                     "text-align: center;"
                                     "}"
                                     "QTreeWidget{border: transparent;}"

                                     "QTreeView::item {"
                                         "border-bottom: 1px solid  #686868;"
                                         "height: 23px"
                                     "}"
                                     "QTreeView::item:hover {"
                                         "background: transparent;"
                                         "border: 1px solid #ffffff;"
                                         "border-left: 0px solid #25282A;"
                                         "border-right: 0px solid #25282A;"
                                         "color: #d8ff00;"
                                     "}"
                                     "QTreeView::item:selected:active{"
                                        "background: transparent;"
                                        "border: 1px solid #ffffff;"
                                        "border-left: 0px"
                                        "border-right: 0px"
                                        "color: #ffffff;"

                                     "}"
                                     "QTreeView  {"
                                         "show-decoration-selected: 1;"
                                         "color: #d8ff00;"

                                     "}"
                                     );
    ui->Content_Musica->setStyleSheet("QHeaderView::section {background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #505050, stop: 0.2 #404040, stop: 0.4 #303030 , stop: 0.6 #202020 , stop: 0.8 #101010, stop: 1 #080808 );border: transparent; "
                                     "height: 38px;"
                                     "text-align: center;"
                                     "}"
                                     "QTreeWidget{border: transparent;}"

                                     "QTreeView::item {"
                                         "border-bottom: 1px solid  #686868;"
                                         "height: 23px"
                                     "}"
                                     "QTreeView::item:hover {"
                                         "background: transparent;"
                                         "border: 1px solid #ffffff;"
                                         "border-left: 0px solid #25282A;"
                                         "border-right: 0px solid #25282A;"
                                         "color: #d8ff00;"
                                     "}"
                                     "QTreeView::item:selected:active{"
                                        "background: transparent;"
                                        "border: 1px solid #ffffff;"
                                        "border-left: 0px"
                                        "border-right: 0px"
                                        "color: #ffffff;"

                                     "}"
                                     "QTreeView  {"
                                         "show-decoration-selected: 1;"
                                         "color: #d8ff00;"

                                     "}"


                                     );
    ui->Content_Imagenes->setStyleSheet("QHeaderView::section {background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #505050, stop: 0.2 #404040, stop: 0.4 #303030 , stop: 0.6 #202020 , stop: 0.8 #101010, stop: 1 #080808 );border: transparent; "
                                        "height: 38px;"
                                        "text-align: center;"
                                        "}"
                                        "QTreeWidget{border: transparent;}"

                                        "QTreeView::item {"
                                            "border-bottom: 1px solid  #686868;"
                                            "height: 23px"
                                        "}"
                                        "QTreeView::item:hover {"
                                            "background: transparent;"
                                            "border: 1px solid #ffffff;"
                                            "border-left: 0px solid #25282A;"
                                            "border-right: 0px solid #25282A;"
                                            "color: #d8ff00;"
                                        "}"
                                        "QTreeView::item:selected:active{"
                                           "background: transparent;"
                                           "border: 1px solid #ffffff;"
                                           "border-left: 0px"
                                           "border-right: 0px"
                                           "color: #ffffff;"

                                        "}"
                                        "QTreeView  {"
                                            "show-decoration-selected: 1;"
                                            "color: #d8ff00;"

                                        "}"

                                     );

    ui->Content_Shows->header()->setDefaultAlignment(Qt::AlignCenter);

    ui->Content_Imagenes->header()->setDefaultAlignment(Qt::AlignCenter);

    ui->Content_Musica->header()->setDefaultAlignment(Qt::AlignCenter);

    ui->treeWidget->setStyleSheet("QHeaderView::section {background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #505050, stop: 0.2 #404040, stop: 0.4 #303030 , stop: 0.6 #202020 , stop: 0.8 #101010, stop: 1 #080808 );border: transparent; "
                                  "height: 45px;"
                                  "text-align: center;"
                                  "}"
                                  "QTreeWidget{border: transparent;}"

                                  "QTreeView::item {"

                                      "border-bottom: 1px solid  #686868;"
                                      "height: 30px"
                                  "}"
                                  "QTreeView::item:hover {"
                                      "background: transparent;"
                                      "border-top: 1px solid #ffffff;"
                                      "border-bottom: 1px solid  #ffffff;"

                                      "color: #d8ff00;"
                                  "}"

                                  );
    ui->treeWidget->header()->setDefaultAlignment(Qt::AlignCenter);
    ui->treeWidget->setFocusPolicy(Qt::NoFocus);
    ui->btn_addPlaylist->setIcon(QIcon(":/images/Iconos/listanueva_icon.png"));
    ui->btn_addPlaylist->setIconSize(QSize(30,30));
    ui->btn_addPlaylist->setStyleSheet("QToolButton{ background: rgba(0,0,0,0%);  border: transparent;}");

    ui->verticalSlider->setStyleSheet("QSlider::groove:vertical {"
                                      "border: 0px solid #d0eb46;"
                                      "background: white;"
                                      "border-radius: 4px;"
                                      "}"

                                      "QSlider::sub-page:vertical {"
                                      "background: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,"
                                          "stop: 0 #d0eb46;, stop: 1 #d0eb46;);"
                                      "background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,"
                                          "stop: 0 #d0eb46 , stop: 1 #000000;);"
                                      "border: 1px solid #d0eb46;"
                                      "border-radius: 4px;"
                                      "}"

                                      "QSlider::add-page:vertical {"
                                      "background: #d0eb46;"
                                      "border: 1px solid #777;"

                                      "border-radius: 4px;"
                                      "}"

                                      "QSlider::handle:vertical {"
                                      "background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
                                          "stop:0 #eee, stop:1 #ccc);"
                                      "border: 1px solid #777;"
                                      "width: 3px;"
                                      "margin-top: 5px;"
                                      "margin-bottom: 5px;"
                                      "border-radius: 4px;"
                                      "}"

                                      "QSlider::handle:vertical:hover {"
                                      "background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
                                      "stop:0 #fff, stop:1 #ddd);"
                                      "border: 1px solid #444;"
                                      "border-radius: 4px;"
                                      "}"

                                      "QSlider::sub-page:vertical:disabled {"
                                      "background: #d0eb46;"
                                      "border-color: #999;"
                                      "}"

                                      "QSlider::add-page:vertical:disabled {"
                                      "background: #eee;"
                                      "border-color: #999;"
                                      "}"

                                      "QSlider::handle:vertical:disabled {"
                                      "background: #eee;"
                                      "border: 1px solid #aaa;"
                                      "border-radius: 4px;"
                                      "}");
    ui->playlist->setStyleSheet("QScrollArea { border: transparent; background-color: rgba(0,0,0,0%); }");
    
    //DECLARACION DE LOS BOTONES DE ACCESO DIRECTO
    QAction *shortcut = new QAction(this);
    shortcut->setShortcut(Qt::CTRL | Qt::ALT | Qt::Key_H);;

    connect(shortcut, SIGNAL(triggered(bool)), this, SLOT(addXaman()));
    this->addAction(shortcut);
}

Xaman::~Xaman()
{
    delete ui;
}

void Xaman::actualizarList()
{
    //Actualizacion de la listas de reproduccion
    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->setHeaderLabel("PlayList");

    QDomDocument playlist;
    QFile file (QDir::currentPath()+"/PlayList/ListasdeReproduccion.xml");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        //return -1;
    }else{
        if(!playlist.setContent(&file)){
        }
        file.close();
    }
    QDomElement root = playlist.firstChildElement();
    ui->treeWidget->clear();
    listElement(root, "List", "name");
}

void Xaman::addChild(QTreeWidgetItem *parent, QString name){
    QTreeWidgetItem *itm = new QTreeWidgetItem();
    //Se colocan los datos correspondientes
    itm->setText(0,name);
    parent->addChild(itm);
}

void Xaman::addRoot(QDomElement element, QString attribute){
    //Se crea un widget para colocar los datos respectivos
   QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
   itm->setText(0,element.attribute(attribute));
   QDomNodeList items= element.elementsByTagName("Video");
   for(int i=0; i< items.count();i++){
       QDomNode itemnode =items.at(i);
       if(itemnode.isElement()){
           QDomElement itml = itemnode.toElement();
           //Redireccionamiento a otro metodo
           addChild(itm,itml.attribute(attribute));
       }
   }
}

void Xaman::addToList(QString action)
{
    //Insertar un elemento a una lista de reproduccion
    QStringList name= action.split(",");
    QString cadena = name.at(1);
    QStringList fileName = cadena.split("/");

    QDomDocument playlist;

    //Se hace lectura del archivo de las playlist
    QFile file (QDir::currentPath()+"/PlayList/ListasdeReproduccion.xml");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        //return -1;
    }else{
        if(!playlist.setContent(&file)){
          //return -1;
        }
        file.close();
    }
    QDomElement root = playlist.firstChildElement();
    QDomNodeList items= root.elementsByTagName("List");
    for(int i=0; i< items.count();i++){
        QDomNode itemnode =items.at(i);
        if(itemnode.isElement()){
            QDomElement element = itemnode.toElement();
            if(element.attribute("name")==name.at(0)){
                   QDomElement node = playlist.createElement("Video");
                   node.setAttribute("name", fileName.at(fileName.count()-1));
                   node.setAttribute("href", name.at(1));
                   element.appendChild(node);
            }
        }
    }

    //Se guardan los cambios realizados
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        //return -1;
    }else{
        QTextStream stream(&file);
        stream << playlist.toString();
        file.close();
    }
    ui->treeWidget->clear();
    listElement(root, "List", "name");
}

void Xaman::addXaman()
{
    //Visualizar el Dialog de login para la parte del administrador
    LoginDialog w;
    w.exec();
}

void Xaman::contenido(){
    //Colocar contenido de los show
    setContenido(ui->Content_Shows);

    //Colocar contenido de las imagenes
    setContenido(ui->Content_Imagenes);

    //Colocar contenido de la musica
    setContenido(ui->Content_Musica);
}

void Xaman::creartabla()
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

void Xaman::init()
{
    //Inicializar la DB donde se guarda un registro de las maquinas
    openDB();

    //Inicializar la interfaz de los botones para controlar remotamente los equipos
    initXamanButton();

    //Inicializar la seccion de las playlist
    initPlayList();

    //Inicializar el widget para contenido a presentar
    contenido();

    connect(ui->verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));

    //Conectar los menu desplegables de cada seccion
    ui->Content_Shows->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->Content_Shows,SIGNAL(customContextMenuRequested(QPoint)),this, SLOT(menu(const QPoint&)));

    ui->Content_Musica->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->Content_Musica,SIGNAL(customContextMenuRequested(QPoint)),this, SLOT(menuMusica(const QPoint&)));

    ui->Content_Imagenes->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->Content_Imagenes,SIGNAL(customContextMenuRequested(QPoint)),this, SLOT(menuImagenes(const QPoint&)));

    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget,SIGNAL(customContextMenuRequested(QPoint)),this, SLOT(menuList(const QPoint&)));
}

int num_clientes;
void Xaman::initXamanButton ()
{
    QString consulta;
    consulta.append("SELECT * FROM xaman");
    QSqlQuery consultar;
    consultar.prepare(consulta);
    if(!consultar.exec()){
        QMessageBox::warning(this, "Error", "No se ha encontrado información.");
    }
    //Configuracion de la tabla que contiene el estado de las maquinas
    int fila=0;
    int col=0;

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->insertRow(0);
    ui->tableWidget->setRowHeight(0,80);

    //Ciclo por item de la lista de maquinas
    while(consultar.next()){
        //Se crea el boton para representar a cada maquina
        //QToolButton *button = new QToolButton(ui->scrollArea);
        ToolButton *button = new ToolButton(ui->scrollArea);
        //qDebug() << consultar.value(0); //IP
        //qDebug() << consultar.value(1); //MAC
        //qDebug() << consultar.value(2); //Nombre del equipo

        //Configuraciones del boton
        button->setText(consultar.value(2).toString());
        button->setIp(consultar.value(0).toString());
        button->setMac(consultar.value(1).toString());
        button->update();

        if(col<9){
            ui->tableWidget->setCellWidget(fila,col,button);
            ui->tableWidget->setColumnWidth(col,ui->tableWidget->width()/8);
            col++;
            if(col==8){
                col=0;
                fila++;
                ui->tableWidget->insertRow(fila);
                ui->tableWidget->setRowHeight(fila,80);
            }
        }
        buttons.insert(num_clientes, button);
        num_clientes++;
    }
}

void Xaman::initPlayList()
{
    //Configuracion de la seccion de las playlist
    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->setHeaderLabel("Listas de Reproducción");

    //Lectura del documento contenedor de las listas
    QDomDocument playlist;
    QFile file (QDir::currentPath()+"/PlayList/ListasdeReproduccion.xml");

    //Comprobacion de errores de lectura
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this,"Error de PlayList","No se han podido encontrar las listas de reproducción.");
    }else{
        if(!playlist.setContent(&file)){
          QMessageBox::warning(this,"Erros al cargar las PlayList", "No se han podido cargar sus listas de reproducción.");        }
        file.close();
    }
    //Se ubica el primer elemento del documento
    QDomElement root = playlist.firstChildElement();
    //Metodo encargado de interpretar las listas
    listElement(root,"List","name");
}

void Xaman::listToPlaylist(int index)
{
    //Enviar toda la lista predefinida a la reproduccion actual
    QDomDocument playlist;
    QFile file (QDir::currentPath() + "/PlayList/ListasdeReproduccion.xml");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        //return -1;
    }else{
        if(!playlist.setContent(&file)){
          //return -1;
        }
        file.close();
    }
    QDomElement root = playlist.firstChildElement();
    QDomNodeList items= root.elementsByTagName("List");
    QDomNode itm = items.at(index);
    QDomNodeList child = itm.childNodes();

    //SE obtienen los item de la lista y se recorren de manera individual
    for (int i = 0; i<child.count(); i++){
        QDomNode node = child.at(i);
        if(node.isElement()){
            QDomElement element = node.toElement();

            //Se inserta el item en la lista actual de reproduccion
            ui->widget->player->playlist()->addMedia(QUrl::fromLocalFile(element.attribute("href")));
            if(ui->widget->player->playlist()->isEmpty()){
                ui->widget->player->play();
                ui->widget->player->pause();
            }
        }
    }
}

void Xaman::listElement(QDomElement root, QString tagname, QString attribute){
    //Listar cada lista de reproduccion
    QDomNodeList items= root.elementsByTagName(tagname);
    for(int i=0; i< items.count();i++){
        QDomNode itemnode =items.at(i);
        if(itemnode.isElement()){
            QDomElement element = itemnode.toElement();
            //Metodo para agregarlos a la estructura
            addRoot(element,attribute);
        }
    }
}

void Xaman::menu(const QPoint &pos)
{
    //Menu de acuerdo a una posicion en la ventana
    QTreeWidgetItem *nd = ui->Content_Shows->itemAt(pos);

    if(nd){
        QAction *newAct = new QAction(tr("&Añadir a Reproduccion"), this);
        newAct->setStatusTip(tr("new sth"));
        QAction *newAct1 = new QAction(tr("&Agregar"), this);
        newAct1->setStatusTip(tr("new sth"));

        QSignalMapper *sigmapper = new QSignalMapper(this);

        connect(newAct, SIGNAL(triggered(bool)), sigmapper, SLOT(map()));

        sigmapper->setMapping(newAct,"Reproducir,Shows/" + nd->text(0));

        connect(sigmapper, SIGNAL(mapped(QString)), this, SLOT(reproducirContenido(QString)));

        QMenu menu(this);
        menu.addAction(newAct);
        menu.addAction(newAct1);

        QMenu menu2(this);

        QDomDocument playlist;
        QFile file (QDir::currentPath() + "/PlayList/ListasdeReproduccion.xml");
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            //return -1;
        }else{
            if(!playlist.setContent(&file)){
              //return -1;
            }
            file.close();
        }
        QDomElement root = playlist.firstChildElement();
        QDomNodeList items= root.elementsByTagName("List");
        QSignalMapper *mapper = new QSignalMapper(this);
        int cont=1;
        for(int i=0; i< items.count();i++){
            QDomNode itemnode =items.at(i);

            if(itemnode.isElement()){

                QDomElement element = itemnode.toElement();

                QAction *act = new QAction(element.attribute("name"), this);
                connect(act, SIGNAL(triggered(bool)), mapper, SLOT(map()));
                mapper->setMapping(act, element.attribute("name") + "," + QDir::currentPath() + "/Content/Shows/" + nd->text(0));
                if(cont==1){
                connect(mapper, SIGNAL(mapped(QString)), this, SLOT(addToList(QString)));
                cont++;
                }
                menu2.addAction(act);
            }
        }
        newAct1->setMenu(&menu2);

        QPoint pt(pos);
        menu.exec( ui->Content_Shows->mapToGlobal(pos) );
    }

}

void Xaman::menuMusica(const QPoint &pos)
{
    //Menu desplegable de musica con sus correspondientes opciones
    QTreeWidgetItem *nd = ui->Content_Musica->itemAt(pos);
    if(nd){
        //Configuracion de las opciones del menu
        QAction *newAct = new QAction(tr("&Añadir a Reproduccion"), this);
        newAct->setStatusTip(tr("new sth"));
        QAction *newAct1 = new QAction(tr("&Agregar"), this);
        newAct1->setStatusTip(tr("new sth"));
        QSignalMapper *sigmapper = new QSignalMapper(this);
        connect(newAct, SIGNAL(triggered(bool)), sigmapper, SLOT(map()));
        sigmapper->setMapping(newAct,"Reproducir,Musica/" + nd->text(0));
        connect(sigmapper, SIGNAL(mapped(QString)), this, SLOT(reproducirContenido(QString)));

        //Agregar las opciones al menu desplegable
        QMenu menu(this);
        menu.addAction(newAct);
        menu.addAction(newAct1);

        //Submenu de la parte de reproduccion
        QMenu menu2(this);
        QDomDocument playlist;
        QFile file (QDir::currentPath() + "/PlayList/ListasdeReproduccion.xml");
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            //return -1;
        }else{
            if(!playlist.setContent(&file)){
              //return -1;
            }
            file.close();
        }
        QDomElement root = playlist.firstChildElement();
        QDomNodeList items= root.elementsByTagName("List");
        QSignalMapper *mapper = new QSignalMapper(this);
        int cont=1;
        for(int i=0; i< items.count();i++){
            QDomNode itemnode =items.at(i);

            if(itemnode.isElement()){

                QDomElement element = itemnode.toElement();

                QAction *act = new QAction(element.attribute("name"), this);
                connect(act, SIGNAL(triggered(bool)), mapper, SLOT(map()));
                mapper->setMapping(act, element.attribute("name") + "," + QDir::currentPath() + "/Content/Musica/" + nd->text(0));
                if(cont==1){
                connect(mapper, SIGNAL(mapped(QString)), this, SLOT(addToList(QString)));
                cont++;
                }
                menu2.addAction(act);
            }
        }

        //Agregar el submenu
        newAct1->setMenu(&menu2);

        //Posicionamiento del menu de acuerdo al posicionamiento del item en la pantalla
        QPoint pt(pos);
        menu.exec( ui->Content_Shows->mapToGlobal(pos) );
    }
}

void Xaman::menuImagenes(const QPoint &pos)
{
    QTreeWidgetItem *nd = ui->Content_Imagenes->itemAt(pos);

    QAction *newAct = new QAction(tr("&Añadir a Reproduccion"), this);
    newAct->setStatusTip(tr("new sth"));
    QAction *newAct1 = new QAction(tr("&Agregar"), this);
    newAct1->setStatusTip(tr("new sth"));

    QSignalMapper *sigmapper = new QSignalMapper(this);

    connect(newAct, SIGNAL(triggered(bool)), sigmapper, SLOT(map()));

    sigmapper->setMapping(newAct,"Reproducir,Imagenes/" + nd->text(0));

    connect(sigmapper, SIGNAL(mapped(QString)), this, SLOT(toAll(QString)));
    connect(sigmapper, SIGNAL(mapped(QString)), this, SLOT(reproducirContenido(QString)));


    QMenu menu(this);
    menu.addAction(newAct);
    menu.addAction(newAct1);

    QMenu menu2(this);

    QDomDocument playlist;
    QFile file (QDir::currentPath() + "/PlayList/ListasdeReproduccion.xml");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        //return -1;
    }else{
        if(!playlist.setContent(&file)){
          //return -1;
        }
        file.close();
    }
    QDomElement root = playlist.firstChildElement();
    QDomNodeList items= root.elementsByTagName("List");

    QSignalMapper *mapper = new QSignalMapper(this);
    int cont=1;
    for(int i=0; i< items.count();i++){
        QDomNode itemnode =items.at(i);

        if(itemnode.isElement()){

            QDomElement element = itemnode.toElement();

            QAction *act = new QAction(element.attribute("name"), this);
            connect(act, SIGNAL(triggered(bool)), mapper, SLOT(map()));
            mapper->setMapping(act, element.attribute("name") + "," + QDir::currentPath() + "/Content/Imagenes/" + nd->text(0));
            if(cont==1){
            connect(mapper, SIGNAL(mapped(QString)), this, SLOT(addToList(QString)));
            cont++;
            }

            menu2.addAction(act);
        }
    }

    newAct1->setMenu(&menu2);

    QPoint pt(pos);
    menu.exec( ui->Content_Shows->mapToGlobal(pos) );
}

void Xaman::menuList(const QPoint &pos)
{
    //Opciones de los botones "Xaman" para operaciones remotas
    QTreeWidgetItem *nd = ui->treeWidget->itemAt(pos);
    if(nd){
        if(nd->childCount() != -1){
            //Acciones que se presentaran en el menu
            QAction *newAct = new QAction(tr("&Añadir a Reproducción"), this);
            newAct->setStatusTip(tr("new sth"));
            QAction *Eliminar = new QAction(tr("&Eliminar"), this);
            Eliminar->setStatusTip(tr("new sth"));

            //Configuracion de las opciones
            QSignalMapper *sigmapperEliminar = new QSignalMapper(this);
            connect(Eliminar, SIGNAL(triggered(bool)), sigmapperEliminar, SLOT(map()));
            sigmapperEliminar->setMapping(Eliminar, ui->treeWidget->currentIndex().row());
            connect(sigmapperEliminar, SIGNAL(mapped(int)), this, SLOT(EliminarPlaylist(int)));
            QSignalMapper *sigmapper = new QSignalMapper(this);
            connect(newAct, SIGNAL(triggered(bool)), sigmapper, SLOT(map()));
            sigmapper->setMapping(newAct, ui->treeWidget->currentIndex().row());
            connect(sigmapper, SIGNAL(mapped(int)), this, SLOT(listToPlaylist(int)));

            //Incluir el menu en cada boton del apartado de sistema
            QMenu menu(this);
            if(nd->childCount()==0){
                if(!nd->parent()){
                    menu.addAction(Eliminar);
                }
            }else{
                menu.addAction(newAct);
                menu.addAction(Eliminar);
            }
            QPoint pt(pos);
            menu.exec( ui->treeWidget->mapToGlobal(pos) );
        }
    }
}

void Xaman::openDB()
{
    QString nombre;
    nombre.append("XAMAN.sqlite");
    db= QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(nombre);
    db.open();
    QStringList tables = db.tables();
    if (!tables.contains("xaman")){
        creartabla();
    }
}

void Xaman::on_btn_reiniciar_todo_clicked()
{
    foreach(ToolButton *btn, buttons){
        btn->reboot();
    }
}

void Xaman::reproducirContenido(QString contenido)
{
    //Envio de los nombre de los archivos para la reproduccion actual
    QStringList list = contenido.split(",");
    ui->widget->player->playlist()->addMedia(QUrl::fromLocalFile(QDir::currentPath() + "/Content/" + list.at(1)));
    if(c==0){
        ui->widget->player->play();
        ui->widget->player->pause();
        c++;
    }
}

void Xaman::setContenido(QTreeWidget *widget)
{
    //Configuracion de la tabla de contenidos
    widget->setColumnCount(2);
    QStringList headers;
    headers << "  NOMBRE" << "CLASIFICACION" << "DURACIÓN";
    widget->setHeaderLabels(headers);
    widget->setColumnWidth( 0 ,  324);
    widget->setColumnWidth( 1 ,  324);
    widget->setColumnWidth( 2 ,  324);
    widget->setFocusPolicy(Qt::NoFocus);
    QStringList filtros;

    //Lectura especifica de un tipo de archivo
    QStringList list = widget->objectName().split("_");
    if(list.at(1)=="Shows"){
        filtros << "*.mp4";
    }
    if(list.at(1)=="Efectos3D"){
        filtros << "*.exe";
    }
    if(list.at(1)=="Imagenes"){
        filtros << "*.jpg" << "*.png" << "*.bmp";
    }
    if(list.at(1)=="Musica"){
        filtros << "*.mp3" << "*.wav"<< "*.m4a";
    }
    //Ruta donde se encuentran los archivos
    QDir mDir (QDir::currentPath() + "/" + list.at(0) + "/" + list.at(1));

    //Ciclo que obtiene la informacion de cada archivo
    foreach (QString mItm, mDir.entryList(filtros)) {
      if(mItm!="."){
          if(mItm!=".."){
            QTreeWidgetItem *itm = new QTreeWidgetItem(widget);
            itm->setTextAlignment(0,Qt::AlignCenter);
            itm->setTextAlignment(1,Qt::AlignHCenter | Qt::AlignCenter);
            itm->setTextAlignment(2,Qt::AlignHCenter | Qt::AlignCenter);
            itm->setText(0,mItm);
            itm->setText(1, "AA");
            itm->setText(2, "00:000:00");
          }
      }
    }
}

void Xaman::setValue(int)
{
    //Verificar el valor del slider que controla las opciones de encendido de los "xaman"
    if(ui->verticalSlider->value() == 0){
        foreach (ToolButton *btn, buttons) {
            QString cmd = "shutdown -m " + btn->getIp() + " -s -t 1";
            QProcess::execute(cmd.toStdString().c_str());
        }
    } else if(ui->verticalSlider->value() == 1){
        sender.wakeOnLanAll();
    }
}

void Xaman::toAll(QString action)
{
    //Broadcast de una señal - instruccion
    sender.sendDatagram(action);
}

void Xaman::EliminarPlaylist(int node)
{

    QDomDocument playlist;
    QFile file (QDir::currentPath() + "/PlayList/ListasdeReproduccion.xml");

    //qDebug()<< QString(node);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        //return -1;
    }else{
        if(!playlist.setContent(&file)){
          //return -1;
        }
        file.close();
    }
    QDomElement root = playlist.firstChildElement();
    QDomNodeList items= root.elementsByTagName("List");

    root.removeChild(items.at(node));

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        //return -1;
    }else{
        QTextStream stream(&file);
        stream << playlist.toString();
        file.close();
    }

    actualizarList();
}

void Xaman::on_btn_addPlaylist_clicked()
{
    DialogAddLista w;
    w.exec();
    actualizarList();
}

void Xaman::server_Error()
{
    qDebug() << this->server->errorString();
}

void Xaman::server_Connected()
{
    QTcpSocket *sckt = server->nextPendingConnection();
    TcpSocket *socketX = new TcpSocket(sckt);

    connect(socketX, SIGNAL(dataReady(QTcpSocket*,TcpSocket*)), this, SLOT(receive_client(QTcpSocket*,TcpSocket*)));
}

void Xaman::receive_client(QTcpSocket *socket, TcpSocket *socketX)
{
    QByteArray Data = socket->readAll();
    qDebug() << buttons.count();

    foreach (ToolButton *btn, buttons) {
        QString dir = btn->getIp();
        if (Data == dir){
            btn->setSocket(socket);
            btn->setState(true);
            btn->update();
            break;
        }
    }
    disconnect(socketX, SIGNAL(dataReady(QTcpSocket*,TcpSocket*)), this, SLOT(receive_client(QTcpSocket*,TcpSocket*)));
}

void Xaman::remotePlay()
{
    Player *player = ui->widget;
    player->playlist->setCurrentIndex(player->playlistView->currentIndex().row());
    if (!player->player->playlist()->isEmpty()){
        QString path = player->player->playlist()->media(0).canonicalResource().url().toString();
        QStringList list = path.split("/");
        if(player->player->playlist()->currentIndex() == -1){
            QFile file(player->player->playlist()->media(0).canonicalResource().url().toString());
            QFileInfo info(file);

            if (list.at(list.size()-2) != "Musica"){
                player->player->setVolume(100);
                player->player->play();
            } else {
                player->player->setMuted(false);
                player->player->play();
            }
        } else {
            QFile file(player->player->currentMedia().canonicalResource().url().toString());
            QFileInfo info(file);

            if(info.fileName()== "RollerCoaster_3.mp4"){
                player->controls->stopButton->setEnabled(false);
                //sender.sendDatagram("Reproducir," + list.at(list.size()-2) + "/" + info.fileName());
                foreach (ToolButton *btn, buttons) {
                    if(btn->getState()){
                        QString msg = "Reproducir," + list.at(list.size()-2) + "/" + info.fileName();
                        btn->sendMessage(msg);
                    }
                }
                QTime dieTimer= QTime::currentTime().addSecs(5);
                while (QTime::currentTime() < dieTimer){
                           QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
                }
                player->player->play();
                player->controls->stopButton->setDisabled(true);
                QTime dieTimer1= QTime::currentTime().addSecs(25);
                while (QTime::currentTime() < dieTimer1){
                       QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
                }
                player->controls->stopButton->setEnabled(true);

            }else if(info.fileName() == "RollerCoaster_2.mp4"){
                player->controls->stopButton->setEnabled(false);

                //sender.sendDatagram("Reproducir," + list.at(list.size()-2) + "/" + info.fileName());
                foreach (ToolButton *btn, buttons) {
                    if(btn->getState()){
                        QString msg = "Reproducir," + list.at(list.size()-2) + "/" + info.fileName();
                        btn->sendMessage(msg);
                    }
                }
                QTime dieTimer= QTime::currentTime().addSecs(9);
                while (QTime::currentTime() < dieTimer){
                           QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
                }
                player->player->play();
                player->controls->stopButton->setDisabled(true);
                QTime dieTimer1= QTime::currentTime().addSecs(25);
                while (QTime::currentTime() < dieTimer1){
                       QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
                }
                player->controls->stopButton->setEnabled(true);
            }else if(info.fileName() == "RollerCoaster_1.mp4"){
                player->controls->stopButton->setEnabled(false);

                //sender.sendDatagram("Reproducir," + list.at(list.size()-2) + "/" + info.fileName());
                foreach (ToolButton *btn, buttons) {
                    if(btn->getState()){
                        QString msg = "Reproducir," + list.at(list.size()-2) + "/" + info.fileName();
                        btn->sendMessage(msg);
                    }
                }
                QTime dieTimer= QTime::currentTime().addSecs(5);
                while (QTime::currentTime() < dieTimer){
                           QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
                }
                player->player->play();
                player->controls->stopButton->setDisabled(true);
                QTime dieTimer1= QTime::currentTime().addSecs(25);
                while (QTime::currentTime() < dieTimer1){
                       QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
                }
                player->controls->stopButton->setEnabled(true);

            }else{
                if(list.at(list.size()-2)!= "Musica"){
                    //player->setMuted(true);
                    player->player->setVolume(100);
                    foreach (ToolButton *btn, buttons) {
                        if(btn->getState()){
                            QString msg = "Reproducir," + list.at(list.size()-2) + "/" + info.fileName();
                            btn->sendMessage(msg);
                        }
                    }
                    player->player->play();
                    //emit playRemote(list.at(list.size()-2) + "/" + info.fileName());
                    //sender.sendDatagram("Reproducir," + list.at(list.size()-2) + "/" + info.fileName());
                }else{
                    player->player->setMuted(false);
                    player->player->play();
                }
            }
        }
    }
}

void Xaman::remoteStop()
{
    foreach (ToolButton *btn, buttons){
        if(btn->getState()){
            btn->sendMessage("Detener");
        }
    }
    ui->widget->player->stop();
}

void Xaman::remotePrevious()
{
    if (ui->widget->player->position() <= 5000)
        ui->widget->playlist->previous();
    else
        ui->widget->player->setPosition(0);
}

void Xaman::remotePause()
{

}

void Xaman::remoteNext()
{

}

void Xaman::playlistPositionChanged(int currentItem)
{
    Player *player = ui->widget;
   if(currentItem != 0 && currentItem != -1){
        //sender.sendDatagram("Detener");
        this->remoteStop();
        QTime dieTime1= QTime::currentTime().addSecs(5);
        while (QTime::currentTime() < dieTime1){
               QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
        QFile file(player->playlist->media(currentItem).canonicalResource().url().toString());
        QString path = player->playlist->media(currentItem).canonicalResource().url().toString();
        QStringList list = path.split("/");

        QFileInfo info(file);

       // sender.sendDatagram("Reproducir,"+ list.at(list.size()-2) + "/" + info.fileName());

        if(list.at(list.size()-2)!= "Musica"){
          //player->setMuted(true);
          player->player->setVolume(100);
          player->player->play();
          //sender.sendDatagram("Reproducir," + list.at(list.size()-2) + "/" + info.fileName());
          foreach (ToolButton *btn, buttons) {
              if(btn->getState()){
                  QString msg = "Reproducir," + list.at(list.size()-2) + "/" + info.fileName();
                  btn->sendMessage(msg);
              }
          }
        }else{
            player->player->setMuted(false);
            foreach (ToolButton *btn, buttons) {
                if(btn->getState()){
                    QString msg = "Reproducir," + list.at(list.size()-2) + "/" + info.fileName();
                    btn->sendMessage(msg);
                }
            }
            player->player->play();
        }

        QTime dieTime= QTime::currentTime().addMSecs(500);
        while (QTime::currentTime() < dieTime){
               QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
    }
   if(currentItem == -1){
        //sender.sendDatagram("Detener");
       this->remoteStop();
   }

}

void Xaman::test(int n)
{
    foreach (ToolButton *btn, buttons) {
        if (btn->getState()){
            btn->sendMessage(QString::number(n));
        }
    }
}
