/*
 * Created by Giuseppe Cioffi on 15/07/2025
 * Simple radar simulation with periodic beep sound using Qt6/C++
 */

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "soundworker.h"
#include <QPainterPath>
#include <QRandomGenerator>
#include <QThread>


/*** CONSTANTS ***/
constexpr float MARGIN = 20.f;
constexpr float SPEED_MULTIPLIER = 8.f;
constexpr float MAX_RADIUS = 90.0f;
constexpr float MIN_RADIUS = 10.0f;


/*** Constructor ***/
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){

    ui->setupUi(this);

    //Create a thread to handle sound playing
    soundThread = new QThread(this);
    SoundWorker* soundWorker = new SoundWorker;
    soundWorker->moveToThread(soundThread);
    //When thread is created, initalize soundWorker
    connect(soundThread, &QThread::started, soundWorker, &SoundWorker::init);
    //When thread has terminated, object is deleted
    connect(soundThread, &QThread::finished, soundWorker, &QObject::deleteLater);
    //When MainWindow requests bip, soundworker responds playing bip
    connect(this, &MainWindow::requestBip, soundWorker, &SoundWorker::playBeep);
    soundThread->start();


    //Bip repeats each 1.2 secs
    QTimer* soundTimer = new QTimer(this);
    connect(soundTimer, &QTimer::timeout, this, &MainWindow::requestBip);
    soundTimer->start(1200);


    //Generate 7 aircrafts with random positions within the circle of the radar
    for (int i = 0; i < 7; ++i) {
        float angle = QRandomGenerator::global()->generateDouble() * 2 * M_PI;
        float randRadius = std::sqrt(QRandomGenerator::global()->generateDouble());
        float radius = MIN_RADIUS + randRadius * ((MAX_RADIUS - MARGIN) - MIN_RADIUS);
        float x = radius * std::cos(angle);
        float y = radius * std::sin(angle);
        aircrafts.append(QPointF(x, y));
    }

    //Window update timer (about 60 fps)
    QTimer* timer = new QTimer(this);
    timer->start(16);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateWindow);

}

MainWindow::~MainWindow(){
    soundThread->quit();
    soundThread->wait();
    delete ui;
}

/*** Create a simulated radar ***/
void MainWindow::paintEvent(QPaintEvent* event){

    QPainter painter(this);
    this->drawRadarGrid(painter);
    this->drawRadarSweep(painter);
    this->drawAircraft(painter);

}

void MainWindow::updateWindow(){
    this->update();
}

void MainWindow::drawRadarGrid(QPainter &painter){

    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::black);
    fixSysCoordinates(painter);

    //Paint the lines of the perimeter of the radar
    //12 sections, each one with 10 bars = 120
    //Every 10 bar there's a long line
    QPen pen;
    pen.setWidthF(0.25);
    pen.setColor(Qt::green);
    painter.setPen(pen);

    for(int i = 0; i < 120; i++){
        float start_tick = 0.80f * MAX_RADIUS;
        float end_tick = 0.84f * MAX_RADIUS;
        float end_longtick = 0.87f * MAX_RADIUS;

        if ((i % 10) != 0)
            painter.drawLine(start_tick, 0, end_tick, 0);
        else
            painter.drawLine(start_tick, 0, end_longtick, 0);
        painter.rotate(3.0); //Since 360°/120 bars = 3°
    }

    //Draw radiant lines (one every 30°)
    for (int angle = 0; angle < 360; angle += 30) {
        float rad = qDegreesToRadians(float(angle));
        float x = (MAX_RADIUS - MARGIN) * std::cos(rad);
        float y = (MAX_RADIUS - MARGIN) * std::sin(rad);
        painter.drawLine(QPointF(0, 0), QPointF(x, y));
    }

    //Create 7 concentric circles
    for (int i = 1; i <= 7; ++i) {
        float r = (MAX_RADIUS - MARGIN) * i / 7;
        painter.drawEllipse(QPointF(0, 0), r, r);
    }

    //Restoring original coordinates to write text
    painter.restore();
    QFont font("Arial", 10, QFont::Bold);
    painter.setFont(font);
    painter.setPen(Qt::green);


    //Drawing north, south, east and
    float scaleFactor = qMin(width(), height()) / (MAX_RADIUS * 2.f);
    int pixel_radius = 0.93 * scaleFactor * MAX_RADIUS;
    int x_center = width()/2;
    int y_center = height()/2;
    painter.drawText(x_center - 5, y_center - pixel_radius, "N");
    painter.drawText(x_center + pixel_radius, y_center + 5, "E");
    painter.drawText(x_center - 5, y_center + pixel_radius, "S");
    painter.drawText(x_center - pixel_radius, y_center + 5, "W");

}

void MainWindow::drawRadarSweep(QPainter &painter){

    float rotation_angle = getRadarAngle();

    //Draw the radar sweep with conical gradient
    painter.save();
    fixSysCoordinates(painter);
    painter.rotate(rotation_angle);

    QConicalGradient sweepGradient(0, 0, 0);  // center (0,0), starting angle 0
    sweepGradient.setColorAt(0.0, QColor(0, 255, 0, 180));   // Center of the beam
    sweepGradient.setColorAt(0.2, QColor(0, 255, 0, 60));    // Outside of the beam
    sweepGradient.setColorAt(0.4, QColor(0, 255, 0, 0));     // Dissolving

    QBrush sweepBrush(sweepGradient);
    painter.setBrush(sweepBrush);
    painter.setPen(Qt::NoPen);

    // Draw a triangle as a radar beam
    QPainterPath sweepPath;
    sweepPath.moveTo(0, 0);
    sweepPath.lineTo(0.78*MAX_RADIUS, -5);  //width of the beam
    sweepPath.lineTo(0.78*MAX_RADIUS, 20);
    sweepPath.closeSubpath();

    painter.drawPath(sweepPath);
    painter.restore();


}

void MainWindow::drawAircraft(QPainter &painter){

    painter.save();
    fixSysCoordinates(painter);

    for(const QPointF& aircraft : std::as_const(aircrafts)){
        float radar_angle = getRadarAngle();
        float aircraft_angle = std::atan2(aircraft.y(), aircraft.x()) * 180.0 / M_PI;

        if(aircraft_angle < 0)
            aircraft_angle = 360 + aircraft_angle;

        float diff_angle = std::fabs(radar_angle - aircraft_angle);

        //Visibility angle of the radar
        if (diff_angle < 7.0 ) {
            QRadialGradient gradient(aircraft, 10);
            gradient.setColorAt(0, QColor(0, 255, 0, 200));  // deep green and opaque in the center
            gradient.setColorAt(1, QColor(0, 255, 0, 0));    // transparent on the outside
            painter.setBrush(gradient);
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(aircraft, 15, 15);

        }
    }

    painter.restore();
}

void MainWindow::fixSysCoordinates(QPainter &painter){

    const int x_center = width()/2;
    const int y_center = height()/2;
    const int side = qMin(width(), height());
    const float scaleFactor = side/(MAX_RADIUS *2.f);
    painter.translate(x_center, y_center);
    painter.scale(scaleFactor, -scaleFactor);

}


float MainWindow::getRadarAngle() const {

    QTime time = QTime::currentTime();
    float angle = fmod((time.second() + time.msec() / 1000.0) * 360.0 / 60.0 * SPEED_MULTIPLIER, 360.0f);
    return angle;
}

