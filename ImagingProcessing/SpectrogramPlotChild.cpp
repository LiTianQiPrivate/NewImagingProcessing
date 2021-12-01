#include "SpectrogramPlotChild.h"
#include <QMessageBox>
#include "PaintDrawPicture.h"
#include "PaintDrawElipse.h"
#include "PaintDrawRect.h"
#include "PaintDrawText.h"
#include <QPushButton>
class MyZoomer1: public QwtPlotZoomer
{
public:
    MyZoomer1( QWidget *canvas ):
        QwtPlotZoomer( canvas )
    {
        setTrackerMode( AlwaysOn );
    }

    virtual QwtText trackerTextF( const QPointF &pos ) const
    {
//        QColor bg( Qt::white );
//        bg.setAlpha( 200 );

////        QString str = QwtPlotZoomer::trackerTextF( pos ).text() + " 5 5 ";
////        QwtText text(str);
//        QwtText text = QwtPlotZoomer::trackerTextF( pos );

//        text.setBackgroundBrush( QBrush( bg ) );
        QString text = "";
        return text;
    }
};
class ColorMap1 : public QwtLinearColorMap
{
public:
    ColorMap1() : QwtLinearColorMap(Qt::darkBlue, Qt::red)
    {
        int count = 10;
        QColor cc[10];
        cc[0].setRgb(127,127,127);    cc[1].setRgb(192,192,254);   cc[2].setRgb(122,114,238.5);    cc[3].setRgb(1,160,246);
        cc[4].setRgb(0,236,236);   cc[5].setRgb(1,144,0); cc[6].setRgb(255,255,0);  cc[7].setRgb(231,192,0);
        cc[8].setRgb(255,144,0); cc[9].setRgb(255,0,0);

        for(int i = 0;i < count;i++)
        {
            QRgb mRgb = qRgb(cc[i].red(),cc[i].green(),cc[i].blue());
            QColor mColor = QColor(mRgb);
            addColorStop(double(i+1)/double(count), mColor);
        }
    }
};
SpectrogramPlotChild::SpectrogramPlotChild(QWidget *parent, QWidget* colorScaleParent,QString className):
    SpectrogramPlot(parent,colorScaleParent,className)
{
    h_PlotCurve = new QwtPlotCurve;
    v_PlotCurve = new QwtPlotCurve;
    zoomer1 = new MyQwtZoomer(canvas());
    zoomer1->setSpectrogramPlotChild(this);
    zoomer1->setStateMachine(new QwtPickerTrackerMachine());
    zoomer1->setRubberBandPen(QColor(Qt::blue));
    zoomer1->setMousePattern(QwtEventPattern::MouseSelect2,Qt::RightButton,Qt::ControlModifier);
    zoomer1->setRubberBand(QwtPicker::CrossRubberBand);
    zoomer1->setTrackerMode(QwtPicker::AlwaysOn);
    zoomer1->setTrackerPen(QColor(Qt::black));
    zoomer->setTrackerMode(QwtPicker::AlwaysOff);
    panner = new QwtPlotPanner(canvas());
    panner->setMouseButton(Qt::RightButton);
    panner->setAxisEnabled(QwtPlot::xBottom,false);
    panner->setAxisEnabled(QwtPlot::yLeft,false);
    magnifier->setAxisEnabled(QwtPlot::xBottom,false);
    magnifier->setAxisEnabled(QwtPlot::yLeft,false);
    //parent->setWindowFlags(Qt::WindowStaysOnBottomHint);
    if(zoomer)
    {
        delete zoomer;
        zoomer = NULL;
    }
    contoursCurve = new QwtPlotCurve;
    contoursCurve->attach(this);
    leftButton = new QPushButton(this);
    rightButton = new QPushButton(this);
    upButton = new QPushButton(this);
    downButton = new QPushButton(this);
    leftButton->move(this->width()*2-60,30);
    leftButton->setIcon(QIcon("../PNG/left.png"));
    rightButton->move(this->width()*2,30);
    rightButton->setIcon(QIcon("../PNG/right.png"));
    upButton->move(this->width()*2-30,5);
    upButton->setIcon(QIcon("../PNG/up.png"));
    downButton->move(this->width()*2-30,55);
    downButton->setIcon(QIcon("../PNG/down.png"));
    resetButton = new QPushButton(this);
    resetButton->setIcon(QIcon("../PNG/reset.png"));
    resetButton->move(this->width()*2-30,30);
    leftButton->hide();
    rightButton->hide();
    upButton->hide();
    downButton->hide();
    resetButton->hide();
}

