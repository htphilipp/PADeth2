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

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);

    std::cout<<"starting1..."<< std::endl;
    PADspace::PADdataVac *paddy;
    std::cout<<"starting2..."<< std::endl;

    paddy = new PADspace::PADdataVac();
    std::cout<<"starting3..."<< std::endl;

    std::ofstream outanalog;
    std::ofstream outdigital;

    outanalog.open("../outanalog.bin",std::ios::out | std::ios::binary);
    if (!outanalog.is_open()) std::cout<<"outanalog.bin not open\n";
    outdigital.open("../outdig.bin",std::ios::out | std::ios::binary);
    if (!outdigital.is_open()) std::cout<<"outdigital.bin not open\n";


    double minAn,maxAn;
    cv::Point minAnP,maxAnP;


    cv::Mat analogDat;
    analogDat = cv::Mat::zeros(16,19,CV_16U);
    cv::Mat digDat;
    digDat = cv::Mat::zeros(16,19,CV_16U);
    cv::Mat anaScaled;
    anaScaled = cv::Mat::zeros(16,19,CV_8U);

    cv::Mat digScaled;
    digScaled = cv::Mat::zeros(16,19,CV_8U);

    std::cout<<"starting..."<< std::endl;

    cv::namedWindow("analog", cv::WINDOW_NORMAL);
    cv::namedWindow("digital",cv::WINDOW_NORMAL);
    cv::waitKey(1);

    auto j = 0;

    while(1)
    {

        j++;

        paddy->getFrame(reinterpret_cast<uint16_t *>(analogDat.data), reinterpret_cast<uint16_t *>(digDat.data));

//        for(auto xi = 0;xi<19;xi++)
//        {
//            for(auto yi = 0;yi<16;yi++)
//            {
//                std::cout<<analogDat.at<uint16_t>(cv::Point(xi,yi))<<", ";
//            }
//            std::cout<<std::endl;
//        }


        if(!(j%10))
        {
            //cv::convertScaleAbs(analogDat, anaScaled, 255 / 1000);
            cv::minMaxLoc(analogDat,&minAn,&maxAn,&minAnP,&maxAnP);
            //std::cout<<"min: "<< analogDat.at<uint16_t>(minAnP) <<", max: "<< analogDat.at<uint16_t>(maxAnP)<< std::endl;

            minAn = 1000; maxAn=15000;

            cv::convertScaleAbs(analogDat, anaScaled, 255/(minAn-maxAn),-255*minAn/(maxAn-minAn));
            cv::imshow("analog",anaScaled);
            cv::imshow("digital",digDat);
            if(j==0)
            {
                cv::resizeWindow("analog",300,300);
                cv::resizeWindow("digital",300,300);
            }

            cv::waitKey(5);
        }
        outanalog.write(const_cast<char *>(reinterpret_cast<char *>(analogDat.data)),sizeof(uint16_t)*16*19);
        outdigital.write(const_cast<char *>(reinterpret_cast<char *>(digDat.data)),sizeof(uint16_t)*16*19);

        outanalog.flush();
        outdigital.flush();

    }

    delete paddy;
    return a.exec();
}
