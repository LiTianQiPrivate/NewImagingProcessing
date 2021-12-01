#ifndef RIGHTUPWIDGET_H
#define RIGHTUPWIDGET_H

#include <QWidget>
#include "ModelBases.h"
#include "SpectrogramPlotChild.h"
#include "WaterfallWidget.h"
#include "ModelViewer.h"
#include "LineObject.h"
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QCandlestickSet>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtCore/QRandomGenerator>
#include <QtCharts/QBarCategoryAxis>
#include <QtWidgets/QApplication>
#include <QtCharts/QValueAxis>
#include "ModelGraph.h"
namespace Ui {
class RightUpWidget;
}
QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE
QT_CHARTS_USE_NAMESPACE

class RightUpWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RightUpWidget(QWidget *parent = nullptr);
    ~RightUpWidget();
    void addModelBases(ModelBases *object);
    void paintEvent(QPaintEvent *event);
    SpectrogramPlotChild *spectrogramPlot = nullptr;
    WaterfallWidget *waterfallWidget = nullptr;   // 初始化失败 程序崩溃 原因暂未知 先加载数据显示 后续进行调试
    std::vector<ModelBases *> modelBases;
    bool toolButton2Flag = true;
    QTimer *timer;
    // 存储播放中的数据
    double xStartData = 0.0;
    double yStartData = 0.0;
    // 记录QSliDer滑动比例
    double upScaleNumber = 1.0;
    double downScaleNumber = 1.0;
private slots:

    void on_toolButton_2_clicked(bool checked);

    void freshData();

    void on_toolButton_clicked(bool checked);

    void on_toolButton_3_clicked(bool checked);
    void on_checkBox_clicked(bool checked);

    void on_lineEdit_textChanged(const QString &arg1);

    void on_verticalSlider_2_valueChanged(int value);

    void on_verticalSlider_valueChanged(int value);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_lineEdit_3_textChanged(const QString &arg1);

    void on_lineEdit_4_textChanged(const QString &arg1);

    void on_checkBox_2_clicked(bool checked);

    void loadingContours();

    void on_tabWidget_tabBarClicked(int index);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

public:
    void getMovePoint(QPointF point);
    void drawLabel(int index);
    QPointF movePoint;
    QChart *upChart = nullptr;
    QChartView *upChartView = nullptr;
    QChart *leftChart = nullptr;
    QChartView *leftChartView = nullptr;
    void upDataUpCuve();
    void upDataLeftCurve();
    void loadingVTKMode(ModelBases *modelObject);
    void drawTwoCurve(double v,double h);
    PolyDataObject rcsfPolyData;
    vtkSmartPointer<vtkLookupTable> pColorTable; // 颜色条映射
    vtkSmartPointer<vtkScalarBarWidget> scalarWidget; // 颜色条窗口
    ModelGraph* polyDataObject;
    ModelGraph* polyDataGif;
    ModelGraph* modelFem;
    int gif_number = 0;
    ModelViewer modelViewer;
    ModelViewer gifViewer;
    ModelGraph *newPolyData;
private:
    Ui::RightUpWidget *ui;
};

#endif // RIGHTUPWIDGET_H
