#include<QTableWidget>
#include<QSplitter>
#include<QMenu>
#include<QAction>
#include<QTableWidgetItem>
#include<objectschoose.h>
#include "object.h"
#ifndef QTABLEWIDGET_TZTEK_H
#define QTABLEWIDGET_TZTEK_H


class QTableWidget_Tztek:public QTableWidget
{
public:
    QTableWidget_Tztek(QSplitter* spl);
    void Init();//初始化,主要是设置行列信息

public slots:
    void AddRow(QString clas,bool dif);
    void ChooseRows(std::vector<int> index);
    void DeleteRows(std::vector<int> index);
    void DeleteAllRows();
//    void ChangeClass(int* index, int len, QString clas);//改变index指示的Object的类别为clas
//    void ChangeDifficulty(int* index,int len,bool dif);//改变index指示的Object的Difficulty为dif
    void ChangeObject(std::vector<int> index, QString clas,bool dif);
    void AddRows(std::vector<Object>& objectSet);
};

#endif // QTABLEWIDGET_TZTEK_H
