#ifndef MYQWTZOOMER_H
#define MYQWTZOOMER_H

#include <QWidget>
#include "qwt_plot_zoomer.h"

class SpectrogramPlotChild;
class MyQwtZoomer : public QwtPlotZoomer
{
public:
    explicit MyQwtZoomer(QWidget *parent = nullptr);
    ~MyQwtZoomer();
    enum DrawShapeType{
        EnumDrawNull,
        EnumDrawEllipse,
        EnumDrawRect,
        EnumDrawText,
        EnumDrawPicture
    };
    int drawShapeType = EnumDrawNull;
    void setDrawShapeType(int index);
    void widgetMousePressEvent(QMouseEvent* event);
    void widgetMouseMoveEvent(QMouseEvent* event);
    void widgetMouseReleaseEvent(QMouseEvent *event);
    SpectrogramPlotChild *spectrogramPlotChild = nullptr;
    void setSpectrogramPlotChild(SpectrogramPlotChild *value);
private:
    QPointF beginPoint;
    QPointF endPoint;
    QPointF changeBeginPoint;
    QPointF changeEndPoint;
};

#endif // MYQWTZOOMER_H
