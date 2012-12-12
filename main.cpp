#include <QtGui/QApplication>
#include "mickammainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MicKamMainWindow w;
    w.show();
    
    return a.exec();
}
