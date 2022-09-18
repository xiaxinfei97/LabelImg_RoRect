#include<QLabel>
#include<QSplitter>
#include<vector>
#include<QPainter>
#include<QMouseEvent>
#include<QMenu>
#include<QAction>
#include<string>
#include "object.h"
#include "objectschoose.h"

#ifndef QLABEL_TZTEK_H
#define QLABEL_TZTEK_H


class QLabel_Tztek:public QLabel
{
    Q_OBJECT
public:
    QLabel_Tztek(QWidget* spl);
    void SetObjectSet(std::vector<Object> os);//设置当前label坐标系下的标注框集合
    std::vector<Object> GetObjectSet();//获取标注框在当前label坐标系下的坐标
    void SetObjectSetPixel(std::vector<Object> osp, int picW, int picH);//设置像素坐标系下的标注框集合
    std::vector<Object> GetObjectSetPixel();//获取标注框像素坐标
    void SetDrawType(DRAWTYPE type);
    void setPixmap(QPixmap pix);
    void SetIsDraw(bool status);
    void SetMoveStep(int step);
    void SetRotateStep(int step);//注意使用角度值！！！！
    void SetObjectList(QVector<QString> menulist, QVector<QColor> colorlist);
    void GetObjectList(QVector<QString>& classlist, QVector<QColor>& colorlist);

    //**************************尚未测试**************************
    void DeleteObject(std::vector<int> index);//删除index指示的Object
    void ChooseObject(std::vector<int> index);//选择index指示的Object
    //**************************尚未测试**************************
    void ChangeClass(std::vector<int> index, QString clas);//改变index指示的Object的类别为clas
    void ChangeDifficulty(std::vector<int> index, bool dif);//改变index指示的Object的Difficulty为dif

    static double Distance(QPointF p1,QPointF p2);

    bool IsModified();//是否被修改了
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void keyPressEvent(QKeyEvent *ev) override;
    void keyReleaseEvent(QKeyEvent *ev) override;

    std::vector<QPointF> m_PointSet;//正在绘制的点集
    std::vector<Object> m_ObjectSet;//标注框集合
    void clearPointSet();//清空正在绘制点集
private:
    DRAWTYPE m_drawType = DRAWTYPE::TypeOfRoRect;//绘制类别，默认为斜矩形
    bool m_isDraw = false;//是否正在绘图
    double m_scale = 1;//放缩比例
    QPointF m_tempPoint;//临时存储点
    ObjectsChoose* m_menu;//选择标注类型
    QMenu* m_menu_select;//选择所有标注框 删除选择标注框 取消选择

    bool HaveSelct();//判断此时是否有标注框被选中
    std::vector<int> GetSelectIndex();

    void fuc_select_all();//选择所有
    void fuc_select_cancel();//取消所有选择
    void fuc_delete_select();//删除选择
    void fuc_delete_all();//删除所有

    void MoveObject(DIRECTION_M d);//移动选中的标注框
    void RotateObject(DIRECTION_R d);//旋转选中的标注框

    int m_moveStep = 5;//移动步进
    int m_RotateStep = 3;//旋转步进---注意使用角度值

    //键盘事件
    QPoint m_pos_mouse;//存放鼠标位置
    bool m_leftmouse_press = false;//同时按下空格和鼠标左键

    int m_currentSelectIndexOfObject = 0;//当前选中标注框
    int m_currentSelectIndexOfNode = 0;//当前选中的标注框中选中的节点
    bool m_isAdjust = false;//正在调整标注框或标注框的节点

    bool m_leftmouse = false;//按下鼠标左键---有物体被选中且鼠标在节点被按下
    bool m_keydown_space = false;//按下空格

    bool m_modified = false;//是否被修改了
signals:
    void newObject(QString clas,bool dif);
    void chooseObject(std::vector<int> index);
    void deleteObject(std::vector<int> index);
    void deleteAllObject();
    void changeObject(std::vector<int> index, QString clas,bool dif);
};

#endif // QLABEL_TZTEK_H
