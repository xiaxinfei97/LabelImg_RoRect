#include "objectschoose.h"
#include "ui_objectschoose.h"
#include<QDebug>

ObjectsChoose::ObjectsChoose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ObjectsChoose)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowCloseButtonHint);
    connect(ui->listWidget,&QListWidget::doubleClicked,[=]()
    {
        m_ObjectClass = ui->listWidget->currentItem()->text();
        m_ObjectColor = m_colorlist[ui->listWidget->currentRow()];
        accept();
    });
    connect(ui->checkBox,&QCheckBox::stateChanged,[=]()
    {
       m_difficulty = ui->checkBox->isChecked();
    });
}

ObjectsChoose::~ObjectsChoose()
{
    delete ui;
}

void ObjectsChoose::closeEvent(QCloseEvent *)
{
    m_ObjectClass = "";
    m_difficulty = NULL;
}

//void ObjectsChoose::SetObject(QVector<QString> menulist)
//{
//    ui->listWidget->clear();
//    for (int i = 0;i<menulist.length();i++)
//    {
//        ui->listWidget->addItem(menulist[i]);
//    }
//}

void ObjectsChoose::SetObject(QVector<QString>menulist, QVector<QColor> colorlist)
{
    ui->listWidget->clear();
    for (int i = 0;i<menulist.length();i++)
    {
        ui->listWidget->addItem(menulist[i]);
    }
    m_classlist = menulist;
    m_colorlist = colorlist;
}

void ObjectsChoose::GetObejct(QVector<QString>& classlist,QVector<QColor>& colorlist)
{
    classlist = m_classlist;
    colorlist = m_colorlist;
}

QString ObjectsChoose::GetObejctClass()
{
    return m_ObjectClass;
}

QColor ObjectsChoose::GetObjectColor()
{
    return m_ObjectColor;
}

bool ObjectsChoose::GetDifficulty()
{
    return m_difficulty;
}
