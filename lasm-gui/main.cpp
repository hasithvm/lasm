#include <QApplication>
#include "qwmain.h"


#ifdef QICO_NEEDED
#include <QtPlugin>
Q_IMPORT_PLUGIN(qico)
#endif
int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    QWMain w;
    w.show();
    
    return a.exec();
}
