#include "RightUpWidget.h"
#include "ui_RightUpWidget.h"
#include <QTimer>
#include "qwt_plot_curve.h"
#include "ModelRCSF.h"
#include "QGraphicsLayout"
#include "vtkFloatArray.h"
#include "VTKGeometry_1.h"
#include "vtkPointData.h"
#include "vtkScalarBarActor.h"
#include "ModelGraph.h"
#include "VTKGeometry.h"
#include "PublicUse.h"
#include <QFileDialog>
RightUpWidget::RightUpWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RightUpWidget)
{
    ui->setupUi(this);
    modelViewer.setBackgroundColor(100.0/255.0,149.0/255.0,237.0/255.0);
    gifViewer.setBackgroundColor(100.0/255.0,149.0/255.0,237.0/255.0);
    pColorTable = vtkSmartPointer<vtkLookupTable>::New(); // 颜色条映射
    scalarWidget = vtkSmartPointer<vtkScalarBarWidget>::New(); // 颜色条窗口
    QDoubleValidator *m_double =new QDoubleValidator;
    ui->lineEdit->setValidator(m_double);
    ui->lineEdit_2->setValidator(m_double);
    ui->lineEdit_3->setValidator(m_double);
    ui->lineEdit_4->setValidator(m_double);
    ui->lineEdit_5->setValidator(m_double);
    ui->lineEdit_6->setValidator(m_double);
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QStringList header;
    header << QString::fromLocal8Bit("选择")
           << QString::fromLocal8Bit("目标-状态")
           << QString::fromLocal8Bit("频率")
           << QString::fromLocal8Bit("角度")
           << QString::fromLocal8Bit("波段")
           << QString::fromLocal8Bit("极化")
           << QString::fromLocal8Bit("文件名") ;
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); // 整行选取
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
//    ui->tableWidget->setDragDropMode(QAbstractItemView::DragDrop);
    ui->tableWidget->setDragEnabled(true);
    //ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    // ui.tab 第一页   二维像初始化
    spectrogramPlot = new SpectrogramPlotChild(ui->widget_7,ui->widget_8);
    spectrogramPlot->setRightUpWidget(this);
    QHBoxLayout* layout1 = new QHBoxLayout;
    layout1->addWidget(spectrogramPlot);
    ui->widget_7->setLayout(layout1);
    QHBoxLayout* layout2 = new QHBoxLayout;
    layout2->addWidget(spectrogramPlot->getQwtScaleWidget());
    ui->widget_8->setLayout(layout2);
    // ui.tab_2 第二页 三维像初始化
    QHBoxLayout* layout3 = new QHBoxLayout;
    layout3->addWidget(&modelViewer);
    ui->widget_10->setLayout(layout3);
    polyDataObject = new ModelGraph;
    polyDataObject->loadBackGroupFEM("../Test02/FJ_500mm.fem");
    polyDataObject->drawBackGroupFEM();
    polyDataObject->rotate(90, Normal(0, 0, 1), Point3D(0, 0, 0));
    //polyDataObject->calNormal(0);
    modelViewer.addActor(polyDataObject->getActor());
    /************************/
//    modelFem = new ModelGraph;
//    modelFem->loadBackGroupFEM("../Test02/target.fem");
//    modelFem->drawBackGroupFEM();
    //modelViewer.addActor(modelFem->getActor());
    modelViewer.resetCamera();
