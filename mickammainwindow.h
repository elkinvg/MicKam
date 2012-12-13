#ifndef MICKAMMAINWINDOW_H
#define MICKAMMAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

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

    QGraphicsScene *scene;
    // camera function
    bool SetCamera();

    // qt window function
    void fileOpenImage(QString namefile);
    void viewOpenPixmap(const QPixmap &pixmap);
    void viewOpenQimage(const QImage &image);    
    void ViewAppendText(QString message);

    //virtual
    void resizeEvent(QResizeEvent *);
    bool eventFilter(QObject *, QEvent *);

private slots:
    void on_actionSave_Picture_as_triggered();

    void on_actionOpen_image_file_triggered();

private:
    Ui::MicKamMainWindow *ui;
    void viewHelloImage(const QPixmap &pixmap);
    void viewHelloText(QString);
};

#endif // MICKAMMAINWINDOW_H
