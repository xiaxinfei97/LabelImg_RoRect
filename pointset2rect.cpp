#include "pointset2rect.h"


cv::RotatedRect Change2RoRect(std::vector<cv::Point> PointSet)
{
    cv::RotatedRect roRect = cv::minAreaRect(PointSet);
    return roRect;
}

cv::RotatedRect Change2RoRect(std::vector<QPointF> PointSet)
{
    std::vector<cv::Point> set;
    for (size_t i = 0;i<PointSet.size();i++)
    {
        set.push_back(cv::Point((int)PointSet[i].x(),(int)PointSet[i].y()));
    }
    return Change2RoRect(set);
}


cv::Rect Change2Rect(std::vector<cv::Point> PointSet)
{
    cv::Rect rect = cv::boundingRect(PointSet);
    return rect;
}

cv::Rect Change2Rect(std::vector<QPointF> PointSet)
{
    std::vector<cv::Point> set;
    for (size_t i = 0;i<PointSet.size();i++)
    {
        set.push_back(cv::Point((int)PointSet[i].x(),(int)PointSet[i].y()));
    }
    return Change2Rect(set);
}