SpectrogramPlotChild::~SpectrogramPlotChild()
{
    delete h_PlotCurve;
    delete v_PlotCurve;
}
/**
 * @brief SpectrogramPlotChild::setXMinMaxValue
 * 设置X轴取值范围  并在parent上增加竖线
 * @param x1
 * @param x2
 */
void SpectrogramPlotChild::setXMinMaxValue(double x1, double x2)
{
    //qwtMatrixRasterData->setInterval( Qt::XAxis,QwtInterval( x1,x2, QwtInterval::ExcludeMaximum ) );
    if(v_PlotCurve)
    {
        delete v_PlotCurve;
        v_PlotCurve = nullptr;
    }
    QVector<QPointF> point;
    QPointF point1;
    point1.setX(x1);
    point1.setY(ymin);
    QPointF point2;
    point2.setX(x1);
    point2.setY(ymax);
    point.append(point1);
    point.append(point2);
    v_PlotCurve = new QwtPlotCurve;
    v_PlotCurve->setSamples(point);
    v_PlotCurve->attach(this);
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(4);
    v_PlotCurve->setPen(pen);
    v_PlotCurve->setLegendAttribute(v_PlotCurve->LegendShowLine);
    this->replot();
    setAutoReplot(true);
}
/**
 * @brief SpectrogramPlotChild::setYMinMaxValue
 * 设置Y轴取值范围  并在parent上增加横线
 * @param y1
 * @param y2
 */
void SpectrogramPlotChild::setYMinMaxValue(double y1, double y2)
{
    //qwtMatrixRasterData->setInterval( Qt::YAxis,QwtInterval( y1,y2, QwtInterval::ExcludeMaximum ) );
    if(h_PlotCurve)
    {
        delete h_PlotCurve;
        h_PlotCurve = nullptr;
    }
    QVector<QPointF> point;
    QPointF point1;
    point1.setX(xmin);
    point1.setY(y1);
    QPointF point2;
    point2.setX(xmax);
    point2.setY(y1);
    point.append(point1);
    point.append(point2);
    h_PlotCurve = new QwtPlotCurve;
    h_PlotCurve->setSamples(point);
    h_PlotCurve->attach(this);
    QPen pen;
    pen.setColor(Qt::yellow);
    pen.setWidth(4);
    h_PlotCurve->setPen(pen);
    h_PlotCurve->setLegendAttribute(v_PlotCurve->LegendShowLine);
    this->replot();
    setAutoReplot(true);
}
/**
 * @brief SpectrogramPlotChild::getQwtMatrixRasterData
 * 获取data数据  当x或y进行改变时 重新赋值
 * @return
 */
QwtMatrixRasterData *SpectrogramPlotChild::getQwtMatrixRasterData()
{
    return qwtMatrixRasterData;
}

QwtPlotSpectrogram *SpectrogramPlotChild::getQwtPlotSpectrogram()
{
    return d_spectrogram;
}
/**
 * @brief SpectrogramPlotChild::setZAxisValue
 * 当QSlider移动时 获取相应的比例 进行运算 筛选Z轴范围
 * @param upScale
 * @param downScale
 */
void SpectrogramPlotChild::setZAxisValue(double upScale, double downScale)
{
    zCurrentMin = (zmin+zmax)/2-((zmin+zmax)/2-zmin)*upScale;
    zCurrentMax = zmax-(zmax-(zmin+zmax)/2)*downScale;
    qwtMatrixRasterData->setInterval( Qt::ZAxis,QwtInterval( zCurrentMin,zCurrentMax, QwtInterval::ExcludeMaximum ) );
    const QwtInterval zInterval = d_spectrogram->data()->interval(Qt::ZAxis);
    d_colorScale->setColorMap(zInterval,new ColorMap1());
    replot();
}

void SpectrogramPlotChild::addModelBase(ModelBases *modelObject)
{
    this->initData(modelObject);
    // 初始化当前z值
    zCurrentMin = zmin;
    zCurrentMax = zmax;
    this->setSpectrogramMode(0);//光谱显示模式
    this->setColorMap(1);//设置色板类型
}
/**
 * @brief SpectrogramPlotChild::getCurrentZMin
 * 获取zmin
 * @return
 */
