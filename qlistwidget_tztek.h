#include<QListWidget>
#include<QSplitter>
#ifndef QLISTWIDGET_TZTEK_H
#define QLISTWIDGET_TZTEK_H


class QListWidget_Tztek:public QListWidget
{
public:
    QListWidget_Tztek(QSplitter* spl);
    void addItems(const QStringList &labels);
    //下面两个函数会主动发射currentRowChanged信号
    void Next();//下一张
    void Back();//上一张
};

#endif // QLISTWIDGET_TZTEK_H
