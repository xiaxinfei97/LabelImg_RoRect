#include "setting.h"
#include "ui_setting.h"
#include<QColorDialog>
#include<QDebug>

Setting::Setting(QWidget *parent, QVector<QString> classlist, QVector<QColor> colorlist, bool copy) :
    QDialog(parent), ui(new Ui::Setting)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(2);
    connect(ui->_addBtn,SIGNAL(clicked()),this,SLOT(AddObject()));
    connect(ui->tableWidget,&QTableWidget::cellDoubleClicked,this,&Setting::ChangeColor);
    connect(ui->_deleteBtn,SIGNAL(clicked()),this,SLOT(DeleteObject()));
    AddObject(classlist, colorlist);
    ui->_chooseCkBox->setChecked(copy);
}

void Setting::AddObject()
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
    QString clas = QString("标注") + QString::number(ui->tableWidget->rowCount());
    QColor color = m_color[(ui->tableWidget->rowCount() % m_color.length())];
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1,1,new QTableWidgetItem(clas));
    QTableWidgetItem* temp = new QTableWidgetItem();
    temp->setBackground(QBrush(color));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1,0,temp);
}

void Setting::AddObject(QVector<QString> classlist, QVector<QColor> colorlist)
{
    //Q_ASSERT(classlist.length()!=colorlist.length());
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + classlist.length());
    for(int i = ui->tableWidget->rowCount() - classlist.length();i<ui->tableWidget->rowCount();i++)
    {
        QString clas = classlist[i];
        QColor color = colorlist[i];
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(clas));
        QTableWidgetItem* temp = new QTableWidgetItem();
        temp->setBackground(QBrush(color));
        ui->tableWidget->setItem(i,0,temp);
    }
}

void Setting::DeleteObject()
{
    QList<QTableWidgetSelectionRange>ranges=ui->tableWidget->selectedRanges();
    if (ranges.length()!=0)
    {
        for(int i=0;i< ranges.length();i++)
        {
           int topRow=ranges.at(i).topRow();
           int bottomRow=ranges.at(i).bottomRow();
           for(int j=bottomRow;j<=topRow;j--)
           {
              ui->tableWidget->removeRow(j);
           }
        }
    }
    else if (ui->tableWidget->rowCount()!=0)
    {
        ui->tableWidget->removeRow(ui->tableWidget->rowCount() - 1);
    }

}

void Setting::ChangeColor(int row, int col)
{
    if(col == 0)
    {
        QColor color = QColorDialog::getColor(Qt::red, this);
        ui->tableWidget->item(row,col)->setBackground(QBrush(color));
        ui->tableWidget->setCurrentCell(row,1);
    }
}

void Setting::GetObjectList(QVector<QString>& classlist, QVector<QColor>& colorlist, bool& copy)
{
    for(int i = 0;i<ui->tableWidget->rowCount();i++)
    {
        classlist.push_back(ui->tableWidget->item(i,1)->text());
        colorlist.push_back(ui->tableWidget->item(i,0)->backgroundColor());
    }
    copy = ui->_chooseCkBox->isChecked();
}

Setting::~Setting()
{
    delete ui;
}
