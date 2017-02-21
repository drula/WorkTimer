#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/data/images/sitting-rabbit.png"));

    MainWindow w;
    w.show();

    return a.exec();
}