//    polyDataObject->setOpacity(0.2);
    // ui.tab_3 第三页 GIF初始化
    QHBoxLayout* layoutGIF = new QHBoxLayout;
    layoutGIF->addWidget(&gifViewer);
    ui->widget_11->setLayout(layoutGIF);
    polyDataGif = new ModelGraph;
    // 播放 暂停 停止按钮图标设置
    ui->toolButton->setIcon(QIcon("../PNG/start.png"));
    ui->toolButton->setFixedSize(30,30);
    ui->toolButton_2->setIcon(QIcon("../PNG/pause.png"));
    ui->toolButton_2->setFixedSize(30,30);
    ui->toolButton_3->setIcon(QIcon("../PNG/suspend.png"));
    ui->toolButton_3->setFixedSize(30,30);
    //创建动态定时器
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(freshData()));
    // 动态曲线加载
    upChart = new QChart;
    upChart->setContentsMargins(0,0,0,0);
    upChartView = new QChartView(upChart,ui->widget_3);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->addWidget(upChartView);
    ui->widget_3->setLayout(layout);

    leftChart = new QChart;
    leftChart->setContentsMargins(0,0,0,0);
    leftChartView = new QChartView(leftChart,ui->qCustomPlot);
    QHBoxLayout *leftlayout = new QHBoxLayout;
    leftlayout->setMargin(0);
    leftlayout->addWidget(leftChartView);
    ui->qCustomPlot->setLayout(leftlayout);
    ui->widget_7->addAction(ui->loadContours);
    ui->widget_7->setContextMenuPolicy(Qt::ActionsContextMenu);
    connect(ui->loadContours,SIGNAL(triggered()),this,SLOT(loadingContours()));
    QDoubleValidator *mDouble = new QDoubleValidator();
    ui->lineEdit_7->setValidator(mDouble);
    ui->lineEdit_8->setValidator(mDouble);
    ui->lineEdit_9->setValidator(mDouble);
    ui->widget_13->hide();
    ui->lineEdit_10->setValidator(mDouble);
    ui->lineEdit_12->setValidator(mDouble);
}

RightUpWidget::~RightUpWidget()
{
    delete ui;
}
/**
 * @brief RightUpWidget::addModelBases
 *  tableWidget窗口添加数据
 * @param object
 */
void RightUpWidget::addModelBases(ModelBases *object)
{
    modelBases.push_back(object);
    QTableWidgetItem *check = new QTableWidgetItem;
    check->setCheckState(Qt::Checked);
    check->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    int rows = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rows);
    ui->tableWidget->setItem(rows,0,check);
    ui->tableWidget->setItem(rows,1,new QTableWidgetItem(object->findDataInfo(QString::fromLocal8Bit(TARGET_NAME))));
    ui->tableWidget->setItem(rows,2,new QTableWidgetItem(object->findDataInfo(QString::fromLocal8Bit(WORK_FPS))));
    ui->tableWidget->setItem(rows,3,new QTableWidgetItem(object->findDataInfo(QString::fromLocal8Bit(INCIDENT_ANGLE))));
    ui->tableWidget->setItem(rows,4,new QTableWidgetItem("P"));
    ui->tableWidget->setItem(rows,5,new QTableWidgetItem(object->findDataInfo(QString::fromLocal8Bit(POLARIZATION_COMBINATION))));
    ui->tableWidget->setItem(rows,6,new QTableWidgetItem(object->FileName()));
    for (int i=0;i<ui->tableWidget->rowCount();i++)
    {
       for (int j=0;j<ui->tableWidget->columnCount();j++)
       {
           ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
       }
    }
    if(spectrogramPlot)
    {
        delete spectrogramPlot->getQwtScaleWidget();
        delete spectrogramPlot;
    }
    spectrogramPlot = new SpectrogramPlotChild(ui->widget_7,ui->widget_8);
    spectrogramPlot->setGeometry(0,0,ui->widget_7->width(),ui->widget_7->height());
    spectrogramPlot->setColorScaleGeometry(0,0,ui->widget_8->width(),ui->widget_8->height());
    spectrogramPlot->show();
    spectrogramPlot->setRightUpWidget(this);
    spectrogramPlot->setModelBases(modelBases);
    double xMin, xMax;
    polyDataObject->getcalBox(xMin, xMax);
    std::vector<ModelBases *> graphBase(modelBases);
    ((ModelGraph *)object)->zoomPolyData(xMin,xMax);
    polyDataObject->addPointAttribute((ModelGraph *)object,object->FileName());
//    for(int i=0;i<graphBase.size();i++)
//    {
//        ModelGraph* modelGraph = (ModelGraph*)graphBase[i];
//        modelGraph->zoomPolyData(xMin, xMax);
//        polyDataObject->addPointAttribute(modelGraph);
//    }
    modelViewer.resetCamera();
}
/**
 * @brief RightUpWidget::paintEvent
 * 重写paintEvent保持标注在上层
 * @param event
 */
void RightUpWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    spectrogramPlot->setGeometry(0,0,ui->widget_7->width(),ui->widget_7->height());
    spectrogramPlot->setColorScaleGeometry(0,0,ui->widget_8->width(),ui->widget_8->height());
    spectrogramPlot->show();
}
/**
 * @brief RightUpWidget::on_toolButton_2_clicked
 * 暂停播放 继续播放
 * @param checked
 */
void RightUpWidget::on_toolButton_2_clicked(bool checked)
{
    if(toolButton2Flag)
    {
        timer->stop();
        ui->toolButton_2->setIcon(QIcon("../PNG/continue.png"));
        ui->toolButton_2->setFixedSize(30,30);
        toolButton2Flag = false;
    }
    else
    {
        timer->start(200);
        ui->toolButton_2->setIcon(QIcon("../PNG/pause.png"));
        ui->toolButton_2->setFixedSize(30,30);
        toolButton2Flag = true;
    }
}
/**
 * @brief RightUpWidget::freshData
 * 定时器启动 刷新
 */
void RightUpWidget::freshData()
{
    polyDataGif->loadBackGroupFEM("../Test02/FJ_500mm.fem");
    polyDataGif->drawBackGroupFEM();
//    polyDataGif->loadPolyData("../Test02/model.stl");
//    polyDataGif->rotate(-90, Normal(1, 0, 0), Point3D(0, 0, 0));
//    polyDataGif->calNormal(0);
    gifViewer.addActor(polyDataGif->getActor());
    double xmin,xmax;
    polyDataGif->getcalBox(xmin,xmax);
    std::vector<ModelBases *> graphBase(modelBases);
    //for(int i=0;i<graphBase.size();i++)
    {
        ModelGraph* modelGraph = (ModelGraph*)graphBase[gif_number];
        //modelGraph->zoomPolyData(xmin, xmax);
        polyDataGif->addPointAttribute(modelGraph,modelGraph->FileName());
    }
    gifViewer.resetCamera();
    gifViewer.updateView();
    gif_number++;
    if(gif_number >= modelBases.size())
    {
        gif_number = 0;
        return ;
    }
}
/**
 * @brief RightUpWidget::on_toolButton_clicked
 * 开始播放
 * @param checked
 */
void RightUpWidget::on_toolButton_clicked(bool checked)
{
    if(ui->tabWidget->currentIndex() == 2)
    {
        timer->start(200);
    }
}
/**
 * @brief RightUpWidget::on_toolButton_3_clicked
 * 停止播放
 * @param checked
 */
void RightUpWidget::on_toolButton_3_clicked(bool checked)
{
    timer->stop();
    gif_number = 0;
}
/**
 * @brief RightUpWidget::upDataUpCuve
 * 动态更新上方曲线
 */
void RightUpWidget::upDataUpCuve()
{
    upChart->removeAllSeries();
    double xMin = 10000000, xMax = -10000000;
    double yMin = 10000000, yMax = -10000000;
    double currentValue = -999;
    double minusValue = -999;
    for(int i=0;i<modelBases.size();i++)
    {
        std::vector<Point2D> points;
        if(modelBases[i]->getClassName() == "ModelRCSF")
        {
            ModelRCSF *rcsf = (ModelRCSF *)modelBases[i];
            rcsf->setCurrentValue(xStartData);
            rcsf->setXStype(0);
            points = rcsf->getPoint2D();
        }
        else if(modelBases[i]->getClassName() == "ModelGraph")
        {
            std::vector<std::vector<Point3D>> point3 = modelBases[i]->getPointDatas();
            currentValue = point3[0][0].x;
            minusValue = fabs(xStartData - currentValue);
            for(int k = 0;k<point3.size();k++)
            {
                std::vector<Point3D> p = point3[k];
                for(int h = 0;h<p.size();h++)
                {
                    double value = fabs(xStartData - p[h].x);
                    if(value < minusValue)
                    {
                        minusValue = value;
                        currentValue = p[h].x;
                    }
                }                
            }
            for(int k=0;k<point3.size();k++)
            {
                std::vector<Point3D> p = point3[k];
                int number = 0;
                for(int h = 0;h<p.size();h++)
                {
                    if(fabs(currentValue - p[h].x) < 0.001)
                    {
                        double y1 = spectrogramPlot->getCurrentZMin();
                        double y2 = spectrogramPlot->getCurrentZMax();
                        //if( p[h].z > y1 && p[h].z <y2 )
                        {
                            Point2D p1(p[h].y,p[h].z);
                            points.push_back(p1);
                        }
                    }
                }
            }
        }
        QLineSeries *upSeries = new QLineSeries(ui->widget_3);
        for(int j = 0; j < points.size(); j++)
        {
            Point2D p = points[j];
            double x = p.x, y = p.y;
            upSeries->append(QPointF(p.x,p.y));
            if(xMin > x)
            {
                xMin = x;
            }
            if(xMax < x)
            {
                xMax = x;
            }
            if(yMin > y)
            {
                yMin = y;
            }
            if(yMax < y)
            {
                yMax = y;
            }
        }
        upChart->addSeries(upSeries);
    }
    QLineSeries *line = new QLineSeries(ui->widget_3);
    line->append(QPointF(yStartData,yMin));
    line->append(QPointF(yStartData,yMax));
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::black);
    line->setPen(pen);
    upChart->addSeries(line);
    upChart->createDefaultAxes();
    upChart->legend()->hide();
    upChart->axes(Qt::Horizontal).first()->setRange(xMin, xMax);
    upChart->axes(Qt::Vertical).first()->setRange(yMin, yMax);
    upChart->axisX()->hide();
    upChart->axisY()->hide();
}
/**
 * @brief RightUpWidget::upDataLeftCurve
 * 动态更新左侧曲线
 */
