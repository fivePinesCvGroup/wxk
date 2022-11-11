#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
using namespace cv;
using namespace std;

Mat matrix , imgWarp;

float w =250 , h = 350;


int main()
{
    string path = "/Users/alexking/Desktop/111/003.jpg";
    Mat img = imread(path);
    
    Point2f src[4] = { {503,273}, {562,328},{393,315}, {453,375} };
    Point2f dst[4] = { {0.0f,0.0f}, {w,0.0f},{0.0f,h}, {w,h} };
    
    matrix = getPerspectiveTransform(src , dst);
    warpPerspective(img , imgWarp , matrix , Point(w,h));
    
    for (int i = 0 ; i <4 ; i++)
    {
        circle( img , src[i]  ,10 , Scalar(0,0,255), FILLED);
    }
    
    imshow("Img",img);
    imshow("Image Warp", imgWarp);
    waitKey(0);
}