double SpectrogramPlotChild::getCurrentZMin()
{
    return zCurrentMin;
}
/**
 * @brief SpectrogramPlotChild::getCurrentZMax
 * 获取zmax
 * @return
 */
double SpectrogramPlotChild::getCurrentZMax()
{
    return zCurrentMax;
}

void SpectrogramPlotChild::setRightUpWidget(RightUpWidget *value)
{
    rightUpWidget = value;
}

RightUpWidget *SpectrogramPlotChild::getRightUpWidget()
{
    return rightUpWidget;
}

void SpectrogramPlotChild::setDrawShapeType(int index)
{
    if(index == 8)
    {
        zoomer = new MyZoomer1( canvas() );
        zoomer->setMousePattern( QwtEventPattern::MouseSelect2,
            Qt::RightButton, Qt::ControlModifier );
        zoomer->setMousePattern( QwtEventPattern::MouseSelect3,
            Qt::RightButton );
        zoomer->setRubberBandPen( QColor( Qt::green ) ); //勾选扩大区域边界颜色
        zoomer1->setRubberBand(QwtPicker::CrossRubberBand);
        zoomer1->setDrawShapeType(0);
        this->replot();
        update();
        return ;
    }
    if(zoomer)
    {
       delete zoomer;
        zoomer = NULL;
    }
    zoomer1->setRubberBand(QwtPicker::NoRubberBand);
    zoomer1->setDrawShapeType(index);
    this->replot();
    update();
}
/**
 * @brief SpectrogramPlotChild::paintDrawPicture
 * 鼠标移动停止后 开始绘画
 * @param beginPoint
 * @param endPoint
 */
void SpectrogramPlotChild::paintDrawPicture(QPointF beginPoint, QPointF endPoint)
{
    if(drawBases.empty())
    {
        return;
    }
    QString path = QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("选择图片"),".","Image Files(*.png *.jpg)");
    QPixmap img;
    if(!(img.load(path)))
    {
         QMessageBox::warning(this, QString("打开图像失败"), QString("打开图像失败!"));
         return;
    }
    drawBase = getLastDrawBase();
    drawBase->setPoint(beginPoint,endPoint);
    drawBase->setPicture(img);
    drawBase->setRenderHint(QwtPlotItem::RenderAntialiased);
    drawBase->setZ(8);
    drawBase->attach(this);
    this->replot();
}

void SpectrogramPlotChild::paintDrawRect(QPointF beginPoint, QPointF endPoint)
{
    if(drawBases.empty())
    {
        return ;
    }
    drawBase = getLastDrawBase();
    drawBase->setPoint(beginPoint,endPoint);
    drawBase->setRenderHint(QwtPlotItem::RenderAntialiased);
    drawBase->setZ(8);     //  设置图层  保持新的item在d_spectrogam上层  不设置则会被覆盖 无法显示
    drawBase->attach(this);
    this->replot();
}

void SpectrogramPlotChild::paintDrawElipse(QPointF beginPoint, QPointF endPoint)
{
    if(drawBases.empty())
    {
        return ;
    }
    drawBase = getLastDrawBase();
    drawBase->setPoint(beginPoint,endPoint);
    drawBase->setRenderHint(QwtPlotItem::RenderAntialiased);
    drawBase->setZ(8);
//    d_spectrogram->detach();
    drawBase->attach(this);
//    d_spectrogram->attach(this);
    this->replot();
//    d_spectrogram->hide();

}

void SpectrogramPlotChild::paintDrawText(QPointF beginPoint, QPointF endPoint)
{
    if(drawBases.empty())
    {
        return ;
    }
    drawBase = getLastDrawBase();
    drawBase->setPoint(beginPoint,endPoint);
    drawBase->setRenderHint(QwtPlotItem::RenderAntialiased);
    drawBase->setZ(8);
    drawBase->attach(this);
    this->replot();
}
/**
 * @brief SpectrogramPlotChild::createDrawPicture
 * 移动开始 创建PaintDrawPicture开始绘画
 * @param beginPoint
 * @param endPoint
 */
void SpectrogramPlotChild::createDrawPicture(QPointF beginPoint, QPointF endPoint)
{
    PaintDrawBase *drawPicture = new PaintDrawPicture(beginPoint,endPoint,Qt::blue);
    drawBases.push_back(drawPicture);
}

