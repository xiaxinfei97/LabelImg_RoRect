#include "qlabel_tztek.h"
#include<QScrollArea>
#include<QApplication>
#include<math.h>
#include<algorithm>
#include<QDebug>
#include<QDateTime>
#include<opencv2/opencv.hpp>
#include "pointset2rect.h"
#include <QTime>

QLabel_Tztek::QLabel_Tztek(QWidget* wdg):QLabel(wdg)
{
    m_menu = new ObjectsChoose();
    this->setMouseTracking(true);
    this->grabKeyboard();
    m_menu_select = new QMenu();//选择所有标注框 删除选择标注框 取消选择
    QAction* select_all = new QAction("选择所有标注框");
    QAction* select_cancel = new QAction("取消选择标注框");
    QAction* delete_select = new QAction("删除所选标注框");
    QAction* delete_all = new QAction("删除所有标注框");
    QAction* change_select = new QAction("修改选择标注框");
    m_menu_select->addAction(select_all);
    m_menu_select->addAction(select_cancel);
    m_menu_select->addSeparator();
    m_menu_select->addAction(delete_select);
    m_menu_select->addAction(delete_all);
    m_menu_select->addAction(change_select);
    connect(select_all, &QAction::triggered,[=]()
    {
        fuc_select_all();
    });
    connect(select_cancel, &QAction::triggered,[=]()
    {
        fuc_select_cancel();
    });
    connect(delete_select, &QAction::triggered,[=]()
    {
        fuc_delete_select();
    });
    connect(delete_all, &QAction::triggered,[=]()
    {
        fuc_delete_all();
    });
    connect(change_select, &QAction::triggered,[=]()
    {
        m_menu->setGeometry(QCursor::pos ().x(),QCursor::pos ().y(),m_menu->width(),m_menu->height());
        int res = m_menu->exec();
        if(res == 1 && HaveSelct())
        {
            ChangeClass(GetSelectIndex(),m_menu->GetObejctClass());
            ChangeDifficulty(GetSelectIndex(),m_menu->GetDifficulty());
            emit changeObject(GetSelectIndex(),m_menu->GetObejctClass(),m_menu->GetDifficulty());
        }
    });
}

void QLabel_Tztek::SetIsDraw(bool status)
{
    m_isDraw = status;
}

void QLabel_Tztek::SetMoveStep(int step)
{
    m_moveStep = step;
}

void QLabel_Tztek::SetRotateStep(int step)
{
    m_RotateStep = step;
}

void QLabel_Tztek::SetObjectSet(std::vector<Object> os)
{
    m_ObjectSet = os;
}

std::vector<Object> QLabel_Tztek::GetObjectSet()
{
    return m_ObjectSet;
}

///
/// \brief QLabel_Tztek::SetObjectSetPixel 设置像素坐标系下的标注框集合
/// \param osp 像素坐标系下的标注框集合
///
void QLabel_Tztek::SetObjectSetPixel(std::vector<Object> osp, int picW, int picH)
{
    fuc_delete_all();
    m_modified = false;
    double scale_temp = fmax(picW*1.0/this->width(), picH*1.0/this->height());
    for(size_t i = 0;i<osp.size();i++)
    {
        std::vector<QPointF> set_temp;
        for (size_t j = 0;j<osp[i].GetPointSet().size();j++)
        {
            set_temp.push_back(QPointF(osp[i].GetPointSet()[j].x()/scale_temp,osp[i].GetPointSet()[j].y()/scale_temp));
        }
        m_ObjectSet.push_back(Object(set_temp,osp[i].GetType()));
        m_ObjectSet[i].SetClass(osp[i].GetClass());
        m_ObjectSet[i].SetDifficulty(osp[i].GetDifficulty());
        m_ObjectSet[i].SetColor(osp[i].GetColor());
    }
}

///
/// \brief QLabel_Tztek::GetObjectSetPixel
/// \return 返回像素坐标下的标注框集合
///
std::vector<Object> QLabel_Tztek::GetObjectSetPixel()
{
    std::vector<Object> temp;
    for(size_t i = 0;i<m_ObjectSet.size();i++)
    {
        std::vector<QPointF> set_temp;
        for(size_t j = 0;j<m_ObjectSet[i].GetPointSet().size();j++)
        {
            set_temp.push_back(QPointF(m_ObjectSet[i].GetPointSet()[j].x()*m_scale,m_ObjectSet[i].GetPointSet()[j].y()*m_scale));
        }
        temp.push_back(Object(set_temp,m_ObjectSet[i].GetType()));
        temp[i].SetClass(m_ObjectSet[i].GetClass());
        temp[i].SetDifficulty(m_ObjectSet[i].GetDifficulty());
        temp[i].SetColor(m_ObjectSet[i].GetColor());
    }
    return temp;
}

