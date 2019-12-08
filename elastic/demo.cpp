#define TIME_FRAME 0.025
#define CIRCLE 30

#include "demo.h"

Demo::Demo(QWidget *parent) : QDialog(parent)
{
    setupUi(this);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerShot()));

    start = false;
    maxFaze = 0;
    faze = 0;
}

bool Demo::initDemo(float *masR, float *masFI, float *masT, float numPoint, float maxTime)
{
    long q = 0;
    float dt = 0;

    maxFaze = maxTime / TIME_FRAME;

    masXaxis = new float[maxFaze];
    masYaxis = new float[maxFaze];

    for(int i = 0; i < maxFaze; i++)
    {
        float timeOfFaze = i * TIME_FRAME;

        while(q < numPoint)
        {
            float tmp_dt = fabs(*(masT + q) - timeOfFaze);

            if(tmp_dt > dt)
            {
                dt = TIME_FRAME;
                break;
            }

            dt = tmp_dt;
            q++;
        }

        *(masXaxis + i) = *(masR + q) * cos((*(masFI + q) - 1.570796327));
        *(masYaxis + i) = *(masR + q) * sin((*(masFI + q) - 1.570796327)) * (-1);
    }

    float minX;
    float minY;

    setMinMaxValue(masXaxis, maxFaze, &minX, &maxX, 105);
    setMinMaxValue(masYaxis, maxFaze, &minY, &maxY, 105);

    maxX = fabs(maxX) > fabs(minX) ? fabs(maxX) : fabs(minX);
    maxY = fabs(maxY) > fabs(minY) ? fabs(maxY) : fabs(minY);

    slider->setRange(0, maxFaze - 1);
    faze = 0;

    return true;
}

void Demo::setMinMaxValue(float *massOfPoints, unsigned long numberOfPoints, float *minValue, float *maxValue, int scalePercent)
{
    unsigned long cou;

    *minValue = *massOfPoints;
    *maxValue = *massOfPoints;

    for(cou = 1; cou < numberOfPoints; cou++)
    {
        if(*(massOfPoints + cou) < *minValue) *minValue = *(massOfPoints + cou);
        if(*(massOfPoints + cou) > *maxValue) *maxValue = *(massOfPoints + cou);
    }

    if(*minValue == *maxValue)
    {
        *minValue -= 1;
        *maxValue += 1;
    }

    if(scalePercent -= 100)
    {
        *minValue -= (*maxValue - *minValue) * scalePercent / 100.0;
        *maxValue += (*maxValue - *minValue) * scalePercent / 100.0;
    }
}

void Demo::setupUi(QDialog *parent)
{
    parent->setWindowTitle("Demonstration");
    parent->setMinimumSize(400, 300);
    parent->resize(QSize(600, 450));
    parent->setModal(true);

    QGridLayout *layout = new QGridLayout(parent);

    widget = new QWidget();
    layout->addWidget(widget, 0, 0, 1, 5);

    pushButton1 = new QPushButton("Start");
    pushButton1->setDefault(true);
    layout->addWidget(pushButton1, 1, 0);
    connect(pushButton1, SIGNAL(clicked()), this, SLOT(clickDemo()));

    QPushButton *pushButton2 = new QPushButton("<-");
    layout->addWidget(pushButton2, 1, 1);
    connect(pushButton2, SIGNAL(clicked()), this, SLOT(backFrame()));

    QPushButton *pushButton3 = new QPushButton("->");
    layout->addWidget(pushButton3, 1, 2);
    connect(pushButton3, SIGNAL(clicked()), this, SLOT(nextFrame()));

    slider = new QSlider(Qt::Horizontal);
    layout->addWidget(slider, 1, 3);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(dropSlider(int)));

    label = new QLabel("000.000");
    layout->addWidget(label, 1, 4);
}

void Demo::clickDemo()
{
    start = !start;

    if(start)
    {
        pushButton1->setText("Pause");
        timer->start(1000 * TIME_FRAME);
    }
    else
    {
        pushButton1->setText("Start");
        timer->stop();
    }
}

void Demo::dropSlider(int value)
{
    faze = value;
    update();
}

void Demo::paintEvent(QPaintEvent *)
{
    QPainter canvas(this);
    //canvas.setRenderHint(QPainter::Antialiasing);

    lengh_pendulum = (widget->width()>>1)/maxX < (widget->height()>>1)/maxY ?
                     (widget->width()>>1)/maxX : (widget->height()>>1)/maxY;

    drawJmper(&canvas, widget->width()>>1, widget->height()>>1,
              (widget->width()>>1) + *(masXaxis + faze) * lengh_pendulum,
              (widget->height()>>1) + *(masYaxis + faze) * lengh_pendulum, lengh_pendulum, 20);

    char table[8];
    sprintf(table, "%07.3f", faze * TIME_FRAME);

    label->setText(table);
}

void Demo::drawJmper(QPainter *painter, int x1, int y1, int x2, int y2, int length, int wight)
{
    float s = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
    float dx = (x2 - x1) / s;
    float dy = (y2 - y1) / s;

    s = sqrt(pow((x2 - x1 - wight * dx), 2) + pow((y2 - y1 - wight * dy), 2));
    dx = (x2 - x1 - wight * dx) / s;
    dy = (y2 - y1 - wight * dy) / s;

    float step_dx = s / (4 * CIRCLE);
    float step_dy = length * wight / (s + length);

    int dop_x1, dop_y1, dop_x2, dop_y2, weight_ind1, weight_ind2;

    for(int i = 0; i < 4 * CIRCLE; i++)
    {
        weight_ind1 = i % 4;

        if(weight_ind1 == 2) weight_ind1 = 0;
        else if(weight_ind1 == 3) weight_ind1 = -1;

        weight_ind2 = (i + 1) % 4;

        if(weight_ind2 == 2) weight_ind2 = 0;
        else if(weight_ind2 == 3) weight_ind2 = -1;

        dop_x1 = x1 + step_dx * dx * i + step_dy * dy * weight_ind1;
        dop_y1 = y1 + step_dx * dy * i - step_dy * dx * weight_ind1;
        dop_x2 = x1 + step_dx * dx * (i + 1) + step_dy * dy * weight_ind2;
        dop_y2 = y1 + step_dx * dy * (i + 1) - step_dy * dx * weight_ind2;

        painter->setPen(QPen(QColor(0xff0000 + i * 0x000004 - i * 0x040000)));
        painter->drawLine(dop_x1, dop_y1, dop_x2, dop_y2);
    }

    painter->setBrush(QBrush(QColor(0x643296)));
    painter->drawEllipse(QPoint(x2, y2), wight, wight);
}

void Demo::nextFrame()
{
    if(++faze < maxFaze) slider->setValue(faze);
    else faze--;
}

void Demo::backFrame()
{
    if(--faze >= 0) slider->setValue(faze);
    else faze++;
}

void Demo::timerShot()
{
    if(++faze < maxFaze) slider->setValue(faze);
    else
    {
        faze--;
        emit clickDemo();
    }
}
