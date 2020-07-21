#include "Thecompare/Thecompare.h"
#include <iostream>
#include <fstream>
#include <map>


int main() {

    map<int,int> countmap; //临时计数map
    cout << "请输入想要保存的位置（真实路径）" << endl;
    char theoutpath[1024];
    cin>>theoutpath;

    Thecompare c_compare; //对比对象
    countmap = c_compare.getsameimage(); //返回对比内容

    ofstream oFile; //打开一个文件
    oFile.open(theoutpath,ios::out|ios::trunc); //创建一个文件test.csv

    //写入内容
    oFile<<"time"<<","<<"mark"<<endl;
    int test = countmap.size();
    int thecaton_time = 0;
    int i;
    for(i=0;i<test-1;i++){
        int tempresult = countmap[i];
        oFile<<i<<","<<tempresult<<endl;
        if(countmap[i] == 1)thecaton_time++;
    }
    float catonrate = (float)thecaton_time/((float)(test-1));
    oFile<< "caton_rate"<<","<<catonrate <<endl;
    int caton = countmap[test+1];
    oFile<< "caton_time"<<","<< caton <<endl;
    oFile.close();


    return 0;
}







//====================================================以下是面向过程方法，以上是面向对象方法================================================================
//#include <iostream>
/*#include "/usr/local/include/opencv4/opencv2/core/cvdef.h"
#include "/usr/local/include/opencv4/opencv2/core.hpp"       // 核心功能
#include "/usr/local/include/opencv4/opencv2/imgproc.hpp"    // 图像处理模块
#include "/usr/local/include/opencv4/opencv2/highgui.hpp"    // 读写图像和视频的函数，以及用户交互函数
#include "/usr/local/include/opencv4/opencv2/highgui/highgui_c.h"*/
//#include <fstream>
//#include <stdio.h>
//#include <map>

//using namespace std;
//using namespace cv;


/*int imageSubtract(Mat &image1, Mat &image2)
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
    //imshow("absFrameDifferece", absFrameDifferece);
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
    //imshow("segmentation", segmentation);

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
}*/

/*map<int,int> getsame(){
    char str1[128];
    char str2[128];
    int count = 0;
    map<int,int> tempmap;
    int thecaton_count = 0;
    int i;
    for (i = 1; i < 7717; i++) {
        snprintf(str1, 127, "/Users/fangruiqian/Downloads/test/foo-0%04d.jpeg", i);
        snprintf(str2, 127, "/Users/fangruiqian/Downloads/test/foo-0%04d.jpeg", i+1);

        Mat srcImage1 = imread(str1);
        Mat srcImage2 = imread(str2);
        //Mat srcImage1 = imread("/Users/fangruiqian/Downloads/test/foo-00550.jpeg");
        //Mat srcImage2 = imread("/Users/fangruiqian/Downloads/test/foo-00560.jpeg");
        if (!srcImage1.data || !srcImage2.data)return tempmap;
        Rect rect(100, 375, 200, 100);
        Mat image_roi1 = srcImage1(rect);
        Mat image_roi2 = srcImage2(rect);
        if(imageSubtract(image_roi1, image_roi2)){
            count++;
        }else{
            count=0;
        }
        if(count >= 5){

            if(count == 5){
                thecaton_count++;
            }
            for(int j=i-count+1;j<i+1;j++){
                tempmap[j] = 1;
            }
            continue;
        }
        tempmap[i] = 0;
    }
    tempmap[i+1] = thecaton_count;
    return tempmap;
}*/



/*int main() {

    map<int,int> countmap;
    countmap=getsame();

    ofstream oFile;
    oFile.open("/Users/fangruiqian/Desktop/test.csv",ios::out|ios::trunc);
    oFile<<"time"<<","<<"mark"<<endl;
    int test = countmap.size();
    int thecaton_time = 0;
    int i;
    for(i=0;i<test-1;i++){
        int tempresult = countmap[i];
        oFile<<i<<","<<tempresult<<endl;
        if(countmap[i] == 1)thecaton_time++;
    }
    float catonrate = (float)thecaton_time/((float)(test-1));
    oFile<< "caton_rate"<<","<<catonrate <<endl;
    int caton = countmap[test+1];
    oFile<< "caton_time"<<","<< caton <<endl;
    oFile.close();


    return 0;
}*/
