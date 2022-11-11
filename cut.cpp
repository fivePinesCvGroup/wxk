#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;

Mat mask,imgHSV,imgCrop,img1;

int hmin= 0, smin= 0, vmin=64;
int hmax=179, smax=255,vmax=255;

/*Mat koutu(Mat mask , Mat img)
{
    Mat imgCrop;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    
    findContours(mask, contours , hierarchy ,RETR_TREE,CHAIN_APPROX_SIMPLE);
    vector<Rect> boundRect(contours.size());
    vector<RotatedRect> box (contours.size());
    
    for( int i =0 ; i <contours.size() ; i++)
    {
        box[i] = minAreaRect(contours[i]);
         Point2f p[4];
        box[i].points(p);
        
        boundRect[i] = boundingRect(contours[i]);
        
        

        //for (int j =0 ; j<4 ; j++ )
        //{
          //  line(img,p[j],p[(j+1)%4],Scalar(0,0,255),2,8);
        //}
                        
                        
    }
   
    
    Rect roi(Rect(boundRect[0].x,boundRect[0].y,boundRect[0].width,boundRect[0].height));
    imgCrop = img(roi);
    
    return imgCrop;
}*/

int main()
{
    string path = "/Users/alexking/Desktop/111/image-20221010210409895.png";
   Mat img = imread(path);
    
    
    Scalar lower(hmin ,smin ,vmin);
    Scalar upper(hmax ,smax ,vmax);
    
    cvtColor(img,imgHSV,COLOR_BGR2HSV);
    inRange(imgHSV,lower,upper,mask);
    
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    
    findContours(mask, contours , hierarchy ,RETR_TREE,CHAIN_APPROX_SIMPLE);
    vector<Rect> boundRect(contours.size());
    vector<RotatedRect> box (contours.size());
    
    for( int i =0 ; i <contours.size() ; i++)
    {
        box[i] = minAreaRect(contours[i]);
         Point2f p[4];
        box[i].points(p);
        
        boundRect[i] = boundingRect(contours[i]);
        
                        
                        
    }
   
    
    Rect roi(Rect(boundRect[0].x,boundRect[0].y,boundRect[0].width,boundRect[0].height));
    imgCrop = img(roi);
    
    
    //Mat M(128,64,CV_8UC3, Scalar(255,255,255));
    Mat imgROI =img(Rect(Rect(0,0,boundRect[0].width,boundRect[0].height)));
   
    imgCrop.copyTo(imgROI);
    
    namedWindow("result");
    imshow("result", img);
    imshow("Image", mask);
    //imshow("Image Crop", imgCrop);
    
    
    waitKey(0);
}
