﻿#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>

namespace Ui {
class TabWidget;
}

class TabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit TabWidget(QWidget *parent = nullptr);
    ~TabWidget();
    void paintEvent(QPaintEvent *event);
private:
    Ui::TabWidget *ui;
};

#endif // TABWIDGET_H