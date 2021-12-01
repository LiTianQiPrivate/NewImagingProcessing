#include "MyWidget.h"
#include "ui_MyWidget.h"
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);

}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    beginPoint = event->pos();
}

void MyWidget::mousePressEvent(QMouseEvent *event)
{
    // 鼠标 按下
    QWidget::mousePressEvent(event);
    endPoint = event->pos();
}

void MyWidget::mouseReleaseEvent(QMouseEvent *event)
{
    // 鼠标抬起
    QWidget::mouseReleaseEvent(event);
    endPoint = event->pos();
}

void MyWidget::paintEvent(QPaintEvent *event)
{
//    QPainter painter(this);
//    QPen pen;
//    pen.setWidth(2);
//    pen.setColor(QColor(Qt::red));
//    painter.setPen(pen);
//    painter.drawRect(beginPoint.x(),beginPoint.y(),endPoint.x()-beginPoint.x(),endPoint.y()-beginPoint.y());
}
