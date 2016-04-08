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

#include "playercontrols.h"
#include "windows.h"

#include <QBoxLayout>
#include <QSlider>
#include <QStyle>
#include <QToolButton>
#include <QComboBox>

PlayerControls::PlayerControls(QWidget *parent)
    : QWidget(parent)
    , playerState(QMediaPlayer::StoppedState)
    , playerMuted(false)
    , playButton(0)
    , stopButton(0)
    , nextButton(0)
    , previousButton(0)
    , muteButton(0)
    , volumeSlider(0)
    //, rateBox(0)
{
    playButton = new QToolButton(this);
    playButton->setIcon(QIcon(":/images/Iconos/play_icon.png"));
    playButton->setIconSize(QSize(30,40));

    playButton->setStyleSheet("QToolButton{ background: rgba(0,0,0,0%);  border: transparent;}");


    connect(playButton, SIGNAL(clicked()), this, SLOT(playClicked()));

    stopButton = new QToolButton(this);
    //stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    stopButton->setIcon(QIcon(":/images/Iconos/stop_icon.png"));
    stopButton->setIconSize(QSize(30,30));
    stopButton->setStyleSheet("QToolButton{ background: rgba(0,0,0,0%);  border: transparent;}");
    stopButton->setEnabled(false);

    connect(stopButton, SIGNAL(clicked()), this, SIGNAL(stop()));

    nextButton = new QToolButton(this);
    //nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    nextButton->setIcon(QIcon(":/images/Iconos/forward_icon.png"));
    nextButton->setIconSize(QSize(30,30));
    nextButton->setStyleSheet("QToolButton{ background: rgba(0,0,0,0%);  border: transparent;}");


    connect(nextButton, SIGNAL(clicked()), this, SIGNAL(next()));

    previousButton = new QToolButton(this);
    previousButton->setIcon(QIcon(":/images/Iconos/rewind_icon.png"));
    previousButton->setIconSize(QSize(30,30));
    previousButton->setStyleSheet("QToolButton{ background: rgba(0,0,0,0%);  border: transparent;}");

    connect(previousButton, SIGNAL(clicked()), this, SIGNAL(previous()));

    muteButton = new QToolButton(this);
   // muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    muteButton->setIcon(QIcon(":/images/Iconos/sound_icon.png"));
    muteButton->setStyleSheet("QToolButton{ background: rgba(0,0,0,0%);  border: transparent;}");
    muteButton->setIconSize(QSize(30,30));

    /*SoundButton= new QToolButton(this);
    SoundButton->setIcon(QIcon(":/images/Iconos/sound_icon.png"));
    SoundButton->setStyleSheet("QToolButton{ background: rgba(0,0,0,0%);  border: transparent;}");
    SoundButton->setIconSize(QSize(30,30));*/

    //muteButton->setIcon(QIcon(":/images/Iconos/sound_icon.png"));

    connect(muteButton, SIGNAL(clicked()), this, SLOT(muteClicked()));
    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->blockSignals(false);//bloqueo de slider
    volumeSlider->setRange(0, 100);
    volumeSlider->setStyleSheet("QSlider{ background: rgba(0,0,0,0%);  border: transparent;}"
                                "QSlider::groove:horizontal {"
                                "border: 1px solid #d0eb46;"
                                "background: black;"
                                "height: 7px;"
                                "border-radius: 4px;"
                                "}"

                                "QSlider::sub-page:horizontal {"
                                "background: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,"
                                    "stop: 0 #25282a, stop: 1 #25282a);"
                                "background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,"
                                    "stop: 0 #808080, stop: 1 	#808080);"
                                "border: 1px solid #d0eb46;"
                                "height: 10px;"
                                "border-radius: 4px;"
                                "}"

                                "QSlider::add-page:horizontal {"
                                "background: #000000;"
                                "border: 1px solid #777;"
                                "height: 10px;"
                                "border-radius: 4px;"
                                "}"

                                "QSlider::handle:horizontal {"
                                "background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
                                    "stop:0 #eee, stop:1 #ccc);"
                                "border: 1px solid #777;"
                                "width: 16px;"
                                "margin-top: -2px;"
                                "margin-bottom: -2px;"
                                "border-radius: 4px;"
                                "}"

                                "QSlider::handle:horizontal:hover {"
                                "background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
                                    "stop:0 #fff, stop:1 #ddd);"
                                "border: 1px solid #444;"
                                "border-radius: 4px;"
                                "}"

                                "QSlider::sub-page:horizontal:disabled {"
                                "background: #000000;"
                                "border-color: #999;"
                                "}"

                                "QSlider::add-page:horizontal:disabled {"
                                "background: #000000;"
                                "border-color: #999;"
                                "}"

                                "QSlider::handle:horizontal:disabled {"
                                "background: #eee;"
                                "border: 1px solid #aaa;"
                                "border-radius: 4px;"
                                "}"
                                );
   connect(volumeSlider, SIGNAL(sliderMoved(int)), this, SIGNAL(changeVolume(int)));



muteClicked(); //silence sound
    QBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->addWidget(muteButton);
    layout->addWidget(volumeSlider);
    //layout->addWidget(SoundButton);
    layout->addSpacing(30);

    layout->addWidget(stopButton);
    layout->addWidget(previousButton);
    layout->addWidget(playButton);
    layout->addWidget(nextButton);
   // layout->addWidget(rateBox);
    setLayout(layout);
}

