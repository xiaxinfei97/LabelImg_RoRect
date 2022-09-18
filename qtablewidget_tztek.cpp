#include "qtablewidget_tztek.h"
#include<QCheckBox>
#include<functional>
#include<QDebug>
#include <QTime>

QTableWidget_Tztek::QTableWidget_Tztek(QSplitter* spl):QTableWidget(spl)
{
    Init();
    setEnabled(false);
}

void QTableWidget_Tztek::Init()
{
    setStyleSheet("selection-background-color:lightblue;");
    setSelectionMode(QAbstractItemView::MultiSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    clearContents();
    setColumnCount(2);
    setHorizontalHeaderLabels(QStringList({"类别"," Difficulty"}));
}

void QTableWidget_Tztek::AddRow(QString clas,bool dif)
{
    setRowCount(rowCount() + 1);
    setItem(rowCount() - 1,0,new QTableWidgetItem(clas));
    QCheckBox* c = new QCheckBox();
    c->setChecked(dif);
    setCellWidget(rowCount() - 1,1,c);
}

void QTableWidget_Tztek::ChooseRows(std::vector<int> index)
{
    clearSelection();
    setCurrentItem(NULL);
    for (size_t i=0;i<index.size();i++)
    {
        selectRow(index.at(i));
    }
}

void QTableWidget_Tztek::DeleteRows(std::vector<int> index)
{
    sort(index.begin(),index.end(),std::greater<int>());
    for(size_t i = 0;i<index.size();i++)
    {
        removeRow(index[i]);
    }
}

void QTableWidget_Tztek::DeleteAllRows()
{
    clearContents();
    setRowCount(0);
}

//void QTableWidget_Tztek::ChangeClass(int* index, int len, QString clas)
//{
//    for(int i = 0; i < len; i++)
//    {
//        item(index[i],0)->setText(clas);
//    }
//}

//void QTableWidget_Tztek::ChangeDifficulty(int* index,int len,bool dif)
//{
//    for(int i = 0; i < len; i++)
//    {
//        ((QCheckBox*)cellWidget(index[i],0))->setChecked(dif);
//    }
//}

void QTableWidget_Tztek::ChangeObject(std::vector<int> index, QString clas,bool dif)
{
    for(size_t i = 0; i<index.size(); i++)
    {
        item(index[i],0)->setText(clas);
        static_cast<QCheckBox*>(cellWidget(index[i],1))->setChecked(dif);
    }
}

void QTableWidget_Tztek::AddRows(std::vector<Object>& objectSet)
{
    DeleteAllRows();
    setRowCount(objectSet.size());
    for(size_t i = 0; i<objectSet.size(); i++)
    {
        setItem(i,0,new QTableWidgetItem(objectSet[i].GetClass()));
        QCheckBox* c = new QCheckBox();
        c->setChecked(objectSet[i].GetDifficulty());
        setCellWidget(i,1,c);
    }
}
