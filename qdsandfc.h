#ifndef QDSANDFC_H
#define QDSANDFC_H

//#include "detectorscan.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QImage>

using namespace std;

enum {
    R_CANAL = 1, G_CANAL = 2, B_CANAL = 3,
    H_CANAL = 4, S_CANAL = 5, V_CANAL = 6,
    GRAY = 7
    };

class QDSandFC
{
public:
    QDSandFC();
    explicit QDSandFC(const QImage &qimage);
    QDSandFC(const uchar*,long int);

    cv::Mat Loadimage; //

    cv::Mat QDSQImage2MatConvert(const QImage &qimage);
    QImage QDSMat2QImageConvert(const cv::Mat &matimage);

    // Get value of contrast \/
    double QGetContrast(const QImage &qimage, int grid, int koorx, int koory);
    vector<vector<double > > QGetContrast(const cv::Mat &MatImage, int grid);
    vector<vector<double > > QGetContrast(const QImage &qimage, int grid);
    // ---
    vector<vector<double > > QGetContrast(int grid);
    double QGetContrast(int grid, int koorx, int koory);
    // Get value of contrast /\

    cv::Mat QDSGetMatFromCharData(const uchar*,long int);


    //temp method \/
    void QSetDSpreamp(QImage &qimage,int radius, int iteration, int canal, int ifnegative, int ifthreshold, int threshold);
    //temp method /\

    // from DS methods
    void SelectChannel(cv::Mat &image, int ifnegative, int canal = GRAY);
    void Negative1ch(cv::Mat& image1ch);

private:
    void GetContrast(const cv::Mat &image3chorgray,int sizeofmatforcon, vector<vector<double> > &valofcon);
    double ValueOfContrast(const cv::Mat &image1ch);
    double SigmaForMat(const cv::Mat &image1ch);

};

#endif
