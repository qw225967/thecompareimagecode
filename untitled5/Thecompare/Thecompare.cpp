//
// Created by 方睿虔 on 2020/7/20.
//
#include "Thecompare.h"
#include "../Theimageclass/Theimage.h"
#include <iostream>

map<int,int> Thecompare::getsameimage(){
    char str1[128];
    char str2[128];
    int count = 0;
    map<int,int> tempmap;
    int thecaton_count = 0;
    //============================输入内容
    std::cout << "请输入想要对比的图片序号的开始"<< endl;
    int i_start;
    std::cin >> i_start;
    std::cout<< "请输入想要对比的图片序号的结束" << endl;
    int i_end;
    std::cin>>i_end;
    std::cout<< "请输入想要统计的卡顿时间(建议测试100ms时填写2，其他时间可以按每100毫秒为1累加)" << endl;
    int thecatoncount;
    std::cin>>thecatoncount;
    std::cout<< "请输入存储图片的文件夹的详细路径" << endl;
    char theimagedirpath[1024];
    std::cin>>theimagedirpath;

    strcat(theimagedirpath,"/foo-0%05d.jpeg");
    //============================输入内容
    int i;
    for (i = i_start; i < i_end; i++) {


        //snprintf(str1, 127, "/Users/fangruiqian/Downloads/test/foo-0%04d.jpeg", i);
        //snprintf(str2, 127, "/Users/fangruiqian/Downloads/test/foo-0%04d.jpeg", i+1);
        snprintf(str1, 127, theimagedirpath, i);
        snprintf(str2, 127, theimagedirpath, i+1);

        Mat srcImage1 = imread(str1);
        Mat srcImage2 = imread(str2);


        if (!srcImage1.data || !srcImage2.data)return tempmap;
        Rect rect(150, 480, 400, 200);
        Mat image_roi1 = srcImage1(rect);
        Mat image_roi2 = srcImage2(rect);
        imshow("效果图", image_roi1);
        waitKey(0);

        imagetest c_imagetest;//图片对比对象

        if(c_imagetest.imageSubtract(image_roi1, image_roi2)){
            count++;
        }else{
            count=0;
        }
        if(count >= thecatoncount){

            if(count == thecatoncount){
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
}