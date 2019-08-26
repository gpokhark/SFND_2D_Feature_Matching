/* INCLUDES FOR THIS PROJECT */
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <limits>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

#include "dataStructures.h"
#include "matching2D.hpp"

// using namespace std;

/* MAIN PROGRAM */
int main(int argc, const char *argv[])
{

    /* INIT VARIABLES AND DATA STRUCTURES */

    // data location
    std::string dataPath = "../";

    // camera
    std::string imgBasePath = dataPath + "images/";
    std::string imgPrefix = "KITTI/2011_09_26/image_00/data/000000"; // left camera, color
    std::string imgFileType = ".png";
    int imgStartIndex = 0; // first file index to load (assumes Lidar and camera names have identical naming convention)
    int imgEndIndex = 9;   // last file index to load
    int imgFillWidth = 4;  // no. of digits which make up the file index (e.g. img-0001.png)

    // misc
    int dataBufferSize = 2;            // no. of images which are held in memory (ring buffer) at the same time
    std::vector<DataFrame> dataBuffer; // list of data frames which are held in memory at the same time
    bool bVis = false;                 // visualize results

    double timeTotal = 0;
    int keypointTotal = 0;
    int matchesTotal = 0;
    // SHITOMASI, HARRIS, FAST, BRISK, ORB, AKAZE, SIFT
    std::string detectorType = "SHITOMASI";

    // BRISK, BRIEF, ORB, FREAK, AKAZE, SIFT
    std::string descriptorType = "BRISK";

    // DES_BINARY, DES_HOG
    std::string descriptorType_HOG_BIN = "DES_BINARY";

    /* MAIN LOOP OVER ALL IMAGES */

    for (size_t imgIndex = 0; imgIndex <= imgEndIndex - imgStartIndex; imgIndex++)
    {
        /* LOAD IMAGE INTO BUFFER */

        // assemble filenames for current index
        std::ostringstream imgNumber;
        imgNumber << std::setfill('0') << std::setw(imgFillWidth) << imgStartIndex + imgIndex;
        std::string imgFullFilename = imgBasePath + imgPrefix + imgNumber.str() + imgFileType;

        // load image from file and convert to grayscale
        cv::Mat img, imgGray;
        img = cv::imread(imgFullFilename);
        cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);

        //// STUDENT ASSIGNMENT
        //// TASK MP.1 -> replace the following code with ring buffer of size dataBufferSize

        // push image into data frame buffer
        DataFrame frame;
        frame.cameraImg = imgGray;
        dataBuffer.push_back(frame);
        if (dataBuffer.size() > dataBufferSize)
        {
            dataBuffer.erase(dataBuffer.begin());
        }

        //// EOF STUDENT ASSIGNMENT
        // std::cout << "#1 : LOAD IMAGE INTO BUFFER done" << std::endl;

        /* DETECT IMAGE KEYPOINTS */

        // extract 2D keypoints from current image
        std::vector<cv::KeyPoint> keypoints; // create empty feature list for current image

        //// STUDENT ASSIGNMENT
        //// TASK MP.2 -> add the following keypoint detectors in file matching2D.cpp and enable string-based selection based on detectorType
        //// -> HARRIS, FAST, BRISK, ORB, AKAZE, SIFT
        double timeDetect = 0;

        if (detectorType.compare("SHITOMASI") == 0)
        {
            timeDetect = detKeypointsShiTomasi(keypoints, imgGray, false);
        }

        else if (detectorType.compare("HARRIS") == 0)
        {
            timeDetect = detKeypointsHarris(keypoints, imgGray, false);
        }
        // FAST, BRISK, ORB, AKAZE, SIFT
        else
        {
            timeDetect = detKeypointsModern(keypoints, imgGray, detectorType, false);
        }
        keypointTotal += keypoints.size();
        //// EOF STUDENT ASSIGNMENT

        //// STUDENT ASSIGNMENT
        //// TASK MP.3 -> only keep keypoints on the preceding vehicle

        // only keep keypoints on the preceding vehicle
        bool bFocusOnVehicle = true;
        cv::Rect vehicleRect(535, 180, 180, 150);
        if (bFocusOnVehicle)
        {
            for (auto it = keypoints.begin(); it < keypoints.end(); it++)
            {
                // cv::KeyPoint newKeyPoint;
                // newKeyPoint.pt = cv::Point2f((*it).pt.x, (*it).pt.y);
                // int pointx = newKeyPoint.pt.x;
                // int pointy = newKeyPoint.pt.y;
                // int leftx = vehicleRect.x;
                // int rightx = vehicleRect.x + vehicleRect.width;
                // int topy = vehicleRect.y + vehicleRect.height;
                // int bottomy = vehicleRect.y;
                // if (leftx > pointx || pointx > rightx || pointy > topy || pointy < bottomy)
                // {
                //     keypoints.erase(it);
                // }

                // remove the kep-point if it is not in the RoI
                if (!vehicleRect.contains(it->pt))
                {
                    keypoints.erase(it);
                }
            }
            // std::cout << "Focus on vehicle keypoints = " << keypoints.size() << "\n";
        }

        //// EOF STUDENT ASSIGNMENT

        // optional : limit number of keypoints (helpful for debugging and learning)
        bool bLimitKpts = false;
        if (bLimitKpts)
        {
            int maxKeypoints = 50;

            if (detectorType.compare("SHITOMASI") == 0)
            { // there is no response info, so keep the first 50 as they are sorted in descending quality order
                keypoints.erase(keypoints.begin() + maxKeypoints, keypoints.end());
            }
            cv::KeyPointsFilter::retainBest(keypoints, maxKeypoints);
            std::cout << " NOTE: Keypoints have been limited!" << std::endl;
        }

        // push keypoints and descriptor for current frame to end of data buffer
        (dataBuffer.end() - 1)->keypoints = keypoints;
        // std::cout << "#2 : DETECT KEYPOINTS done" << std::endl;

        /* EXTRACT KEYPOINT DESCRIPTORS */

        //// STUDENT ASSIGNMENT
        //// TASK MP.4 -> add the following descriptors in file matching2D.cpp and enable string-based selection based on descriptorType
        //// -> BRIEF, ORB, FREAK, AKAZE, SIFT

        cv::Mat descriptors;
        double timeDes = 0;

        timeDes = descKeypoints((dataBuffer.end() - 1)->keypoints, (dataBuffer.end() - 1)->cameraImg, descriptors, descriptorType);
        //// EOF STUDENT ASSIGNMENT

        // push descriptors for current frame to end of data buffer
        (dataBuffer.end() - 1)->descriptors = descriptors;

        // std::cout << "#3 : EXTRACT DESCRIPTORS done" << std::endl;

        if (dataBuffer.size() > 1) // wait until at least two images have been processed
        {

            /* MATCH KEYPOINT DESCRIPTORS */

            std::vector<cv::DMatch> matches;
            std::string matcherType = "MAT_BF"; // MAT_BF, MAT_FLANN
            // std::string descriptorType_HOG_BIN = "DES_BINARY"; // DES_BINARY, DES_HOG
            std::string selectorType = "SEL_KNN"; // SEL_NN, SEL_KNN

            //// STUDENT ASSIGNMENT
            //// TASK MP.5 -> add FLANN matching in file matching2D.cpp
            //// TASK MP.6 -> add KNN match selection and perform descriptor distance ratio filtering with t=0.8 in file matching2D.cpp

            matchDescriptors((dataBuffer.end() - 2)->keypoints, (dataBuffer.end() - 1)->keypoints,
                             (dataBuffer.end() - 2)->descriptors, (dataBuffer.end() - 1)->descriptors,
                             matches, descriptorType_HOG_BIN, matcherType, selectorType);

            matchesTotal += matches.size();
            //// EOF STUDENT ASSIGNMENT

            // store matches in current data frame
            (dataBuffer.end() - 1)->kptMatches = matches;

            // std::cout << "#4 : MATCH KEYPOINT DESCRIPTORS done" << std::endl;

            // visualize matches between current and previous image
            bVis = false;
            if (bVis)
            {
                cv::Mat matchImg = ((dataBuffer.end() - 1)->cameraImg).clone();
                cv::drawMatches((dataBuffer.end() - 2)->cameraImg, (dataBuffer.end() - 2)->keypoints,
                                (dataBuffer.end() - 1)->cameraImg, (dataBuffer.end() - 1)->keypoints,
                                matches, matchImg,
                                cv::Scalar::all(-1), cv::Scalar::all(-1),
                                std::vector<char>(), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

                std::string windowName = "Matching keypoints between two camera images";
                cv::namedWindow(windowName, 7);
                cv::imshow(windowName, matchImg);
                std::cout << "Press key to continue to next image" << std::endl;
                // cv::waitKey(0); // wait for key to be pressed
            }
            bVis = false;
        }
        // std::cout << "=======================================\n";
        timeTotal += timeDetect + timeDes;

    } // eof loop over all images
    /*
    std::cout << "\n**********SUMMARY**********\n";
    std::cout << detectorType << " + " << descriptorType << " + " << descriptorType_HOG_BIN << "\n";
    std::cout << "Total number of Key-points = " << keypointTotal << "\n";
    std::cout << "Total number of Matches = " << matchesTotal << "\n";
    std::cout << "Total time = " << timeTotal * 1000.0 << " ms \n";
    std::cout << "Ratio = " << matchesTotal / (timeTotal * 1000.0) << " matches/ms \n";
    */

    // Generate output table for README.md 
    std::cout << "| Detector + Descriptor |" << "Total Keypoints |" << "Total Matches |" << "Total Time (ms) |" << "Ratio (matches/time) |" << "\n";
    std::cout << "|:---:|:----:|:-----:|:-----:|:-----:|\n";
    std::cout << "| " << detectorType << " + " << descriptorType << " |" << keypointTotal << " |" << matchesTotal << " |" << timeTotal * 1000.0 << " |" << matchesTotal / (timeTotal * 1000.0) << " |"<< "\n";
    return 0;
}
