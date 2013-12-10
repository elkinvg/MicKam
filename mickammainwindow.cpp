#include "mickammainwindow.h"
#include "ui_mickammainwindow.h"
#include <QFileDialog>
#include <QTime>
#include <QScrollBar>
#include <QTimer>

MicKamMainWindow::MicKamMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MicKamMainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene;
    //timervalue = 40;
    timervalue = 240;
    scalenow = 1;
    isReopen = true;

    resset = SetCamera();

    ui->MicKamView->installEventFilter(this);
    ui->MicKamView->verticalScrollBar()->installEventFilter(this);

    if (!resset)
    {
        scene->clear();
        QPixmap prew;
        if(prew.load(":/new/prefix1/res/nichtpic.cap"))
        {
            viewHelloImage(prew);
        }
        else
        {
            viewHelloText("OOPS!");
        }

    }
    else
    {
        SetTimerForNotebookCamera();
        timer->start(timervalue);
    }
}

MicKamMainWindow::~MicKamMainWindow()
{
    delete scene;
    delete ui;
}

void MicKamMainWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

bool MicKamMainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == (QObject*)ui->MicKamView)
    {
        if (((QMouseEvent*)event)->type() == QEvent::Wheel)
        {
            if(((QWheelEvent*)event)->delta()>0 && ui->MicKamView->matrix().m11()<100)
            {

                ui->MicKamView->scale(1.25,1.25);
                scalenow = scalenow*1.25;
                //scalenow = ui->MicKamView->matrix().m11();
            }
            else if (ui->MicKamView->matrix().m11()>0.05)
            {
                ui->MicKamView->scale(0.8,0.8);
                scalenow = scalenow*0.8;
                //scalenow = ui->MicKamView->matrix().m11();

            }
            return true;
        }
    }
    if (obj == (QObject*)ui->MicKamView->verticalScrollBar() && ((QMouseEvent*)event)->type() == QEvent::Wheel && ui->MicKamView->verticalScrollBar()->isVisible())
    {
        if (((QWheelEvent*)event)->delta()>0 && ui->MicKamView->matrix().m11()<100)
        {
            ui->MicKamView->scale(1.25,1.25);
            scalenow = scalenow*1.25;
            //scalenow = ui->MicKamView->matrix().m11();
        }

        else if (ui->MicKamView->matrix().m11()>0.05)
        {
            ui->MicKamView->scale(0.8,0.8);
            scalenow = scalenow*1.25;
            //scalenow = ui->MicKamView->matrix().m11();
        }
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void MicKamMainWindow::on_actionSave_Picture_as_triggered()
{
}

void MicKamMainWindow::on_actionOpen_image_file_triggered()
{
    QFileDialog *fod = new QFileDialog;
    QString ImageFileName = fod->getOpenFileName(this, tr("File..."),/*QString()*/"", tr("Image-Files (*.jpg *.jpeg *.png);;All Files (*)"));

    if (ImageFileName.isEmpty())
    {
        return;
    }

    if (ui->actionPreview_Mode->isChecked())
    {
        ui->actionPreview_Mode->setChecked(false);
        if(timer->isActive()) timer->stop();
    }

    fileOpenImage(ImageFileName);
}

void MicKamMainWindow::viewHelloImage(const QPixmap &pixmap)
{
    scene->clear();
    scene->addPixmap(pixmap);
    scene->setSceneRect(0,0,pixmap.width(),pixmap.height());
    ui->MicKamView->setScene(scene);
}

void MicKamMainWindow::viewHelloText(QString text)
{
    scene->clear();
    scene->addText(text,QFont("times",50));
    ui->MicKamView->setScene(scene);
}

void MicKamMainWindow::fileOpenImage(QString namefile)
{
//    QImage qimage(namefile);
//    viewOpenQimage(qimage);
    QPixmap pixmap;
    if(pixmap.load(namefile))
    {
        ViewAppendText("Open image file: "+namefile);
        //if (timer->isActive()) timer->stop();
    }
    else
    {
        ViewAppendText(namefile+" is not image file!");
        return;
    }
    viewOpenPixmap(pixmap);

}

void MicKamMainWindow::viewOpenPixmap(const QPixmap &pixmap)
{
    scene->clear();
    scene->addPixmap(pixmap);
    scene->setSceneRect(0,0,pixmap.width(),pixmap.height());
    if (isReopen)
    {
        if (ui->MicKamView->matrix().m11()!=1. || ui->MicKamView->matrix().m22()!=1.)
        {
            ui->MicKamView->scale((1./ui->MicKamView->matrix().m11()),(1./ui->MicKamView->matrix().m11()));
        }
        ui->MicKamView->scale((double)ui->MicKamView->width()/(double)pixmap.width(),(double)ui->MicKamView->width()/(double)pixmap.width());
    }
    ui->MicKamView->setScene(scene);
}

void MicKamMainWindow::viewOpenQimage(const QImage &image)
{
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(image));
    scene->setSceneRect(0,0,image.width(),image.height());
    if (isReopen)
    {
        scalenow = ui->MicKamView->matrix().m11();
        if (ui->MicKamView->matrix().m11()!=1. || ui->MicKamView->matrix().m22()!=1.)
        {
            ui->MicKamView->scale((1./ui->MicKamView->matrix().m11()),(1./ui->MicKamView->matrix().m11()));
        }
        ui->MicKamView->scale((double)ui->MicKamView->width()/(double)image.width(),(double)ui->MicKamView->width()/(double)image.width());
        isReopen=false;
    }

    ui->MicKamView->setScene(scene);
}

