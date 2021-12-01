#include "ModelGraph.h"
#include <QTextStream>
#include <QFile>
#include <QFileInfo>
#include <sstream>
#include <QDebug>
#include "vtkDataArray.h"
#include "vtkFloatArray.h"
#include "VTKGeometry_1.h"
#include "vtkPointData.h"
#include "opencv.hpp"
#include "opencv2/opencv.hpp"
//#include "cv.h"
using namespace cv;

ModelGraph::ModelGraph(QString className) :
    ModelBases(className)
{
    conditionsDatas.push_back(TASK_DATA(QString::fromLocal8Bit("测量时刻")));
    conditionsDatas.push_back(TASK_DATA(QString::fromLocal8Bit("起始工作频率(GHz)")));
    conditionsDatas.push_back(TASK_DATA(QString::fromLocal8Bit("终止工作频率(GHz)")));
    conditionsDatas.push_back(TASK_DATA(QString::fromLocal8Bit("工作频率步长(MHz)")));
    conditionsDatas.push_back(TASK_DATA(QString::fromLocal8Bit("极化组合")));
    conditionsDatas.push_back(TASK_DATA(QString::fromLocal8Bit("目标距离（m）")));
    conditionsDatas.push_back(TASK_DATA(QString::fromLocal8Bit("目标高度（m）")));
    conditionsDatas.push_back(TASK_DATA(QString::fromLocal8Bit("天线高度（m）")));
    conditionsDatas.push_back(TASK_DATA(QString::fromLocal8Bit("目标俯仰角(°)")));
    conditionsDatas.push_back(TASK_DATA(QString::fromLocal8Bit("目标横滚角(°)")));
    conditionsDatas.push_back(TASK_DATA(QString::fromLocal8Bit("双站角（°）")));
    conditionsDatas.push_back(TASK_DATA(QString::fromLocal8Bit("起始目标方位角(°)")));
    conditionsDatas.push_back(TASK_DATA(QString::fromLocal8Bit("终止目标方位角(°)")));
    conditionsDatas.push_back(TASK_DATA(QString::fromLocal8Bit("目标方位角步长(°)")));
    conditionsDatas.push_back(TASK_DATA(QString::fromLocal8Bit("起始径向距离坐标(m)")));
    conditionsDatas.push_back(TASK_DATA(QString::fromLocal8Bit("终止径向距离坐标(m)")));
    conditionsDatas.push_back(TASK_DATA(QString::fromLocal8Bit("径向距离坐标步长(m)")));
    conditionsDatas.push_back(TASK_DATA(QString::fromLocal8Bit("起始横向距离坐标(m)")));
    conditionsDatas.push_back(TASK_DATA(QString::fromLocal8Bit("终止横向距离坐标(m)")));
    conditionsDatas.push_back(TASK_DATA(QString::fromLocal8Bit("横向距离坐标步长(m)")));
    conditionsDatas.push_back(TASK_DATA(QString::fromLocal8Bit("备注")));

    systemDatas.clear();
    systemDatas.push_back(TASK_DATA(QString::fromLocal8Bit(DEVE_NAME)));
    systemDatas.push_back(TASK_DATA(QString::fromLocal8Bit(WORK_SYSTEM)));
    systemDatas.push_back(TASK_DATA(QString::fromLocal8Bit("动态范围（dB）")));
    systemDatas.push_back(TASK_DATA(QString::fromLocal8Bit("发射机平均功率（W）")));
    systemDatas.push_back(TASK_DATA(QString::fromLocal8Bit(POWER_STABILITY)));
    systemDatas.push_back(TASK_DATA(QString::fromLocal8Bit(SENSITIVITY)));
    systemDatas.push_back(TASK_DATA(QString::fromLocal8Bit(NOISE_FACTOR)));
    systemDatas.push_back(TASK_DATA(QString::fromLocal8Bit(LINEARITY)));
    systemDatas.push_back(TASK_DATA(QString::fromLocal8Bit(FRE_STABILITY)));
    systemDatas.push_back(TASK_DATA(QString::fromLocal8Bit(REMARK)));
    pColorTable = vtkSmartPointer<vtkLookupTable>::New();
}

ModelGraph::~ModelGraph()
{

}

void ModelGraph::zoomPolyData(double xMin, double xMax)
{
    double planexMin, planexMax; // 飞机模型的包围盒
    getcalBox(planexMin, planexMax);
    double* center = polyData->GetCenter();
    double value = (xMax - xMin)/(planexMax - planexMin);
    vtkPoints* newPoints = polyData->GetPoints();
    for(int i = 0; i < polyData->GetNumberOfPoints(); i++)
    {
        double* p = polyData->GetPoint(i);
        p[0] = p[0] - center[0];
        p[1] = p[1] - center[1];
        p[2] = p[2] - center[2];
        p[0] = p[0]*value;
        p[1] = p[1]*value;
        newPoints->SetPoint(i, p[0], p[1], p[2]);
    }
    polyData->SetPoints(newPoints);
}