QMediaPlayer::State PlayerControls::state() const
{
    return playerState;
}

void PlayerControls::setState(QMediaPlayer::State state)
{
    if (state != playerState) {
        playerState = state;

        switch (state) {
        case QMediaPlayer::StoppedState:
            stopButton->setEnabled(false);
            playButton->setEnabled(true);

            playButton->setIcon(QIcon(":/images/Iconos/play_icon.png"));
            playButton->setIconSize(QSize(35,40));
            playButton->setStyleSheet("QToolButton{ background: rgba(0,0,0,0%);  border: transparent;}");

            break;
        case QMediaPlayer::PlayingState:
            stopButton->setEnabled(true);
            playButton->setEnabled(false);
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));


            break;
        case QMediaPlayer::PausedState:
            stopButton->setEnabled(true);
            playButton->setEnabled(true);
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        }
    }
}

int PlayerControls::volume() const
{
    return volumeSlider ? volumeSlider->value() : 0;
}

void PlayerControls::setVolume(int volume)
{

    if (volumeSlider)
        volumeSlider->setValue(volume);

        if(volumeante>=volume){
           qDebug()<<"volumante="<<volumeante<<"baja"<<volume;

            INPUT ip={0};
            ip.type = INPUT_KEYBOARD;
            ip.ki.wVk = VK_VOLUME_DOWN;
            SendInput(1, &ip, sizeof(INPUT));
            ip.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &ip, sizeof(INPUT));

        }else if(volumeante<=volume){

            INPUT ip2={0};
            ip2.type = INPUT_KEYBOARD;
            ip2.ki.wVk = VK_VOLUME_UP;
            SendInput(1, &ip2, sizeof(INPUT));
            ip2.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &ip2, sizeof(INPUT));

            qDebug()<<"volumante="<<volumeante<<"sube"<<volume;
        }
       volumeante=volume;
}

bool PlayerControls::isMuted() const
{
    return playerMuted;
}

void PlayerControls::setMuted(bool muted)
{
    if (muted != playerMuted) {
        playerMuted = muted;

        if(muted){
           // muteButton->setIcon(QIcon(":/images/Iconos/silence_icon.png"));
            /*
            INPUT mute={0};
            mute.type = INPUT_KEYBOARD;
            mute.ki.wVk = VK_VOLUME_MUTE;
            SendInput(1, &mute, sizeof(INPUT));
            mute.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &mute, sizeof(INPUT));
            */
       }else{
            muteButton->setIcon(QIcon(":/images/Iconos/sound_icon.png"));
            /*
            INPUT muteup={0};
            muteup.type = INPUT_KEYBOARD;
            muteup.ki.wVk = VK_VOLUME_UP;
            SendInput(1, &muteup, sizeof(INPUT));
            muteup.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &muteup, sizeof(INPUT));
            */
        }
    }
}

void PlayerControls::playClicked()
{
    switch (playerState) {
    case QMediaPlayer::StoppedState:
        emit stop();
    case QMediaPlayer::PausedState:
        emit play();
        break;
    case QMediaPlayer::PlayingState:
        emit pause();
        break;
    }
}

void PlayerControls::muteClicked()
{
    emit changeMuting(!playerMuted);
}
