#ifndef SETTING_H
#define SETTING_H

#include <QDialog>

namespace Ui {
class Setting;
}

class Setting : public QDialog
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = nullptr, QVector<QString> classlist=QVector<QString>(0), QVector<QColor> colorlist = QVector<QColor>(0), bool copy = false);
    void GetObjectList(QVector<QString>& classlist, QVector<QColor>& colorlist, bool& copy);
    ~Setting();
public slots:
    void AddObject();//添加一个Object
    void AddObject(QVector<QString> classlist, QVector<QColor> colorlist);
    void DeleteObject();//删除Object
    void ChangeColor(int row, int col);//修改标注颜色
private:
    Ui::Setting *ui;
    QVector<QColor> m_color = {QColor(255,0,0),QColor(0,255,0),QColor(0,0,255),QColor(255,255,0),QColor(0,255,255),QColor(255,0,255)};
};

#endif // SETTING_H