void ModelGraph::getcalBox(double &xMin, double &xMax)
{
    polyData->ComputeBounds();
    double* box = polyData->GetBounds();
    xMin = box[0];
    xMax = box[1];
}

void ModelGraph::loadBackGroupFEM(QString filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return ;
    }
    QFileInfo fileInfo = QFileInfo(filePath);
    this->fileName = fileInfo.fileName();
    this->oriFileName = fileInfo.fileName();
    QTextStream in(&file);
    QString line = in.readLine();
    while(!line.isNull())
    {
        line = in.readLine();
        if(line.size() < 47)
        {
            continue;
        }
        if(line.left(4) == "GRID")
        {
            std::istringstream s(line.toStdString());
            int size = line.size();
            String str1;
            int number;
            s >> str1 >> number;
            QString xPoint = line.mid(24,8);
            QString yPoint = line.mid(32,8);
            QString zPoint = line.mid(40,8);
            Point3D p(xPoint.toDouble(),yPoint.toDouble(),zPoint.toDouble());
            femPoints.push_back(p);
        }
        else if(line.left(6) == "CTRIA3")
        {
            std::istringstream s(line.toStdString());
            String str;
            int number1,number2,number3,number4,number5;
            s >> str >> number1 >> number2 >> number3 >> number4 >> number5;
            FEM_DATA p;
            p.n1 = number3-1;
            p.n2 = number4-1;
            p.n3 = number5-1;
            femData.push_back(p);
        }
        else
        {
            continue;
        }
    }
}

void ModelGraph::drawBackGroupFEM()
{
    if(femData.size() < 1)
    {
        return ;
    }
    vtkSmartPointer<vtkPoints> outPoints = vtkSmartPointer<vtkPoints>::New();
    for(int i=0;i<femPoints.size();i++)
    {
        Point3D p = femPoints[i];
        outPoints->InsertNextPoint(p.x,p.y,p.z);
    }
    auto polys = vtkSmartPointer<vtkCellArray>::New();
    for(int i=0;i< femData.size();i++)
    {
        FEM_DATA p = femData[i];
        polys->InsertNextCell(3);
        polys->InsertCellPoint(p.n1);
        polys->InsertCellPoint(p.n2);
        polys->InsertCellPoint(p.n3);
    }
    polyData->SetPoints(outPoints);
    polyData->SetPolys(polys);
    polyData->BuildCells();
    polyData->Modified();
}
/**
 * @brief ModelGraph::calContour
 * 计算轮廓图
 */
void ModelGraph::calContour()
{
    if(polyData->GetNumberOfPoints() < 0)
    {
        return ;
    }
    double xMin = 1000000,xMax = -1000000,yMin = 1000000,yMax = -1000000;
    for(int i=0;i<polyData->GetNumberOfPoints();i++)
    {
        double *p = polyData->GetPoint(i);
        if(xMin > p[0])
        {
            xMin = p[0];
        }
        if(xMax < p[0])
        {
            xMax = p[0];
        }
        if(yMin > p[1])
        {
            yMin = p[1];
        }
        if(yMax < p[1])
        {
            yMax = p[1];
        }
    }
    int w = (int)(xMax - xMin);
    int h = (int)(yMax - yMin);

    std::vector<cv::Point2i> point2s;
    for(int i = 0;i< polyData->GetNumberOfPoints();i++)
    {
        double *p =polyData->GetPoint(i);
        cv::Point2i pp;
        pp.x = int(p[0] - xMin)+10;
        pp.y = int(p[1] - yMin)+10;
        point2s.push_back(pp);
    }
    cv::Mat mat = cv::Mat(h+100,w+100,CV_8UC1,cv::Scalar(0));
    for(int i = 0;i<point2s.size();i++)
    {
        cv::Point2i p = point2s[i];
        mat.at<uchar>(p.y,p.x) = 255;
    }

    Mat structureElement = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(15,15),cv::Point(-1,-1));
    Mat dst;
    dilate(mat,dst,structureElement);
    erode(dst,mat,structureElement);
    cv::blur(mat,mat,Size(9,9));
    int threshold = 100;
    for(int i =0;i < mat.rows;i++)
    {
        uchar *p = mat.ptr<uchar>(i);
        for(int j=0;j<mat.cols;j++)
        {
            if(p[j] < threshold)
                p[j] = 0;
            else p[j] = 255;
        }
    }

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mat,contours,RETR_EXTERNAL, CHAIN_APPROX_NONE);
    curvePoints.clear();
    for(int i=0;i<contours.size();i++)
    {
        std::vector<cv::Point> points = contours[i];
        for(int j=0;j<points.size();j++)
        {
            cv::Point p = points[j];
            curvePoints.push_back(Point3D(p.x,p.y,0));
        }
    }
}

