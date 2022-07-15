#include<iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace cv;
using namespace std;

Mat img ,mask , imgBlur , imgBright , kernel1 , imgEro ,imgDil ,imgCanny;

int hmin =18, smin =50 , vmin =103;
int hmax =60, smax =255, vmax =255;
int main ()
{
    VideoCapture cap(0);
    while(true)
    {
        cap.read(img);
        cvtColor(img , mask ,COLOR_BGR2HSV);
        
        Scalar lower(hmin ,smin ,vmin);
        Scalar upper(hmax ,smax ,vmax);
        
        inRange(mask ,lower ,upper,mask);
        GaussianBlur(mask ,imgBlur ,Size(11,11),2,2);
        threshold(imgBlur ,imgBright ,127,255, THRESH_BINARY)
        
        Mat kernel = getStructuringElement(MORPH_RECT,Size(3,3));
        morphologyEx(imgBright ,mask ,MORPH_CLOSE,kernel);
        kernel1= getStructuringElement(MORPH_RECT,Size(2,2));
        erode(mask ,imgEro ,kernel1 , Point(-1,-1),5);
        
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(imgEro.clone(), contours , hierarchy ,RETR_TREE,CHAIN_APPROX_SIMPLE);
        
        for( int i =0 ; i <contours.size() ; i++)
        {
            RotatedRect rect =minAreaRect(contours[i]);
            Point2f p[4];
            rect.points(p);
            
            int area = contourArea(contours[i]);
            if (area<25000 && area >2000)
            {
                for (int j=0 ; j<4 ; j++)
                {
                    line(img, p[j], p[(j+1)%4] ,Scalar(0,0,255),2,8);
                }
                
                line(img ,p[0], p[2] ,Scalar(255,255,0),2);
                
                double k = (rect.size.width + rect.size.height)*10;
                double k2 =(rect.size.width / rect.size.height)*10;
                
                if (k<1200)
                {
                    cout <<"空白面"<<endl;
                    break;
                }
                if (contours.size()== 6)
                {
                if (k2 >12 || k2 <7)
                {
                    cout <<"条形码面"<<endl;
                    break;
                }
                else if ( k2 <11 && k2 >7)
                {
                    cout << "R面" <<endl;
                    break;
                }
                }
            }
        }
        imshow("Image", imgEro);
        imshow("Image 2" , img);
        waitKey(1);
        
    }
}
