//
//  main.cpp
//  wxk
//
//  Created by Alex·King on 2022/7/10.
//

#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

Mat imgGray,imgBlur,imgCanny,imgDil;

void getContours(Mat imgDil,Mat img1)
{
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    
    findContours(imgDil,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);
    
    vector<Rect> boundRect(contours.size());
    
    vector<RotatedRect> box(contours.size());
    
    Point2f rect[4];
    
    for(int i =0 ;i <contours.size(); i++)
    {
        box[i] =minAreaRect(Mat(contours[i]));
        
        boundRect[i] = boundingRect(Mat(contours[i]));
        
        box[i].points(rect);
        
        int area = contourArea(contours[i]);
        
        cout<<area<<endl;
        
        if (area>5000)
        {
            for (int j =0; j<4; j++)
            {
                line( img1, rect[j] , rect[(j+1)%4], Scalar(0 , 0 , 255 ),2, 8);
            }
            
            drawContours(img1 , contours , i ,Scalar(255 ,0 ,255),10 );
            
        }
        
        
        
    }
}

int hmin = 100 , smin = 150 , vmin = 150;
int hmax  =123 , smax = 255,  vmax = 255;
Mat imgHSV , mask;

int main()
{
    Mat img, img1 ;
    
    VideoCapture cap(0);
    
    while(ture)
    {
        cap.read(img);
        
        resize(img , img1,Size(500,500));
        Scalar lower(hmin,smin,vmin);
        Scalar upper(hmax, smax, vmax);
        cvtColor(img1 , imgHSV, COLOR_BGR2HSV)；
        inRange(imgHSV, lower , upper ,mask);
        
        Mat kernel = getStructuringElement(MORPH_RECT,Size(7,7));
        
        dilate( mask , imgDil , kernel);
        
        getContours(imgDil , img1);
        
        imshow("Image",imgDil);
        
        imshow("Image Draw",img1);
        
        waitKey(1);
    }
        
        
        
        
    }
    
    
    
    
    
    
    
}
