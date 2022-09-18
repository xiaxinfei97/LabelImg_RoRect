#include<QPoint>
#include<opencv2/opencv.hpp>
#include<string>
#include<QString>
#include<QColor>

#ifndef DATATYPE_H
#define DATATYPE_H

///
/// \brief 绘制形状
///
enum DRAWTYPE
{
    TypeOfRect,//矩形
    TypeOfRoRect,//旋转矩形
    TypeOfPoly//四边形
};

enum DIRECTION_M
{
    Up,//向上平移
    Down,
    Left,
    Right
};

enum DIRECTION_R
{
    Clock,//顺时针
    CounterClock//逆时针
};

class Object
{
public:
    Object(std::vector<QPointF> set, DRAWTYPE type, bool status=false, bool compute = true);
    bool isIner(const QPointF &p);
    DRAWTYPE GetType();//返回绘制的类型
    void SetPointSet(std::vector<QPointF> s);
    std::vector<QPointF> GetPointSet();
    void SetStatus(bool s);
    bool GetStatus();
    void SetClass(QString clas);
    QString GetClass();
    void SetDifficulty(bool status);
    bool GetDifficulty();
    void SetColor(QColor c);
    QColor GetColor();
    void SetSelectPoint(const QPointF p,int index);//更新选中节点(index指示更新节点索引)
    void MoveObject(QPointF vec);//整体移动Object，即同时同方向同步长的移动所有节点
    void RotateObject(int angel);//绕质心旋转Object，只能旋转类型为RotateObject类型的标注框   弧度制！！！
    static QPointF ComputeCentroid(std::vector<QPointF> set);
private:
    std::vector<QPointF> m_pointset;
    DRAWTYPE m_type;//绘制的类型
    QString m_class;//当前标注框的类别
    bool m_difficulty;
    QPointF m_centroid;//质心
    bool m_status;//状态，是否被选中
    QColor m_color;//颜色，该标注框的颜色
};
#endif
