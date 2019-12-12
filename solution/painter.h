#ifndef PAINTER_H
#define PAINTER_H

#include <QPainter>
#include <math.h>

class Painter
{
public:
    static void drawGraph(QPainter *painter, unsigned long clrGraph,
                          float *pointsX, float *pointsY, unsigned long numPoints,
                          int boundX, int boundY, int boundWidth, int boundHeight,
                          bool methodPoints = false, QString nameX = "X", QString nameY = "Y");
    static void drawLoader(QPainter *painter, unsigned long clrLoader,
                           unsigned char periodPhase, char nowPhase,
                           QPoint center, int radiusMin, int radiusBig,
                           QString caption = "Rendering...");

private:
    static void setMinMaxValue(float *pointsX, float *pointsY, unsigned long numPoints, int scalePercent, float *minX, float *maxX, float *minY, float *maxY);
    static int setOrderNumber(float *tempNum);
    static bool isInsideNum(float number, float leftBound, float rightBound);
};

#endif  // PAINTER_H
