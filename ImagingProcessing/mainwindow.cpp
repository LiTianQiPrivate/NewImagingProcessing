#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "PublicUse.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString qss;
//    QFile qssFile("../ImagingProcessing/QSS/myQss.qss");
//    qssFile.open(QFile::ReadOnly);
//    if(qssFile.isOpen())
//    {
//        qss = QLatin1String(qssFile.readAll());
//        qApp->setStyleSheet(qss);
//        qssFile.close();
//    }
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(&modelTreeWidget);
    ui->widget_5->setLayout(layout);
    PublicUse::getPublicUse()->setModelTreeWidget(&modelTreeWidget);
    QHBoxLayout *rightDownLayout = new QHBoxLayout;
    rightDownLayout->addWidget(&rightDownWidget);
    ui->widget_6->setLayout(rightDownLayout);
    PublicUse::getPublicUse()->setRightDownWidget(&rightDownWidget);
    QHBoxLayout *rightUpLayout1 = new QHBoxLayout;
    rightUpLayout1->addWidget(&rightUpWidget1);
    ui->widget_2->setLayout(rightUpLayout1);
    QHBoxLayout *rightUpLayout2 = new QHBoxLayout;
    rightUpLayout2->addWidget(&rightUpWidget2);
    ui->widget_3->setLayout(rightUpLayout2);
    QHBoxLayout *rightUpLayout3 = new QHBoxLayout;
    rightUpLayout3->addWidget(&rightUpWidget3);
    ui->widget_4->setLayout(rightUpLayout3);
    PublicUse::getPublicUse()->setRightUpWidget1(&rightUpWidget1);
    PublicUse::getPublicUse()->setRightUpWidget2(&rightUpWidget2);
    PublicUse::getPublicUse()->setRightUpWidget3(&rightUpWidget3);
    QFile file("./fileList.txt");   // 读取数据文件
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << file.errorString();
    }
    else
    {
        QTextStream inputFile(&file);
        QString line = inputFile.readLine();
        QVector<QString> inputList;
        while(!line.isNull())
        {
            inputList.push_back(line);
            line = inputFile.readLine();
        }
        // 减5是为了去掉.mpro的后缀
        QString fileNames = inputList[0].left(inputList[0].length()-5) + "/data";
        qDebug() << "fileNames == " << fileNames;
        for(int i=1;i<inputList.size();i++)
        {
            // inputList[0] 为工程名字  后续的为文件名字
            QDir dir(fileNames);
            dir.setFilter(QDir::Files);  //  设置过滤器  只加载文件
            QStringList list = dir.entryList(QDir::Files);
            for(int j=0;j<list.size();j++)
            {
                QFileInfo file = list[j];
                if(file.suffix() == "2dGraph")
                {
                    if(file.fileName() == inputList[i])
                    {
                        ModelBases* modelObject = PublicUse::getPublicUse()->loadModelBases(list[j],fileNames);
                        modelTreeWidget.addModelObject(modelObject);
                        rightDownWidget.addModelBases(modelObject);
                    }
                }
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
/**
 * @brief MainWindow::on_openAction_triggered
 * 选择文件打开
 */
void MainWindow::on_openAction_triggered()
{
    QStringList fileList = QFileDialog::getOpenFileNames(this, tr("file names"), "./", "*.ram *.rcs *.rcsf *.2dGraph");
    for(int i = 0; i < fileList.size(); i++)
    {
        ModelBases* modelObject = PublicUse::getPublicUse()->loadModelBases(fileList[i].toUtf8());
        modelTreeWidget.addModelObject(modelObject);
        rightDownWidget.addModelBases(modelObject);
    }
}
/**
 * @brief MainWindow::on_rectAction_triggered
 * 矩形标注
 */
void MainWindow::on_rectAction_triggered()
{
    PublicUse::getPublicUse()->getRightUpWidget1()->drawLabel(2);
    PublicUse::getPublicUse()->getRightUpWidget2()->drawLabel(2);
    PublicUse::getPublicUse()->getRightUpWidget3()->drawLabel(2);
}

void MainWindow::on_elipseAction_triggered()
{
    PublicUse::getPublicUse()->getRightUpWidget1()->drawLabel(1);
    PublicUse::getPublicUse()->getRightUpWidget2()->drawLabel(1);
    PublicUse::getPublicUse()->getRightUpWidget3()->drawLabel(1);
}

void MainWindow::on_textAction_triggered()
{
    PublicUse::getPublicUse()->getRightUpWidget1()->drawLabel(3);
    PublicUse::getPublicUse()->getRightUpWidget2()->drawLabel(3);
    PublicUse::getPublicUse()->getRightUpWidget3()->drawLabel(3);
}

void MainWindow::on_pictureAction_triggered()
{
    PublicUse::getPublicUse()->getRightUpWidget1()->drawLabel(4);
    PublicUse::getPublicUse()->getRightUpWidget2()->drawLabel(4);
    PublicUse::getPublicUse()->getRightUpWidget3()->drawLabel(4);
}

void MainWindow::on_exitAction_triggered()
{
    PublicUse::getPublicUse()->getRightUpWidget1()->drawLabel(8);
    PublicUse::getPublicUse()->getRightUpWidget2()->drawLabel(8);
    PublicUse::getPublicUse()->getRightUpWidget3()->drawLabel(8);
}
