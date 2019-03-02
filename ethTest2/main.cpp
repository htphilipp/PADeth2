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


    cv::Mat analogDat;
    //analogDat = cv::Mat::zeros(16,19,int16_t());
    analogDat = cv::Mat::zeros(19,16,uint16_t());
    cv::Mat digDat;
    //digDat = cv::Mat::zeros(16,19,int32_t());
    digDat = cv::Mat::zeros(19,16,uint32_t());
    cv::Mat anaScaled;
    //anaScaled = cv::Mat::zeros(16,19,CV_8UC1);
    anaScaled = cv::Mat::zeros(19,16,uint16_t());

    cv::Mat digScaled;
    //digScaled = cv::Mat::zeros(16,19,CV_8UC1);
    digScaled = cv::Mat::zeros(19,16,uint32_t());

    std::cout<<"starting..."<< std::endl;

    cv::namedWindow("analog", cv::WINDOW_NORMAL);
    cv::namedWindow("digital",cv::WINDOW_NORMAL);
    cv::waitKey(1);

    auto j = 0;

    while(1)
    {

        j++;

        paddy->getFrame(reinterpret_cast<uint16_t *>(analogDat.data), reinterpret_cast<uint32_t *>(digDat.data));

        if(!(j%5))
        {
            //cv::convertScaleAbs(analogDat, anaScaled, 255 / 1000);
            cv::convertScaleAbs(analogDat, anaScaled, 255 / 9000);

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
        outdigital.write(const_cast<char *>(reinterpret_cast<char *>(digDat.data)),sizeof(uint32_t)*16*19);

        outanalog.flush();
        outdigital.flush();

    }

    delete paddy;
    return a.exec();
}
