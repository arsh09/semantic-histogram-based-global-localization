#include "pointCloudMapping.hpp"
#include "neighborhood.hpp"
#include "toolfile.hpp"
#include <string.h>
#include <boost/thread/thread.hpp>
#include "matcher.hpp"
#include "registration.hpp"


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

    string dir1 = "/data/airsim_2/" + string(argv[1]) + "/";  // fullpath2;
    string dir2 = "/data/airsim_2/" + string(argv[4]) + "/"; // fullpath1;


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

    Mat pose1 = cv::Mat::zeros(1000, 7, CV_64FC1);
    Mat pose2 = cv::Mat::zeros(1000, 7, CV_64FC1);

    //generate the correspondent rgb value of all labels in the segmentation image
    Mat Label = (cv::Mat_<int>(11, 3)<< 175, 6, 140,
                                    65, 54, 217,
                                    156, 198, 23,
                                    184, 145, 182,
                                    211, 80, 208,
                                    232, 250, 80,
                                    234, 20, 250,
                                    99, 242, 104,
                                    142, 1, 246,
                                    81, 13, 36,
                                    112, 105, 191);

    //Obtain the label number
    vector<uchar> label_gray(11);
    label_gray[0] = 72; label_gray[1] = 76; label_gray[2] = 165; label_gray[3] = 161; label_gray[4] = 134;
    label_gray[5] = 225; label_gray[6] = 110; label_gray[7] = 184; label_gray[8] = 71; label_gray[9] = 36;
    label_gray[10] = 117;


    //insert the odeometry value
    insertPose(dir1,pose1, 1000);
    insertPose(dir2,pose2, 1000);
    
    std::cout << "RGB and gray labels are read with sizes: " << Label.size() << "\t" << label_gray.size() << std::endl;
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


    std::cout << "Number of nodes in map graph: " << centerpoint1.size() << std::endl;
    std::cout << "Number of edges in map graph: " << Nei1.getNeighborCount() << std::endl;

    std::cout << "Number of nodes in query graph: " << centerpoint2.size() << std::endl;
    std::cout << "Number of edges in query graph: " << Nei2.getNeighborCount() << std::endl;


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
    // cout<<"Before: matches.getGoodMatcher();"<<endl;
    matcherID = matches.getGoodMatcher();
    cout<<"after: matches.getGoodMatcher() " << matcherID.size() <<endl;
    std::cout << "Number of good matches between map and query graphs: " << matcherID.size() << std::endl;


    //begain registration
    //insert the value
    // cout<<"Before: registration;"<<endl;
    registration registration(centerpoint1, centerpoint2, matcherID);
    // cout<<"After: registration;"<<endl;

    //reject the outliers with ICP-RANSAC method
    registration.matcherRANSAC(10);
    MatrixXi inlierID;
    inlierID = registration.inlierID;

    //final pose estiamation
    registration.Alignment();

    
    R = registration.Rotation;
    T = registration.Translation;
    
    // float finalDistance = sqrt(pow(T(0, 0), 2) + pow(T(1, 0),2) + pow(T(2, 0),2));
    
    // cout<<"final distance: "<<finalDistance<<endl;


    // //plot the semantic point and matching with PCL library  
    pointCloudMapping pointCloudMapping;
    pointCloudMapping.pointVisuallize(cloud1, cloud2, inlierID, R, T);
    
}