void SpectrogramPlotChild::createDrawRect(QPointF beginPoint, QPointF endPoint)
{
    PaintDrawBase *drawRect = new PaintDrawRect(beginPoint.x(),beginPoint.y(),endPoint.x(),endPoint.y(),Qt::blue);
    drawBases.push_back(drawRect);
}

void SpectrogramPlotChild::createDrawElipse(QPointF beginPoint, QPointF endPoint)
{
    PaintDrawElipse *drawElipse = new PaintDrawElipse(beginPoint,endPoint,Qt::blue);
    drawBases.push_back(drawElipse);
}

void SpectrogramPlotChild::createDrawText(QPointF beginPoint, QPointF endPoint)
{
    PaintDrawBase *drawText = new PaintDrawText(beginPoint,endPoint);
    drawBases.push_back(drawText);
}
/**
 * @brief SpectrogramPlotChild::getLastDrawBase
 * 获取最后一次的标注  撤销使用
 * @return
 */
PaintDrawBase *SpectrogramPlotChild::getLastDrawBase()
{
    if(drawBases.size() > 0)
    {
        return drawBases[drawBases.size()-1];
    }
    return NULL;
}

void SpectrogramPlotChild::setInputText(QString str)
{
    drawBase->setInputText(str);
}

void SpectrogramPlotChild::resizeEvent(QResizeEvent *e)
{
    QwtPlot::resizeEvent(e);
}

void SpectrogramPlotChild::paintEvent(QPaintEvent *e)
{
    QwtPlot::paintEvent(e);

}

void SpectrogramPlotChild::drawContoursCurve(std::vector<Point3D> points)
{
    xVector.clear();
    yVector.clear();
    orixVector.clear();
    oriyVector.clear();
    for(int i =0;i<points.size();i++)
    {
        xVector.push_back(points[i].x);
        orixVector.push_front(points[i].x);
        yVector.push_back(points[i].y);
        oriyVector.push_back(points[i].y);
    }
    contoursCurve->setSamples(xVector,yVector);
    contoursCurve->setRenderHint(QwtPlotItem::RenderAntialiased,true);//线条光滑化
    contoursCurve->setPen(QColor(Qt::black),2);//设置曲线颜色 粗细
    contoursCurve->setLegendAttribute(contoursCurve->LegendShowLine);//显示图例的标志，这里显示线的颜色。
    setAxisScale(QwtPlot::yLeft,ymin, ymax);
    setAxisScale(QwtPlot::xBottom,xmin,xmax);
    contoursCurve->show();
    contoursCurve->setZ(10);
    leftButton->show();
    rightButton->show();
    upButton->show();
    downButton->show();
    resetButton->show();
    this->replot();
    connect(leftButton,SIGNAL(clicked()),this,SLOT(clickLeftButton()));
    connect(rightButton,SIGNAL(clicked()),this,SLOT(clickRightButton()));
    connect(upButton,SIGNAL(clicked()),this,SLOT(clickUpButton()));
    connect(downButton,SIGNAL(clicked()),this,SLOT(clickDownButton()));
    connect(resetButton,SIGNAL(clicked()),this,SLOT(clickResetButton()));
}
void SpectrogramPlotChild::drawScaleContoursCurve(double scale)
{
    if(fabs(scale-0) < 0.0000001)
        return;
    xVector.clear();
    yVector.clear();
    for(int i=0;i<orixVector.size();i++)
    {
        xVector.push_back(orixVector[i]*scale);
        yVector.push_back(oriyVector[i]*scale);
    }
    qDebug() << xVector.size();
    contoursCurve->setSamples(xVector,yVector);
    contoursCurve->setRenderHint(QwtPlotItem::RenderAntialiased,true);//线条光滑化
    contoursCurve->setPen(QColor(Qt::black),2);//设置曲线颜色 粗细
    contoursCurve->setLegendAttribute(contoursCurve->LegendShowLine);//显示图例的标志，这里显示线的颜色。
    setAxisScale(QwtPlot::yLeft,ymin, ymax);
    setAxisScale(QwtPlot::xBottom,xmin,xmax);
    contoursCurve->show();
    contoursCurve->setZ(10);
    this->replot();
}
/**
 * @brief SpectrogramPlotChild::clickLeftButton
 * 轮廓图向左移动 暂定步距为0.01
 */
