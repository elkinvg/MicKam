#include "qdsandfc.h"


QDSandFC::QDSandFC()
{
}

QDSandFC::QDSandFC(const QImage &qimage)
{
    Loadimage = QDSQImage2MatConvert(qimage);
}

QDSandFC::QDSandFC(const uchar *cameradata, long size)
{
    std::vector<uchar> buffer(cameradata, cameradata+size);
    Loadimage = cv::imdecode(buffer,1);
}

cv::Mat QDSandFC::QDSQImage2MatConvert(const QImage &qimage)
{
    cv::Mat mat(qimage.height(),qimage.width(),CV_8UC4,(uchar*)qimage.bits(), qimage.bytesPerLine());
    cv::Mat outputmat = cv::Mat(mat.rows, mat.cols, CV_8UC3 );
    cv::cvtColor(mat,outputmat,CV_RGBA2RGB);
    return outputmat;
}

QImage QDSandFC::QDSMat2QImageConvert(const cv::Mat &matimage)
{
    // 8-bits unsigned, NO. OF CHANNELS=1

    if(matimage.type()==CV_8UC1)
    {
        QVector<QRgb> colorTable;
        // Set the color table (used to translate colour indexes to qRgb values)
        for (int i=0;i<256;i++) colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar* qImageBuffer1 = (const uchar*)matimage.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer1,matimage.cols,matimage.rows,matimage.step,QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    if(matimage.type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar* qImageBuffer3 = (const uchar*)matimage.data;
        QImage img(qImageBuffer3,matimage.cols,matimage.rows,matimage.step,QImage::Format_RGB888);
        //rgbSwapped() - инверсия цвета
        return img.rgbSwapped();
    }
    else
    {
        cerr << " ERROR: Mat could not be converted to QImage. " << endl;
        return QImage();
    }
}

double QDSandFC::QGetContrast(const QImage &qimage, int grid, int koorx, int koory)
{
    cv::Mat QimToMat = QDSQImage2MatConvert(qimage);
    vector<vector<double> > contrast;
    GetContrast(QimToMat,grid,contrast);
    if (koorx >grid || koory>grid)
    {
        if (koorx>grid) koorx = grid-1;
        if (koory>grid) koory = grid-1;
        // if koorx || koory MORE THAN grid+1
        if (koorx>grid) koorx = 0;
        if (koory>grid) koory = 0;
    }
    return contrast[koorx][koory];
}

double QDSandFC::QGetContrast(int grid, int koorx, int koory)
{
    vector<vector<double> > contrast;
    //if (!Loadimage.data) return 0;
    GetContrast(Loadimage,grid,contrast);
    if (koorx >grid || koory>grid)
    {
        if (koorx>grid) koorx = grid-1;
        if (koory>grid) koory = grid-1;
        // if koorx || koory MORE THAN grid+1
        if (koorx>grid) koorx = 0;
        if (koory>grid) koory = 0;
    }
    return contrast[koorx][koory];
}

cv::Mat QDSandFC::QDSGetMatFromCharData(const uchar *cameradata, long int size)
{
    std::vector<uchar> buffer(cameradata, cameradata+size);
    return cv::imdecode(buffer,1);
}

//void QDSandFC::QSetDSpreamp(QImage &qimage, int radius, int iteration, int canal, int ifnegative, int ifthreshold, int threshold)
//{
//    cv::Mat matimage = QDSQImage2MatConvert(qimage);
//    dsErodePixel(matimage,radius,iteration);

//    if (canal>0)
//    {
//        SelectChannel(matimage,ifnegative,canal);
//        if (ifthreshold)
//        {
//            dsPostThreshold(matimage,(double)threshold,255.);
//        }
//    }


//    //cout << matimage.channels() << "  ";
//    qimage = QDSMat2QImageConvert(matimage);
//}

void QDSandFC::GetContrast(const cv::Mat &image3chorgray, int sizeofmatforcon, vector<vector<double> > &valofcon)
{
    //    better values for int sizeofmatforcon
    //    MATRIXONEEL = 1,
    //    MATRIXTHREE = 3,
    //    MATRIXFIVE = 5,
    //    MATRIXSEVEN = 7,
    //    MATRIXNINE = 9

        int Ncols,Nrows;
        cv::Mat image3chorgraycopy;
        if (image3chorgray.channels()==3)
        {
            image3chorgraycopy = image3chorgray.clone();
            /*image3chorgray = */SelectChannel(image3chorgraycopy,0,GRAY);
        }
        else
        {
            image3chorgraycopy = image3chorgray;
        }
        Ncols = image3chorgraycopy.cols;
        Nrows = image3chorgraycopy.rows;
        int colstep = (int)Ncols/sizeofmatforcon;
        int rowstep = (int)Nrows/sizeofmatforcon;
        int tempcol = 0;
        int temprow;// = 0;
        vector<double> tempvec;
        cv::Rect rectroi;
        for (int i=0;i<sizeofmatforcon;i++)
        {
            temprow = 0;
            rectroi.x = tempcol;
            if (i==sizeofmatforcon-1)
            {
                rectroi.width = Ncols - tempcol;
            }
            else
            {
                rectroi.width = colstep;
            }
            tempvec.clear();
            for (int j=0;j<sizeofmatforcon;j++)
            {
                rectroi.y = temprow;
                if (j==sizeofmatforcon-1)
                {
                    rectroi.height = Nrows - temprow;
                }
                else
                {
                    rectroi.height = rowstep;
                    temprow += rowstep;
                }
                tempvec.push_back(ValueOfContrast(image3chorgraycopy(rectroi)));
            }
            tempcol += colstep;
            valofcon.push_back(tempvec);
            //dsValueOfContrast(image3chorgraycopy());
        }
        //dsValueOfContrast(image3chorgraycopy);
}

double QDSandFC::ValueOfContrast(const cv::Mat &image1ch)
{
    return SigmaForMat(image1ch);
}

double QDSandFC::SigmaForMat(const cv::Mat &image1ch)
{
    int numcol = image1ch.cols;
        int numrow = image1ch.rows;
        cv::Scalar meanval=cv::mean(image1ch);
        double sigma;
        double squarediv = 0;
        int iter = 0;
        for (int i=0;i<numcol;i++)
        {
            for (int j=0;j<numrow;j++)
            {
                squarediv = squarediv + pow((image1ch.at<uchar>(j,i) - meanval[0]),2.0);
                iter++;
            }
        }
        sigma = sqrt((squarediv)/iter);
        return sigma;
}

vector<vector<double> > QDSandFC::QGetContrast(const cv::Mat& MatImage,int grid)
{
    vector<vector<double> > contrast;
    GetContrast(MatImage,grid,contrast);
    return contrast;
}

vector<vector<double> > QDSandFC::QGetContrast(const QImage &qimage, int grid)
{
    cv::Mat QimToMat = QDSQImage2MatConvert(qimage);
    vector<vector<double> > contrast;
    GetContrast(QimToMat,grid,contrast);
    return contrast;
}

vector<vector<double> > QDSandFC::QGetContrast(int grid)
{
    vector<vector<double> > contrast;
    GetContrast(Loadimage,grid,contrast);
    return contrast;
}

void QDSandFC::SelectChannel(cv::Mat &image, int ifnegative, int canal)
{
    /*
      Выбор канала, для преобразования.
      Входное изображение должно быть 3х канальным.
      */


    if (image.channels() == 1)
        {
            return /*image*/;
        }

        vector<cv::Mat> tempin;

        if(canal == H_CANAL || canal == S_CANAL || canal == V_CANAL)
        {
            cvtColor( image, image, CV_BGR2HSV );
        }

        switch (canal)
        {
        case B_CANAL:
            split(image,tempin);
            image = tempin[0];
            break;
        case G_CANAL:
            split(image,tempin);
            image = tempin[1];
            break;
        case R_CANAL:
            split(image,tempin);
            image = tempin[2];
            break;
        case H_CANAL:
            split(image,tempin);
            image = tempin[0];
            break;
        case S_CANAL:
            split(image,tempin);
            image = tempin[1];
            break;
        case V_CANAL:
            split(image,tempin);
            image = tempin[2];
            break;
        case GRAY:
            cvtColor( image, image, CV_BGR2GRAY);
            break;
       default:
            cvtColor( image, image, CV_BGR2GRAY);
        }

        if (ifnegative == 1)
        {
            Negative1ch(image);
        }

}

void QDSandFC::Negative1ch(cv::Mat &image1ch)
{
    if (image1ch.channels()!=1)
        {
    #ifdef DEBUG
            cout << " void DetectorScan::Negative1ch(Mat& image1ch) " << endl;
            cout << " input image must be 1 channel " << endl;
    #endif
            return;
        }
        uchar value,newvalue;
        for (int i = 0; i<image1ch.cols; i++)
        {
            for (int j=0; j<image1ch.rows;j++)
            {
                value = image1ch.at<uchar>(j,i);
                newvalue = 255 - value;
                image1ch.at<uchar>(j,i) = newvalue;
            }
        }
}


//QImage QDSandFC::QDSMat2QImageConvert(const cv::Mat &matimage) // OLDVERSION
//{
//    cv::Mat color;
//    if (matimage.channels() == 1)
//    {
//        std::vector<cv::Mat> mergeqimage3ch;
//        color.create(cv::Size(matimage.cols,matimage.rows),CV_8UC3);
//        mergeqimage3ch.push_back(matimage);
//        mergeqimage3ch.push_back(matimage);
//        mergeqimage3ch.push_back(matimage);
//        cv::merge(mergeqimage3ch,color);
//    }
//    else
//    {
//        if (matimage.channels() == 3)
//        {
//            color = matimage.clone;
//            cv::cvtColor(color,color,CV_BGR2RGB);

//            QImage pImage(color.cols,color.rows,QImage::Format_RGB888);

//            IplImage im = (IplImage)color;
//            //По неизвестным причинам pImage->bytesPerLine() и im.widthStep могут не совпадать
//            if(pImage.bytesPerLine()!=im.widthStep)
//            {
//                //Копирование по строкам
//                for(int i=0;i<pImage.height();i++)
//                {
//                    memcpy(pImage.bits()+i*pImage.bytesPerLine(),color.data+im.widthStep*i,im.widthStep);
//                }
//            }else{
//                //Копирование всего изображения
//                memcpy(pImage.bits(),color.data,im.imageSize);
//            }
//            return pImage;
//        }
//        else
//        {
//            cerr << " Unknown format Mat image \n";
//            return QImage();
//        }
//    }
//}
//convert(cv::Mat image)
//{
//    cv::flip(image,image,1); // process the image
//    // change color channel ordering
//    cv::cvtColor(image,image,CV_BGR2RGB); // or rgbSwapped() for Qimage???
//    // Qt image
//    QImage img= QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);
//}
