#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QLocale polishLocale(QLocale::Polish);
    QLocale::setDefault(polishLocale);
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    return a.exec();
}
