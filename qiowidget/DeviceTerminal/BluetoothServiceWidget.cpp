#include "BluetoothServiceWidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtDebug>

BluetoothServiceWidget::BluetoothServiceWidget(QWidget *parent):
    QIOWidget(parent),
    theScanButton("Scan"),
    theBtStatus("Bluetooth Status: ")
{
    QVBoxLayout* largestContainer = new QVBoxLayout(this);

    QHBoxLayout* topRow = new QHBoxLayout(this);
    topRow->addWidget(&theServiceSelector);
    topRow->addWidget(&theScanButton);
    largestContainer->addLayout(topRow);

    largestContainer->addWidget(&theBtStatus);

    setLayout(largestContainer);
}

QIODevice* BluetoothServiceWidget::getQIoDevice()
{

}

void BluetoothServiceWidget::connect()
{

}
