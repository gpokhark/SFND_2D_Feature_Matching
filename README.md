# SFND 2D Feature Tracking

<img src="images/keypoints.png" width="820" height="248" />

The idea of the camera course is to build a collision detection system - that's the overall goal for the Final Project. As a preparation for this, you will now build the feature tracking part and test various detector / descriptor combinations to see which ones perform best. This mid-term project consists of four parts:

* First, you will focus on loading images, setting up data structures and putting everything into a ring buffer to optimize memory load.
* Then, you will integrate several keypoint detectors such as HARRIS, FAST, BRISK and SIFT and compare them with regard to number of keypoints and speed.
* In the next part, you will then focus on descriptor extraction and matching using brute force and also the FLANN approach we discussed in the previous lesson.
* In the last part, once the code framework is complete, you will test the various algorithms in different combinations and compare them with regard to some performance measures.

See the classroom instruction and code comments for more details on each of these parts. Once you are finished with this project, the keypoint matching part will be set up and you can proceed to the next lesson, where the focus is on integrating Lidar points and on object detection using deep-learning.

## Dependencies for Running Locally
* cmake >= 2.8
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* OpenCV >= 4.1
  * This must be compiled from source using the `-D OPENCV_ENABLE_NONFREE=ON` cmake flag for testing the SIFT and SURF detectors.
  * The OpenCV 4.1.0 source code can be found [here](https://github.com/opencv/opencv/tree/4.1.0)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./2D_feature_tracking`.

Or else you can also use following commands -
1. `./clean.sh` to clean the project.
2. `./build.sh` to build the project.
3. `./run.sh` to run the project.

# Benchmark

I have created bash script `loopingCombination.sh` which loops through all the possible combination of detector and descriptor pairs.

Run it: `./loopingCombination.sh`.

This creates a `output/output.md` file.

I used  KNN match selection (k=2) and performed descriptor distance ratio filtering with t=0.8 in file `matching2D.cpp`.


|Sr. No. | Detector + Descriptor |Total Keypoints |Total Matches |Total Time (ms) |Ratio (matches/time) |
|:---:|:---:|:----:|:-----:|:-----:|:-----:|
|1 | SHITOMASI + BRISK |13423 |2255 |130.866 |17.2314 |
|2 | SHITOMASI + BRIEF |13423 |3234 |88.4513 |36.5625 |
|3 | SHITOMASI + ORB |13423 |2856 |104.508 |27.3281 |
|4 | SHITOMASI + FREAK |13423 |2299 |328.829 |6.99148 |
|5 | SHITOMASI + AKAZE |N/A |N/A |N/A |N/A|
|6 | SHITOMASI + SIFT |13423 |3213 |148.844 |21.5863 |
|7 | HARRIS + BRISK |728 |219 |87.8995 |2.49148 |
|8 | HARRIS + BRIEF |728 |257 |91.3042 |2.81477 |
|9 | HARRIS + ORB |728 |252 |84.9445 |2.96664 |
|10 | HARRIS + FREAK |728 |209 |303.417 |0.688821 |
|11 | HARRIS + AKAZE |N/A |N/A |N/A |N/A|
|12 | HARRIS + SIFT |728 |256 |138.352 |1.85036 |
|13 | FAST + BRISK |17874 |3170 |65.9564 |48.0621 |
|14 | FAST + BRIEF |17874 |4904 |28.2515 |173.584 |
|15 | FAST + ORB |17874 |4254 |20.8537 |203.993 |
|16 | FAST + FREAK |17874 |3164 |273.283 |11.5778 |
|17 | FAST + AKAZE |N/A |N/A |N/A |N/A|
|18 | FAST + SIFT |17874 |5734 |145.081 |39.5226 |
|19 | BRISK + BRISK |27116 |5073 |1927.9 |2.63136 |
|20 | BRISK + BRIEF |27116 |7474 |1868.13 |4.0008 |
|21 | BRISK + ORB |27116 |5095 |1880.68 |2.70913 |
|22 | BRISK + FREAK |27116 |5008 |2117.78 |2.36474 |
|23 | BRISK + AKAZE |N/A |N/A |N/A |N/A|
|24 | BRISK + SIFT |27116 |6934 |2151.19 |3.22333 |
|25 | ORB + BRISK |5000 |1378 |148.827 |9.2591 |
|26 | ORB + BRIEF |5000 |1403 |124.765 |11.2451 |
|27 | ORB + ORB |5000 |1466 |151.452 |9.67964 |
|28 | ORB + FREAK |5000 |627 |354.735 |1.76751 |
|29 | ORB + AKAZE |N/A |N/A |N/A |N/A|
|30 | ORB + SIFT |5000 |1578 |334.629 |4.71567 |
|31 | AKAZE + BRISK |13429 |3240 |396.324 |8.17513 |
|32 | AKAZE + BRIEF |13429 |4041 |373.533 |10.8183 |
|33 | AKAZE + ORB |13429 |3340 |379.433 |8.80261 |
|34 | AKAZE + FREAK |13429 |3228 |577.112 |5.59337 |
|35 | AKAZE + AKAZE |13429 |3463 |649.513 |5.33169 |
|36 | AKAZE + SIFT |13429 |3633 |477.364 |7.61054 |
|37 | SIFT + BRISK |13860 |2458 |433.876 |5.66522 |
|38 | SIFT + BRIEF |13860 |3243 |427.952 |7.57795 |
|39 | SIFT + ORB |OOM |OOM |OOM |OOM |
|40 | SIFT + FREAK |13860 |2428 |667.155 |3.63933 |
|41 | SIFT + AKAZE |N/A |N/A |N/A |N/A|
|42 | SIFT + SIFT |13860 |2557 |749.038 |3.41371 |

## Top 3 detector/ descriptor pairs

As you can see in the table above the efficiency ratio i.e. matches/time is highest for following three detector descriptor pair.

|Sr. No. | Detector + Descriptor |Total Keypoints |Total Matches |Total Time (ms) |Ratio (matches/time) |
|:---:|:---:|:----:|:-----:|:-----:|:-----:|
|1 | FAST + ORB |17874 |4254 |20.8537 |203.993 |
|2 | FAST + BRIEF |17874 |4904 |28.2515 |173.584 |
|3 | FAST + BRISK |17874 |3170 |65.9564 |48.0621 |
