#include "painter.h"

#define STROKE_X 60.0
#define STROKE_Y 45.0

void Painter::drawGraph(QPainter *painter, unsigned long clrGraph,
                               float *pointsX, float *pointsY, unsigned long numPoints,
                               int boundX, int boundY, int boundWidth, int boundHeight,
                               bool methodPoints, QString nameX, QString nameY)
{
    if(boundWidth < 2) return;
    if(boundHeight < 2) return;
    if(numPoints < 2) return;

    float minX;
    float maxX;
    float minY;
    float maxY;

    setMinMaxValue(pointsX, pointsY, numPoints, 105, &minX, &maxX, &minY, &maxY);

    double scaleX = boundWidth / (maxX - minX);
    double scaleY = boundHeight / (maxY - minY);
    int zeroX = boundX - scaleX * minX;
    int zeroY = boundY + scaleY * maxY;

    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(QColor(0xCCCCCC)));
    painter->drawRect(QRect(boundX, boundY, boundWidth, boundHeight));

//-----begin draw axis--------------------------------------------

    painter->setPen(QPen(QColor(0x330022), 0.5, Qt::SolidLine));

    painter->drawLine(boundX, boundY + boundHeight, boundX, boundY);
    painter->drawLine(boundX, boundY + boundHeight, boundX + boundWidth, boundY + boundHeight);

    painter->drawText(boundX + 3, boundY - 16, 64, 16, Qt::AlignBottom, nameY);
    painter->drawText(boundX + 3 + boundWidth, boundY - 16 + boundHeight, 64, 16, Qt::AlignBottom, nameX);

    float tempX = (maxX - minX) * STROKE_X / (float)boundWidth;
    float tempY = (maxY - minY) * STROKE_Y / (float)boundHeight;

    int orderX = setOrderNumber(&tempX);
    int orderY = setOrderNumber(&tempY);

    tempX = floor(tempX) * pow(10.0, (double)orderX);
    tempY = floor(tempY) * pow(10.0, (double)orderY);

    float bar;
    long cou;

    for(cou = 0; (bar = cou * tempX) < maxX; cou++)
    {
        int x = zeroX + bar * scaleX;
        if(!isInsideNum(x, boundX, boundX + boundWidth)) continue;

        int lfX = zeroX + (bar - tempX) * scaleX;
        int whX = 2 * tempX * scaleX;

        painter->drawLine(x, boundY + boundHeight - 3, x, boundY + boundHeight);
        painter->drawText(lfX, boundY + boundHeight, whX, 16, Qt::AlignCenter, QString::number(bar));
    }
    for(cou = -1; (bar = cou * tempX) > minX; cou--)
    {
        int x = zeroX + bar * scaleX;
        if(!isInsideNum(x, boundX, boundX + boundWidth)) continue;

        int lfX = zeroX + (bar - tempX) * scaleX;
        int whX = 2 * tempX * scaleX;

        painter->drawLine(x, boundY + boundHeight - 3, x, boundY + boundHeight);
        painter->drawText(lfX, boundY + boundHeight, whX, 16, Qt::AlignCenter, QString::number(bar));
    }
    for(cou = 0; (bar = cou * tempY) < maxY; cou++)
    {
        int y = zeroY - bar * scaleY;
        if(!isInsideNum(y, boundY, boundY + boundHeight)) continue;

        int rtY = zeroY - (bar + tempY) * scaleY;
        int whY = 2 * tempY * scaleY;

        painter->drawLine(boundX, y, boundX + 3, y);
        painter->drawText(boundX - 66, rtY, 64, whY, Qt::AlignRight | Qt::AlignVCenter, QString::number(bar));
    }
    for(cou = -1; (bar = cou * tempY) > minY; cou--)
    {
        int y = zeroY - bar * scaleY;
        if(!isInsideNum(y, boundY, boundY + boundHeight)) continue;

        int rtY = zeroY - (bar + tempY) * scaleY;
        int whY = 2 * tempY * scaleY;

        painter->drawLine(boundX, y, boundX + 3, y);
        painter->drawText(boundX - 66, rtY, 64, whY, Qt::AlignRight | Qt::AlignVCenter, QString::number(bar));
    }

