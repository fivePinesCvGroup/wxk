#include <iostream>
#include <sstream>
#include <time.h>
#include <stdio.h>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <string>
#include <algorithm>

using namespace cv;
using namespace std;


int main()
{
    ifstream fin("calibdata.txt");             //标定所用图像文件的路径
    ofstream fout("caliberation_result.txt");// 保存标定结果的文件
    //fout.open("/Users/alexking/Desktop/pnp/cml/build/caliberation_result.txt");
    
    // 读取每一幅图像，从中提取出角点，然后对角点进行亚像素精确化
    int image_count = 0;  //图像数量
    Size image_size;      // 图像的尺寸
    Size board_size = Size(9, 6);             //标定板上每行、列的角点数
    vector<Point2f> image_points_buf;         //缓存每幅图像上检测到的角点
    vector<vector<Point2f>> image_points_seq; // 保存检测到的所有角点
    string filename;      // 图片名
    vector<string> filenames;
        while (getline(fin, filename))
        {
            ++image_count; //计数图片数量
            Mat imageInput = imread(filename);
            filenames.push_back(filename);
            
            // 读入第一张图片时获取图片大小
            if (image_count == 1)
            {
                image_size.width = imageInput.cols;
                image_size.height = imageInput.rows;
            }
            
            //提取角点
            if (0 == findChessboardCorners(imageInput, board_size, image_points_buf))//找角点
            {
                cout << "can not find chessboard corners!\n";  // 找不到角点
                exit(1);
            }
            else
            {
                Mat view_gray;
                cvtColor(imageInput, view_gray, COLOR_RGB2GRAY);  // 转灰度图
                cornerSubPix(view_gray, image_points_buf, Size(5, 5), Size(-1, -1), TermCriteria(cv::TermCriteria::Type::EPS+ cv::TermCriteria::Type::MAX_ITER, 30, 0.1));//亚像素级角点检测
                image_points_seq.push_back(image_points_buf);  // 保存亚像素角点
                drawChessboardCorners(view_gray, board_size, image_points_buf, false); // 用于在图片中标记角点
                imshow("Camera Calibration", view_gray);       // 显示图片
                waitKey(1000); //暂停1S
            }
        

            
            int CornerNum = board_size.width * board_size.height;  // 每张图片上总的角点数
            
            
            
            Size square_size = Size(10, 10);         // 实际测量得到的标定板上每个棋盘格的大小
            vector<vector<Point3f>> object_points;   //保存标定板上角点的三维坐标
            
            Mat cameraMatrix = Mat(3, 3, CV_32FC1, Scalar::all(0));  //摄像机内参数矩阵
            vector<int> point_counts;   // 每幅图像中角点的数量
            Mat distCoeffs = Mat(1, 5, CV_32FC1, Scalar::all(0));       //摄像机的5个畸变系数：k1,k2,p1,p2,k3
            vector<Mat> tvecsMat;      // 每幅图像的旋转向量
            vector<Mat> rvecsMat;      // 每幅图像的平移向量
            //初始化标定板上角点的三维坐标
            int i, j, t;
            for (t = 0; t < image_count; t++)
            {
                vector<Point3f> tempPointSet;
                for (i = 0; i < board_size.height; i++)
                {
                    for (j = 0; j < board_size.width; j++)
                    {
                        Point3f realPoint;
                        realPoint.x = i * square_size.width;
                        realPoint.y = j * square_size.height;
                        realPoint.z = 0;//标定板放在世界坐标系中z=0的平面上
                        tempPointSet.push_back(realPoint);
                    }
                }
                object_points.push_back(tempPointSet);    // object_points 世界坐标系中的角点的三维坐标
                
            }
            
            
            // 初始化每幅图像中的角点数量，假定每幅图像中都可以看到完整的标定板
            for (i = 0; i < image_count; i++)
            {
                point_counts.push_back(board_size.width * board_size.height);
            }
            calibrateCamera(object_points, image_points_seq, image_size, cameraMatrix, distCoeffs, rvecsMat, tvecsMat, 0);//相机标定函数
            
            double total_err = 0.0;         //所有图像的平均误差的总和
            double err = 0.0;               //每幅图像的平均误差
            vector<Point2f> image_points2;  // 保存重新计算得到的投影点
            fout << "每幅图像的标定误差：\n";
            
            for (i = 0; i < image_count; i++)
            {
                vector<Point3f> tempPointSet = object_points[i];
                projectPoints(tempPointSet, rvecsMat[i], tvecsMat[i], cameraMatrix, distCoeffs, image_points2);//三维点转二维
                
                // 计算新的投影点和旧的投影点之间的误差
                vector<Point2f> tempImagePoint = image_points_seq[i];
                Mat tempImagePointMat = Mat(1, tempImagePoint.size(), CV_32FC2);
                Mat image_points2Mat = Mat(1, image_points2.size(), CV_32FC2);
                
                for (int j = 0; j < tempImagePoint.size(); j++)
                {
                    image_points2Mat.at<Vec2f>(0, j) = Vec2f(image_points2[j].x, image_points2[j].y);
                    tempImagePointMat.at<Vec2f>(0, j) = Vec2f(tempImagePoint[j].x, tempImagePoint[j].y);
                }
                err = norm(image_points2Mat, tempImagePointMat, NORM_L2);//获取复数的范数，NORM_L2为L2范数
                total_err += err /= point_counts[i];
                fout << "第" << i + 1 << "幅图像的平均误差：" << err << "像素" << endl;
            }
            fout << "总体平均误差：" << total_err / image_count << "像素" << endl << endl;
            
            
            Mat rotation_matrix = Mat(3, 3, CV_32FC1, Scalar::all(0));  // 保存每幅图像的旋转矩阵
            fout << "相机内参数矩阵：" << endl;
            fout << cameraMatrix << endl << endl;
            fout << "畸变系数：\n";
            fout << distCoeffs << endl << endl << endl;
            for (int i = 0; i < image_count; i++)
            {
                fout << "第" << i + 1 << "幅图像的旋转向量：" << endl;
                fout << tvecsMat[i] << endl;
                
                // 将旋转向量转换为相对应的旋转矩阵
                Rodrigues(tvecsMat[i], rotation_matrix);//罗德里格斯变换矩阵
                fout << "第" << i + 1 << "幅图像的旋转矩阵：" << endl;
                fout << rotation_matrix << endl;
                fout << "第" << i + 1 << "幅图像的平移向量：" << endl;
                fout << rvecsMat[i] << endl << endl;
            }
            fout << endl;
            
            
            Mat mapx = Mat(image_size, CV_32FC1);
            Mat mapy = Mat(image_size, CV_32FC1);
            Mat R = Mat::eye(3, 3, CV_32F);
            string imageFileName;
            std::stringstream StrStm;
            for (int i = 0; i != image_count; i++)
            {
                initUndistortRectifyMap(cameraMatrix, distCoeffs, R, cameraMatrix, image_size, CV_32FC1, mapx, mapy);//计算无畸变和修正转换关系
                Mat imageSource = imread(filenames[i]);
                Mat newimage = imageSource.clone();
                remap(imageSource, newimage, mapx, mapy, INTER_LINEAR);//一幅图像中某位置的像素放置到另一个图片指定位置。
                StrStm.clear();
                imageFileName.clear();
                StrStm << i + 1;
                StrStm >> imageFileName;
                imageFileName += "_d.jpg";
                imwrite(imageFileName, newimage);
            }
            //fin.close();
            //fout.close();
        }
fin.close();
fout.close();
}

    

