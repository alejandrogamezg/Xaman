/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "player.h"

#include "playercontrols.h"
#include "playlistmodel.h"
#include <QMediaService>
#include <QMediaPlaylist>
#include <QVideoProbe>
#include <QMediaMetaData>
#include <QtWidgets>
#include <QIcon>

Player::Player(QWidget *parent)
    : QWidget(parent)
    , videoWidget(0)
    , coverLabel(0)
    , slider(0)
    /*
#ifndef PLAYER_NO_COLOROPTIONS
    , colorDialog(0)
#endif*/
{
//! [create-objs]
    player = new QMediaPlayer(this);
    player->setMuted(true);
    playlist = new QMediaPlaylist();
    player->setPlaylist(playlist);
//! [create-objs]

    connect(player, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    connect(player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(player, SIGNAL(metaDataChanged()), SLOT(metaDataChanged()));
    connect(playlist, SIGNAL(currentIndexChanged(int)), SLOT(playlistPositionChanged(int)));
    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this, SLOT(statusChanged(QMediaPlayer::MediaStatus)));
    connect(player, SIGNAL(bufferStatusChanged(int)), this, SLOT(bufferingProgress(int)));
    connect(player, SIGNAL(videoAvailableChanged(bool)), this, SLOT(videoAvailableChanged(bool)));
    connect(player, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(displayErrorMessage()));

//! [2]
    videoWidget = new VideoWidget(this);
    player->setVideoOutput(videoWidget);

    playlistModel = new PlaylistModel(this);
    playlistModel->setPlaylist(playlist);
//! [2]

    playlistView = new QListView(this);
    playlistView->setModel(playlistModel);
    playlistView->setCurrentIndex(playlistModel->index(playlist->currentIndex(), 0));

    connect(playlistView, SIGNAL(activated(QModelIndex)), this, SLOT(jump(QModelIndex)));

    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, player->duration() / 1000);

    playlistView->setStyleSheet("QListView { \
                                    background-color: transparent;\
                                    border: 0px;\
                                    border-bottom: 0px solid black;\
                                    border-top: 0px solid black;\
                                    margin: 0px;\
                                 }\
                                 QListView::item {                              \
                                 border-bottom: 1px solid  #686868; height: 30px }  \
                                 QListView::item:selected {background: transparent; border: 1px solid #ffffff;\
                                 border-left: 0px; border-right: 0px; color: #d8ff00;\
                                }\
                                 QListView::item:hover {\
                                      background: transparent;\
                                      border: 1px solid #ffffff;\
                                      border-left: 0px solid #25282A;\
                                      border-right: 0px solid #25282A;\
                                      color: #d8ff00; }         \
                                      QTreeView::item:selected:active{ \
                                         background: #d8ff00;   \
                                         border: 1px solid #ffffff;  \
                                         border-left: 0px           \
                                         border-right: 0px      \
                                         color: #d8ff00;   \
                                      } \
                                         QListView::item:alternate { \
                                             background: #EEEEEE; \
                                         }\
                                         QListView::item:selected:!active {\
                                             background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
                                                                         stop: 0 #ABAFE5, stop: 1 #8588B2);\
                                         }"


                               );
    playlistView->setFocusPolicy(Qt::NoFocus);

     slider->setStyleSheet("QSlider{ background: rgba(0,0,0,0%);  border: transparent;}"
                          "QSlider::groove:horizontal {"

                          "border: 1px solid #bbb;"
                          "background: white;"
                          "height: 4px;"
                          "border-radius: 2px;"
                          "}"

                          "QSlider::sub-page:horizontal {"
                          "background: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,"
                              "stop: 0 #66e, stop: 1 #bbf);"
                          "background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,"
                              "stop: 0 #000000, stop: 1 #000000);"
                          "border: 1px solid #000000;"
                          "height: 7px;"
                          "border-radius: 2px;"
                          "}"

                          "QSlider::add-page:horizontal {"
                          "background: #fff;"
                          "border: 1px solid #777;"
                          "height: 37px;"
                          "border-radius: 2px;"
                          "}"

                          "QSlider::handle:horizontal {"
                          "background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
                              "stop:0 #eee, stop:1 #ccc);"
                          "border: 1px solid #777;"
                          "width: 13px;"
                          "margin-top: -2px;"
                          "margin-bottom: -2px;"
                          "border-radius: 2px;"
                          "}"

                          "QSlider::handle:horizontal:hover {"
                          "background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
                              "stop:0 #fff, stop:1 #ddd);"
                          "border: 1px solid #444;"
                          "border-radius: 2px;"
                          "}"

                          "QSlider::sub-page:horizontal:disabled {"
                          "background: #000000;"
                          "border-color: #999;"
                          "}"

                          "QSlider::add-page:horizontal:disabled {"
                          "background: #eee;"
                          "border-color: #999;"
                          "}"

                          "QSlider::handle:horizontal:disabled {"
                          "background: #eee;"
                          "border: 1px solid #aaa;"
                          "border-radius: 2px;"
                          "}"
                          );


    labelDuration = new QLabel(this);
    connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(seek(int)));

    QToolButton *limpiar = new QToolButton(this);
    limpiar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    limpiar->setText("Eliminar");
    connect(limpiar, SIGNAL(clicked()), this, SLOT(clearlist()));

    QToolButton *Eliminaruno = new QToolButton(this);
    Eliminaruno->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    Eliminaruno->setText("Eliminar Uno");
    connect(Eliminaruno, SIGNAL(clicked()), this, SLOT(Eliminaruno()));


    limpiar->setIcon(QIcon(":/images/Iconos/eliminar_icon.png"));
    limpiar->setIconSize(QSize(25,25));
    limpiar->setStyleSheet("QToolButton{ background: transparent;  border: transparent;}");
    Eliminaruno->setIcon(QIcon(":/images/Iconos/eliminar_icon.png"));
    Eliminaruno->setIconSize(QSize(25,25));
    Eliminaruno->setStyleSheet("QToolButton{ background: transparent;  border: transparent;}");




    //QPushButton *openButton = new QPushButton(tr("Open"), this);
    //connect(openButton, SIGNAL(clicked()), this, SLOT(open()));

    controls = new PlayerControls(this);
    controls->setState(player->state());
    controls->setVolume(player->volume());
    controls->setMuted(controls->isMuted());


    //connect(controls, SIGNAL(play()), this, SLOT(playSender()));
    //connect(controls, SIGNAL(pause()), player, SLOT(pause()));
    //connect(controls, SIGNAL(stop()), this, SLOT(stopSender()));
    //connect(controls, SIGNAL(next()), playlist, SLOT(next()));
    //connect(controls, SIGNAL(previous()), this, SLOT(previousClicked()));
    connect(controls, SIGNAL(changeVolume(int)), player, SLOT(setVolume(int)));
    connect(controls, SIGNAL(changeMuting(bool)), player, SLOT(setMuted(bool)));

    connect(controls, SIGNAL(stop()), videoWidget, SLOT(update()));

    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)),
            controls, SLOT(setState(QMediaPlayer::State)));
    connect(player, SIGNAL(volumeChanged(int)), controls, SLOT(setVolume(int)));
    connect(player, SIGNAL(mutedChanged(bool)), controls, SLOT(setMuted(bool)));

    fullScreenButton = new QPushButton(this);

    fullScreenButton->setIcon(QIcon(":/images/Iconos/bigpantalla_icon.png"));
    fullScreenButton->setIconSize(QSize(30,30));
    fullScreenButton->setStyleSheet("QPushButton{ background: rgba(0,0,0,0%);  border: transparent;}");
    fullScreenButton->setCheckable(false);

    QBoxLayout *displayLayout = new QHBoxLayout;
    displayLayout->addWidget(videoWidget);
    videoWidget->setMaximumWidth(495);

    videoWidget->setStyleSheet("VideoWidget{ background: black; }");

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);

    //controlLayout->
    QToolButton *Title = new QToolButton(this);
    //Title->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    Title->setText("Reproduciendo");
    Title->setStyleSheet("QToolButton{background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #505050, stop: 0.2 #404040, stop: 0.4 #303030 , stop: 0.6 #202020 , stop: 0.8 #101010, stop: 1 #080808 );border: transparent;height: 40px;text-align: center;  width: 295px;}");
    Title->setEnabled(false);


    controlLayout->addWidget(controls);
    //controlLayout->addStretch(1);
    controlLayout->setSpacing(130);

    controlLayout->addWidget(fullScreenButton);
    controlLayout->addStretch(1);

    QBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(displayLayout);
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(slider);
    hLayout->addWidget(labelDuration);
    //slider->setMaximumWidth(200);
    labelDuration->setMaximumHeight(9);

    QBoxLayout *LayoutH = new QHBoxLayout;
    QBoxLayout *LayoutVericalList = new QVBoxLayout;
    QBoxLayout *H1 = new QHBoxLayout;
    LayoutVericalList->addWidget(Title,0);
    LayoutVericalList->addWidget(playlistView,0);
    H1->addWidget(Eliminaruno,0);
    H1->addWidget(limpiar,0,Qt::AlignRight);

    layout->addLayout(hLayout);
    layout->addLayout(controlLayout);
    LayoutH->addLayout(layout);
    LayoutH->addLayout(LayoutVericalList);
    LayoutVericalList->addLayout(H1);

    hLayout->setMargin(0);
    controlLayout->setMargin(0);

    displayLayout->setMargin(0);
    LayoutVericalList->setMargin(0);
    layout->setMargin(0);
    LayoutH->setMargin(0);
    LayoutH->setSpacing(0);


    playlistView->setMaximumWidth(300);
    playlistView->setMaximumHeight(253);

    setLayout(LayoutH);

    if (!player->isAvailable()) {
        QMessageBox::warning(this, tr("Service not available"),
                             tr("The QMediaPlayer object does not have a valid service.\n"\
                                "Please check the media service plugins are installed."));

        controls->setEnabled(false);
        playlistView->setEnabled(false);
        //openButton->setEnabled(false);

        fullScreenButton->setEnabled(false);
    }

    metaDataChanged();

    QStringList arguments = qApp->arguments();
    arguments.removeAt(0);
    addToPlaylist(arguments);
}