void RightUpWidget::upDataLeftCurve()
{
    leftChart->removeAllSeries();
    double xMin = 10000000, xMax = -10000000;
    double yMin = 10000000, yMax = -10000000;
    double currentValue = -999;
    double minusValue = -999;
    for(int i=0;i<modelBases.size();i++)
    {
        std::vector<Point2D> points;
        if(modelBases[i]->getClassName() == "ModelRCSF")
        {
            ModelRCSF *rcsf = (ModelRCSF *)modelBases[i];
            rcsf->setCurrentValue(yStartData);
            rcsf->setXStype(1);
            points = rcsf->getPoint2D();
        }
        else if(modelBases[i]->getClassName() == "ModelGraph")
        {
            std::vector<std::vector<Point3D>> point3 = modelBases[i]->getPointDatas();
            for(int k = 0;k<point3.size();k++)
            {
                std::vector<Point3D> p = point3[k];
                currentValue = point3[0][0].x;
                minusValue = fabs(xStartData - currentValue);
                for(int h = 0;h<p.size();h++)
                {
                    double value = fabs(yStartData - p[h].y);
                    if(value < minusValue)
                    {
                        minusValue = value;
                        currentValue = p[h].y;
                    }

                }

            }
            for(int k = 0;k<point3.size();k++)
            {
                std::vector<Point3D> p = point3[k];
                for(int h = 0;h<p.size();h++)
                {
                    double y1 = spectrogramPlot->getCurrentZMin();
                    double y2 = spectrogramPlot->getCurrentZMax();
                    //if( p[h].z > y1 && p[h].z < y2)
                    {
                        if(fabs(currentValue - p[h].y) < 0.001)
                        {
                            Point2D p1(p[h].x,p[h].z);
                            points.push_back(p1);
                        }
                    }
                }
            }
        }
        QLineSeries *leftSeries = new QLineSeries(ui->qCustomPlot);
        for(int j = 0; j < points.size(); j++)
        {
            Point2D p = points[j];
            double x = p.x, y = p.y;
            leftSeries->append(QPointF(p.y,p.x));
            if(xMin > x)
            {
                xMin = x;
            }
            if(xMax < x)
            {
                xMax = x;
            }
            if(yMin > y)
            {
                yMin = y;
            }
            if(yMax < y)
            {
                yMax = y;
            }
        }
        leftChart->addSeries(leftSeries);
    }
    QLineSeries *line = new QLineSeries(ui->qCustomPlot);
    line->append(QPointF(yMin,xStartData));
    line->append(QPointF(yMax,xStartData));
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::black);
    line->setPen(pen);
    leftChart->addSeries(line);
    leftChart->createDefaultAxes();
    leftChart->legend()->hide();
    leftChart->axes(Qt::Horizontal).first()->setRange(yMin, yMax);
    leftChart->axes(Qt::Vertical).first()->setRange(xMin, xMax);
    leftChart->axisX()->hide();
    leftChart->axisY()->hide();
}
/**
 * @brief RightUpWidget::loadingVTKMode
 * 加载VTK数据
 */
