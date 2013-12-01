#ifndef MICKAMMAINWINDOW_H
#define MICKAMMAINWINDOW_H
//QT
#include <QMainWindow>
#include <QGraphicsScene>
#include <setsettingsforcamera.h>
// OPENCV
#include <opencv/cv.h>
#include <opencv/highgui.h>
using namespace cv;

namespace Ui {
class MicKamMainWindow;
}

class MicKamMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MicKamMainWindow(QWidget *parent = 0);
    ~MicKamMainWindow();

    bool resset;  // if camera is active return true
    double scalenow; // value for scale of graphicsviews

    QGraphicsScene *scene;
    SetSettingsForCamera *CameraSettings;
    VideoCapture *videocap;

    // camera function
    bool SetCamera();

    // qt window function
    void fileOpenImage(QString namefile);
    void viewOpenPixmap(const QPixmap &pixmap);
    void viewOpenQimage(const QImage &image);    


    //virtual
    virtual void resizeEvent(QResizeEvent *);
    virtual bool eventFilter(QObject *, QEvent *);

private slots:
    void on_actionSave_Picture_as_triggered();

    void on_actionOpen_image_file_triggered();

    void on_actionSettings_triggered();

    void on_actionActivate_triggered();

private:
    Ui::MicKamMainWindow *ui;
    void ViewAppendText(QString message);
    void viewHelloImage(const QPixmap &pixmap);
    void viewHelloText(QString);
};

#endif // MICKAMMAINWINDOW_H
