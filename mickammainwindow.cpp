#include "mickammainwindow.h"
#include "ui_mickammainwindow.h"
#include <QFileDialog>
#include <QTime>
#include <QScrollBar>

MicKamMainWindow::MicKamMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MicKamMainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene;

    resset = SetCamera();

    if (!resset)
    {
        QPixmap prew;
        if(prew.load("nichtpic.cap"))
        {
            viewHelloImage(prew);
        }
        else
        {
            viewHelloText("OOPS!");
        }
        ui->MicKamView->installEventFilter(this);
        ui->MicKamView->verticalScrollBar()->installEventFilter(this);
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
            }
            else if (ui->MicKamView->matrix().m11()>0.05)
            {
                ui->MicKamView->scale(0.8,0.8);
                scalenow = ui->MicKamView->matrix().m11();
            }
            return true;
        }
    }
    if (obj == (QObject*)ui->MicKamView->verticalScrollBar() && ((QMouseEvent*)event)->type() == QEvent::Wheel && ui->MicKamView->verticalScrollBar()->isVisible())
    {
        if (((QWheelEvent*)event)->delta()>0 && ui->MicKamView->matrix().m11()<100)
        {
            ui->MicKamView->scale(1.25,1.25);
            scalenow = ui->MicKamView->matrix().m11();
        }

        else if (ui->MicKamView->matrix().m11()>0.05)
        {
            ui->MicKamView->scale(0.8,0.8);
            scalenow = ui->MicKamView->matrix().m11();
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
    QString ImageFileName = QFileDialog::getOpenFileName(this, tr("File..."),QString(), tr("Image-Files (*.jpg *.jpeg *.png);;All Files (*)"));
    if (ImageFileName.isEmpty())
    {
        return;
    }
    ui->actionPreview_Mode->setChecked(false);
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
    if (ui->MicKamView->matrix().m11()!=1. || ui->MicKamView->matrix().m22()!=1.)
    {
        ui->MicKamView->scale((1./ui->MicKamView->matrix().m11()),(1./ui->MicKamView->matrix().m11()));
    }
    ui->MicKamView->scale((double)ui->MicKamView->width()/(double)pixmap.width(),(double)ui->MicKamView->width()/(double)pixmap.width());
    ui->MicKamView->setScene(scene);
}

void MicKamMainWindow::viewOpenQimage(const QImage &image)
{
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(image));
    scene->setSceneRect(0,0,image.width(),image.height());
    ui->MicKamView->setScene(scene);
    if (ui->MicKamView->matrix().m11()!=1. || ui->MicKamView->matrix().m22()!=1.)
    {
        ui->MicKamView->scale((1./ui->MicKamView->matrix().m11()),(1./ui->MicKamView->matrix().m11()));
    }
    ui->MicKamView->scale((double)ui->MicKamView->width()/(double)image.width(),(double)ui->MicKamView->width()/(double)image.width());
}

bool MicKamMainWindow::SetCamera()
{
//    //videocap->
//    videocap = new VideoCapture();
//    Mat frame;
//    videocap->read(frame);
//    imwrite("as.jpg",frame);
//    if (videocap->isOpened()) return true;
//    else    return FALSE;
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
