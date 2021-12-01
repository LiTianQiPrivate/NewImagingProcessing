#include "PublicUse.h"
#include <QFileInfo>
#include "ModelRAM.h"
#include "ModelRCS.h"
#include "ModelRCSF.h"
#include "ModelGraph.h"

PublicUse* PublicUse::publicUse = NULL;
PublicUse::PublicUse()
{

}

PublicUse *PublicUse::getPublicUse()
{
    if(publicUse == NULL)
    {
        publicUse = new PublicUse();
    }
    return publicUse;
}
/**
 * @brief Business::loadModelBase
 * 加载文件转换为modelBases格式
 * @param fileName
 * @return
 */
ModelBases* PublicUse::loadModelBases(QString fileName,QString fileNames)
{
    ModelBases* modelBase = NULL;
    QFileInfo fileinfo = QFileInfo(fileName);
    QString file_suffix = fileinfo.suffix();
    if(file_suffix == "ram")
    {
        ModelRAM* modelObject = new ModelRAM;
        modelObject->loadFile(fileName);
        modelBase = modelObject;
    } else if(file_suffix == "rcs") {
        ModelRCS* modelObject = new ModelRCS;
        modelObject->loadFile(fileName);
        modelBase = modelObject;
    } else if(file_suffix == "rcsF" || file_suffix == "rcsf" ) {
        ModelRCSF* modelObject = new ModelRCSF;
        modelObject->loadFile(fileName);
        modelBase = modelObject;
    }
    else if(file_suffix == "2dGraph")
    {
        ModelGraph* modelObject = new ModelGraph;
        modelObject->loadFile(fileName,fileNames);
        modelBase = modelObject;
    }
    if(modelBase)
    {
        modelObjects.push_back(modelBase);
    }
    return modelBase;
}

ModelTreeWidget *PublicUse::getModelTreeWidget()
{
     return modelTreeWidget;
}

void PublicUse::setModelTreeWidget(ModelTreeWidget *value)
{
    modelTreeWidget = value;
}

RightDownWidget *PublicUse::getRightDownWidget()
{
    return rightDownWidget;
}

void PublicUse::setRightDownWidget(RightDownWidget *value)
{
    rightDownWidget = value;
}

RightUpWidget *PublicUse::getRightUpWidget1()
{
    return rightUpWidget1;
}

void PublicUse::setRightUpWidget1(RightUpWidget *value)
{
    rightUpWidget1 = value;
}

RightUpWidget *PublicUse::getRightUpWidget2()
{
    return rightUpWidget2;
}

void PublicUse::setRightUpWidget2(RightUpWidget *value)
{
    rightUpWidget2 = value;
}
RightUpWidget *PublicUse::getRightUpWidget3()
{
    return rightUpWidget3;
}

void PublicUse::setRightUpWidget3(RightUpWidget *value)
{
    rightUpWidget3 = value;
}
