#include "readandwritexml.h"
#include<QFile>
#include<QDomDocument>
#include<QTextStream>
#include<QFileInfo>
#include"pointset2rect.h"
#include "math.h"
#include<algorithm>
#include<QDebug>

#include <sys/time.h>



void WriteXml(QString xmlPath, std::vector<Object> objectSet, int w, int h ,int depth, QString suffix)
{
    QFile file(xmlPath); //相对路径、绝对路径、资源路径都可以
    if(!file.open(QFile::WriteOnly|QFile::Truncate)) //可以用QIODevice，Truncate表示清空原来的内容
        return;
    QDomDocument doc;
    QDomProcessingInstruction instruction; //添加xml说明
    instruction=doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);
    QDomElement root=doc.createElement("annotation");
    root.setAttribute("verified","no");
    doc.appendChild(root);

    QFileInfo  fileInfo = QFileInfo(xmlPath);
    QString fileName = fileInfo.baseName();

    QString a = fileInfo.absolutePath();
    int index = a.lastIndexOf('/');

    QDomElement picElement=doc.createElement("folder");
    picElement.appendChild(doc.createTextNode(a.mid(index + 1, -1)));
    root.appendChild(picElement);
    QDomElement filenameElement=doc.createElement("filename");
    filenameElement.appendChild(doc.createTextNode(fileName));
    root.appendChild(filenameElement);
    QDomElement pathElement=doc.createElement("path");
    pathElement.appendChild(doc.createTextNode(fileInfo.absolutePath() + QString("/") +fileName + suffix));
    root.appendChild(pathElement);

    QDomElement source=doc.createElement("source");
    QDomElement databaseElement=doc.createElement("database");
    databaseElement.appendChild(doc.createTextNode("Unkonwn"));
    source.appendChild(databaseElement);
    root.appendChild(source);

    QDomElement size=doc.createElement("size");
    QDomElement widthElement=doc.createElement("width");
    widthElement.appendChild(doc.createTextNode(QString::number(w)));
    size.appendChild(widthElement);
    QDomElement heightElement=doc.createElement("height");
    heightElement.appendChild(doc.createTextNode(QString::number(h)));
    size.appendChild(heightElement);
    QDomElement depthElement=doc.createElement("depth");
    depthElement.appendChild(doc.createTextNode(QString::number(depth)));
    size.appendChild(depthElement);
    root.appendChild(size);

    QDomElement segmentElement=doc.createElement("segmented");
    segmentElement.appendChild(doc.createTextNode("0"));
    root.appendChild(segmentElement);
    for(std::vector<Object>::iterator iter = objectSet.begin();iter != objectSet.end();iter++)
    {
        QDomElement object=doc.createElement("object");
        QDomElement typeElement=doc.createElement("type");
        DRAWTYPE t = (*iter).GetType();
        if (t == DRAWTYPE::TypeOfRoRect)
        {
            typeElement.appendChild(doc.createTextNode("robndbox"));
        }
        else if(t == DRAWTYPE::TypeOfRect)
        {
            typeElement.appendChild(doc.createTextNode("bndbox"));
        }
        object.appendChild(typeElement);
        QDomElement nameElement=doc.createElement("name");
        nameElement.appendChild(doc.createTextNode((*iter).GetClass()));
        object.appendChild(nameElement);
        QDomElement poseElement=doc.createElement("pose");
        poseElement.appendChild(doc.createTextNode("Unspecified"));
        object.appendChild(poseElement);
        QDomElement truncatedElement=doc.createElement("truncated");
        truncatedElement.appendChild(doc.createTextNode("0"));
        object.appendChild(truncatedElement);
        QDomElement difficultElement=doc.createElement("difficult");
        if((*iter).GetDifficulty())
        {
            difficultElement.appendChild(doc.createTextNode("1"));
        }
        else
        {
            difficultElement.appendChild(doc.createTextNode("0"));
        }
        object.appendChild(difficultElement);

        if (t == DRAWTYPE::TypeOfRoRect)
        {
            cv::RotatedRect r = Change2RoRect((*iter).GetPointSet());
            QDomElement objectinfo = doc.createElement("robndbox");
            QDomElement cx=doc.createElement("cx");
            cx.appendChild(doc.createTextNode(QString::number(r.center.x)));
            objectinfo.appendChild(cx);
            QDomElement cy=doc.createElement("cy");
            cy.appendChild(doc.createTextNode(QString::number(r.center.y)));
            objectinfo.appendChild(cy);
            QDomElement w=doc.createElement("w");
            w.appendChild(doc.createTextNode(QString::number(r.size.width)));
            objectinfo.appendChild(w);
            QDomElement h=doc.createElement("h");
            h.appendChild(doc.createTextNode(QString::number(r.size.height)));
            objectinfo.appendChild(h);
            QDomElement angle=doc.createElement("angle");
            angle.appendChild(doc.createTextNode(QString::number(r.angle*M_PI/180)));
            objectinfo.appendChild(angle);
            object.appendChild(objectinfo);
            root.appendChild(object);
        }
        else if(t == DRAWTYPE::TypeOfRect) // !!!!!!!!
        {
            cv::Rect r = Change2Rect((*iter).GetPointSet());
            QDomElement objectinfo = doc.createElement("bndbox");
            QDomElement cx=doc.createElement("xmin");
            cx.appendChild(doc.createTextNode(QString::number(r.tl().x))); // !!!
            objectinfo.appendChild(cx);
            QDomElement cy=doc.createElement("ymin");
            cy.appendChild(doc.createTextNode(QString::number(r.tl().y))); // !!!
            objectinfo.appendChild(cy);
            QDomElement w=doc.createElement("xmax");
            w.appendChild(doc.createTextNode(QString::number(r.br().x))); // !!!
            objectinfo.appendChild(w);
            QDomElement h=doc.createElement("ymax");
            h.appendChild(doc.createTextNode(QString::number(r.br().y))); // !!!
            objectinfo.appendChild(h);
            object.appendChild(objectinfo);
            root.appendChild(object);
        }
    }
    QTextStream out_stream(&file); //输出到文件
    doc.save(out_stream,2); //缩进4格
    file.close();
}