Player::~Player()
{
}
void Player::clearlist(){
    playlist->clear();
}

void Player::playSender()
{
    playlist->setCurrentIndex( playlistView->currentIndex().row());
    if(!player->playlist()->isEmpty()){
        QString path = player->playlist()->media(0).canonicalResource().url().toString();
        QStringList list = path.split("/");
        //qDebug()<< player->playlist()->currentIndex();
        if(player->playlist()->currentIndex() == -1){
            QFile file(player->playlist()->media(0).canonicalResource().url().toString());

            QFileInfo info(file);

            if(list.at(list.size()-2)!= "Musica"){
              //player->setMuted(true);
              player->setVolume(100);
              player->play();
              emit playRemote(list.at(list.size()-2) + "/" + info.fileName());
              //sender.sendDatagram("Reproducir," + list.at(list.size()-2) + "/" + info.fileName());
            }else{
                //sender.sendDatagram("Detener");
                player->setMuted(false);
                player->play();
            }
        } else {

            qDebug()<<  playlistView->currentIndex().row();

            QFile file(player->currentMedia().canonicalResource().url().toString());
            QFileInfo info(file);

            if(info.fileName() == "RollerCoaster_5.mp4"){

                controls->stopButton->setEnabled(false);
                sender.sendDatagram("Reproducir," + list.at(list.size()-2) + "/" + info.fileName());
                QTime dieTimer= QTime::currentTime().addSecs(5);
                while (QTime::currentTime() < dieTimer){
                       QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
                }
                player->play();
                controls->stopButton->setDisabled(true);
                QTime dieTimer1= QTime::currentTime().addSecs(25);
                while (QTime::currentTime() < dieTimer1){
                       QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
                }
                controls->stopButton->setEnabled(true);

            }else if(info.fileName() == "RollerCoaster_4.mp4"){
                controls->stopButton->setEnabled(false);
                sender.sendDatagram("Reproducir," + list.at(list.size()-2) + "/" + info.fileName());
                QTime dieTimer= QTime::currentTime().addSecs(6);
                while (QTime::currentTime() < dieTimer){
                       QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
                }

                player->play();
                controls->stopButton->setDisabled(true);
                QTime dieTimer1= QTime::currentTime().addSecs(25);
                while (QTime::currentTime() < dieTimer1){
                       QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
                }
                controls->stopButton->setEnabled(true);

            }else if(info.fileName()== "RollerCoaster_3.mp4"){
                controls->stopButton->setEnabled(false);
                sender.sendDatagram("Reproducir," + list.at(list.size()-2) + "/" + info.fileName());
                QTime dieTimer= QTime::currentTime().addSecs(5);
                while (QTime::currentTime() < dieTimer){
                           QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
                }
                player->play();
                controls->stopButton->setDisabled(true);
                QTime dieTimer1= QTime::currentTime().addSecs(25);
                while (QTime::currentTime() < dieTimer1){
                       QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
                }
                controls->stopButton->setEnabled(true);

            }else if(info.fileName() == "RollerCoaster_2.mp4"){
                controls->stopButton->setEnabled(false);

                sender.sendDatagram("Reproducir," + list.at(list.size()-2) + "/" + info.fileName());
                QTime dieTimer= QTime::currentTime().addSecs(9);
                while (QTime::currentTime() < dieTimer){
                           QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
                }
                player->play();
                controls->stopButton->setDisabled(true);
                QTime dieTimer1= QTime::currentTime().addSecs(25);
                while (QTime::currentTime() < dieTimer1){
                       QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
                }
                controls->stopButton->setEnabled(true);
            }else if(info.fileName() == "RollerCoaster_1.mp4"){
                 controls->stopButton->setEnabled(false);

                sender.sendDatagram("Reproducir," + list.at(list.size()-2) + "/" + info.fileName());
                QTime dieTimer= QTime::currentTime().addSecs(5);
                while (QTime::currentTime() < dieTimer){
                           QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
                }
                player->play();
                controls->stopButton->setDisabled(true);
                QTime dieTimer1= QTime::currentTime().addSecs(25);
                while (QTime::currentTime() < dieTimer1){
                       QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
                }
                controls->stopButton->setEnabled(true);

            }else{
                if(list.at(list.size()-2)!= "Musica"){
                    //player->setMuted(true);
                    player->setVolume(100);
                    player->play();
                    emit playRemote(list.at(list.size()-2) + "/" + info.fileName());
                    //sender.sendDatagram("Reproducir," + list.at(list.size()-2) + "/" + info.fileName());
                }else{
                    player->setMuted(false);
                    player->play();
                }
            }
        }
    }
}

