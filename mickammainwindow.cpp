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
            viewOpenPixmap(prew);
        }
    }

}

MicKamMainWindow::~MicKamMainWindow()
{
    delete scene;
    delete ui;
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
    if (pixmap.width()>ui->MicKamView->minimumWidth())
    {
        ui->MicKamView->scale(((double)ui->MicKamView->width()/(double)pixmap.width()),((double)ui->MicKamView->width()/(double)pixmap.width()));
    }
    ui->MicKamView->setScene(scene);
    ViewAppendText("MiyAu");
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
    QString Stime = QTime::currentTime().toString().toUtf8();
    ui->MicKamMessage->appendPlainText(Stime+"\t "+message);
}
