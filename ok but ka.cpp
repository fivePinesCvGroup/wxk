#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

Mat koutu(Mat imgDi;, Mat img1)
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
        
        img2=img1(Rect(boundRect[i].x,boundRect[i].y,boundRect[i].width,boundRect[i].height));
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
        Scalar lower(hmin1 , smin1 , vmin1);
        Scalar upper(hmax1 , smax1 , vmax1);
        cvtColor(img1,imgHSV,COLOR_BGR2HSV);
        inRange(imgHSV,lower,upper,mask);
        
        Mat kernel = hetStructuringElement(MORPH_RECT,Size(7,7));
        
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
        findContours(imgDil, contours , hierarchy, RETR_TREE ,CHAIN_APPROX_SIMPLE);
        vector<Rect> boundRect(contours.size());
        vector<RotateRect> box(contours.size());
        int img_width =img2.cols;
        int img_height = img2.rows;
        Vec4f fitline;
        
        for(int i =0; i<contours.size(); i++)
        {
            box[i] =minAreaRect(Mat(contours[i]));
            Point2f rect[4];
            boundRect[i] = boundingRect(Mat(contours[i]));
            box[i].points(rect);
            
            int area = contourArea(contours[i]);
            
            if (area >5000)
            {
                for (int j =0 ; j<4 ; j++ )
                {
                    line(img2,rect[j],rect[(j+1)%4],Scalar(0,0,255),2,8);
                }
            }
            
            fitLine(box[i],fitline,CV-DIST_L2,0,0.01,0.01);//或 contours[i];
            
            double k_line = fitline[1]/fitline[0];
            
            Point p1(0,k_line * (0-fitline[2])+ fitline[3]);
            Point p2(img_width-1, k_line *(img_width-1-fitline[2] + fitline[3]) +fitline[3]);
            
            line(img2,p1,p2,Scalar(0,0,255),2);
            cout<<"k"<<k_line<<endl;
            }
        
        
        imshow("Image",imgDil);
        imshow("Image open",img1);
        imshow("Image 2",img2);
        
        waitKey(1);}
    }

