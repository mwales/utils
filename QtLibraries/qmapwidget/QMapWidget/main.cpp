#include "MapTestApp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MapTestApp w;
    w.show();

    return a.exec();
}
