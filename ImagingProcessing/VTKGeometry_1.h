#ifndef VTKGEOMETRY_1_H
#define VTKGEOMETRY_1_H
#include "Point3D.h"
#include "vtkPolyData.h"
#include "vtkOBBTree.h"
#include "vtkCleanPolyData.h"
#include <vtkCell.h>
#include <vtkCellData.h>
#include <QString>
#include <QColor>

#define PI 3.1415926
class VTKGeometry_1
{
public:
    VTKGeometry_1();
    static void makeTriangles(std::vector<std::vector<Point3D>> points, vtkPolyData *polyData, bool isClose = false); // 多排点构建网格
};

#endif // VTKGEOMETRY_H
