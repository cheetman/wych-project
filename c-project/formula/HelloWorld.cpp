#include <stdio.h>
#include <windows.h>
#include <iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
using namespace std;
using namespace cv;
int main()
{


    printf("Hello World\n");
    cout << "Hello World" << endl;

    Mat imgRgb, imgGary;
    imgRgb=cv::imread("C:\\Users\\cheet\\Desktop\\QQ20200625190228.jpg");

    const uchar* dd = imgRgb.datastart;
    
    cv::namedWindow("src img", CV_WINDOW_AUTOSIZE);
    cv::imshow("src img",imgRgb);
    // cv::cvtColor(imgRgb,cv::COLOR_BGR2GRAY);

    // 转换灰度图
    cvtColor(imgRgb, imgGary, cv::COLOR_BGR2GRAY); 
    // cvtColor(imgRgb, imgGary, CV_BGR2GRAY); 
    // cvtColor(src, dst, CV_BGR2RGB); //bgr to rgb
    // cvtColor(src, dst, CV_BGR2HSV);
    namedWindow("gray img");
    imshow("gray img", imgGary);



    // 老方法 c接口
    // IplImage *pic = cvLoadImage("C:\\Users\\cheet\\Desktop\\QQ20200625190228.jpg", 1);
	// cvShowImage("load", pic);
    // cvWaitKey(0);
    


    cv::waitKey();

    
    system("pause");
    return 0;
}