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

    Mat img=imread("C:\\Users\\cheet\\Desktop\\QQ20200625190228.jpg");
    cv::imshow("image",img);
    cv::waitKey();

    system("pause");
    return 0;
}