void Player::stopSender()
{
    //sender.sendDatagram("Detener");
    player->stop();
    emit stopRemote();

}

void Player::addToPlaylist(const QStringList& fileNames)
{
    foreach (QString const &argument, fileNames) {
        QFileInfo fileInfo(argument);
        if (fileInfo.exists()) {
            QUrl url = QUrl::fromLocalFile(fileInfo.absoluteFilePath());
            if (fileInfo.suffix().toLower() == QLatin1String("m3u")) {
                playlist->load(url);
            } else
                playlist->addMedia(url);
        } else {
            QUrl url(argument);
            if (url.isValid()) {
                playlist->addMedia(url);
            }
        }
    }
}

void Player::durationChanged(qint64 duration)
{
    this->duration = duration/1000;
    slider->setMaximum(duration / 1000);
    //sender.sendDatagram("Detener");


}

void Player::positionChanged(qint64 progress)
{
    if (!slider->isSliderDown()) {
        slider->setValue(progress / 1000);

    }
    updateDurationInfo(progress / 1000);
     //qDebug()<< "is over";
}

void Player::metaDataChanged()
{
    if (player->isMetaDataAvailable()) {
        setTrackInfo(QString("%1 - %2")
                .arg(player->metaData(QMediaMetaData::AlbumArtist).toString())
                .arg(player->metaData(QMediaMetaData::Title).toString()));

        if (coverLabel) {
            QUrl url = player->metaData(QMediaMetaData::CoverArtUrlLarge).value<QUrl>();

            coverLabel->setPixmap(!url.isEmpty()
                    ? QPixmap(url.toString())
                    : QPixmap());
        }
    }
}

