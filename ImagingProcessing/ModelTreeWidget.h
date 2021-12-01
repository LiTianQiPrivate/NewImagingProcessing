#ifndef MODELTREEWIDGET_H
#define MODELTREEWIDGET_H

#include <QWidget>
#include "ModelBases.h"

namespace Ui {
class ModelTreeWidget;
}

class ModelTreeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ModelTreeWidget(QWidget *parent = nullptr);
    ~ModelTreeWidget();
    void addModelObject(ModelBases* object);
    void closeItem(); // 合并item
    void showItem(int index); // 展开

private:
    Ui::ModelTreeWidget *ui;
};

#endif // MODELTREEWIDGET_H
