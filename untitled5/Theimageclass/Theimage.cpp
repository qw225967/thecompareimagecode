//
// Created by 方睿虔 on 2020/7/20.
//
#include "Theimage.h"
#include <vector>
using namespace std;


int imagetest::imageSubtract(Mat &image1, Mat &image2)
{
    if ((image1.rows != image2.rows) || (image1.cols != image2.cols))
    {
        if (image1.rows > image2.rows)
        {
            resize(image1, image1, image2.size(), 0, 0, INTER_LINEAR);
        }
        else if (image1.rows < image2.rows)
        {
            resize(image2, image2, image1.size(), 0, 0, INTER_LINEAR);
        }
    }

    Mat image1_gary, image2_gary;
    if (image1.channels() != 1)
    {
        cvtColor(image1, image1_gary, COLOR_BGR2GRAY);
    }
    if (image2.channels() != 1)
    {
        cvtColor(image2, image2_gary, COLOR_BGR2GRAY);
    }

    Mat frameDifference, absFrameDifferece;
    Mat previousGrayFrame = image2_gary.clone();
    //图1减图2
    subtract(image1_gary, image2_gary, frameDifference, Mat(), CV_16SC1);

    //取绝对值
    absFrameDifferece = abs(frameDifference);

    //位深的改变
    absFrameDifferece.convertTo(absFrameDifferece, CV_8UC1, 1, 0);
    imshow("absFrameDifferece", absFrameDifferece);
    waitKey(0);

    Mat segmentation;

    //阈值处理（这一步很关键，要调好二值化的值）
    threshold(absFrameDifferece, segmentation,100, 255, THRESH_BINARY);

    //中值滤波
    medianBlur(segmentation, segmentation, 3);

    //形态学处理(开闭运算)
    //形态学处理用到的算子
    Mat morphologyKernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
    morphologyEx(segmentation, segmentation, MORPH_CLOSE, morphologyKernel, Point(-1, -1), 2, BORDER_REPLICATE);

    //显示二值化图片
    imshow("segmentation", segmentation);
    waitKey(0);


    //找边界
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(segmentation, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));//CV_RETR_TREE
    vector< vector<Point> > contours_poly(contours.size());

    vector<Rect> boundRect;
    boundRect.clear();

    for (int index = 0; index < contours.size(); index++)
    {
        approxPolyDP(Mat(contours[index]), contours_poly[index], 3, true);
        Rect rect = boundingRect(Mat(contours_poly[index]));
        if(rect.area() > 5)
            return 0;
        //rectangle(image2, rect, Scalar(0, 255, 0), 2);
    }
    return 1;
    //imshow("效果图", image2);
}