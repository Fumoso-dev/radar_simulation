#ifndef SOUNDWORKER_H
#define SOUNDWORKER_H

#include <QObject>
#include <QSoundEffect>

class SoundWorker : public QObject
{
    Q_OBJECT
public:
    explicit SoundWorker(QObject *parent = nullptr);
    void init();

signals:

public slots:
    void playBeep();

private:
    QSoundEffect* beepSound;
};

#endif // SOUNDWORKER_H
