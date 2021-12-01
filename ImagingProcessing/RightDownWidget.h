#ifndef RIGHTDOWNWIDGET_H
#define RIGHTDOWNWIDGET_H

#include <QWidget>
#include "ModelBases.h"
#include <QTableWidgetItem>
namespace Ui {
class RightDownWidget;
}

class RightDownWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RightDownWidget(QWidget *parent = nullptr);
    ~RightDownWidget();
    void addModelBases(ModelBases *object);
    std::vector<ModelBases *> modelBases;
private:
    Ui::RightDownWidget *ui;
private slots:
    void on_tableWidget_customContextMenuRequested(const QPoint &pos);
    void on_action_1_triggered();
    void on_action_2_triggered();
    void on_action_3_triggered();
    void on_tableWidget_itemClicked(QTableWidgetItem *item);
};

#endif // RIGHTDOWNWIDGET_H
