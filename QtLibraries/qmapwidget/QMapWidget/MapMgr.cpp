#include "MapMgr.h"

#include<cartotype_framework.h>

char const * const DEFAULT_FONT_PATH = "font/DejaVuSans.ttf";
char const * const STYLE_FILE_PATH = "style/osm-neo-style.xml";

char const * const CARTOTYPE_PATH = "/home/mwales/Downloads/cartotype-ubuntu-sdk";


MapMgr::MapMgr(std::string mapFile, std::string sdkPath)
{
    std::string font = CARTOTYPE_PATH;
    std::string style = CARTOTYPE_PATH;

    if (!sdkPath.empty())
    {
        font = sdkPath;
        style = sdkPath;
    }

    if (font.back() != '/')
    {
        font += '/';
    }

    if (style.back() != '/')
    {
        style += '/';
    }

    style += STYLE_FILE_PATH;
    font += DEFAULT_FONT_PATH;

    xSize = 640;
    ySize = 480;

    CartoType::TResult result;
    theGisFramework = CartoType::CFramework::New(result, mapFile, style, font, xSize, ySize);

    std::cout << "Sytle File; " << style << std::endl;
    std::cout << "Font File: " << font << std::endl;

    std::cout << "MapMgr(" << mapFile << ") created, result = " << result << std::endl;

    if (result != CartoType::KErrorNone)
    {
        std::cerr << "Error initializing the CartoType framework: " << result << std::endl;
    }
}

QImage MapMgr::getImage()
{
    CartoType::TResult result;

    double minX, maxX, minY, maxY;
    theGisFramework->GetMapExtent(minX, minY, maxX, maxY, CartoType::TCoordType::Display);

    std::cout << "Mins: " << minX << ", " << minY << std::endl;
    std::cout << "Maxs: " << maxX << ", " << maxY << std::endl;

    CartoType::TBitmap const * bmp = theGisFramework->MapBitmap(result);
    if (result != CartoType::KErrorNone)
    {
        std::cerr << "Error reading CartoType bitmap: " << result << std::endl;
        return QImage();
    }

    std::cout << "Type: " << getImageTypeString(bmp->Type()) << std::endl;

    std::cout << "getPixmap returning a bitmap " << bmp->Width() << " x " << bmp->Height() << std::endl;

    QImage retVal(bmp->Width(), bmp->Height(), QImage::Format_RGB32);

    std::cout << "Bits Per Pixel: " << bmp->BitsPerPixel() << ", RowBytes = "
              << bmp->RowBytes() << std::endl;

    for(int curRow = 0; curRow < ySize; curRow++)
    {
        uint32_t* pixelPtr = (uint32_t*)( bmp->Data() + curRow * bmp->RowBytes());

        for(int curX = 0; curX < xSize; curX++)
        {
            // Did not help, map looks a little weird color wise
            uint32_t pixData;
            pixData = pixelPtr[curX] & 0x000000ff << 24;
            pixData = pixelPtr[curX] & 0x0000ff00 << 8;
            pixData = pixelPtr[curX] & 0x00ff0000 >> 8;
            pixData = pixelPtr[curX] & 0xff000000 >> 24;


            retVal.setPixel(curX, curRow, pixelPtr[curX]);
        }
    }

    return retVal;
}

std::string MapMgr::getImageTypeString(CartoType::TBitmapType t)
{
    std::string retVal;
    int bitsPerPixel = (int) t;

    retVal += "[";
    retVal += std::to_string((int) t);
    retVal += "] ";

    if ( bitsPerPixel & (int) CartoType::TBitmapType::KColored)
    {
        retVal += "Colored ";
    }

    if ( bitsPerPixel & (int) CartoType::TBitmapType::KPalette)
    {
        retVal += "Palette";
    }

    std::map<int, std::string> convMap;
    convMap[(int) CartoType::TBitmapType::A1]     = "1 Bit Per Pixel";
    convMap[(int) CartoType::TBitmapType::A8]     = "8 Bit Per Pixel";
    convMap[(int) CartoType::TBitmapType::A16]    = "16 Bits Per Pixel (Mono)";
    convMap[(int) CartoType::TBitmapType::RGB16]  = "16 Bits Per Pixel (RGB565)";
    convMap[(int) CartoType::TBitmapType::RGB24]  = "24 Bits Per Pixel";
    convMap[(int) CartoType::TBitmapType::RGBA32] = "32 Bits Per Pixel (ABGR)";
    convMap[(int) CartoType::TBitmapType::P8]     = "8 Bits Per Pixel";

    if (convMap.find(bitsPerPixel) != convMap.end())
    {
        retVal += convMap[bitsPerPixel];
    }
    else
    {
        retVal += "Unknown Type";
    }

    return retVal;

}
