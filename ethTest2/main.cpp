/*
This software is used to collect data from small scale pixel array detector prototypes.
It requires the Qt (not really but it's there), libtins, and opencv libraries.
Version 0.01 Super Beta-, Hugh Philipp 2019_03_06, Cornell University

If bugs are found, please inform me so I can fix the code.
If code is copied or modified and used elsewhere, please note its origin.
*/


#include <QCoreApplication>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
//#include<opencv2/imgproc.hpp>
//#include<opencv2/imgcodecs.hpp>

#include "paddatavac.h"
#include <fstream>
#include <iostream>
#include <string>

void onMouse(int, int, int , int, void*);


int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);

    std::cout<<"starting1..."<< std::endl;
    PADspace::PADdataVac *paddy;
    std::cout<<"starting2..."<< std::endl;

    paddy = new PADspace::PADdataVac();
    std::cout<<"starting3..."<< std::endl;

    std::ofstream outanalog;  // stream for raw analog
    std::ofstream outdigital; // stream for raw digital
    std::ofstream outgain;    // stream for gain bits
    std::ofstream outcombi;   // stream for combined

    outanalog.open("../outanalog.bin",std::ios::out | std::ios::binary);
    if (!outanalog.is_open()) std::cout<<"outanalog.bin not open\n";
    outdigital.open("../outdig.bin",std::ios::out | std::ios::binary);
    if (!outdigital.is_open()) std::cout<<"outdigital.bin not open\n";
    outgain.open("../outgain.bin",std::ios::out | std::ios::binary);
    if (!outgain.is_open()) std::cout<<"outgain.bin not open\n";
//    outcombi.open("../outcombi.bin",std::ios::out | std::ios::binary);
//    if (!outcombi.is_open()) std::cout<<"outcombi.bin not open\n";


    double minAn,maxAn;
    double minDg,maxDg;
    double minGn,maxGn;

    double highGn = 1;
    double lowGn  = 10;
    double countGn = lowGn*8500;

    cv::Point minAnP,maxAnP;

    //arrays containing the raw data from the detector
    cv::Mat analogDat;
    analogDat = cv::Mat::zeros(16,19,CV_16U);
    cv::Mat digDat;
    digDat = cv::Mat::zeros(16,19,CV_16U);
    cv::Mat gainDat;
    gainDat = cv::Mat::zeros(16,19,CV_8U);

    //array for combined data - going with 32 bit float
    cv::Mat combiDat;
    combiDat = cv::Mat::zeros(16,19,CV_32FC1);
// ------------------------------------------------------------------
    // Calibration mats needed
    //arrays for the various gains and darks - gain of 'high gain', gain of  'low gain', and gain of 'counter value' - setting high gain to nominal value of 1

    cv::Mat highGnDat;
    highGnDat = cv::Mat::zeros(16,19,CV_32FC1);
    highGnDat.setTo(highGn);

    cv::Mat lowGnDat;
    lowGnDat = cv::Mat::zeros(16,19,CV_32FC1);
    lowGnDat.setTo(lowGn);

    cv::Mat countGnDat;
    countGnDat = cv::Mat::zeros(16,19,CV_32FC1);
    countGnDat.setTo(countGn);

    cv::Mat darkDat; // high gain background
    darkDat = cv::Mat::zeros(16,19,CV_32FC1);
    darkDat.setTo(8100); // this should be acquired

    cv::Mat LGdarkDat;  // low gain effective background
    LGdarkDat = cv::Mat::zeros(16,19,CV_32FC1);
    LGdarkDat.setTo(8100); // this should be acquired

    // and repeating for alternate frames

    cv::Mat highGnDat2;
    highGnDat2 = cv::Mat::zeros(16,19,CV_32FC1);
    highGnDat2.setTo(highGn);

    cv::Mat lowGnDat2;
    lowGnDat2 = cv::Mat::zeros(16,19,CV_32FC1);
    lowGnDat2.setTo(lowGn);

    cv::Mat countGnDat2;
    countGnDat2 = cv::Mat::zeros(16,19,CV_32FC1);
    countGnDat2.setTo(countGn);

    cv::Mat darkDat2; // high gain background
    darkDat2 = cv::Mat::zeros(16,19,CV_32FC1);
    darkDat2.setTo(8100); // this should be acquired

    cv::Mat LGdarkDat2;  // low gain effective background
    LGdarkDat2 = cv::Mat::zeros(16,19,CV_32FC1);
    LGdarkDat2.setTo(8100); // this should be acquired