void RightUpWidget::loadingVTKMode(ModelBases *modelObject)
{
    vtkSmartPointer<vtkFloatArray> pointArray = vtkSmartPointer<vtkFloatArray>::New();
    pointArray->SetNumberOfComponents(1);
    std::vector<std::vector<Point3D>>& lines = modelObject->getPointDatas();
    if(lines.size() <= 1)
    {
        return;
    }
    double min = 10000000, max = -100000000;
    for(int i = 0; i < lines.size(); i++)
    {
        std::vector<Point3D>& points = lines[i];
        for(int j = 0; j < points.size(); j++)
        {
            Point3D& p = points[j];
            pointArray->InsertNextValue(p.z);
            if(min > p.z)
            {
                min = p.z;
            }
            if(max < p.z)
            {
                max = p.z;
            }
        }
    }
}
/**
 * @brief RightUpWidget::drawTwoCurve
 * 跟随鼠标画交叉线
 * @param v  垂直竖线
 * @param h  水平横线
 */
void RightUpWidget::drawTwoCurve(double v, double h)
{
    spectrogramPlot->setH_MarkerPos(v,h);
    spectrogramPlot->setV_MarkerPos(v,h);
    this->update();
}
/**
 * @brief RightUpWidget::getMovePoint
 * 获取鼠标移动坐标
 * @param point
 */
void RightUpWidget::getMovePoint(QPointF point)
{
    movePoint = point;
    xStartData = point.x();
    yStartData = point.y();

    upDataUpCuve();
    upDataLeftCurve();
}
/**
 * @brief RightUpWidget::drawLabel
 * 画标住 0 代表NULL 1 代表矩形 2 代表圆形 3 代表文字
 * @param index
 */
void RightUpWidget::drawLabel(int index)
{
    if(spectrogramPlot)
    {
        spectrogramPlot->setDrawShapeType(index);
    }
}
/**
 * @brief RightUpWidget::on_checkBox_clicked
 * x轴范围设置复选框 true 自动调整  false 可进行设置
 * @param checked
 */
void RightUpWidget::on_checkBox_clicked(bool checked)
{
    if(checked)
    {
        ui->lineEdit->setEnabled(false);
        ui->lineEdit_2->setEnabled(false);
    }
    else
    {
        ui->lineEdit->setEnabled(true);
        ui->lineEdit_2->setEnabled(true);
    }
}
/**
 * @brief RightUpWidget::on_checkBox_clicked
 * y轴范围设置复选框 true 自动调整  false 可进行设置
 * @param checked
 */
void RightUpWidget::on_checkBox_2_clicked(bool checked)
{
    if(checked)
    {
        ui->lineEdit_3->setEnabled(false);
        ui->lineEdit_4->setEnabled(false);
    }
    else
    {
        ui->lineEdit_3->setEnabled(true);
        ui->lineEdit_4->setEnabled(true);
    }
}

void RightUpWidget::loadingContours()
{
    double xmin,xmax;
    for(int i=0;i< modelBases.size();i++)
    {
        ModelGraph *modelGraph = (ModelGraph *)modelBases[i];
        modelGraph->getcalBox(xmin,xmax);
    }
    newPolyData = new ModelGraph;
    newPolyData->loadPolyData("../Test02/model.stl");
    newPolyData->rotate(90, Normal(1, 0, 0),Point3D(0, 0, 0));
    newPolyData->calContour();
    double *center = newPolyData->getPolyData()->GetCenter();
    std::vector<Point3D> curvePoints = newPolyData->getCurvePoints();
    std::vector<Point3D> newPoints;
    double x_min = 9999,x_max = -9999;
    double y_min = 9999,y_max = -9999;
    for(int i=0;i<curvePoints.size();i++)
    {
        Point3D p = curvePoints[i];
        if(p.x < x_min)
        {
            x_min = p.x;
        }
        if(p.x > x_max)
        {
            x_max = p.x;
        }
        if(p.y < y_min)
        {
            y_min = p.y;
        }
        if(p.y > y_max)
        {
            y_max = p.y;
        }
    }

    double value = (xmax-xmin)/(x_max-x_min);
    double yvalue = (xmax-xmin)/(y_max-y_min);
    Point3D c = Point3D((x_max+x_min)/2, (y_max+y_min)/2, 0);
    VTKGeometry::movePoints(curvePoints, -c);
    for(int i=0;i<curvePoints.size();i++)
    {
        Point3D p = curvePoints[i];
        p.x = p.x*yvalue;
        p.y = p.y*yvalue;
        p.z = p.z*yvalue;
        newPoints.push_back(p);
    }
    spectrogramPlot->drawContoursCurve(newPoints);
    ui->widget_13->show();
}
/**
 * @brief RightUpWidget::on_lineEdit_textChanged
 * 当text进行改变时 先判断enable状态 在判断lineedit是否为空
 * @param arg1
 */
