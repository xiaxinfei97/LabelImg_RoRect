#ifndef OBJECTSCHOOSE_H
#define OBJECTSCHOOSE_H

#include <QDialog>

namespace Ui {
class ObjectsChoose;
}

class ObjectsChoose : public QDialog
{
    Q_OBJECT

public:
    explicit ObjectsChoose(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *) override;
    ~ObjectsChoose();
    //void SetObject(QVector<QString> menulist);
    void SetObject(QVector<QString>menulist, QVector<QColor> colorlist);
    void GetObejct(QVector<QString>& classlist,QVector<QColor>& colorlist);
    QString GetObejctClass();//获得选取Object类别
    QColor GetObjectColor();//获取选取Object类别颜色
    bool GetDifficulty();//获取是否考虑difficulty
private:
    Ui::ObjectsChoose *ui;
    QString m_ObjectClass = "";
    bool m_difficulty = NULL;
    QVector<QString> m_classlist = QVector<QString>(0);
    QVector<QColor> m_colorlist = QVector<QColor>(0);
    QColor m_ObjectColor;
};

#endif // OBJECTSCHOOSE_H
