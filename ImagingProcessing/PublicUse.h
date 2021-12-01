#ifndef PUBLICUSE_H
#define PUBLICUSE_H

#include <QString>
#include "ModelBases.h"
class ModelTreeWidget;
class RightDownWidget;
class RightUpWidget;
class PublicUse
{
public:
    PublicUse();
    static PublicUse *getPublicUse();
    static PublicUse *publicUse;
    ModelBases* loadModelBases(QString fileName, QString fileNames = "");
    std::vector<ModelBases*> modelObjects; // 文件数据
public:
    ModelTreeWidget *getModelTreeWidget();
    void setModelTreeWidget(ModelTreeWidget *value);
    ModelTreeWidget* modelTreeWidget = NULL;
public:
    RightDownWidget *getRightDownWidget();
    void setRightDownWidget(RightDownWidget *value);
    RightDownWidget *rightDownWidget = nullptr;
public:
    RightUpWidget *getRightUpWidget1();
    void setRightUpWidget1(RightUpWidget *value);
    RightUpWidget *rightUpWidget1 = nullptr;
public:
    RightUpWidget *getRightUpWidget2();
    void setRightUpWidget2(RightUpWidget *value);
    RightUpWidget *rightUpWidget2 = nullptr;
public:
    RightUpWidget *getRightUpWidget3();
    void setRightUpWidget3(RightUpWidget *value);
    RightUpWidget *rightUpWidget3 = nullptr;
};
#endif // PUBLICUSE_H
