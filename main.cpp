#include "mickammainwindow.h"
#if QT_VERSION < 0x050000
    #include <QtGui/QApplication>
#else
    #include <QtWidgets/QApplication>
#endif
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MicKamMainWindow w;
    w.show();
    
    return a.exec();
}
