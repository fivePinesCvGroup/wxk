#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

Mat koutu(Mat imgDil, Mat img1)
{
    Mat img2;
    vector<vector<Point>> contours;
    vector<Vec4i>hierarchy;
    findContours(imgDil,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    vector<Rect> boundRect(contours.size());
    vector<RotatedRect> box(contours.size());
    
    for(int i=0 ; i<contours.size(); i++)
    {
        box[i] = minAreaRect(Mat(contours[i]));
        Point2f rect[4];
        
        boundRect[i] = boundingRect(Mat(contours[i]));
        box[i].points(rect);
        int area = contourArea(contours[i]);
        if ( area >2000 )
        {
        img2=img1(Rect(boundRect[i].x,boundRect[i].y,boundRect[i].width,boundRect[i].height));
        }
        if((img2.empty()))
        {
            continue;
        }
        else
        {
            resize(img2,img2,Szie(400,400),1);
        }
      
        }
    return img2;
    }

Mat img1,imgGray,imgBlur,imgCanny,imgDil,imgcrop,imgHSV,mask,img2;
Mat imgHSV1,mask1,imgGray1,imgDill1;
int hmin=100, smin=150 ,vmin =150;
int hmax=123, smax=255 ,vmax =255;
int hmin1=0, smin1=0 ,vmin1 =221;
int hmax1=180, smax1=30 ,vmax =255;

int main()
{
    Mat img;
    VideoCapture cap(0);
    while(true)
    {
        cap.read(img);
        resize(img,img1,Size(500,500));
        Scalar lower(hmin , smin , vmin);
        Scalar upper(hmax , smax , vmax);
        Scalar lower1(hmin1 , smin1 , vmin1);
        Scalar upper1(hmax1 , smax1 , vmax1);
        cvtColor(img1,imgHSV,COLOR_BGR2HSV);
        inRange(imgHSV,lower,upper,mask);
        
        Mat kernel = getStructuringElement(MORPH_RECT,Size(7,7));
        
        dilate(mask , imgDil ,kernel);
        
        img2 = koutu(imgDil,img1);
        
        if((img2.empty()))
        {
            continue;
        }
        cvtColor(img2,imgHSV1,COLOR_BGR2HSV);
        inRange(imgHSV1,lower1,upper1,mask1);
        
        erode(mask1, imgDil1,kernel);
        
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(imgDil1, contours , hierarchy, RETR_TREE ,CHAIN_APPROX_SIMPLE);
        
        for(int i =0; i<contours.size(); i++)
        {
            RotatedRect rect = minAreaRect(contours[i]);
            Point2f p[4];
            double angle = rect.angle;
            rect.points(p);
            
            int area = contourArea(contours[i]);
            
            if (area >4000)
            {
                for (int j =0 ; j<4 ; j++ )
                {
                    line(img2,p[j],p[(j+1)%4],Scalar(0,0,255),2,8);
                }
                
                line(img2,p[0],p[2],Scalar(255,255,0),2);
                
                cout << angle << endl;
                
                if (angle <-60 || angle > -20)
                {
                    cout << "直行" << endl;
                    break;
                }
                
                float value1 =Point(p[0]).dot(Point(p[2]));
                
                if(value1 > 60000)
                {
                    cout<<"向左"<<endl;
                    break;
                }
                
                else
                {
                    cout<<"向右"<<endl;
                    break;
                }
            }
        }
            
            
        
        
        imshow("Image",imgDil);
        imshow("Image open",img1);
        imshow("Image 2",img2);
        imshow("Img Dil1",imgDil1);
        
        waitKey(1);}
    }