void SpectrogramPlotChild::clickLeftButton()
{
    for(int i=0;i<xVector.size();i++)
    {
        xVector[i] -= 0.01;
    }
    contoursCurve->setSamples(xVector,yVector);
    contoursCurve->setRenderHint(QwtPlotItem::RenderAntialiased,true);//线条光滑化
    contoursCurve->setPen(QColor(Qt::black),2);//设置曲线颜色 粗细
    contoursCurve->setLegendAttribute(contoursCurve->LegendShowLine);//显示图例的标志，这里显示线的颜色。
    setAxisScale(QwtPlot::yLeft,ymin, ymax);
    setAxisScale(QwtPlot::xBottom,xmin,xmax);
    contoursCurve->show();
    contoursCurve->setZ(10);
    this->replot();
}
/**
 * @brief SpectrogramPlotChild::clickRightButton
 * 轮廓图向右移动 暂定步距为0.01
 */
void SpectrogramPlotChild::clickRightButton()
{
    for(int i=0;i<xVector.size();i++)
    {
        xVector[i] += 0.01;
    }
    contoursCurve->setSamples(xVector,yVector);
    contoursCurve->setRenderHint(QwtPlotItem::RenderAntialiased,true);//线条光滑化
    contoursCurve->setPen(QColor(Qt::black),2);//设置曲线颜色 粗细
    contoursCurve->setLegendAttribute(contoursCurve->LegendShowLine);//显示图例的标志，这里显示线的颜色。
    setAxisScale(QwtPlot::yLeft,ymin, ymax);
    setAxisScale(QwtPlot::xBottom,xmin,xmax);
    contoursCurve->show();
    contoursCurve->setZ(10);
    this->replot();
}
/**
 * @brief SpectrogramPlotChild::clickUpButton
 * 轮廓图向上移动 暂定步距为0.01
 */
void SpectrogramPlotChild::clickUpButton()
{
    for(int i=0;i<yVector.size();i++)
    {
        yVector[i] += 0.01;
    }
    contoursCurve->setSamples(xVector,yVector);
    contoursCurve->setRenderHint(QwtPlotItem::RenderAntialiased,true);//线条光滑化
    contoursCurve->setPen(QColor(Qt::black),2);//设置曲线颜色 粗细
    contoursCurve->setLegendAttribute(contoursCurve->LegendShowLine);//显示图例的标志，这里显示线的颜色。
    setAxisScale(QwtPlot::yLeft,ymin, ymax);
    setAxisScale(QwtPlot::xBottom,xmin,xmax);
    contoursCurve->show();
    contoursCurve->setZ(10);
    this->replot();
}
/**
 * @brief SpectrogramPlotChild::clickDownButton
 * 轮廓图向下移动 暂定步距为0.01
 */
void SpectrogramPlotChild::clickDownButton()
{
    for(int i=0;i<yVector.size();i++)
    {
        yVector[i] -= 0.01;
    }
    contoursCurve->setSamples(xVector,yVector);
    contoursCurve->setRenderHint(QwtPlotItem::RenderAntialiased,true);//线条光滑化
    contoursCurve->setPen(QColor(Qt::black),2);//设置曲线颜色 粗细
    contoursCurve->setLegendAttribute(contoursCurve->LegendShowLine);//显示图例的标志，这里显示线的颜色。
    setAxisScale(QwtPlot::yLeft,ymin, ymax);
    setAxisScale(QwtPlot::xBottom,xmin,xmax);
    contoursCurve->show();
    contoursCurve->setZ(10);
    this->replot();
}
/**
 * @brief SpectrogramPlotChild::clickResetButton
 * 轮廓图重置
 */
void SpectrogramPlotChild::clickResetButton()
{
    xVector = orixVector;
    yVector = oriyVector;
    contoursCurve->setSamples(orixVector,oriyVector);
    contoursCurve->setRenderHint(QwtPlotItem::RenderAntialiased,true);//线条光滑化
    contoursCurve->setPen(QColor(Qt::black),2);//设置曲线颜色 粗细
    contoursCurve->setLegendAttribute(contoursCurve->LegendShowLine);//显示图例的标志，这里显示线的颜色。
    setAxisScale(QwtPlot::yLeft,ymin, ymax);
    setAxisScale(QwtPlot::xBottom,xmin,xmax);
    contoursCurve->show();
    contoursCurve->setZ(10);
    this->replot();
}


