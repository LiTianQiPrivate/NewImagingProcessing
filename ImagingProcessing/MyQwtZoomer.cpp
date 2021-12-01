#include "MyQwtZoomer.h"
#include "QMouseEvent"
#include "PublicUse.h"
#include "RightUpWidget.h"
#include "DialogText.h"
MyQwtZoomer::MyQwtZoomer(QWidget *parent) :
    QwtPlotZoomer(parent)
{
    setTrackerMode(AlwaysOn);

}

MyQwtZoomer::~MyQwtZoomer()
{

}

void MyQwtZoomer::setDrawShapeType(int index)
{
    drawShapeType = index;
}

void MyQwtZoomer::widgetMousePressEvent(QMouseEvent *event)
{
    QwtPlotZoomer::widgetMousePressEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        beginPoint = event->pos();
        changeBeginPoint = QwtPlotZoomer::invTransform(event->pos());
        switch(drawShapeType)
        {
        case EnumDrawNull:
            {
                QPointF point = QwtPlotZoomer::invTransform(event->pos());
                spectrogramPlotChild->getRightUpWidget()->drawTwoCurve(point.x(),point.y());
                spectrogramPlotChild->getRightUpWidget()->getMovePoint(changeEndPoint);
            }
            break;
        case EnumDrawEllipse:
            if(spectrogramPlotChild)
            {
                spectrogramPlotChild->createDrawElipse(changeBeginPoint,changeBeginPoint);
            }
            break;
        case EnumDrawRect:
            if(spectrogramPlotChild)
            {
                spectrogramPlotChild->createDrawRect(changeBeginPoint,changeBeginPoint);
            }
            break;
        case EnumDrawText:
            if(spectrogramPlotChild)
            {
                spectrogramPlotChild->createDrawText(changeBeginPoint,changeBeginPoint);
            }
            break;
        case EnumDrawPicture:
            if(spectrogramPlotChild)
            {
                spectrogramPlotChild->createDrawPicture(changeBeginPoint,changeBeginPoint);
            }
            break;
        }
    }
}

void MyQwtZoomer::widgetMouseMoveEvent(QMouseEvent *event)
{
    QwtPlotZoomer::widgetMouseMoveEvent(event);
    if(event->buttons() & Qt::LeftButton)
    {
        endPoint = event->pos();
        changeEndPoint = QwtPlotZoomer::invTransform(event->pos());
        switch (drawShapeType)
        {
        case EnumDrawNull:
            if(spectrogramPlotChild)
            {

            }
            break;
        case EnumDrawEllipse:
            if(spectrogramPlotChild)
            {
                spectrogramPlotChild->paintDrawElipse(changeBeginPoint,changeEndPoint);
            }
            break;
        case EnumDrawRect:
            if(spectrogramPlotChild)
            {
                spectrogramPlotChild->paintDrawRect(changeBeginPoint,changeEndPoint);
            }
            break;
        case EnumDrawText:
            if(spectrogramPlotChild)
            {
                spectrogramPlotChild->paintDrawText(changeBeginPoint,changeEndPoint);
            }
            break;
//        case EnumDrawPicture:
//            if(spectrogramPlotChild)
//            {
//                spectrogramPlotChild->paintDrawPicture(changeBeginPoint,changeEndPoint);
//            }
//            break;
        default:
            break;
        }
    }
    else
    {

    }
}

void MyQwtZoomer::widgetMouseReleaseEvent(QMouseEvent *event)
{
    QwtPlotZoomer::widgetMouseReleaseEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        endPoint = event->pos();
        changeEndPoint = QwtPlotZoomer::invTransform(event->pos());
        switch(drawShapeType)
        {
        case EnumDrawNull:

            break;
        case EnumDrawEllipse:
            if(spectrogramPlotChild)
            {
                DialogText *text = new DialogText;
                text->exec();
                QString str = text->getEditText();
                spectrogramPlotChild->setInputText(str);
                spectrogramPlotChild->paintDrawElipse(changeBeginPoint,changeEndPoint);
            }
            break;
        case EnumDrawRect:
            if(spectrogramPlotChild)
            {
                DialogText *text = new DialogText;
                text->exec();
                QString str = text->getEditText();
                spectrogramPlotChild->setInputText(str);
                spectrogramPlotChild->paintDrawRect(changeBeginPoint,changeEndPoint);
            }
            break;
        case EnumDrawText:
            if(spectrogramPlotChild)
            {
                DialogText *text = new DialogText;
                text->exec();
                QString str = text->getEditText();
                spectrogramPlotChild->setInputText(str);
                spectrogramPlotChild->paintDrawText(changeBeginPoint,changeEndPoint);
            }
            break;
        case EnumDrawPicture:
            if(spectrogramPlotChild)
            {
                spectrogramPlotChild->paintDrawPicture(changeBeginPoint,changeEndPoint);
            }
            break;
        default:
            spectrogramPlotChild->getRightUpWidget()->getMovePoint(changeEndPoint);
            break;
        }
    }
}

void MyQwtZoomer::setSpectrogramPlotChild(SpectrogramPlotChild *value)
{
    spectrogramPlotChild = value;
}

