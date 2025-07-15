#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QTimer>
#include <QPainter>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    virtual void paintEvent(QPaintEvent* event);

signals:
    void requestBip();

public slots:
    void updateWindow();


private:
    Ui::MainWindow *ui;
    QThread* soundThread;
    QVector<QPointF> aircrafts;
    void drawRadarGrid(QPainter& painter);
    void drawRadarSweep(QPainter& painter);
    void drawAircraft(QPainter& painter);
    void fixSysCoordinates(QPainter& painter);
    float getRadarAngle() const;


};
#endif // MAINWINDOW_H
