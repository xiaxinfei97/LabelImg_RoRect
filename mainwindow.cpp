#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include<QDir>
#include<QDebug>
#include"setting.h"
#include<string>
#include<opencv2/core/core.hpp>
#include"objectschoose.h"
#include<Eigen/Eigen>
#include"readandwritexml.h"
#include<QMessageBox>



#include<QImageReader>

#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 窗体设计
    this->setWindowState(Qt::WindowMaximized);
    this->ui->_picLabel->SetObjectList({QString("car"),QString("person")},{QColor(0,255,255),QColor(0,255,0)});
    // 信号与槽
    connect(ui->_openDirBtn,&QPushButton::clicked,[=](){
        ImportFile();
    });//选择标注图片路径
    connect(ui->_setBtn,&QPushButton::clicked, [=](){
        QVector<QString> claslist;
        QVector<QColor> colorlist;
        ui->_picLabel->GetObjectList(claslist,colorlist);
        Setting settingDlg(nullptr,claslist,colorlist,m_copy);
        if(settingDlg.exec() == QDialog::Accepted)
        {
            QVector<QString> classlist;
            QVector<QColor> colorlist;
            bool copy;
            settingDlg.GetObjectList(classlist, colorlist, copy);
            ui->_picLabel->SetObjectList(classlist,colorlist);
            m_copy = copy;
        }
    });//打开设置界面
    connect(ui->_drawRectBtn,&QPushButton::clicked,[=](){
        ui->_picLabel->SetIsDraw(true);
        ui->_picLabel->SetDrawType(DRAWTYPE::TypeOfRect);
    });
    connect(ui->_DrawRoReactBtn,&QPushButton::clicked,[=](){
        ui->_picLabel->SetIsDraw(true);
        ui->_picLabel->SetDrawType(DRAWTYPE::TypeOfRoRect);
    });
    connect(ui->_DrawTrapezoidBtn,&QPushButton::clicked,[=](){
        ui->_picLabel->SetIsDraw(true);
        ui->_picLabel->SetDrawType(DRAWTYPE::TypeOfPoly);
    });
    connect(ui->_backwardBtn,SIGNAL(clicked()),this,SLOT(Next()));
    connect(ui->_forwardBtn,SIGNAL(clicked()),this,SLOT(Back()));

    connect(ui->_saveBtn,SIGNAL(clicked()),this,SLOT(SaveXml()));

    connect(ui->listWidget,&QListWidget_Tztek::currentRowChanged,[=](){
        SetPixmap();
    });
    connect(ui->_picLabel,&QLabel_Tztek::newObject,ui->tableWidget,&QTableWidget_Tztek::AddRow);
    connect(ui->_picLabel,&QLabel_Tztek::chooseObject,ui->tableWidget,&QTableWidget_Tztek::ChooseRows);
    connect(ui->_picLabel,&QLabel_Tztek::deleteObject,ui->tableWidget,&QTableWidget_Tztek::DeleteRows);
    connect(ui->_picLabel,&QLabel_Tztek::deleteAllObject,ui->tableWidget,&QTableWidget_Tztek::DeleteAllRows);
    connect(ui->_picLabel,&QLabel_Tztek::changeObject,ui->tableWidget,&QTableWidget_Tztek::ChangeObject);
}

void MainWindow::ImportFile()
{
    QString directory = QFileDialog::getExistingDirectory(this,"选择待标注图片所在文件夹");
    if (directory != "")
    {
        m_fileDir = directory;
        ui->listWidget->clear();
        QDir dir = QDir(m_fileDir);
        dir.setFilter(QDir::Files);
        QStringList filters;
        filters << "*.png" << "*.jpg" << "*.jpeg";
        dir.setNameFilters(filters);
        QFileInfoList list = dir.entryInfoList();
        QStringList namelist;
        for(int i = 0;i<list.size();i++)
        {
            namelist.push_back(list[i].fileName());
        }
        this->ui->listWidget->addItems(namelist);
    }
}

void MainWindow::SetPixmap()
{
    QString filepath = m_fileDir + "/" + ui->listWidget->currentItem()->text();
    QFileInfo fileInfo = QFileInfo(filepath);
    QString fileName = fileInfo.baseName();
    QString xmlpath = m_fileDir + "/" + fileName + ".xml";
    std::vector<Object> set;
    QVector<QString> claslist;
    QVector<QColor> colorlist;
    ui->_picLabel->GetObjectList(claslist, colorlist);
    int result = ReadXml(xmlpath,set,claslist,colorlist);
    QPixmap p = QPixmap(filepath);  // 费时
    ui->_picLabel->setPixmap(p);
    if (result == 0)
    {
        ui->_picLabel->SetObjectSetPixel(set,p.width(),p.height());
    }
    ui->tableWidget->AddRows(set);

}

void MainWindow::SaveXml()
{
    QString filepath = m_fileDir + "/" + ui->listWidget->currentItem()->text();
    QFileInfo fileInfo = QFileInfo(filepath);
    QString fileName = fileInfo.baseName();
    QString xmlpath = m_fileDir + "/" + fileName + ".xml";
    WriteXml(xmlpath, ui->_picLabel->GetObjectSetPixel(), ui->_picLabel->pixmap()->width(), ui->_picLabel->pixmap()->height(), ui->_picLabel->pixmap()->depth(), ".jpg");
}

void MainWindow::Next()
{
    if (ui->_picLabel->IsModified())
    {
        if(QMessageBox::question(this,"提示", "是否保存？")==QMessageBox::Yes)
        {
            SaveXml();
        }
    }
    ui->listWidget->Next();
}

void MainWindow::Back()
{
    if (ui->_picLabel->IsModified())
    {
        if(QMessageBox::question(this,"提示", "是否保存？")==QMessageBox::Yes)
        {
            SaveXml();
        }
    }
    ui->listWidget->Back();
}

MainWindow::~MainWindow()
{
    delete ui;
}

