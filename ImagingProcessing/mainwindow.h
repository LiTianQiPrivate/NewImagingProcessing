#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ModelTreeWidget.h"
#include "RightDownWidget.h"
#include "RightUpWidget.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public:
    ModelTreeWidget modelTreeWidget;
    RightDownWidget rightDownWidget;
    RightUpWidget rightUpWidget1;
    RightUpWidget rightUpWidget2;
    RightUpWidget rightUpWidget3;
private slots:
    void on_openAction_triggered();

    void on_rectAction_triggered();

    void on_elipseAction_triggered();

    void on_textAction_triggered();

    void on_pictureAction_triggered();

    void on_exitAction_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
