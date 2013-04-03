#include <QApplication>
#include "qwmain.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWMain w;
    w.show();
    
    return a.exec();
}
