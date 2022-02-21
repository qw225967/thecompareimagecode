//
// Created by 方睿虔 on 2020/7/20.
//
#include "/opt/homebrew/Cellar/opencv/4.5.4_3/include/opencv4/opencv2/core/hal/interface.h"
#include "/opt/homebrew/Cellar/opencv/4.5.4_3/include/opencv4/opencv2/core/cvdef.h"
#include "/opt/homebrew/Cellar/opencv/4.5.4_3/include/opencv4/opencv2/core.hpp"       // 核心功能
#include "/opt/homebrew/Cellar/opencv/4.5.4_3/include/opencv4/opencv2/imgproc.hpp"    // 图像处理模块
#include "/opt/homebrew/Cellar/opencv/4.5.4_3/include/opencv4/opencv2/highgui.hpp"    // 读写图像和视频的函数，以及用户交互函数
#include "/opt/homebrew/Cellar/opencv/4.5.4_3/include/opencv4/opencv2/highgui/highgui_c.h"


using namespace cv;

class imagetest{
    public:
        imagetest(){}
        ~imagetest(){}
    public:
        int imageSubtract(Mat &image1, Mat &image2);//图片对比函数
};