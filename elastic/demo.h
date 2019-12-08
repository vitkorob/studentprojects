#ifndef DEMO_H
#define DEMO_H

#include <QtWidgets>
#include <math.h>
#include <stdlib.h>

class Demo : public QDialog
{
Q_OBJECT
public:
    explicit Demo(QWidget *parent = 0);
    bool initDemo(float *masR, float *masFI, float *masT, float numPoint, float maxTime);

private:
    QPushButton *pushButton1;
    QLabel *label;
    QSlider *slider;
    QWidget *widget;
    QTimer *timer;

    bool start;
    int faze;
    int maxFaze;
    int lengh_pendulum;

    float maxX;
    float maxY;

    float *masXaxis;
    float *masYaxis;

    void drawJmper(QPainter *painter, int x1, int y1, int x2, int y2, int length, int wight);
    void setMinMaxValue(float *massOfPoints, unsigned long numberOfPoints, float *minValue, float *maxValue, int scalePercent = 100);
    void setupUi(QDialog *parent);

    void paintEvent(QPaintEvent *);

signals:

private slots:
    void clickDemo();
    void dropSlider(int value);
    void timerShot();
    void nextFrame();
    void backFrame();

};

#endif // DEMO_H
