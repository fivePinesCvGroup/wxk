#include sopencv2/opencv .hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include sopencv2/img codecs .hpp>
#include <iostream>

using namespace cv;
using namespace std;

class LightDescriptor {
public:
LightDescriptor(){}
LightDescriptor(const cv::RotatedRect & light)
{
    width = light.size.width;
    length = light.size.height;
    center = light.center;
    angle= light.angle;
    area = light.size.area();
    light.points(pts);
}

const LightDescriptor & operator = (const LightDescriptor & ld)
{
    this->width = ld.width;
    this->length = ld.length;
    this->center = ld.center;
    this->angle = ld.angle;
    this->area = ld.area;
    
    return *this;
}

cv::RotatedRect rec() constf
    {
    return cv::RotatedRect(center , cv::Size2f (width, length),angle) ;
        
    }
public:

float width;
float length;
cv:: Point2f center;
float angle;
float area;
cv:: Point2f pts[4];
};


int red_brightness_threshold = 10;
int blue_brightness_threshold = 10;
float light_min_area = 30.0;
float light_min_size= 4.0;
float light_extend_ratio = 1.1;
float light_width_height_ratio = 0.9;
float light_contour_rect_ratio= 0.65;
float light_max_angle_diff = 7.0;
float light_max_height_diff_ratio= 0.2;
float small_armor_min_aspect_ratio = 1.86;
float small_armor_max_aspect_ratio= 2.2;
float big_armor_max_aspect_ratio=4.2;
float big_armor_min_aspect_ratio=3.8;
string enemy_color="BLUE";
int threshold_param;
vector<Mat> channels;
Mat binBrightImage_light, grayImage;


int main ()
{
 VideoCapture cap(0);
 VideoCapture(
 Mat img;

while (true)
{

    cap. read (img);
    Mat img1 = Mat::zeros (img.size(),img.type());
    Mat BrightnessLut(1,256,CV_8UC1);
for (int i =0: i<256: i++)
{
    BrightnessLut.at<uchar>(i) = saturate_cast<uchar>(i + -133);
}

    LUT(img, BrightnessLut, img1);//降低图像亮度

    Mat kernel= getStructuringElement(MORPH_RECT ‚Size(3,3)) ;

    Scalar lower (0,20,120);
    Scalar upper (200,255,255);
    Mat mask, copy;
    waitKey(1);
    img1. copyTo(copy);
    cvtColor(img1 , mask , COLOR BGR2HSV);
    inRange (mask ,lower ,upper ,mask);
    split(img1 ,channels) ;
   
    if (enemy_color=="RED")
    {
        grayImage = channels.at (2) - channels.at (0) ;
        threshold_param = red_brightness_threshold;
    }
    else
    {
        grayImage = channels.at (0) - channels.at (2) ;
        threshold_param = blue_brightness_threshold;
    }
    blur (grayImage ,grayImage,Size(1,3));//均值滤波
    threshold(grayImage ,binBrightImage_light , threshold_param,255, THRESH_BINARY);
    bitwise_and (binBrightImage_light,mask,binBrightImage_light);
    dilate (binBrightImage_light, binBrightImage_light,kernel);
   
    imshow ("总", binBrightImage_light);
    waitKey(1):
 
    vector<vector<Point>> lightContours;
    vector<LightDescriptor> lightInfos;
    findContours(binBrightImage_light.clone(), lightContours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    for (int i= 0; i<lightContours.size() ; i++)
    {
        drawContours (copy , lightContours , i,(255,0,255),4) ;
    }
    imshow( "copy" , copy) ;
    waitKey(1);
       
    for(const auto & contour: lightContours)
    {
        float lightContourArea = contourArea(contour);
        if (contour.size() < light_min_size || lightContourArea< light_min_area)
            continue;
        RotatedRect lightRec= minAreaRect(contour);
    
 if(lightRec.size.width / lightRec.size.height > light_width_height_ratio || lightContourAre lightRec.size.area() < light_contour_rect_ratio)
continue;
        lightRec.size.width *= light_extend_ratio;
        lightRec.size.height *= light_extend_ratio;
        circle(img1 , lightRec.center,2, Scalar (255,0,0),2);
        lightInfos.push_back(LightDescriptor(lightRec));//将ligtRec拷贝到lighInfos的底部
    }
if (!lightInfos.empty())
{
    float Len;
    for (size_t i =0; i< lightInfos.size();i++)
    {
    for (size_t j =i+l; (j< lightInfos.size()); j++)
    {
        cost LightDescriptor & leftLight = lightInfos[i];
        cost LightDescriptor & rightLight= lightInfos[j];
        
    float lenDiff_ratio = abs(leftLight.length - rightLight.length) / max(leftLight.length, rightLight.length);
    if (lenDiff_ratio > light_max_height_diff_ratio)
    {
        continue;
    }
        float dis = sqrt(powf((leftLight.center.x - rightLight.center.x).2) + powf((leftLight.center.y - rightLight.center.y),2));//灯条间距离
        Len =(leftLight. length + rightLight.length) / 2;
        float ratio= dis / Len; //距离与长度之比
        
    if (ratio > small_armor_max_aspect_ratio)
    {
        cout <<"BIG"<<endl;
    }
    else
    {
        cout <<"SMALL" <<endl;
    }
    }
    }
}
    imshow("Image",img1);
    waitKey(1);
}
}
