#include "qlistwidget_tztek.h"

QListWidget_Tztek::QListWidget_Tztek(QSplitter* spl):QListWidget(spl)
{

}

void QListWidget_Tztek::addItems(const QStringList &labels)
{
    QListWidget::addItems(labels);
    if(count() > 0)
    {
        setCurrentRow(0);
    }
}

void QListWidget_Tztek::Next()
{
    if(currentRow() + 1 < count())
    {
        setCurrentRow(currentRow() + 1);
    }
}

void QListWidget_Tztek::Back()
{
    if(currentRow() - 1 >= 0 && count() > 0)
    {
        setCurrentRow(currentRow() - 1);
    }
}
