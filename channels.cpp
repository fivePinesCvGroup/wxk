#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
using namespace cv;
using namespace std;

Mat grayImage , imgHSV , mask , kernel1, imgEro , imgcanny;
Mat grayImage1 , grayImage2;


int hmin =89, smin =74 , vmin =0;
int hmax =169, smax =255, vmax =255;

int main()
{
    string path = "/Users/alexking/Desktop/111/image-20221010221706278.png";
    Mat img = imread(path);
    
    
    //cvtColor(img , imgHSV , COLOR_BGR2HSV);
    
   // Scalar lower(hmin,smin,vmin);
    //Scalar upper(hmax,smax,vmax);
    
    
    //inRange(imgHSV,lower,upper,mask);
    
    vector<Mat> channels;
    
    split(img , channels);
    
    grayImage =  channels.at(0) *1.5 ;
    grayImage1 =  channels.at(1) *1.298;
    grayImage2 =  channels.at(2) *1.5;
    
   
    
    
  /*  blur (grayImage ,grayImage,Size(1,3));//均值滤波
    threshold(grayImage,grayImage ,0, 255, THRESH_BINARY + THRESH_OTSU);
    
    bitwise_not(grayImage,grayImage);
    
    kernel1= getStructuringElement(MORPH_RECT,Size(5,5));
    
    Canny(grayImage,imgcanny,25,75);
    
    
    dilate(imgcanny,imgEro ,kernel1 );*/
   // bitwise_not(grayImage,grayImage);
    //bitwise_not(grayImage1,grayImage1);
    //bitwise_not(grayImage2,grayImage2);
    
    imshow("Img",img);
   imshow("Grayimg", grayImage);
    imshow("Grayimg1", grayImage1);
    imshow("Grayimg2", grayImage2);
    //imshow("GrayEro", imgcanny);
    waitKey(0);
}