int ReadXml(QString xmlPath, std::vector<Object>& objectSet, const QVector<QString>& classlist, const QVector<QColor>& colorlist)
{
    objectSet.clear();
    QFile file(xmlPath);
    if(!file.open(QFile::ReadOnly))
        return 1;
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return 1;
    }
    file.close();
    QDomElement objectStoreElement=doc.documentElement(); //返回根元素
    QDomNodeList bookNodeList=objectStoreElement.childNodes();
    objectSet.reserve(bookNodeList.count());
    for(int i=0; i<bookNodeList.count(); i++)
    {
        if(bookNodeList.at(i).toElement().tagName() == QString("object"))
        {
            QDomElement obejctElement=bookNodeList.at(i).toElement();
            QDomNodeList childNodeList=obejctElement.childNodes();
            QString type = childNodeList.at(1).toElement().text();
            QString difficult = childNodeList.at(4).toElement().text();
            QDomNodeList subobjectlist = childNodeList.at(5).toElement().childNodes();
            int index = classlist.indexOf(type);
            if(childNodeList.at(5).toElement().tagName() == QString("robndbox"))
            {
                QString cx = subobjectlist.at(0).toElement().text();
                QString cy = subobjectlist.at(1).toElement().text();
                QString w = subobjectlist.at(2).toElement().text();
                QString h = subobjectlist.at(3).toElement().text();
                QString angle = subobjectlist.at(4).toElement().text();
                cv::RotatedRect r(cv::Point2f(cx.toFloat(),cy.toFloat()),cv::Size2f(w.toFloat(),h.toFloat()),angle.toFloat()*180/M_PI);
                cv::Point2f p[4];
                r.points(p);
                std::vector<QPointF> set(4);
                for(int i = 0;i<4;i++)
                {
                    set[i] = QPointF(p[i].x,p[i].y);
                }

                Object objectTemp(set,DRAWTYPE::TypeOfRoRect);

                objectTemp.SetClass(type);
                if (difficult == QString("1"))
                {
                    objectTemp.SetDifficulty(true);
                }
                else
                {
                    objectTemp.SetDifficulty(false);
                }
                if (index != -1)
                {
                    objectTemp.SetColor(colorlist.at(index));
                }
                else
                {
                    objectTemp.SetColor(QColor(255, 255, 255));
                }
                objectSet.push_back(objectTemp);
            }
            else if(childNodeList.at(5).toElement().tagName() == QString("bndbox"))//这里需要完善
            {
                QString xmin = subobjectlist.at(0).toElement().text();
                QString ymin = subobjectlist.at(1).toElement().text();
                QString xmax = subobjectlist.at(2).toElement().text();
                QString ymax = subobjectlist.at(3).toElement().text();
                cv::Rect r(cv::Point(xmin.toInt(),ymin.toInt()),cv::Point(xmax.toInt(),ymax.toInt()));
                std::vector<QPointF> set = {QPointF(r.x,r.y),QPointF(r.x + r.width,r.y),QPointF(r.x + r.width,r.y + r.width),QPointF(r.x,r.y + r.height)};
                Object objectTemp(set,DRAWTYPE::TypeOfRoRect);
                objectTemp.SetClass(type);
                if (difficult == QString("1"))
                {
                    objectTemp.SetDifficulty(true);
                }
                else
                {
                    objectTemp.SetDifficulty(false);
                }
                if (index != -1)
                {
                    objectTemp.SetColor(colorlist.at(index));
                }
                else
                {
                    objectTemp.SetColor(QColor(255, 255, 255));
                }
                objectSet.push_back(objectTemp);
            }
        }
    }
    return 0;
}
