/*
 * Created by Giuseppe Cioffi on 15/07/2025
 * Simple radar simulation with periodic beep sound using Qt6/C++
 */

#include "soundworker.h"
#include <QSoundEffect>
#include <QUrl>
#include <QDebug>

SoundWorker::SoundWorker(QObject *parent)
    : QObject{parent}
{
    // No QSoundEffect created here yet
}

void SoundWorker::init(){
    beepSound = new QSoundEffect(this);
    beepSound->setSource(QUrl("qrc:/bip.wav"));
    beepSound->setLoopCount(1);
    beepSound->setVolume(0.8);
}
void SoundWorker::playBeep(){

    if(beepSound->isLoaded())
        beepSound->play();
}
