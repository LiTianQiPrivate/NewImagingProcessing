#ifndef SPECTROGRAMPLOTCHILD_H
#define SPECTROGRAMPLOTCHILD_H

#include "SpectrogramPlot.h"
#include "qwt_plot_curve.h"
#include "MyQwtZoomer.h"
#include "PaintDrawBase.h"
#include "QPushButton"
/**
 * @brief The SpectrogramPlotChild class
 *  光谱图子类  新增一些功能用于模块开发
 */
class RightUpWidget;
class SpectrogramPlotChild : public SpectrogramPlot
{
    Q_OBJECT

public:
    SpectrogramPlotChild(QWidget * parent = NULL, QWidget *colorScaleParent = nullptr,QString className = "SpectrogramPlot");
    ~SpectrogramPlotChild();
public:
    void setXMinMaxValue(double x1,double x2);
    void setYMinMaxValue(double y1,double y2);
    QwtPlotCurve *h_PlotCurve = nullptr;
    QwtPlotCurve *v_PlotCurve = nullptr;
    QwtMatrixRasterData *getQwtMatrixRasterData();
    QwtPlotSpectrogram *getQwtPlotSpectrogram();
    void setZAxisValue(double upScale,double downScale);
    void addModelBase(ModelBases *modelObject);
    double getCurrentZMin();
    double getCurrentZMax();
    double zCurrentMin,zCurrentMax;
    MyQwtZoomer *zoomer1;
    QwtPlotPanner *panner = nullptr;
    RightUpWidget *rightUpWidget = nullptr;
    void setRightUpWidget(RightUpWidget *value);
    RightUpWidget *getRightUpWidget();
    void setDrawShapeType(int index);
public:
    std::vector<PaintDrawBase *> drawBases;
    void paintDrawPicture(QPointF beginPoint,QPointF endPoint);
    void paintDrawRect(QPointF beginPoint,QPointF endPoint);
    void paintDrawElipse(QPointF beginPoint,QPointF endPoint);
    void paintDrawText(QPointF beginPoint,QPointF endPoint);
    void createDrawPicture(QPointF beginPoint,QPointF endPoint);
    void createDrawRect(QPointF beginPoint,QPointF endPoint);
    void createDrawElipse(QPointF beginPoint,QPointF endPoint);
    void createDrawText(QPointF beginPoint,QPointF endPoint);
    PaintDrawBase *drawBase = nullptr;
    PaintDrawBase *getLastDrawBase();
    void setInputText(QString str);
public:
    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent *e);
public:
    QwtPlotCurve *contoursCurve = nullptr;
    void drawContoursCurve(std::vector<Point3D> points);
    void drawScaleContoursCurve(double scale);
    QPushButton *leftButton;
    QPushButton *rightButton;
    QPushButton *upButton;
    QPushButton *downButton;
    QPushButton *resetButton;
    QVector<double> xVector,yVector;
    QVector<double> orixVector,oriyVector;
public slots:
    void clickLeftButton();
    void clickRightButton();
    void clickUpButton();
    void clickDownButton();
    void clickResetButton();
};

#endif // SPECTROGRAMPLOTCHILD_H
