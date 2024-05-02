# Semantic Histogram Based Graph Matching for Real-Time Multi-Robot Global Localization in Large Scale Environment

### GAIA Changes: 

```bash 
# build instructions are same as below. Except, you may need to copy the 
# libpango*.so files as cmake has hard time finding it. 

$ sudo apt-get install libpango-1.0.0-dev libopencv-dev libeignen3-dev libpcl-dev 
$ git clone https://https://github.com/gxytcrc/Semantic-Graph-based--global-Localization.git
$ mkdir build
$ cd build 
$ cmake ..
# if the build fails due to pango lib, copy the pango*.so files in build folder. 
# as cmake is having a hard time finding it.
$ make -j8
```

I have added another example to make this work with GAIA project. Please see example 'main_airsim_xview.cpp'. 

You need the dataset in the following format: 

1) Two different day or view datasets, let say folder1 and folder2. 
2) Folder one looks like: 

```
dataset\
    forward/
        segmentation_pallet.png
        depth/  
            image1.png
            image2.png
            .
            .
            .

        segmentation/
            image1.png
            imgae2.png
            .
            .
            .
        
    backward/ 
        segmentation_pallet.png
        depth/  
            image1.png
            image2.png
            .
            .
            .

        segmentation/
            image1.png
            imgae2.png
            .
            .
            .


```


Where segmentation_pallet.png is the segment-color-to-index image (1-column image). 

### Run: 

```bash
$ ./example_2 /path/to/dataset/ start-frame-index end-frame-index forward start-frame-index end-frame-index backward /path/to/dataset/segmentation_pallet.png 

# note that segmentation_pallet.png will be same for forward and backward, but for brevity, I copy it in both the folder. 
```



### Related Publications:

Xiyue Guo, Junjie Hu, Junfeng Chen, Fuqin Deng, Tin Lun Lam, **Semantic Histogram Based Graph Matching for Real-Time Multi-Robot Global Localization in Large Scale Environment**, IEEE Robotics and Automation Letters, 2021. **[PDF](https://arxiv.org/pdf/2010.09297.pdf)**. 

<a href="https://www.youtube.com/watch?v=xB8WHj8K9cE" target="_blank"><img src="https://github.com/gxytcrc/Semantic-Graph-based--global-Localization/blob/main/example/fengmian.png" 
alt="ORB-SLAM3" width="240" height="150" border="10" /></a>

Results
-
![](https://github.com/gxytcrc/Semantic-Graph-based--global-Localization/blob/main/example/result1.png)
![](https://github.com/gxytcrc/Semantic-Graph-based--global-Localization/blob/main/example/result2.png)

# 1. Prerequisites #
* Ubuntu
* CMake
* Eigen
* Pangolin
* OpenCV
* PCL (version below 1.10)

# 2. Running #
Clone the repository and catkin_make:
```
    git clone https://https://github.com/gxytcrc/Semantic-Graph-based--global-Localization.git
    mkdir build
    cd build
    cmake ..
    make -j8
```
Download the dataset that is created from Airsim, and save them into the Datset . Download link: https://drive.google.com/file/d/106sPA48vFThLK0RB4WBcj-i8FZPQPmcV/view?usp=sharing.

Launch it as follows:
```
./mapAlignment robot1-foldername startFrameNumber endFrameNumber robot2-foldername startFrameNumber endFrameNumber
```
### Citation
```
@inproceedings{XiyueGuo2021,
title={Semantic Histogram Based Graph Matching for Real-Time Multi-Robot Global Localization in Large Scale Environment},
author={Xiyue Guo, Junjie Hu, Junfeng Chen, Fuqin Deng, Tin Lun Lam},
booktitle=IEEE Robotics and Automation Letters (RA-L)},
year={2021}
}
```

