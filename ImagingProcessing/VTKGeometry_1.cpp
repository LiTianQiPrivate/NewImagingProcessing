#include "VTKGeometry_1.h"
#include "vtkTriangleFilter.h"
#include <vtkDelaunay2D.h>
#include <vtkPolygon.h>
#include <vtkPoints.h>
#include <vtkKdTreePointLocator.h>
#include <vtkDataSet.h>
#include "vtkPolyDataConnectivityFilter.h"
#include "vtkClipPolyData.h"
#include "vtkPlanes.h"
#include "vtkPlane.h"
#include "vtkFeatureEdges.h"
#include "vtkStripper.h"
#include "vtkCleanPolyData.h"
#include "vtkCellLocator.h"
#include "vtkGenericCell.h"
#include "QDebug"
#include "vtkTransform.h"
#include "vtkTransformFilter.h"
#include "vtkSTLWriter.h"
#include <QTime>

VTKGeometry_1::VTKGeometry_1()
{

}
void VTKGeometry_1::makeTriangles(std::vector<std::vector<Point3D> > points, vtkPolyData *polyData, bool isClose)
{
    vtkSmartPointer<vtkPoints> outPoints = vtkSmartPointer<vtkPoints>::New();
    for(int i = 0; i < points.size(); i++)
    {
        std::vector<Point3D>& rowsPoints = points[i];
        for(int j = 0; j < rowsPoints.size(); j++)
        {
            Point3D p = rowsPoints[j];
            outPoints->InsertNextPoint(p.x, p.y, p.z);
        }

    }

    int rows = points.size();
    int cols = points[0].size();
    auto polys = vtkSmartPointer<vtkCellArray>::New();
    for(int i = 0; i < rows - 1; i++)
    {
        for(int j = 0; j < cols - 1; j++)
        {
            int v1 = i*cols + j;
            int v2 = (i+1)*cols + j+1;
            int v3 = (i+1)*cols + j;

            int vv1 = i*cols + j;
            int vv2 = i*cols + j+1;
            int vv3 = (i+1)*cols + j+1;
            polys->InsertNextCell(3);
            polys->InsertCellPoint(v1);
            polys->InsertCellPoint(v2);
            polys->InsertCellPoint(v3);
            polys->InsertNextCell(3);
            polys->InsertCellPoint(vv1);
            polys->InsertCellPoint(vv2);
            polys->InsertCellPoint(vv3);
        }
    }
    if(isClose)
    {
        int endRow = rows - 1;
        int startRow = 0;
        for(int j = 0; j < cols - 1; j++)
        {
            int v1 = endRow*cols + j;
            int v2 = startRow*cols + j+1;
            int v3 = startRow*cols + j;

            int vv1 = endRow*cols + j;
            int vv2 = endRow*cols + j+1;
            int vv3 = startRow*cols + j+1;
            polys->InsertNextCell(3);
            polys->InsertCellPoint(v1);
            polys->InsertCellPoint(v2);
            polys->InsertCellPoint(v3);
            polys->InsertNextCell(3);
            polys->InsertCellPoint(vv1);
            polys->InsertCellPoint(vv2);
            polys->InsertCellPoint(vv3);
        }
    }
    polyData->SetPoints(outPoints);
    polyData->SetPolys(polys);
    polyData->BuildCells();
    polyData->Modified();
}