void RightUpWidget::on_lineEdit_textChanged(const QString &arg1)
{
    if(ui->lineEdit->isEnabled())
    {
        if(ui->lineEdit_2->text() != "")
        {
            double x1 = ui->lineEdit->text().toDouble();
            double x2 = ui->lineEdit_2->text().toDouble();
            // 设置x轴范围
            spectrogramPlot->getQwtMatrixRasterData()->setInterval(Qt::XAxis,QwtInterval( x1,x2, QwtInterval::ExcludeMaximum));
            spectrogramPlot->getQwtPlotSpectrogram()->setData(spectrogramPlot->getQwtMatrixRasterData());
            spectrogramPlot->replot();
        }
    }
}
/**
 * @brief RightUpWidget::on_lineEdit_2_textChanged
 * 当text进行改变时 先判断enable状态 在判断lineedit是否为空
 * @param arg1
 */
void RightUpWidget::on_lineEdit_2_textChanged(const QString &arg1)
{
    if(ui->lineEdit_2->isEnabled())
    {
        if(ui->lineEdit->text() != "")
        {
            double x1 = ui->lineEdit->text().toDouble();
            double x2 = ui->lineEdit_2->text().toDouble();
            // 设置x轴范围
            spectrogramPlot->getQwtMatrixRasterData()->setInterval(Qt::XAxis,QwtInterval( x1,x2, QwtInterval::ExcludeMaximum));
            spectrogramPlot->getQwtPlotSpectrogram()->setData(spectrogramPlot->getQwtMatrixRasterData());
            spectrogramPlot->replot();
        }
    }
}

/**
 * @brief RightUpWidget::on_lineEdit_3_textChanged
 * 当text进行改变时 先判断enable状态 在判断lineedit是否为空
 * @param arg1
 */
void RightUpWidget::on_lineEdit_3_textChanged(const QString &arg1)
{
    if(ui->lineEdit_3->isEnabled())
    {
        if(ui->lineEdit_4->text() != "")
        {
            double y1 = ui->lineEdit_3->text().toDouble();
            double y2 = ui->lineEdit_4->text().toDouble();
            // 设置y轴范围
            spectrogramPlot->getQwtMatrixRasterData()->setInterval(Qt::YAxis,QwtInterval( y1,y2, QwtInterval::ExcludeMaximum));
            spectrogramPlot->getQwtPlotSpectrogram()->setData(spectrogramPlot->getQwtMatrixRasterData());
            spectrogramPlot->replot();
        }
    }
}
/**
 * @brief RightUpWidget::on_lineEdit_4_textChanged
 * 当text进行改变时 先判断enable状态 在判断lineedit是否为空
 * @param arg1
 */
void RightUpWidget::on_lineEdit_4_textChanged(const QString &arg1)
{
    if(ui->lineEdit_4->isEnabled())
    {
        if(ui->lineEdit_3->text() != "")
        {
            double y1 = ui->lineEdit_3->text().toDouble();
            double y2 = ui->lineEdit_4->text().toDouble();
            // 设置y轴范围
            spectrogramPlot->getQwtMatrixRasterData()->setInterval(Qt::YAxis,QwtInterval( y1,y2, QwtInterval::ExcludeMaximum));
            spectrogramPlot->getQwtPlotSpectrogram()->setData(spectrogramPlot->getQwtMatrixRasterData());
            spectrogramPlot->replot();
        }
    }
}
/**
 * @brief RightUpWidget::on_verticalSlider_2_valueChanged
 * 上滑动条当值改变时 获取对应的比例 发送至左侧颜色条
 * @param value
 */
