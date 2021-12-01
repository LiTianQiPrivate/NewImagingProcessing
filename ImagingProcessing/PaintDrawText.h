#ifndef PAINTDRAWTEXT_H
#define PAINTDRAWTEXT_H

#include "PaintDrawBase.h"

namespace Ui {
class PaintDrawText;
}

class PaintDrawText : public PaintDrawBase
{
public:
    PaintDrawText(QPointF xPoint,QPointF yPoint)
    {
        beginPoint = xPoint;
        endPoint = yPoint;
        text = "";
    }
    void draw(QPainter *painter, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &) const
    {
        const QPointF bPoint = QwtScaleMap::transform(xMap, yMap, beginPoint);
        const QPointF ePoint = QwtScaleMap::transform(xMap, yMap, endPoint);
        QPen pen;
        pen.setWidth(2);
        pen.setColor(QColor(Qt::white));
        painter->setPen(pen);
        QwtPainter::drawText(painter,bPoint,text);
    }
    void setPoint(QPointF xPoint, QPointF yPoint)
    {
        beginPoint = xPoint;
        endPoint = yPoint;
    }
    void setInputText(QString str)
    {
        text = str;
    }
private:
    QPointF beginPoint;
    QPointF endPoint;
    QString text;
};

#endif // PAINTDRAWTEXT_H