std::vector<ModelBases::TASK_DATA> &ModelGraph::getConditionsDatas()
{
    return conditionsDatas;
}

QString ModelGraph::findDataInfo(QString key)
{
    for(int i = 0; i < taskDatas.size(); i++)
    {
        if(key == taskDatas[i].key)
        {
            return taskDatas[i].value;
        }
    }
    for(int i = 0; i < systemDatas.size(); i++)
    {
        if(key == systemDatas[i].key)
        {
            return systemDatas[i].value;
        }
    }
    for(int i = 0; i < conditionsDatas.size(); i++)
    {
        if(key == conditionsDatas[i].key)
        {
            return conditionsDatas[i].value;
        }
    }
    return "";
}

bool ModelGraph::findDataInfoFlag(QString key)
{
    for(int i = 0; i < taskDatas.size(); i++)
    {
        if(key == taskDatas[i].key)
        {
            return true;
        }
    }
    for(int i = 0; i < systemDatas.size(); i++)
    {
        if(key == systemDatas[i].key)
        {
            return true;
        }
    }
    for(int i = 0; i < conditionsDatas.size(); i++)
    {
        if(key == conditionsDatas[i].key)
        {
            return true;
        }
    }
    return false;
}
/**
 * @brief ModelGraph::loadFile
 * 加载.2dGraph格式文件数据
 * @param filePath
 */
void ModelGraph::loadFile(QString filePath, QString fileNames)
{
    QFile file;
    if(fileNames == "")
    {
        file.setFileName(filePath);
    }
    else
    {
        file.setFileName(fileNames + "/" + filePath);
    }
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    rcsMin = INT_MAX;
    rcsMax = INT_MIN;
    QFileInfo fileInfo = QFileInfo(filePath);
    this->fileName = fileInfo.fileName();
    this->oriFileName = fileInfo.fileName();
    QTextStream in(&file);
    QString line = in.readLine();
    while(!line.isNull())
    {
        line = in.readLine();
        if(line.contains("[TASK]"))
        {
            for(int i = 0; i < taskDatas.size(); i++)
            {
                line = in.readLine();
                if(line.contains("[/TASK]"))
                {
                    break;
                }
                for(int j = 0; j < taskDatas.size(); j++)
                {
                    if(line.contains(taskDatas[j].key))
                    {
                        QString str = line.section('=',1,1);
                        taskDatas[j].value = str;
                        break;
                    }
                }
            }
        }

        if(line.contains("[SYSTEM]"))
        {
            for(int i = 0; i < systemDatas.size(); i++)
            {
                line = in.readLine();
                if(line.contains("[/SYSTEM]"))
                {
                    break;
                }
                for(int j = 0; j < systemDatas.size(); j++)
                {
                    if(line.contains(systemDatas[j].key))
                    {
                        QString str = line.section('=',1,1);
                        systemDatas[j].value = str;
                        break;
                    }
                }
            }
        }

        if(line.contains("[CONDITIONS]"))
        {
            for(int i = 0; i < conditionsDatas.size(); i++)
            {
                line = in.readLine();
                if(line.contains("[/CONDITIONS]"))
                {
                    break;
                }
                for(int j = 0; j < conditionsDatas.size(); j++)
                {
                    if(line.contains(conditionsDatas[j].key))
                    {
                        QString str = line.section('=',1,1);
                        conditionsDatas[j].value = str;
                        break;
                    }
                }
            }
        }
        pointDatas.clear();
        if(line.contains("[DATA]"))
        {
            std::vector<Point3D> points;
            line = in.readLine();
            line = in.readLine();
            while(line != "")
            {
                std::istringstream s(line.toStdString());
                double v1,v2, v3, v4;
                s >> v1 >> v2 >> v3 >> v4;
                if(rcsMin > v3)
                {
                    rcsMin = v3;
                }
                if(rcsMax < v3)
                {
                    rcsMax = v3;
                }
                if(points.size() > 0)
                {
                    if(fabs(v1-points[points.size()-1].x) > 0.01)
                    {
                        pointDatas.push_back(points);
                        points.clear();
                    }
                }
                points.push_back(Point3D(v1, v2, v3));
                line = in.readLine();
            }
        }
    }
    makePolyData();
}

