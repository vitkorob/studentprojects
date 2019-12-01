#include "plotter.h"

Plotter::Plotter(QWidget *parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMouseTracking(true);

    rect_plot = QRect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);

    mp = false; // methodPoints
    lc = 0xff;  // lineColor
    np = 0x00;  // numberPoints
    nx = "X";   // nameX
    ny = "Y";   // nameY

    minX = minY = -8;
    maxX = maxY = 16;
}

QSize Plotter::minimumSizeHint() const
{
    return QSize(6 * Margin, 5 * Margin);
}

QSize Plotter::sizeHint() const
{
    return QSize(10 * Margin, 8 * Margin);
}

void Plotter::refreshPixmap()
{
    pixmap = QPixmap(size());
    pixmap.fill();

    QPainter painter(&pixmap);
    drawRect(&painter);
    drawLine(&painter);
    update();
}

void Plotter::setLine(float *pointsX, float *pointsY, unsigned long numberPoints)
{
    if(pointsX && pointsY)
    {
        pnt_x = pointsX;
        pnt_y = pointsY;
    }
    np = numberPoints;

    refreshPixmap();
}

void Plotter::setLineColor(unsigned long lineColor)
{
    lc = lineColor;
    refreshPixmap();
}

void Plotter::setMethodPoints(bool methodPoints)
{
    mp = methodPoints;
    refreshPixmap();
}

void Plotter::setNameX(QString nameX)
{
    nx = nameX;
    refreshPixmap();
}

void Plotter::setNameY(QString nameY)
{
    ny = nameY;
    refreshPixmap();
}

void Plotter::drawRect(QPainter *painter)
{
    if (!rect_plot.isValid())
    {
        return;
    }

    QPen dark = QPen(Qt::black, 1, Qt::DotLine);
    QBrush mid = QBrush(Qt::white);

    if (np)
    {
        setMinMaxValue(pnt_x, np, &minX, &maxX, 105);
        setMinMaxValue(pnt_y, np, &minY, &maxY, 105);
    }

    double scaleX = rect_plot.width() / (maxX - minX);
    double scaleY = rect_plot.height() / (maxY - minY);
    int zeroX = Margin - scaleX * minX;
    int zeroY = Margin + scaleY * maxY;

    painter->setPen(dark);
    painter->setBrush(mid);
    painter->drawRect(rect_plot);

    painter->setPen(Qt::black);
    painter->drawText(Margin + 3, Margin - 16, 64, 16, Qt::AlignBottom, ny);
    painter->drawText(Margin + 3 + rect_plot.width(), Margin - 16 + rect_plot.height(), 64, 16, Qt::AlignBottom, nx);

    float tempX = (maxX - minX) * Stroke_X / (float)rect_plot.width();
    float tempY = (maxY - minY) * Stroke_Y / (float)rect_plot.height();

    int orderX = setOrderNumber(&tempX);
    int orderY = setOrderNumber(&tempY);

    tempX = floor(tempX) * pow(10.0, (double)orderX);
    tempY = floor(tempY) * pow(10.0, (double)orderY);

    float bar;
    long cou;

    for (cou = 0; (bar = cou * tempX) < maxX; cou++)
    {
        int x = zeroX + bar * scaleX;
        if(!isInsideNum(x, Margin, Margin + rect_plot.width())) continue;

        int lfX = zeroX + (bar - tempX) * scaleX;
        int whX = 2 * tempX * scaleX;

        painter->setPen(dark);
        painter->drawLine(x, Margin, x, Margin + rect_plot.height());
        painter->setPen(Qt::black);
        painter->drawText(lfX, Margin + rect_plot.height(), whX, 16, Qt::AlignCenter, QString::number(bar));
    }

    for (cou = -1; (bar = cou * tempX) > minX; cou--)
    {
        int x = zeroX + bar * scaleX;
        if(!isInsideNum(x, Margin, Margin + rect_plot.width())) continue;

        int lfX = zeroX + (bar - tempX) * scaleX;
        int whX = 2 * tempX * scaleX;

        painter->setPen(dark);
        painter->drawLine(x, Margin, x, Margin + rect_plot.height());
        painter->setPen(Qt::black);
        painter->drawText(lfX, Margin + rect_plot.height(), whX, 16, Qt::AlignCenter, QString::number(bar));
    }

    for (cou = 0; (bar = cou * tempY) < maxY; cou++)
    {
        int y = zeroY - bar * scaleY;
        if(!isInsideNum(y, Margin, Margin + rect_plot.height())) continue;

        int rtY = zeroY - (bar + tempY) * scaleY;
        int whY = 2 * tempY * scaleY;

        painter->setPen(dark);
        painter->drawLine(Margin, y, Margin + rect_plot.width(), y);
        painter->setPen(Qt::black);
        painter->drawText(Margin - 66, rtY, 64, whY, Qt::AlignRight | Qt::AlignVCenter, QString::number(bar));
    }

    for (cou = -1; (bar = cou * tempY) > minY; cou--)
    {
        int y = zeroY - bar * scaleY;
        if(!isInsideNum(y, Margin, Margin + rect_plot.height())) continue;

        int rtY = zeroY - (bar + tempY) * scaleY;
        int whY = 2 * tempY * scaleY;

        painter->setPen(dark);
        painter->drawLine(Margin, y, Margin + rect_plot.width(), y);
        painter->setPen(Qt::black);
        painter->drawText(Margin - 66, rtY, 64, whY, Qt::AlignRight | Qt::AlignVCenter, QString::number(bar));
    }
}

