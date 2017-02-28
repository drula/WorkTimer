#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(IconRc));

    QTranslator translator;
    translator.load(TranslationRc);
    a.installTranslator(&translator);

    MainWindow w;
    w.show();

    return a.exec();
}