void QLabel_Tztek::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_Space)
    {
        m_keydown_space = true;
    }
    else if(ev->key() == Qt::Key_W)
    {
        MoveObject(DIRECTION_M::Up);
    }
    else if(ev->key() == Qt::Key_S)
    {
        MoveObject(DIRECTION_M::Down);
    }
    else if(ev->key() == Qt::Key_A)
    {
        MoveObject(DIRECTION_M::Left);
    }
    else if(ev->key() == Qt::Key_D)
    {
        MoveObject(DIRECTION_M::Right);
    }
    else if(ev->key() == Qt::Key_Q)
    {
        RotateObject(DIRECTION_R::CounterClock);
    }
    else if(ev->key() == Qt::Key_E)
    {
        RotateObject(DIRECTION_R::Clock);
    }
    else if(ev->key() == Qt::Key_Escape)
    {
        fuc_select_cancel();
    }
    else if(ev->key() == Qt::Key_Delete)
    {
        fuc_delete_select();
    }
    else if((ev->modifiers() == Qt::ControlModifier) && (ev->key() == Qt::Key_A))
    {
        fuc_select_all();
    }
    update();
}

void QLabel_Tztek::keyReleaseEvent(QKeyEvent *ev)
{
    if(ev->key()==Qt::Key_Space)
    {
        m_keydown_space = false;
    }
}

void QLabel_Tztek::mousePressEvent(QMouseEvent *ev)//重载鼠标点击事件
{
    if((QApplication::keyboardModifiers() == Qt::ControlModifier) && (ev->button() == Qt::LeftButton))// ctrl + LeftButton
    {
        for (size_t i = 0; i<m_ObjectSet.size(); i++)
        {
            if(!m_ObjectSet[i].GetStatus())
            {
                continue;
            }
            if(m_ObjectSet[i].isIner(ev->pos()))
            {
                m_ObjectSet[i].SetStatus(false);
                break;
            }
        }
        emit chooseObject(GetSelectIndex());
    }
    else if((QApplication::keyboardModifiers() == Qt::ShiftModifier) && (ev->button() == Qt::LeftButton))// shift + LeftButton
    {
        for (size_t i = 0; i<m_ObjectSet.size(); i++)
        {
            if(m_ObjectSet[i].isIner(ev->pos()))
            {
                m_ObjectSet[i].SetStatus(true);
                break;
            }
        }
        emit chooseObject(GetSelectIndex());
    }
    else if(ev->button() == Qt::LeftButton)//鼠标左击  在绘制或在选择已绘制矩形框
    {
        if(m_keydown_space)
        {
            m_leftmouse_press = true;
        }
        else
        {
            if(m_isDraw)//正在绘制
            {
                m_PointSet.push_back(ev->pos());
            }
            else if(cursor().shape() != Qt::OpenHandCursor && cursor().shape() != Qt::SizeAllCursor)//没在绘制 此时判定是否点击在已绘制的矩形框内
            {
                for (size_t i = 0; i<m_ObjectSet.size(); i++)
                {
                    if(m_ObjectSet[i].isIner(ev->pos()))
                    {
                        m_ObjectSet[i].SetStatus(true);
                    }
                    else
                    {
                        m_ObjectSet[i].SetStatus(false);
                    }
                }
                emit chooseObject(GetSelectIndex());
            }
        }

        if(HaveSelct())
        {
            if(cursor().shape() == Qt::OpenHandCursor)
            {
                setCursor(Qt::ClosedHandCursor);
                m_isAdjust = true;
            }
            else if (cursor().shape() == Qt::SizeAllCursor)
            {
                m_isAdjust = true;
            }
        }
    }
    else if (ev->button() == Qt::RightButton)//正在绘制的时候弹出选择标签框 没有正在绘制的时候弹出清除等调整框
    {
        if(m_isDraw)//正在绘制
        {
            m_menu->setGeometry(ev->globalX(),ev->globalY(),m_menu->width(),m_menu->height());
            int res = m_menu->exec();
            if(m_PointSet.size()>=3 && res == 1)
            {
                if(HaveSelct())
                {
                    for(size_t i = 0;i<m_ObjectSet.size();i++)
                    {
                        m_ObjectSet[i].SetStatus(false);
                    }
                }
                Object objectTemp(m_PointSet,m_drawType,true);
                objectTemp.SetClass(m_menu->GetObejctClass());
                objectTemp.SetDifficulty(m_menu->GetDifficulty());
                objectTemp.SetColor(m_menu->GetObjectColor());
                m_ObjectSet.push_back(objectTemp);
                emit newObject(objectTemp.GetClass(),objectTemp.GetDifficulty());
                emit chooseObject(GetSelectIndex());
                m_modified = true;
            }
            clearPointSet();
        }
        else//没在绘制，弹出右击菜单，删除所有标注
        {
            m_menu_select->exec(ev->globalPos());
        }
    }
    update();
    if (ev->button() == Qt::RightButton && m_isDraw)
    {
        m_isDraw = false;
    }
}