void RightUpWidget::on_verticalSlider_2_valueChanged(int value)
{
    upScaleNumber = value*0.01;
    spectrogramPlot->setZAxisValue(upScaleNumber,downScaleNumber);
    upDataUpCuve();
    upDataLeftCurve();
}
/**
 * @brief RightUpWidget::on_verticalSlider_valueChanged
 * 下滑动条当值改变时 获取对应的比例 发送至左侧颜色条
 * @param value
 */
void RightUpWidget::on_verticalSlider_valueChanged(int value)
{
    downScaleNumber = value*0.01;
    spectrogramPlot->setZAxisValue(upScaleNumber,downScaleNumber);
    upDataUpCuve();
    upDataLeftCurve();
}
/**
 * @brief RightUpWidget::on_tabWidget_tabBarClicked
 * 启动定时器  刷新GIF 动图
 * @param index
 */
void RightUpWidget::on_tabWidget_tabBarClicked(int index)
{

}
/**
 * @brief RightUpWidget::on_pushButton_clicked
 * x.y.z坐标轴旋转
 */
void RightUpWidget::on_pushButton_clicked()
{
    double angle_x = ui->lineEdit_7->text().toDouble();
    double angle_y = ui->lineEdit_8->text().toDouble();
    double angle_z = ui->lineEdit_9->text().toDouble();
    polyDataObject->rotate(angle_x, Normal(1, 0, 0), Point3D(0, 0, 0));
    polyDataObject->rotate(angle_y, Normal(0, 1, 0), Point3D(0, 0, 0));
    polyDataObject->rotate(angle_z, Normal(0, 0, 1), Point3D(0, 0, 0));
    polyDataObject->changePointAttribute(modelBases);
    modelViewer.updateView();
}
/**
 * @brief RightUpWidget::on_pushButton_2_clicked
 * 二维像轮廓图旋转
 */
void RightUpWidget::on_pushButton_2_clicked()
{
    double xmin,xmax;
    for(int i=0;i< modelBases.size();i++)
    {
        ModelGraph *modelGraph = (ModelGraph *)modelBases[i];
        modelGraph->getcalBox(xmin,xmax);
    }
    double angle_z = ui->lineEdit_12->text().toDouble();
    newPolyData->rotate(angle_z, Normal(0, 0, 1), Point3D(0, 0, 0));
    newPolyData->calContour();
    double *center = newPolyData->getPolyData()->GetCenter();
    std::vector<Point3D> curvePoints = newPolyData->getCurvePoints();
    std::vector<Point3D> newPoints;
    double x_min = 9999,x_max = -9999;
    double y_min = 9999,y_max = -9999;
    for(int i=0;i<curvePoints.size();i++)
    {
        Point3D p = curvePoints[i];
        if(p.x < x_min)
        {
            x_min = p.x;
        }
        if(p.x > x_max)
        {
            x_max = p.x;
        }
        if(p.y < y_min)
        {
            y_min = p.y;
        }
        if(p.y > y_max)
        {
            y_max = p.y;
        }
    }
    double value = (xmax-xmin)/(x_max-x_min);
    double yvalue = (xmax-xmin)/(y_max-y_min);
    Point3D c = Point3D((x_max+x_min)/2, (y_max+y_min)/2, 0);
    VTKGeometry::movePoints(curvePoints, -c);
    for(int i=0;i<curvePoints.size();i++)
    {
        Point3D p = curvePoints[i];
        p.x = p.x*yvalue;
        p.y = p.y*yvalue;
        p.z = p.z*yvalue;
        newPoints.push_back(p);
    }
    spectrogramPlot->drawContoursCurve(newPoints);
}
/**
 * @brief RightUpWidget::on_pushButton_3_clicked
 * 轮廓图缩小或放大的比例
 */
void RightUpWidget::on_pushButton_3_clicked()
{
    double scale = ui->lineEdit_10->text().toDouble();
    spectrogramPlot->drawScaleContoursCurve(scale);
}