void Player::previousClicked()
{
    if(player->position() <= 5000)
        playlist->previous();
    else
        player->setPosition(0);
}

void Player::jump(const QModelIndex &index)
{
    if (index.isValid()) {
        playlist->setCurrentIndex(index.row());
        player->play();
        //qDebug()<< "next";
    }
}

void Player::playlistPositionChanged(int currentItem)
{
     qDebug()<< currentItem<<"index";
   if(currentItem != 0 && currentItem != -1){

       //---modificacion
        //sender.sendDatagram("Detener");
        emit stopRemote();
        QTime dieTime1= QTime::currentTime().addSecs(5);
        while (QTime::currentTime() < dieTime1){
               QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
        //--------------------

        QFile file(playlist->media(currentItem).canonicalResource().url().toString());
        QString path = playlist->media(currentItem).canonicalResource().url().toString();
        QStringList list = path.split("/");

        QFileInfo info(file);

       // sender.sendDatagram("Reproducir,"+ list.at(list.size()-2) + "/" + info.fileName());

        if(list.at(list.size()-2)!= "Musica"){
          //player->setMuted(true);
          player->setVolume(100);
          player->play();
          //sender.sendDatagram("Reproducir," + list.at(list.size()-2) + "/" + info.fileName());
          emit playRemote(list.at(list.size()-2) + "/" + info.fileName());
        }else{
            player->setMuted(false);
            player->play();
        }

        QTime dieTime= QTime::currentTime().addMSecs(500);
        while (QTime::currentTime() < dieTime){
               QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
    }
   if(currentItem == -1){
        //sender.sendDatagram("Detener");
       emit stopRemote();
   }
    playlistView->setCurrentIndex(playlistModel->index(currentItem, 0));

}

void Player::seek(int seconds)
{
    player->setPosition(seconds * 1000);

}

void Player::statusChanged(QMediaPlayer::MediaStatus status)
{
    handleCursor(status);

    // handle status message
    switch (status) {
    case QMediaPlayer::UnknownMediaStatus:
    case QMediaPlayer::NoMedia:
    case QMediaPlayer::LoadedMedia:
    case QMediaPlayer::BufferingMedia:
        setStatusInfo(tr("Cargando Media..."));
        break;
    case QMediaPlayer::BufferedMedia:
        setStatusInfo(QString());
        break;
    case QMediaPlayer::LoadingMedia:
        setStatusInfo(tr("Loading..."));
        break;
    case QMediaPlayer::StalledMedia:
        setStatusInfo(tr("Media Stalled"));
        break;
    case QMediaPlayer::EndOfMedia:
        QApplication::alert(this);
        break;
    case QMediaPlayer::InvalidMedia:
        displayErrorMessage();
        break;
    }
}

void Player::handleCursor(QMediaPlayer::MediaStatus status)
{
#ifndef QT_NO_CURSOR
    if (status == QMediaPlayer::LoadingMedia ||
        status == QMediaPlayer::BufferingMedia ||
        status == QMediaPlayer::StalledMedia)
        setCursor(QCursor(Qt::BusyCursor));
    else
        unsetCursor();
#endif
}

void Player::bufferingProgress(int progress)
{
    setStatusInfo(tr("Buffering %4%").arg(progress));
}

void Player::videoAvailableChanged(bool available)
{
    if (!available) {
        disconnect(fullScreenButton, SIGNAL(clicked(bool)),
                    videoWidget, SLOT(setFullScreen(bool)));
        disconnect(videoWidget, SIGNAL(fullScreenChanged(bool)),
                fullScreenButton, SLOT(setChecked(bool)));
        videoWidget->setFullScreen(false);
    } else {
        connect(fullScreenButton, SIGNAL(clicked(bool)),
                videoWidget, SLOT(setFullScreen(bool)));
        connect(videoWidget, SIGNAL(fullScreenChanged(bool)),
                fullScreenButton, SLOT(setChecked(bool)));

        if (fullScreenButton->isChecked())
            videoWidget->setFullScreen(true);
    }
}

void Player::Eliminaruno()
{
    if(!playlist->isEmpty()){
        playlist->removeMedia(playlistView->currentIndex().row());
    }
}

void Player::setTrackInfo(const QString &info)
{
    trackInfo = info;
    if (!statusInfo.isEmpty())
        setWindowTitle(QString("%1 | %2").arg(trackInfo).arg(statusInfo));
    else
        setWindowTitle(trackInfo);
}

void Player::setStatusInfo(const QString &info)
{
    statusInfo = info;
    if (!statusInfo.isEmpty())
        setWindowTitle(QString("%1 | %2").arg(trackInfo).arg(statusInfo));
    else
        setWindowTitle(trackInfo);
}

void Player::displayErrorMessage()
{
    setStatusInfo(player->errorString());
}

void Player::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;

    if (currentInfo || duration) {
        QTime currentTime((currentInfo/3600)%60, (currentInfo/60)%60, currentInfo%60, (currentInfo*1000)%1000);
        QTime totalTime((duration/3600)%60, (duration/60)%60, duration%60, (duration*1000)%1000);
        QString format = "mm:ss";

        if (duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);

    }
    labelDuration->setText(tStr);

}