void ModelGraph::makePolyData()
{
    vtkSmartPointer<vtkFloatArray> pointArray = vtkSmartPointer<vtkFloatArray>::New();
    pointArray->SetNumberOfComponents(1);
    if(pointDatas.size() <= 1)
    {
        return;
    }
    for(int i = 0; i < pointDatas.size(); i++)
    {
        std::vector<Point3D>& points = pointDatas[i];
        for(int j = 0; j < points.size(); j++)
        {
            Point3D& p = points[j];
            pointArray->InsertNextValue(p.z);
//            p.z = 0;
        }
    }
    VTKGeometry_1::makeTriangles(pointDatas, polyData, false);

//    // 添加颜色映射与颜色条窗口
    pColorTable->SetRange(rcsMin, rcsMax);
    pColorTable->SetTableRange(rcsMin, rcsMax);
    pColorTable->SetHueRange(0.6, 0);//HSV  H
    pColorTable->Build();
    polyData->GetPointData()->AddArray(pointArray);
    getActor()->GetMapper()->SetLookupTable(pColorTable);
    getActor()->GetMapper()->SetScalarRange(rcsMin, rcsMax);
    getActor()->GetMapper()->ScalarVisibilityOn();
}
/**
 * @brief ModelGraph::addPointAttribute
 * 将modelGraph数据映射到模型中
 * @param modelGraph 数据类型
 * @param fileName  通过名字设置属性
 */
void ModelGraph::addPointAttribute(ModelGraph *modelGraph, QString fileName)
{
    modelGraph->updataKdtree();
    vtkSmartPointer<vtkFloatArray> pointArray = vtkSmartPointer<vtkFloatArray>::New();
    pointArray->SetNumberOfComponents(1);
    pointArray->SetName(fileName.toStdString().c_str());
    rcsMin = 1000000;
    rcsMax = -1000000;
    for(int i = 0; i < polyData->GetNumberOfPoints(); i++)
    {
        double* p = polyData->GetPoint(i);
        int index = modelGraph->findClosestPoint(Point3D(p[0], p[1], p[2]));
        double* a = modelGraph->getPolyData()->GetPointData()->GetArray(0)->GetTuple(index);
        pointArray->InsertNextValue(a[0]);
//        pointArray->SetValue();
        if(rcsMin > a[0])
        {
            rcsMin = a[0];
        }
        if(rcsMax < a[0])
        {
            rcsMax = a[0];
        }
    }

    //    // 添加颜色映射与颜色条窗口
    pColorTable->SetRange(rcsMin, rcsMax);
    pColorTable->SetTableRange(rcsMin, rcsMax);
    pColorTable->SetHueRange(0.6, 0);//HSV  H
    pColorTable->Build();
//    vtkPointData* pointData = polyData->GetPointData();
//    pointData->AddArray(pointArray);
    //getPolyData()->GetPointData()->GetArray(0);
    getPolyData()->GetPointData()->AddArray(pointArray);
    getActor()->GetMapper()->SetLookupTable(pColorTable);
    getActor()->GetMapper()->SetScalarRange(rcsMin, rcsMax);
    getActor()->GetMapper()->GetInput()->GetPointData()->SetActiveScalars(fileName.toStdString().c_str());
    getActor()->GetMapper()->SelectColorArray(fileName.toStdString().c_str());
    getActor()->GetMapper()->SetScalarModeToUsePointData();
    getActor()->GetMapper()->ScalarVisibilityOn();
}
/**
 * @brief ModelGraph::changePointAttribute
 * 每次旋转后  重新计算映射  找到模型的最近点 进行投影
 * @param bases
 */
void ModelGraph::changePointAttribute(std::vector<ModelBases *> bases)
{
    for(int j= 0;j<bases.size(); j++)
    {
        ModelGraph *modelGraph = (ModelGraph *)bases[j];
        modelGraph->updataKdtree();
        getPolyData()->GetPointData()->GetNumberOfTuples();
        vtkSmartPointer<vtkDataArray> pointArray = getPolyData()->GetPointData()->GetArray(j);
        rcsMin = 1000000;
        rcsMax = -1000000;
        for(int i = 0; i < polyData->GetNumberOfPoints(); i++)
        {
            double* p = polyData->GetPoint(i);
            int index = modelGraph->findClosestPoint(Point3D(p[0], p[1], p[2]));
            double* a = modelGraph->getPolyData()->GetPointData()->GetArray(j)->GetTuple(index);
            pointArray->SetTuple(i,a);
    //        pointArray->SetValue();
            if(rcsMin > a[0])
            {
                rcsMin = a[0];
            }
            if(rcsMax < a[0])
            {
                rcsMax = a[0];
            }
        }
    }
}

std::vector<Point3D> ModelGraph::getCurvePoints()
{
    return curvePoints;
}
