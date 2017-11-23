#include "CropperTestMainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CropperTestMainWindow w;
    w.show();

    return a.exec();
}
