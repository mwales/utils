#ifndef MAPMGR_H
#define MAPMGR_H

#include <QImage>

#include<iostream>


#include<cartotype_framework.h>


class MapMgr
{
public:
    MapMgr(std::string mapFile, std::string sdkPath = "");

    QImage getImage();

    std::string getImageTypeString(CartoType::TBitmapType t);

protected:

    int xSize;

    int ySize;

    std::unique_ptr<CartoType::CFramework> theGisFramework;
};

#endif // MAPMGR_H
