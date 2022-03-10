//James Rogers Jan 2022 (c) Plymouth University
//Student ID: 10609827
#include<iostream>
#include <fstream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define PI 3.14159265

int main()
{

    VideoCapture InputStream("../Task2/Video.mp4"); //Load in the video as an input stream
    const Point Pivot(592, 52);                     //Pivot position in the video

    //Open output file for angle data
    ofstream DataFile;
    DataFile.open ("../Task2/Data.csv");

    //Loop through video frames
    while(true){

        //Load next frame
        Mat Frame;
        InputStream.read(Frame);

        //If frame is empty then the video has ended, so break the loop
        if(Frame.empty()){
            break;
        }

        //Video is very high resolution, reduce it to 720p to run faster
        resize(Frame,Frame,Size(1280,720));

        ///// Masking /////
        //Create Mats
        Mat FrameHSV, greenMask;

        //Convert to HSV format
        cvtColor(Frame, FrameHSV, COLOR_BGR2HSV);

        //HSV ranges for green
        Scalar lower_green(65, 60, 60);
        Scalar upper_green(80, 255, 255);

        //Assign upper and lower thresholds for green
        inRange(FrameHSV, lower_green, upper_green, greenMask);

        ///// Moments /////
        Moments m = moments(greenMask, true);
        Point p(m.m10/m.m00, m.m01/m.m00);

        //Pivot and Target Points tracking
        circle(Frame, Pivot, 5, Scalar(0, 0, 255), -1);
        line(Frame, p, Point(592, 52), Scalar(0, 0, 255), 3);
        circle(Frame, p, 20, Scalar(0, 255, 0), 3);

        //Find angle between Pivot and Target
        double angle;
        angle = atan2(p.y - Pivot.y, p.x - Pivot.x) * 180/PI; //Trigonometric function

        //Print angle to terminal and output to csv file
        cout << angle << " degrees." << endl;
        DataFile << angle << endl;

        //Display the frames
        imshow("Video", Frame);
        imshow("HSV Video", FrameHSV);
        imshow("GreenMask Video", greenMask);
        waitKey(10);
    }

    DataFile.close(); //Close output file
}
