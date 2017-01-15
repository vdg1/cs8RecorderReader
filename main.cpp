#include "mainwindow.h"
#include <QApplication>
#include "cs8recordermodel.h"

int main(int argc, char *argv[])
{



    QApplication a(argc, argv);
    MainWindow w;

    //cs8RecorderModel *m=new cs8RecorderModel();
    //m->loadRecording("D:/vdg/Documents/record.rec");
    w.show();

    return a.exec();
}
