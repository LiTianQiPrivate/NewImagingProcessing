#include "TabWidget.h"
#include "ui_TabWidget.h"

TabWidget::TabWidget(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::TabWidget)
{
    ui->setupUi(this);
}

TabWidget::~TabWidget()
{
    delete ui;
}

void TabWidget::paintEvent(QPaintEvent *event)
{
    QTabWidget::paintEvent(event);
}