void Plotter::drawLine(QPainter *painter)
{
    if (np < 2)
    {
        return;
    }

    double scaleX = (width() - 2 * Margin) / (maxX - minX);
    double scaleY = (height() - 2 * Margin) / (maxY - minY);
    int zeroX = Margin - scaleX * minX;
    int zeroY = Margin + scaleY * maxY;

    painter->setPen(QPen(QColor(lc)));

    if (mp)
        for (unsigned long qwe = 0; qwe < np; qwe++)
        {
            int x1 = zeroX + *(pnt_x + qwe) * scaleX;
            int y1 = zeroY - *(pnt_y + qwe) * scaleY;

            painter->drawPoint(x1, y1);
        }
    else
        for (unsigned long qwe = 1; qwe < np; qwe++)
        {
           int x1 = zeroX + *(pnt_x + qwe - 1) * scaleX;
           int y1 = zeroY - *(pnt_y + qwe - 1) * scaleY;
           int x2 = zeroX + *(pnt_x + qwe) * scaleX;
           int y2 = zeroY - *(pnt_y + qwe) * scaleY;

           painter->drawLine(x1, y1, x2, y2);
        }
}

void Plotter::setMinMaxValue(float *massOfPoints, unsigned long numberOfPoints, float *minValue, float *maxValue, int scalePercent)
{
    unsigned long cou;

    *minValue = *massOfPoints;
    *maxValue = *massOfPoints;

    for (cou = 1; cou < numberOfPoints; cou++)
    {
        if(*(massOfPoints + cou) < *minValue) *minValue = *(massOfPoints + cou);
        if(*(massOfPoints + cou) > *maxValue) *maxValue = *(massOfPoints + cou);
    }

    if (*minValue == *maxValue)
    {
        *minValue -= 1;
        *maxValue += 1;
    }

    if (scalePercent -= 100)
    {
        *minValue -= (*maxValue - *minValue) * scalePercent / 100.0;
        *maxValue += (*maxValue - *minValue) * scalePercent / 100.0;
    }
}

void Plotter::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, pixmap);

    if (isInsideNum(mouse_x, rect_plot.x(), rect_plot.width() + rect_plot.x()) &&
       isInsideNum(mouse_y, rect_plot.y(), rect_plot.height() + rect_plot.y()))
    {
        float scaleX = (width() - 2 * Margin) / (maxX - minX);
        float scaleY = (height() - 2 * Margin) / (maxY - minY);
        float valueX = (mouse_x - Margin + scaleX * minX) / scaleX;
        float valueY = (Margin + scaleY * maxY - mouse_y) / scaleY;

        painter.setBrush(Qt::white);
        painter.setPen(Qt::black);

        QString title = ' ' + nx + ": " + QString::number(valueX, 'g', 3) + '\n' +
                        ' ' + ny + ": " + QString::number(valueY, 'g', 3) + ' ';

        QRect temp_rect = painter.boundingRect(mouse_x + 16, mouse_y + 16, 16, 16, Qt::AlignVCenter, title);

        painter.drawRect(temp_rect);
        painter.drawText(temp_rect, Qt::AlignVCenter, title);
    }
}

void Plotter::resizeEvent(QResizeEvent *)
{
    rect_plot = QRect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);
    refreshPixmap();
}

void Plotter::mouseMoveEvent(QMouseEvent *event)
{
    mouse_x = event->x();
    mouse_y = event->y();
    update();
}

int Plotter::setOrderNumber(float *tempNum)
{
    int orderNum = 0;

    while ((*tempNum < 1.0) || (*tempNum >= 10.0))
    {
        if (*tempNum < 1.0)
        {
            *tempNum *= 10.0;
            orderNum--;
        }
        else
        {
            *tempNum /= 10.0;
            orderNum++;
        }
    }

    return orderNum;
}

bool Plotter::isInsideNum(float number, float leftBound, float rightBound)
{
    return leftBound >= number ? false : rightBound <= number ? false : true;
}
