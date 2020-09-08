#include "MapTestApp.h"
#include "ui_MapTestApp.h"

#include "MapMgr.h"

MapTestApp::MapTestApp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MapTestApp)
{
    ui->setupUi(this);

    theMapMgr = new MapMgr("/home/mwales/Downloads/cartotype-ubuntu-sdk/map/boston.ctm1");

    QImage mapGraphic = theMapMgr->getImage();

    ui->label->setPixmap(QPixmap::fromImage(mapGraphic));
}

MapTestApp::~MapTestApp()
{
    delete ui;
}
