#include "mickammainwindow.h"
#include "ui_mickammainwindow.h"
#include <QFileDialog>
#include <QTime>

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
    }
}

MicKamMainWindow::~MicKamMainWindow()
{
    delete scene;
    delete ui;
}

void MicKamMainWindow::resizeEvent(QResizeEvent *event)
{
}

bool MicKamMainWindow::eventFilter(QObject *obj, QEvent *event)
{
    QObject::eventFilter(obj, event);
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
    QPixmap pixmap(namefile);
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
//    ui->MicKamView->scale(((double)MKViewWidth/(double)pixmap.width()),((double)MKViewWidth/(double)pixmap.width()));
    QString message("pixmap.width()="+QString::number(pixmap.width())+"... ui->MicKamView->width()"+" base "+QString::number(ui->MicKamView->window()->width())+"...."+QString::number(ui->widget_view->width()));

    ViewAppendText(message);
//    if (pixmap.width()>ui->MicKamView->minimumWidth())
//    {
//        ui->MicKamView->scale(((double)ui->MicKamView->width()/(double)pixmap.width()),((double)ui->MicKamView->width()/(double)pixmap.width()));
//    }
    ui->MicKamView->setScene(scene);
}

void MicKamMainWindow::viewOpenQimage(const QImage &image)
{
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(image));
    scene->setSceneRect(0,0,image.width(),image.height());
    ui->MicKamView->setScene(scene);
}

bool MicKamMainWindow::SetCamera()
{
    return FALSE;
}

void MicKamMainWindow::ViewAppendText(QString message)
{
    QDateTime DiT = QDateTime::currentDateTime();
    QString SDiT = DiT.toString("yyyy.MM.dd hh:mm:ss");
    ui->MicKamMessage->appendPlainText(SDiT+"\n"+message+"\n");
}
