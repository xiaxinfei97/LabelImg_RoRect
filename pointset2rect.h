#include<vector>
#include<QPoint>
#include<opencv2/opencv.hpp>

#ifndef POINTSET2RECT_H
#define POINTSET2RECT_H
///
/// \brief 将点集转换为斜矩形
///
cv::RotatedRect Change2RoRect(std::vector<cv::Point> PointSet);
cv::RotatedRect Change2RoRect(std::vector<QPointF> PointSet);

///
/// \brief 将点集转化成正矩形
///
cv::Rect Change2Rect(std::vector<cv::Point> PointSet);
cv::Rect Change2Rect(std::vector<QPointF> PointSet);
#endif // POINTSET2RECT_H
