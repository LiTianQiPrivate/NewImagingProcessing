#include "RightDownWidget.h"
#include "ui_RightDownWidget.h"
#include "XYZWidget.h"
#include <QMenu>
#include "PublicUse.h"
#include "ModelTreeWidget.h"
#include "RightUpWidget.h"
RightDownWidget::RightDownWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RightDownWidget)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(9);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QStringList header;
    header << QString::fromLocal8Bit(NUMBER_NAME)
           << QString::fromLocal8Bit("选择")
           << QString::fromLocal8Bit(DATA_TYPE)
           << QString::fromLocal8Bit(TARGET_NAME)
           << QString::fromLocal8Bit("目标状态")
           << QString::fromLocal8Bit(TARGET_PITCH_ANGLE)
           << QString::fromLocal8Bit(TARGET_ROLL_ANGLE)
           << QString::fromLocal8Bit(WORK_FPS)
           << QString::fromLocal8Bit(POLARIZATION_COMBINATION);
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); // 整行选取
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
    ui->tableWidget->setDragEnabled(true);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
}

RightDownWidget::~RightDownWidget()
{
    delete ui;
}
/**
 * @brief RightDownWidget::addModelBases
 * 添加数据
 * @param object
 */
void RightDownWidget::addModelBases(ModelBases *object)
{
    modelBases.push_back(object);
    if(object->getClassName() == "ModelRCSF")
    {
        XYZWidget xyzWidget;
        xyzWidget.setModelRCSF((ModelRCSF*)object);
        xyzWidget.exec();
        if(xyzWidget.getIsOK())
        {
            ModelRCSF* model = (ModelRCSF*)object;
            int xType;
            double value;
            xyzWidget.getReturn(xType, value);
            model->setXStype(xType);
            model->setCurrentValue(value);
        }
    }
    QTableWidgetItem *check = new QTableWidgetItem;
    check->setCheckState(Qt::Checked);
    check->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    int rows = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rows);
    ui->tableWidget->setItem(rows,0,new QTableWidgetItem(QString::number(rows+1)));
    ui->tableWidget->setItem(rows,1,check);
    ui->tableWidget->setItem(rows,2,new QTableWidgetItem(object->findDataInfo(QString::fromLocal8Bit(DATA_TYPE))));
    ui->tableWidget->setItem(rows,3,new QTableWidgetItem(object->findDataInfo(QString::fromLocal8Bit(TARGET_NAME))));
    ui->tableWidget->setItem(rows,4,new QTableWidgetItem(object->findDataInfo(QString::fromLocal8Bit(QUALITY_GRADE))));
    ui->tableWidget->setItem(rows,5,new QTableWidgetItem(object->findDataInfo(QString::fromLocal8Bit(TARGET_PITCH_ANGLE))));
    ui->tableWidget->setItem(rows,6,new QTableWidgetItem(object->findDataInfo(QString::fromLocal8Bit(TARGET_ROLL_ANGLE))));
    ui->tableWidget->setItem(rows,7,new QTableWidgetItem(object->findDataInfo(QString::fromLocal8Bit(WORK_FPS))));
    ui->tableWidget->setItem(rows,8,new QTableWidgetItem(object->findDataInfo(QString::fromLocal8Bit(POLARIZATION_COMBINATION))));
    for (int i=0;i<ui->tableWidget->rowCount();i++)
    {
       for (int j=0;j<ui->tableWidget->columnCount();j++)
       {
           ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
       }
    }
}
/**
 * @brief RightDownWidget::on_tableWidget_customContextMenuRequested
 * QTableWidget右键  槽函数
 * @param pos
 */
void RightDownWidget::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    if(ui->tableWidget->rowCount() == 0)
    {
        return ;
    }
    QMenu menu;
    menu.addAction(ui->action_1);
    menu.addAction(ui->action_2);
    menu.addAction(ui->action_3);
    menu.exec(QCursor::pos());
}
/**
 * @brief RightDownWidget::on_action_1_triggered
 * 窗口1添加
 */
void RightDownWidget::on_action_1_triggered()
{
    std::vector<ModelBases*> modelBase;
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        QTableWidgetItem* item = ui->tableWidget->item(i, 1);
        if( ui->tableWidget->isItemSelected(item))
        {
            PublicUse::getPublicUse()->getRightUpWidget1()->addModelBases(modelBases[i]);
            //modelBases.push_back(modelBases[i]);
        }
    }
}
/**
 * @brief RightDownWidget::on_action_2_triggered
 * 窗口2添加
 */
void RightDownWidget::on_action_2_triggered()
{
    std::vector<ModelBases*> modelBase;
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        QTableWidgetItem* item = ui->tableWidget->item(i, 1);
        if( ui->tableWidget->isItemSelected(item))
        {
            PublicUse::getPublicUse()->getRightUpWidget2()->addModelBases(modelBases[i]);
            //modelBases.push_back(modelBases[i]);
        }
    }
}
/**
 * @brief RightDownWidget::on_action_3_triggered
 * 窗口3添加
 */
void RightDownWidget::on_action_3_triggered()
{
    std::vector<ModelBases*> modelBase;
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        QTableWidgetItem* item = ui->tableWidget->item(i, 1);
        if( ui->tableWidget->isItemSelected(item))
        {
            PublicUse::getPublicUse()->getRightUpWidget3()->addModelBases(modelBases[i]);
            //modelBases.push_back(modelBases[i]);
        }
    }
}
/**
 * @brief RightDownWidget::on_tableWidget_itemClicked
 * 单机item 左侧展开列表显示数据
 * @param item
 */
void RightDownWidget::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    PublicUse::getPublicUse()->getModelTreeWidget()->closeItem();
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        QTableWidgetItem* item = ui->tableWidget->item(i, 1);
        if( ui->tableWidget->isItemSelected(item))
        {
            PublicUse::getPublicUse()->getModelTreeWidget()->showItem(i);
        }
    }
}