void QLabel_Tztek::mouseReleaseEvent(QMouseEvent *ev)
 {
     if(ev->button() == Qt::LeftButton && (QApplication::keyboardModifiers() != Qt::ShiftModifier))
     {
         m_leftmouse_press = false;
         if(HaveSelct() && cursor().shape() == Qt::ClosedHandCursor)
         {
             setCursor(Qt::OpenHandCursor);
         }
     }
     if(m_isAdjust)
     {
         m_isAdjust = false;
     }
 }

void QLabel_Tztek::mouseMoveEvent(QMouseEvent *ev)
{
    if(!m_isAdjust)
    {
        if(m_keydown_space && m_leftmouse_press)
        {
            this->setGeometry(this->pos().x() + ev->pos().x() - m_pos_mouse.x(),this->pos().y() + ev->pos().y() - m_pos_mouse.y(),this->width(),this->height());
            return;
        }

        if(m_isDraw)
        {
            m_tempPoint = ev->pos();
            update();
        }
        unsetCursor();// 还原鼠标形状
    }

    if(HaveSelct() && !m_isAdjust && (QApplication::keyboardModifiers() != Qt::ShiftModifier) && (QApplication::keyboardModifiers() != Qt::ControlModifier))
    {
        for(size_t i = 0;i<m_ObjectSet.size();i++)
        {
            if(!m_ObjectSet[i].GetStatus())
            {
                continue;
            }
            if(m_ObjectSet[i].isIner(ev->pos()))
            {
                m_currentSelectIndexOfObject = i;
                setCursor(Qt::OpenHandCursor);//改变鼠标形状
            }
            for(size_t j = 0; j<m_ObjectSet[i].GetPointSet().size();j++)
            {
                if(Distance(ev->pos(),m_ObjectSet[i].GetPointSet()[j]) <= 6)
                {
                    m_currentSelectIndexOfObject = i;
                    m_currentSelectIndexOfNode = j;
                    setCursor(Qt::SizeAllCursor);
                    break;
                }
            }
        }
    }

    if(m_isAdjust)
    {
        if(cursor().shape() == Qt::ClosedHandCursor)//移动标注框
        {
            m_ObjectSet[m_currentSelectIndexOfObject].MoveObject(QPointF(ev->pos().x() - m_pos_mouse.x(),ev->pos().y() - m_pos_mouse.y()));
        }
        else if(cursor().shape() == Qt::SizeAllCursor)//移动某标注框节点
        {
            m_ObjectSet[m_currentSelectIndexOfObject].SetSelectPoint(ev->pos(),m_currentSelectIndexOfNode);
        }
        update();
        m_modified = true;
    }
    m_pos_mouse = ev->pos();
}

double QLabel_Tztek::Distance(QPointF p1,QPointF p2)
{
    double dis = sqrt(pow(p1.x()-p2.x(),2)+pow(p1.y()-p2.y(),2));
    return dis;
}

