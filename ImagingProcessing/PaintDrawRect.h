#ifndef PAINTDRAWRECT_H
#define PAINTDRAWRECT_H
#include "PaintDrawBase.h"

class PaintDrawRect : public PaintDrawBase
{
public:
    virtual void draw(QPainter *painter,const QwtScaleMap &xMap,
                      const QwtScaleMap &yMap,const QRectF &) const
    {
        // 坐标的转换,_rect是一个逻辑坐标系下的矩形
        // rect是转换后实际物理坐标下的矩形
        const QPointF bPoint = QwtScaleMap::transform(xMap, yMap, beginPoint);
        const QPointF ePoint = QwtScaleMap::transform(xMap, yMap, endPoint);
        const QRectF rect = QwtScaleMap::transform(xMap, yMap, _rect);
        QPen pen;
        pen.setWidth(2);
        pen.setColor(QColor(Qt::white));
        painter->setPen(pen);
        QwtPainter::drawRect(painter,bPoint.x(),bPoint.y(),(ePoint.x()-bPoint.x()),(ePoint.y()-bPoint.y()));
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
    PaintDrawRect(double x1,double y1,double x2,double y2,QColor color)
    {
        // 根据圆心和半径构造出圆的外接矩形
        beginPoint.setX(x1);
        beginPoint.setY(y1);
        endPoint.setX(x2);
        endPoint.setY(y2);
        _rect.setRect(x1,y1,x2,y2);
        _color = color;
        text = "";
    }
private:
    QPointF beginPoint;
    QPointF endPoint;
    QString text;
    QRectF _rect;
    QColor _color;
};
#endif // PAINTDRAWRECT_H
