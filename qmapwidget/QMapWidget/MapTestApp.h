#ifndef MAPTESTAPP_H
#define MAPTESTAPP_H

#include <QMainWindow>


class MapMgr;

namespace Ui {
class MapTestApp;
}

class MapTestApp : public QMainWindow
{
    Q_OBJECT

public:
    explicit MapTestApp(QWidget *parent = 0);
    ~MapTestApp();

private:
    Ui::MapTestApp *ui;

    MapMgr* theMapMgr;
};

#endif // MAPTESTAPP_H