//-----end draw axis----------------------------------------------

    painter->setPen(QPen(QColor(0x330022), 1, Qt::DotLine));

    if(isInsideNum(zeroX, boundX, boundX + boundWidth))
    {
        painter->drawLine(zeroX, boundY, zeroX, boundY + boundHeight);
    }
    if(isInsideNum(zeroY, boundY, boundY + boundHeight))
    {
        painter->drawLine(boundX, zeroY, boundX + boundWidth, zeroY);
    }

    painter->setPen(QPen(QColor(clrGraph), 1, Qt::SolidLine));

    if(methodPoints)
        for(unsigned long qwe = 0; qwe < numPoints; qwe++)
        {
            int x1 = zeroX + *(pointsX + qwe) * scaleX;
            int y1 = zeroY - *(pointsY + qwe) * scaleY;

            painter->drawPoint(x1, y1);
        }
    else
        for(unsigned long qwe = 1; qwe < numPoints; qwe++)
        {
           int x1 = zeroX + *(pointsX + qwe - 1) * scaleX;
           int y1 = zeroY - *(pointsY + qwe - 1) * scaleY;
           int x2 = zeroX + *(pointsX + qwe) * scaleX;
           int y2 = zeroY - *(pointsY + qwe) * scaleY;

           painter->drawLine(x1, y1, x2, y2);
        }
}

void Painter::drawLoader(QPainter *painter, unsigned long clrLoader, unsigned char periodPhase, char nowPhase,
                                QPoint center, int radiusMin, int radiusBig, QString caption)
{
    nowPhase %= periodPhase;

    unsigned char stepAlpha = 255 / periodPhase;
    unsigned char alpha = 255;

    double stepAngle = 2.0 * M_PI / periodPhase;
    double angle = nowPhase * stepAngle;

    QColor color(clrLoader);
    painter->setPen(Qt::NoPen);

    for(int i = 0; i < periodPhase; i++)
    {
        painter->setBrush(QBrush(color));
        painter->drawEllipse(center + QPoint(radiusBig * cos(angle), radiusBig * sin(angle)), radiusMin, radiusMin);

        angle -= stepAngle;
        color.setAlpha(alpha -= stepAlpha);
    }

    color.setAlpha(255);
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(color));
    painter->setFont(QFont("Arial", radiusMin << 2));

    int radiusFull = radiusBig + radiusMin;
    QRect captionRect(center + QPoint(radiusFull + 8, -radiusFull), center + QPoint(radiusFull + 520, radiusFull));

    painter->drawText(captionRect, Qt::AlignVCenter, caption);
}

void Painter::setMinMaxValue(float *pointsX, float *pointsY, unsigned long numPoints, int scalePercent, float *minX, float *maxX, float *minY, float *maxY)
{
    unsigned long cou;

    *minX = *maxX = *pointsX;
    *minY = *maxY = *pointsY;

    for(cou = 1; cou < numPoints; cou++)
    {
        if(*(pointsX + cou) < *minX) *minX = *(pointsX + cou);
        if(*(pointsX + cou) > *maxX) *maxX = *(pointsX + cou);

        if(*(pointsY + cou) < *minY) *minY = *(pointsY + cou);
        if(*(pointsY + cou) > *maxY) *maxY = *(pointsY + cou);
    }

    if(*minX == *maxX)
    {
        *minX -= 1;
        *maxX += 1;
    }

    if(*minY == *maxY)
    {
        *minY -= 1;
        *maxY += 1;
    }

    scalePercent -= 100;

    if(scalePercent)
    {
        *minX -= (*maxX - *minX) * scalePercent / 100.0;
        *maxX += (*maxX - *minX) * scalePercent / 100.0;
        *minY -= (*maxY - *minY) * scalePercent / 100.0;
        *maxY += (*maxY - *minY) * scalePercent / 100.0;
    }
}

int Painter::setOrderNumber(float *tempNum)
{
    int orderNum = 0;

    while((*tempNum < 1.0) || (*tempNum >= 10.0))
    {
        if(*tempNum < 1)
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

bool Painter::isInsideNum(float number, float leftBound, float rightBound)
{
    return leftBound >= number ? false : rightBound <= number ? false : true;
}
