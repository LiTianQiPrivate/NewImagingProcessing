#ifndef MODELGRAPH_H
#define MODELGRAPH_H

#include <ModelBases.h>
#include <Point3D.h>
#include "PolyDataObject.h"
#include "vtkLookupTable.h"

class ModelGraph : public ModelBases, public PolyDataObject
{
public:
    explicit ModelGraph(QString className = "ModelGraph");
    ~ModelGraph();
    struct FEM_DATA
    {
        int n1,n2,n3; // 三角形的三条边映射序号
    };
    struct GRAPH_DATA
    {
        GRAPH_DATA(double v1, double v2, double v3, double v4)
        {
            x = v1;
            y = v2;
            mag = v3;
            phi = v4;
        }
        double x;
        double y;
        double mag;
        double phi;
    };
    void zoomPolyData(double xMin, double xMax);   // 按照包围和比例缩小或放大
    void getcalBox(double& xMin, double& xMax);    // 计算包围和
    void loadBackGroupFEM(QString filePath);       // 加载fem格式背景网格
    void drawBackGroupFEM(); // 解析fem数据后 构建背景网格
    void calContour();
public:
    std::vector<TASK_DATA> & getConditionsDatas();
    QString findDataInfo(QString key);
    bool findDataInfoFlag(QString key);
    void loadFile(QString filePath,QString fileNames = "");
    void makePolyData();
    void addPointAttribute(ModelGraph* modelGraph,QString fileName); // 添加一个属性
    void changePointAttribute(std::vector<ModelBases *> bases); // 修改属性
    std::vector<Point3D> getCurvePoints();
    std::vector<TASK_DATA> conditionsDatas;
    double rcsMin = 0;
    double rcsMax = 0;
    vtkSmartPointer<vtkLookupTable> pColorTable;
    std::vector<Point3D> curvePoints; // 三维模型轮廓线
    std::vector<Point3D> femPoints; // FEM背景网格点集
    std::vector<FEM_DATA> femData; // 构建三角形网格点集合
};

#endif // MODELGRAPH_H