void QLabel_Tztek::paintEvent(QPaintEvent *event)//重载绘图事件
{
    QLabel::paintEvent(event);
    QPainter p;
    p.begin(this);
    // 绘制正在标注节点  连接节点
    p.setPen(QPen(QBrush(QColor(0,0,255), Qt::SolidPattern),2));
    if(m_isDraw && m_PointSet.size()!=0)
    {
        p.drawPolyline(&m_PointSet[0],m_PointSet.size());
        p.drawLine(m_PointSet[m_PointSet.size() - 1],m_tempPoint);
    }
    //绘制已标注节点(斜矩形、正矩形、四边形)
    p.setPen(QPen(QBrush(QColor(255,0,0), Qt::SolidPattern),2));
    for (size_t i = 0;i<m_ObjectSet.size();i++)
    {
        p.setPen(QPen(QBrush(m_ObjectSet[i].GetColor(), Qt::SolidPattern),2));
        p.drawPolygon(&(m_ObjectSet[i].GetPointSet()[0]),m_ObjectSet[i].GetPointSet().size());
        if(m_ObjectSet[i].GetStatus())//该标注框被选中
        {
            p.setPen(QPen(QBrush(QColor(255,255,0), Qt::SolidPattern),6));
            p.drawPoints(&(m_ObjectSet[i].GetPointSet()[0]),m_ObjectSet[i].GetPointSet().size());
            //p.setPen(QPen(QBrush(QColor(255,0,0), Qt::SolidPattern),2));
        }
    }
    p.end();
}

void QLabel_Tztek::wheelEvent(QWheelEvent *event)//放大缩小的时候以鼠标为中心(即放大缩小后鼠标位置不变)
{
    double scale_temp = m_scale;
    //Label放缩
    int posXPixel = event->pos().x() * m_scale;
    int posYPixel = event->pos().y() * m_scale;
    QPoint numDegrees = event->angleDelta();
    if (!numDegrees.isNull() && numDegrees.y() > 0)
    {
        m_scale /= 1.1;
    }
    else if (!numDegrees.isNull() && numDegrees.y() < 0)
    {
        m_scale *= 1.1;
    }
    int currentWidth = this->pixmap()->width() / m_scale + 0.5;//四舍五入取整
    int currentHeight = this->pixmap()->height() / m_scale + 0.5;
    //this->resize(currentWidth, currentHeight);
    this->setGeometry(this->pos().x() + event->pos().x() - posXPixel/m_scale, this->pos().y() + event->pos().y() - posYPixel/m_scale,currentWidth, currentHeight);
    //标注框的放缩
    m_tempPoint = QPoint(m_tempPoint.x()*scale_temp/m_scale, m_tempPoint.y()*scale_temp/m_scale);

    for(size_t i = 0;i<m_PointSet.size();i++)
    {
        m_PointSet[i] = QPoint(m_PointSet[i].x()*scale_temp/m_scale,m_PointSet[i].y()*scale_temp/m_scale);
    }

    for(size_t i = 0;i<m_ObjectSet.size();i++)
    {
        std::vector<QPointF> set_temp;
        for(size_t j = 0;j<m_ObjectSet[i].GetPointSet().size();j++)
        {
            set_temp.push_back(QPointF(m_ObjectSet[i].GetPointSet()[j].x()*scale_temp/m_scale,m_ObjectSet[i].GetPointSet()[j].y()*scale_temp/m_scale));
        }
        m_ObjectSet[i].SetPointSet(set_temp);
    }
}

void QLabel_Tztek::setPixmap(QPixmap pix)
{
    int width = pix.width();
    int height = pix.height();
    int width_widget = ((QWidget*)(this->parent()))->width();
    int height_widget = ((QWidget*)(this->parent()))->height();
    m_scale = fmax(width*1.0/width_widget, height*1.0/height_widget);//计算当前缩放系数
    int width_label = width/m_scale;
    int height_label = height/m_scale;
    int labelPosX = (width_widget - width_label)/2;
    int labelPosY = (height_widget - height_label)/2;
    this->setGeometry(labelPosX,labelPosY,width_label,height_label);
    QLabel::setPixmap(pix);

    m_modified = false;
}

void QLabel_Tztek::SetDrawType(DRAWTYPE type)
{
    this->m_drawType = type;
}

///
/// \brief QLabel_Tztek::SetObject 设置右击显示类别并给不同类别赋予不同颜色
/// \param menulist
/// \param colorlist
///
void QLabel_Tztek::SetObjectList(QVector<QString>menulist, QVector<QColor> colorlist)
{
    assert(menulist.size() == colorlist.size());
    m_menu->SetObject(menulist,colorlist);
}

