#include "pointCloudMapping.hpp"
#include "neighborhood.hpp"
#include <string.h>
#include <boost/thread/thread.hpp>
#include "matcher.hpp"
#include "registration.hpp"
#include "toolfile.hpp"

using namespace cv;
using namespace std;
using namespace Eigen;


int main(int argc, const char * argv[])
{
    if(argc!=7)
    {
        cout << "usage: ./SLAM_Project address frameCount" << endl;
        return 1;
    }

    int startPoint1 = atoi(argv[2]);
    int fileNumber1 = atoi(argv[3]);
    
    int startPoint2 = atoi(argv[5]);
    int fileNumber2 = atoi(argv[6]);

    string dir1 = "/data/airsim_xview/" + string(argv[1]) + "/";  // fullpath2;
    string dir2 = "/data/airsim_xview/" + string(argv[4]) + "/"; // fullpath1;
    
    //generate the camera parameter
    vector<float> camera(4);
    float scale = 1;
    camera[0] = 512; //fx
    camera[1] = 512; //fy
    camera[2] = 512; //cx
    camera[3] = 288;//cy

    //iniltialize the R and T
    MatrixXf R, T;
    R.setIdentity();
    T.setZero();

    //create the value for visual odometry
    Mat frame_pose = cv::Mat::eye(4, 4, CV_64F);
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud2(new pcl::PointCloud<pcl::PointXYZRGB>);
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud1(new pcl::PointCloud<pcl::PointXYZRGB>);

    int num_poses = 5107;
    Mat pose1 = cv::Mat::zeros(num_poses, 7, CV_64FC1);
    Mat pose2 = cv::Mat::zeros(num_poses, 7, CV_64FC1);

    //generate the correspondent rgb value of all labels in the segmentation image
    Mat label_bgr = cv::imread("/data/airsim_xview/segmentation_pallet.png");
    Mat Label; 
    Mat label_gray_img;
    cvtColor(label_bgr, Label, COLOR_BGR2RGB);
    cvtColor(label_bgr, label_gray_img, COLOR_BGR2GRAY);
    std::vector<uchar> label_gray(255);
    if (label_gray_img.isContinuous()) {
        label_gray.assign(label_gray_img.data, label_gray_img.data + label_gray_img.total()*label_gray_img.channels());
    }    

    // Mat Label = (cv::Mat_<int>(11, 3)<< 105,   6,   9,
    //                                 82,   5,   9,
    //                                 92,  32,  10,
    //                                 131,  34,  10,
    //                                 95,  16,  22,
    //                                 34,  40,  15,
    //                                 90,  67,  17,
    //                                 78,  36,  77,
    //                                 87,  35,  55,
    //                                 139,  32, 110,
    //                                 115,  33,  98);

    // vector<uchar> label_gray(11);
    // label_gray[0] = 36; label_gray[1] = 28; label_gray[2] = 47; label_gray[3] = 60; label_gray[4] = 40;
    // label_gray[5] = 35; label_gray[6] = 68; label_gray[7] = 53; label_gray[8] = 53; label_gray[9] = 73;
    // label_gray[10] = 65;



    //insert the odeometry value
    insertPose(dir1, pose1, num_poses);
    insertPose(dir2, pose2, num_poses);

    std::cout << "Poses are read with sizes: " << pose1.size() << "\t" << pose2.size() << std::endl;

    //initiallize the intial value of the keypoint;
    vector<vector<float> > centerpoint2;
    vector<vector<float> > centerpoint1;
    
    gatherPointCloudData(cloud1, centerpoint1, pose1, Label, label_gray, camera, scale, dir1, fileNumber1, startPoint1);
    gatherPointCloudData(cloud2, centerpoint2, pose2, Label, label_gray, camera, scale, dir2, fileNumber2, startPoint2);
    
    //add the edge between the neighborhood
    Neighborhood Nei1(centerpoint1);

    //MatrixXf neighbor;
    Neighborhood Nei2(centerpoint2);
    //neighbor = Nei2.getNeighbor();
    //cout<<neighbor<<endl;

    //obtain the descriptor
    MatrixXf descriptor1;
    Descriptor Des1(Nei1, 1);
    //descriptor1 = Des1.getDescriptor(4);
    //cout<<descriptor1<<endl;

    MatrixXf descriptor2;
    Descriptor Des2(Nei2, 1);
    // descriptor2 = Des2.getDescriptor(23);
    // cout<<descriptor2<<endl;
    
    //matching
    MatrixXi matcherID;
    matcher matches(Des1, Des2, 2);
    cout<<"Before: matches.getGoodMatcher();"<<endl;
    matcherID = matches.getGoodMatcher();
    cout<<"after: matches.getGoodMatcher();"<<endl;

    //begain registration
    //insert the value
    registration registration(centerpoint1, centerpoint2, matcherID);

    //reject the outliers with ICP-RANSAC method
    registration.matcherRANSAC(10);
    MatrixXi inlierID;
    inlierID = registration.inlierID;

    //final pose estiamation
    registration.Alignment();
    
    R = registration.Rotation;
    T = registration.Translation;
    
    float finalDistance = sqrt(pow(T(0, 0), 2) + pow(T(1, 0),2) + pow(T(2, 0),2));
    cout<<"final distance: "<<finalDistance<<endl;


    // //plot the semantic point and matching with PCL library  
    pointCloudMapping pointCloudMapping;
    pointCloudMapping.pointVisuallize(cloud1, cloud2, inlierID, R, T);
    
}
