#ifndef PLOTTER_H
#define PLOTTER_H

#include <QtWidgets>

class Plotter : public QWidget
{
    Q_OBJECT

public:
    explicit Plotter(QWidget *parent = 0);

    void setLineColor(unsigned long lineColor);
    void setLine(float *pointsX, float *pointsY, unsigned long numberPoints);
    void setMethodPoints(bool methodPoints);
    void setNameX(QString nameX);
    void setNameY(QString nameY);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

private:
    void drawRect(QPainter *painter);
    void drawLine(QPainter *painter);
    void refreshPixmap();

    int setOrderNumber(float *tempNum);
    void setMinMaxValue(float *massOfPoints, unsigned long numberOfPoints, float *minValue, float *maxValue, int scalePercent = 100);
    bool isInsideNum(float number, float leftBound, float rightBound);

    bool mp;
    unsigned long lc;
    unsigned long np;
    float *pnt_x;
    float *pnt_y;
    QString nx;
    QString ny;
    QPixmap pixmap;
    int mouse_x;
    int mouse_y;
    QRect rect_plot;

    enum
    {
        Margin = 40,
        Stroke_X = 60,
        Stroke_Y = 70
    };

    float minX;
    float maxX;
    float minY;
    float maxY;

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // PLOTTER_H