void QLabel_Tztek::GetObjectList(QVector<QString>& classlist, QVector<QColor>& colorlist)
{
    m_menu->GetObejct(classlist,colorlist);
}

void QLabel_Tztek::clearPointSet()
{
    this->m_PointSet.clear();
}

void QLabel_Tztek::fuc_select_all()
{
    for(std::vector<Object>::iterator iter = m_ObjectSet.begin();iter != m_ObjectSet.end();iter++)
    {
        iter->SetStatus(true);
    }
}

void QLabel_Tztek::fuc_select_cancel()
{
    for(std::vector<Object>::iterator iter = m_ObjectSet.begin();iter != m_ObjectSet.end();iter++)
    {
        iter->SetStatus(false);
    }
}

void QLabel_Tztek::fuc_delete_select()
{
    emit deleteObject(GetSelectIndex());
    for(std::vector<Object>::iterator iter = m_ObjectSet.begin();iter != m_ObjectSet.end();)
    {
        if(iter->GetStatus())
        {
            iter = m_ObjectSet.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    m_modified = true;
}

void QLabel_Tztek::fuc_delete_all()
{
    m_ObjectSet.clear();
    emit deleteAllObject();
    m_modified = true;
}

void QLabel_Tztek::MoveObject(DIRECTION_M d)
{
    for(std::vector<Object>::iterator iter = m_ObjectSet.begin();iter != m_ObjectSet.end();iter++)
    {
        if(!iter->GetStatus())
        {
            continue;
        }
        if(d == DIRECTION_M::Up)
        {
            iter->MoveObject(QPointF(0,-m_moveStep));
        }
        else if(d == DIRECTION_M::Down)
        {
            iter->MoveObject(QPointF(0,m_moveStep));
        }
        else if(d == DIRECTION_M::Left)
        {
            iter->MoveObject(QPointF(-m_moveStep,0));
        }
        else
        {
            iter->MoveObject(QPointF(m_moveStep,0));
        }
    }
    m_modified = true;
}

void QLabel_Tztek::RotateObject(DIRECTION_R d)
{
    for(std::vector<Object>::iterator iter = m_ObjectSet.begin();iter != m_ObjectSet.end();iter++)
    {
        if(!iter->GetStatus())
        {
            continue;
        }
        if(d == DIRECTION_R::Clock)
        {
            iter->RotateObject(m_RotateStep);
        }
        else
        {
            iter->RotateObject(-m_RotateStep);
        }
    }
    m_modified = true;
}

bool QLabel_Tztek::HaveSelct()
{
    for(size_t i = 0;i<m_ObjectSet.size();i++)
    {
        if(m_ObjectSet[i].GetStatus())
        {
            return true;
        }
    }
    return false;
}

std::vector<int> QLabel_Tztek::GetSelectIndex()
{
    std::vector<int> index;
    for(size_t i = 0;i<m_ObjectSet.size();i++)
    {
        if(m_ObjectSet[i].GetStatus())
        {
            index.push_back(i);
        }
    }
    return index;
}

//下面四个函数可能需要手动触发paint事件
void QLabel_Tztek::DeleteObject(std::vector<int> index)
{
    fuc_select_cancel();
    for(size_t i = 0; i < index.size(); i++)
    {
        m_ObjectSet[index[i]].SetStatus(true);
    }
    fuc_delete_select();
    m_modified = true;
}

void QLabel_Tztek::ChooseObject(std::vector<int> index)
{
    fuc_select_cancel();
    for(size_t i = 0; i < index.size(); i++)
    {
        m_ObjectSet[index[i]].SetStatus(true);
    }
}

void QLabel_Tztek::ChangeClass(std::vector<int> index, QString clas)
{
    for(size_t i = 0; i < index.size(); i++)
    {
        m_ObjectSet[index[i]].SetClass(clas);
    }
    m_modified = true;
}

void QLabel_Tztek::ChangeDifficulty(std::vector<int> index, bool dif)
{
    for(size_t i = 0; i < index.size(); i++)
    {
        m_ObjectSet[index[i]].SetDifficulty(dif);
    }
    m_modified = true;
}

bool QLabel_Tztek::IsModified()
{
    return m_modified;
}
