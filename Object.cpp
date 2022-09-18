#include "object.h"
#include"pointset2rect.h"
#include<Eigen/Eigen>
#include<QDebug>


///
/// \brief Object::Object 若传入类型为Rect或RoRect，这个构造函数每次都会针对传进的浮点型数据计算外接矩形，并将计算结果保存为整形。
/// 这会引入误差，第四个参数控制是否计算，参数值为false时传入数据不计算外接矩形，该参数默认值为true。
/// \param set 点集
/// \param type 类型
/// \param status 状态，是否被选中
/// \param compute 是否计算
///
Object::Object(std::vector<QPointF> set, DRAWTYPE type, bool status, bool compute)
{
    std::vector<QPointF> set_p;
    if(compute)
    {
        if(type == DRAWTYPE::TypeOfRect)
        {
            cv::Rect rect = Change2Rect(set);
            set_p = {QPointF(rect.br().x,rect.br().y),QPointF(rect.br().x,rect.tl().y),QPointF(rect.tl().x,rect.tl().y),QPointF(rect.tl().x,rect.br().y)};
        }
        else if (type == DRAWTYPE::TypeOfRoRect)
        {
            cv::RotatedRect rect = Change2RoRect(set);
            cv::Point2f p[4];
            rect.points(p);
            set_p = {QPointF(p[0].x,p[0].y),QPointF(p[1].x,p[1].y),QPointF(p[2].x,p[2].y),QPointF(p[3].x,p[3].y)};
        }
        else
        {
            set_p = set;
        }
    }
    else
    {
        set_p = set;
    }

    m_pointset = set_p;
    m_type = type;
    m_status = status;
    m_centroid = ComputeCentroid(m_pointset);
}

bool Object::isIner(const QPointF &p)//这里必须加const,否则调用的时候会报错
{
    cv::Point q(p.x(),p.y());
    std::vector<cv::Point> set;
    for(size_t i = 0; i<m_pointset.size();i++)
    {
        set.push_back(cv::Point(m_pointset[i].x(), m_pointset[i].y()));
    }
    double res = cv::pointPolygonTest(set,q,false);
    return res>0;
}

DRAWTYPE Object::GetType()
{
    return m_type;
}

void Object::SetPointSet(std::vector<QPointF> s)
{
    m_pointset = s;
}

std::vector<QPointF> Object::GetPointSet()
{
    return m_pointset;
}

void Object::SetSelectPoint(const QPointF p,int index)
{
    m_pointset[index] = p;
    if(m_type == DRAWTYPE::TypeOfRoRect || m_type == DRAWTYPE::TypeOfRect)
    {
        int index_opposite =index - 2;//对面点索引
        if(index_opposite <0)
        {
            index_opposite += 4;
        }
        int index_adjacent_l = index - 1;//左相邻点索引
        if(index_adjacent_l < 0)
        {
            index_adjacent_l = index_adjacent_l + 4;
        }
        int index_adjacent_r = index + 1;//右相邻点索引
        if(index_adjacent_r > 3)
        {
            index_adjacent_r = index_adjacent_r - 4;
        }
        Eigen::Vector2<double> v02(m_pointset[index].x() - m_pointset[index_opposite].x(),m_pointset[index].y() - m_pointset[index_opposite].y());
        Eigen::Vector2<double> v03(m_pointset[index_adjacent_r].x() - m_pointset[index_opposite].x(),m_pointset[index_adjacent_r].y() - m_pointset[index_opposite].y());
        Eigen::Vector2<double> v01(m_pointset[index_opposite].x() - m_pointset[index_adjacent_l].x(),m_pointset[index_opposite].y() - m_pointset[index_adjacent_l].y());
        v03.normalize();
        v01.normalize();
        Eigen::Vector2<double> v01_new = v01.adjoint() * v02 * v01;
        Eigen::Vector2<double> v03_new = v03.adjoint() * v02 * v03;
        QPointF P1 = QPointF(m_pointset[index_opposite].x() + v01_new[0],m_pointset[index_opposite].y() + v01_new[1]);
        QPointF P3 = QPointF(m_pointset[index_opposite].x() + v03_new[0],m_pointset[index_opposite].y() + v03_new[1]);
        m_pointset[index_adjacent_l] = P1;
        m_pointset[index_adjacent_r] = P3;
    }
    m_centroid = ComputeCentroid(m_pointset);
}

void Object::MoveObject(QPointF vec)
{
    for(size_t i = 0;i<m_pointset.size();i++)
    {
        m_pointset[i] = QPointF(m_pointset[i].x() + vec.x(),m_pointset[i].y() + vec.y());
    }
    m_centroid = ComputeCentroid(m_pointset);
}

void Object::RotateObject(int angel)
{
    if(m_type != DRAWTYPE::TypeOfRoRect)
    {
        return;
    }
    Eigen::Matrix2d R(2,2);
    R(0,0) = cos(angel * M_PI / 180);
    R(0,1) = -sin(angel * M_PI / 180);
    R(1,0) = sin(angel * M_PI / 180);
    R(1,1) = cos(angel * M_PI / 180);
    for(size_t i = 0;i<m_pointset.size();i++)
    {
        Eigen::Vector2d temp;
        temp(0) = m_pointset[i].x() - m_centroid.x();
        temp(1) = m_pointset[i].y() - m_centroid.y();
        m_pointset[i] = QPointF((R * temp)[0] + m_centroid.x(),(R * temp)[1] + m_centroid.y());
    }
}

void Object::SetStatus(bool s)
{
    m_status = s;
}

bool Object::GetStatus()
{
    return m_status;
}

QString Object::GetClass()
{
    return m_class;
}

void Object::SetClass(QString clas)
{
    m_class = clas;
}

void Object::SetDifficulty(bool status)
{
    m_difficulty = status;
}

bool Object::GetDifficulty()
{
    return m_difficulty;
}
void Object::SetColor(QColor c)
{
    m_color = c;
}

QColor Object::GetColor()
{
    return m_color;
}

QPointF Object::ComputeCentroid(std::vector<QPointF> set)
{
    float x = 0;
    float y = 0;
    for(size_t i = 0;i<set.size();i++)
    {
        x += set[i].x();
        y += set[i].y();
    }
    return QPointF(x/set.size(),y/set.size());
}
