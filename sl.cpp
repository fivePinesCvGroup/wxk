#include <opencv2/opencv.hpp>
#include <math.h>
using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    Mat image = imread("/Users/alexking/Desktop/pnp/cml/02.jpg");

    // 2D 特征点像素坐标
    vector<Point2d> image_points;
    image_points.push_back(Point2d(252, 78));
    image_points.push_back(Point2d(540, 133));
    image_points.push_back(Point2d(438, 397));
    image_points.push_back(Point2d(257, 357));

    // 画出四个特征点
    for (int i = 0; i < image_points.size(); i++)
    {
        circle(image, image_points[i], 5, Scalar(0, 0, 255), -1);
    }

    // 3D 特征点世界坐标，与像素坐标对应
    std::vector<Point3d> model_points;
    model_points.push_back(Point3d(-42.5f, -42.5f, 0));
    model_points.push_back(Point3d(+42.5f, -42.5f, 0));
    model_points.push_back(Point3d(+42.5f, +42.5f, 0));
    model_points.push_back(Point3d(-42.5f, +42.5f, 0));
    //　注意世界坐标和像素坐标要一一对应

    // 相机内参矩阵和畸变系数均由相机标定结果得出
    // 相机内参矩阵
    Mat camera_matrix = (Mat_<double>(3, 3) << 532.8566441008406, 0, 341.7910385406415,
    0, 533.0264670278142, 235.3510271642852,
    0, 0, 1);
    // 相机畸变系数
    Mat dist_coeffs = (Mat_<double>(5, 1) << -0.2885088601329109, 0.08898092305030673, 0.001385907982417363, -0.000386361533032381, 0.0366739781617418);

    cout << "Camera Matrix " << endl << camera_matrix << endl << endl;
    // 旋转向量
    Mat rotation_vector;
    // 平移向量
    Mat translation_vector;
    solvePnP(model_points, image_points, camera_matrix, dist_coeffs, rotation_vector, translation_vector, false, 0);//pnp距离解算
    cout << "Rotation Vector " << endl << rotation_vector << endl << endl;
    cout << "Translation Vector" << endl << translation_vector << endl << endl;
    Mat Rvec;
    Mat_<float> Tvec;
    rotation_vector.convertTo(Rvec, CV_32F);  // 旋转向量转换格式
    translation_vector.convertTo(Tvec, CV_32F); // 平移向量转换格式
    Mat_<float> rotMat(3, 3);
    Rodrigues(Rvec, rotMat);
    // 旋转向量转成旋转矩阵
    cout << "rotMat" << endl << rotMat << endl << endl;
    Mat P_oc;
    P_oc = -rotMat.inv() * Tvec;
    // 求解相机的世界坐标，得出p_oc的第三个元素即相机到物体的距离即深度信息，单位是mm
    cout << "P_oc" << endl << P_oc << endl;
    imshow("Output", image);
    waitKey(0);
}
