#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/video/tracking.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int hmin=0, smin=171, vmin=146;
int hmax=85,smax=255, vmax=255;

Mat imgHSV , mask ,imgDil;

int cx,cy;

/*namespace Kalman_example
{
class KalmanFilter
{
public:
    KalmanFilter(int x, int y):
        KF_(4, 2)
        /*
        KalmanFilter( int dynamParams, int measureParams, int controlParams = 0, int type = CV_32F )
        "dynamParams = 4": 4*1 vector of state (x, y, delta x, delta y)
        "measureParams = 2": 2*1 vector of measurement (x, y)
        
        {
            measurement_ = Mat::zeros(2, 1, CV_32F);// (x, y)
            KF_.transitionMatrix = (Mat_<float>(4, 4) << 1, 0, 1, 0,                                                                    0, 1, 0, 1,
                                                         0, 0, 1, 0,
                                                         0, 0, 0, 1);
            setIdentity(KF_.measurementMatrix, Scalar::all(1));
            setIdentity(KF_.processNoiseCov, Scalar::all(1e-10));//**10: Larger, slower regression
            setIdentity(KF_.measurementNoiseCov, Scalar::all(1e-1));//1: Larger, quicker regression
            setIdentity(KF_.errorCovPost, Scalar::all(1));
 
            KF_.statePost = (Mat_<float>(4, 1) << x, y, 0, 0);//Ensure beginner is default value
        }
 
    Point2f run(float x, float y)
    {
        Mat prediction = KF_.predict();
        Point2f predict_pt = Point2f(prediction.at<float>(0),prediction.at<float>(1));
 
        measurement_.at<float>(0, 0) = x;
        measurement_.at<float>(1, 0) = y;
 
        KF_.correct(measurement_);
 
        return predict_pt;
    }
private:
    Mat measurement_;
    cv::KalmanFilter KF_;//Differ from Kalman_example::KalmanFilter
};
 
}*/

const int winHeight=600;
const int winWidth=800;
 


int main()
{
    string path = "/Users/alexking/Desktop/111/orange.mp4";
    VideoCapture cap(path);
    
    Mat img;
    KalmanFilter KF(4, 2, 0);
    KF.transitionMatrix = (Mat_<float>(4, 4) <<1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
        setIdentity(KF.measurementMatrix);                                             //测量矩阵H
        setIdentity(KF.processNoiseCov, Scalar::all(1e-5));                            //系统噪声方差矩阵Q
        setIdentity(KF.measurementNoiseCov, Scalar::all(1e-5));                        //测量噪声方差矩阵R
        setIdentity(KF.errorCovPost, Scalar::all(1));//估计误差协方差
        Mat measurement = Mat::zeros(2, 1, CV_32F);
    float anti_range = 0.1;
    
   while (true)
   {
       cap.read(img);
       
       Mat kernel = getStructuringElement(MORPH_RECT,Size(20,20));
       
       
       cvtColor(img , imgHSV , COLOR_BGR2HSV);
       
       Scalar lower(hmin,smin,vmin);
       Scalar upper(hmax,smax,vmax);
       inRange(imgHSV,lower,upper,mask);
       
       
       
       dilate(mask,imgDil,kernel);
       
       vector<vector<Point>> contours;
       vector<Vec4i> hierarchy;
       
       findContours(imgDil, contours , hierarchy ,RETR_TREE,CHAIN_APPROX_SIMPLE);
       vector<Rect> boundRect(contours.size());
       vector<RotatedRect> box (contours.size());
       
       for( int i =0 ; i <contours.size() ; i++)
       {
           
           Rect rect = cv::boundingRect(contours[i]);
           Point2f p[4];
           // rect.points(p);
           
           
           Point pw;
           Point2f anti_kalmanPoint;
           
           //  rectangle(img,p[0],p[2],Scalar(0,255,0),5);
           // pt.x = ((p[0].x+p[2].x)/2);
           //   pt.y = ((p[0].y+p[2].y)/2);
           
           rectangle(img,rect.tl(),rect.br(),Scalar(0,255,0),5);
           pw.x = (rect.x+rect.width/2);
           pw.y = (rect.y+rect.height/2);
           
           
           
               
               //初始状态值
              // KF.statePost = (Mat_<float>(2, 1) <<pw.x,pw.y);
           //    cout<<"state0="<<KF.statePost<<endl;
            
               //预测
               Mat prediction = KF.predict();
               //计算测量值
               
               Point predict_pt = Point(prediction.at<float>(0),prediction.at<float>(1) );
               measurement.at<float>(0) = (float)pw.x;
               measurement.at<float>(1) = (float)pw.y;
               //更新
               KF.correct(measurement);
           
                       if(abs(pw.x - predict_pt.x) > 3)//When points are closed, no Anti-kalman to reduce shaking
                           anti_kalmanPoint.x = pw.x + anti_range*(pw.x - predict_pt.x);
                       else
                           anti_kalmanPoint.x = pw.x;
           
           if(abs(pw.y - predict_pt.y) > 3)//When points are closed, no Anti-kalman to reduce shaking
               anti_kalmanPoint.y = pw.y + anti_range*(pw.y - predict_pt.y);
           else
               anti_kalmanPoint.y = pw.y;

                   

               
               
           
           circle(img,predict_pt,5,Scalar(0,255,0),3);    //predicted point with green
           circle(img,pw,5,Scalar(255,0,0),3); //current position with red
           circle(img,anti_kalmanPoint,5,Scalar(0,0,255),3);
       }
      
           
           
                           
       
       
       
       
       
       imshow("Img",img);
       imshow("Img mask",mask);
       waitKey(300);
       
       
   }
}