bool MicKamMainWindow::SetCamera()
{
    //videocap->
    //videocap = new VideoCapture(0);
//    if (videocap->isOpened())
//    {
//        Mat frame;
//        for (int i=0;i<10;i++){
//        videocap->read(frame);ui->MicKamMessage->appendPlainText("o");}

//        imwrite("as.jpg",frame);
//        ui->MicKamMessage->appendPlainText("prodano");
//    }
//    else
//    {
//        ui->MicKamMessage->appendPlainText("ne prodano");
//        return FALSE;
    //    }
    videocap = new VideoCapture(-1);
    if (videocap->isOpened())  {ui->MicKamMessage->appendPlainText("prodano");return true;}
    else return false;
}

void MicKamMainWindow::SetTimerForNotebookCamera()
{
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(previewimage()));
}

void MicKamMainWindow::ViewAppendText(QString message)
{
    QDateTime DiT = QDateTime::currentDateTime();
    QString SDiT = DiT.toString("yyyy.MM.dd hh:mm:ss");
    ui->MicKamMessage->appendPlainText(SDiT+"\n"+message+"\n");
}

void MicKamMainWindow::on_actionSettings_triggered()
{
    CameraSettings = new SetSettingsForCamera;
    CameraSettings->show();
}

void MicKamMainWindow::on_actionActivate_triggered()
{
    if (!resset)
    {
        resset = SetCamera();
    }
}

void MicKamMainWindow::previewimage()
{
    QImage preview;
    QDSandFC dsfc;
    Mat VideoFrame;
    if (videocap->isOpened())
    {

        videocap->read(VideoFrame);
        preview = dsfc.QDSMat2QImageConvert(VideoFrame);
        viewOpenQimage(preview);
    }
    else
    {
    }

}


void MicKamMainWindow::on_actionPreview_Mode_triggered()
{
    ViewAppendText("TRIGGER");
    //ui->actionPreview_Mode->
    if(ui->actionPreview_Mode->isChecked())
    {
        if (!timer->isActive()) timer->start(timervalue);
        //if (loadimage) {delete screenphoto; loadimage = false;}
#ifdef TEMP
    ViewAppendText("neOpen file");
#endif
    }
    else
    {
#ifdef TEMP
    ViewAppendText("Open file");
#endif
        if(timer->isActive()) timer->stop();
    }
}