// ---------------------------------------------------------------------

    //arrays containing the scaled data for display
    cv::Mat anaScaled;
    anaScaled = cv::Mat::zeros(16,19,CV_8U);
    cv::Mat digScaled;
    digScaled = cv::Mat::zeros(16,19,CV_8U);
    cv::Mat gainScaled;

    std::cout<<"starting..."<< std::endl;

    //display windows
    //    cv::namedWindow("analog", cv::WINDOW_NORMAL);
    //    cv::namedWindow("digital",cv::WINDOW_NORMAL);
    //    cv::namedWindow("gain",cv::WINDOW_NORMAL);
    cv::namedWindow("analog", cv::WINDOW_GUI_NORMAL);
    cv::namedWindow("digital",cv::WINDOW_GUI_NORMAL);
    cv::namedWindow("gain",cv::WINDOW_GUI_NORMAL);
    cv::setMouseCallback("analog", onMouse, (void*)&analogDat);

    cv::waitKey(1);

    auto j = 0; // variable used to count frames and decide if frame will be displayed - so j mod something in the loop so every frame isn't displayed

    while(1)
    {

        j++;

        paddy->getFrame(reinterpret_cast<uint16_t *>(analogDat.data), reinterpret_cast<uint16_t *>(digDat.data),reinterpret_cast<uint8_t *>(gainDat.data));
// -------------------------------------------------------
//   This was just a debugging loop to display pixel values
//        for(auto xi = 0;xi<19;xi++)
//        {
//            for(auto yi = 0;yi<16;yi++)
//            {
//                std::cout<<analogDat.at<uint16_t>(cv::Point(xi,yi))<<", ";
//            }
//            std::cout<<std::endl;
//        }
// -----------------------------------------------------
// -----------------------------------------------------
            //here is where the experessions for combined data should go. if high gain (no gain bit) pixel-wise combiDat = (analogDat-darkDat)*highGnDat
            // if low gain (gain bit present) pixel-wise combiDat = (analogDat - LGdarkDat)*lowGnDat + digDat*countGnDat
            // need to collect darkDat and LGdarkDat
            // addtionally, there is a difference between alternate frames, so even and odd frames are going to need separate or modified
            // gains and offsets.
// -----------------------------------------------------

        if(!(j%10))
        {
            //cv::convertScaleAbs(analogDat, anaScaled, 255 / 1000);
            cv::minMaxLoc(analogDat,&minAn,&maxAn,&minAnP,&maxAnP);  // was using this for autoscaling
            //std::cout<<"min: "<< analogDat.at<uint16_t>(minAnP) <<", max: "<< analogDat.at<uint16_t>(maxAnP)<< std::endl;

            minAn = 1000; maxAn=15000; // hard coding limits right now.
            minDg = 0;  maxDg = 30;
            minGn = 0;  maxGn = 1;

            cv::convertScaleAbs(analogDat, anaScaled, 255/(minAn-maxAn),-255*minAn/(maxAn-minAn));
            cv::convertScaleAbs(digDat, digScaled, 255/(minDg-maxDg),-255*minDg/(maxDg-minDg));
            cv::convertScaleAbs(gainDat, gainScaled, 255/(minGn-maxGn),-255*minGn/(maxGn-minGn));


            cv::imshow("analog",anaScaled);
            cv::imshow("digital",digScaled);
            cv::imshow("gain",gainScaled);
            if(j==0)
            {
                cv::resizeWindow("analog",300,300);
                cv::resizeWindow("digital",300,300);
                cv::resizeWindow("gain",300,300);
            }

            cv::waitKey(5);
        }
        outanalog.write(const_cast<char *>(reinterpret_cast<char *>(analogDat.data)),sizeof(uint16_t)*16*19);
        outdigital.write(const_cast<char *>(reinterpret_cast<char *>(digDat.data)),sizeof(uint16_t)*16*19);
        outgain.write(const_cast<char *>(reinterpret_cast<char *>(gainDat.data)),sizeof(uint8_t)*16*19);

        outanalog.flush();
        outdigital.flush();
        outgain.flush();

    }

    outanalog.close();
    outdigital.close();
    outgain.close();

    delete paddy;
    return a.exec();
   // return 1;
}



void onMouse(int event, int x, int y, int flags, void* param)
{
    char text[100];
    cv::Mat img2;
    uint16_t p;

    img2 = (reinterpret_cast<cv::Mat *>(param))->clone();

    if (event == CV_EVENT_LBUTTONDOWN)
    {

        p = img2.at<uint16_t>(y,x);
        sprintf(text, "x=%d, y=%d, v=%d", x, y, p);
        cv::putText(img2, text, cv::Point(3,3), cv::FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
        cv::imshow("image", img2);
        std::cout<<text<<std::endl;
    }
    else if (event == CV_EVENT_RBUTTONDOWN)
    {
        cv::imshow("image", img2);
        std::cout<<"rbutton"<<std::endl;
    }
}




