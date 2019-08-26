#!/bin/bash

# src/matching2D_Student.cpp
# bool crossCheck = true;

# src/MidTermProject_Camera_Student.cpp
# line 79 std::string detectorType = "SHITOMASI"; // SHITOMASI, HARRIS, FAST, BRISK, ORB, AKAZE, SIFT
# line 154 std::string descriptorType = "BRISK"; //BRISK, BRIEF, ORB, FREAK, AKAZE, SIFT
# line 169 std::string matcherType = "MAT_BF";        // MAT_BF, MAT_FLANN
# line 170 std::string descriptorType_HOG_BIN = "DES_HOG"; // DES_BINARY, DES_HOG
# line 171 std::string selectorType = "SEL_NN";       // SEL_NN, SEL_KNN

# create output directory to store ouput data
mkdir -p output

detectorType_arr=( SHITOMASI HARRIS FAST BRISK ORB AKAZE SIFT )
descriptorType_arr=( BRISK BRIEF ORB FREAK AKAZE SIFT )
desType_HB_arr=( DES_BINARY DES_BINARY DES_BINARY DES_BINARY DES_BINARY DES_HOG )

# date and time
date +"%m-%d-%Y-%r" &>output/output.md

# clean project
./clean.sh

# total count
COUNT=1

# detector count
det_COUNT=0

for i in "${detectorType_arr[@]}"; do
    # grep and sed detectorType
    if [ ${det_COUNT} -ne 0 ]; then
        # grep -nr "detectorType = \"${detectorType_arr[$((det_COUNT - 1))]}\"\;" # DEBUG
        grep -rl "detectorType = \"${detectorType_arr[$((det_COUNT - 1))]}\"\;" ./src/*.cpp | xargs sed -i "s|detectorType = \"${detectorType_arr[$((det_COUNT - 1))]}\"\;|detectorType = \"$i\"\;|g"
    fi
    
    #descriptor count
    des_COUNT=0 

    for j in "${descriptorType_arr[@]}"; do
        echo -e "\n********************$COUNT********************\n"
        echo -e "\n********************$COUNT********************\n" >>output/output.md
        # echo -e "$i-$j-${desType_HB_arr[${det_COUNT}]}-${det_COUNT}-${des_COUNT}.txt" # DEBUG
        echo -e "\n$i-$j-${desType_HB_arr[${des_COUNT}]}-${det_COUNT}-${des_COUNT}.txt\n" >>output/output.md

        # grep and sed descriptorType
        # echo -e "${COUNT} detectorType = \"${detectorType_arr[${det_COUNT}]}\"\; detectorType = \"${descriptorType_arr[$((des_COUNT))]}\"\; descriptorType_HOG_BIN = \"${desType_HB_arr[$((det_COUNT))]}\"\;" # DEBUG
        if [ ${des_COUNT} -ne 0 ]; then
            # grep -nr  "descriptorType = \"${descriptorType_arr[$((des_COUNT - 1))]}\"\;" # DEBUG
            # echo -e $j # DEBUG
            grep -rl  "descriptorType = \"${descriptorType_arr[$((des_COUNT - 1))]}\"\;" ./src/*.cpp | xargs sed -i "s|descriptorType = \"${descriptorType_arr[$((des_COUNT - 1))]}\"\;|descriptorType = \"${descriptorType_arr[$((des_COUNT))]}\"\;|g"

            # grep and sed descriptorType_HOG_BIN
            grep -rl "descriptorType_HOG_BIN = \"${desType_HB_arr[$((des_COUNT - 1))]}\"\;" ./src/*.cpp | xargs sed -i "s|descriptorType_HOG_BIN = \"${desType_HB_arr[$((des_COUNT - 1))]}\"\;|descriptorType_HOG_BIN = \"${desType_HB_arr[$((des_COUNT))]}\"\;|g"
        fi

        # clean, build and run
        ./clean.sh && ./build.sh
        ./run.sh &>>output/output.md

        COUNT=$((COUNT + 1))
        des_COUNT=$((des_COUNT + 1))

    done
    # grep and sed back to descriptorType = "BRISK";
    des_COUNT=$((des_COUNT - 1))
    grep -rl "descriptorType = \"${descriptorType_arr[${des_COUNT}]}\"\;" ./src/*.cpp | xargs sed -i "s|descriptorType = \"${descriptorType_arr[${des_COUNT}]}\"\;|descriptorType = \"${descriptorType_arr[0]}\"\;|g"

    # grep and sed back to descriptorType_HOG_BIN = "DES_BINARY";
    grep -rl "descriptorType_HOG_BIN = \"${desType_HB_arr[${des_COUNT}]}\"\;" ./src/*.cpp | xargs sed -i "s|descriptorType_HOG_BIN = \"${desType_HB_arr[${des_COUNT}]}\"\;|descriptorType_HOG_BIN = \"${desType_HB_arr[0]}\"\;|g"

    det_COUNT=$((det_COUNT + 1))
done

# grep and sed back to detectorType = "SHITOMASI";
det_COUNT=$((det_COUNT - 1))
grep -rl "detectorType = \"${detectorType_arr[${det_COUNT}]}\"\;" ./src/*.cpp | xargs sed -i "s|detectorType = \"${detectorType_arr[${det_COUNT}]}\"\;|detectorType = \"${detectorType_arr[0]}\"\;|g"

git status
