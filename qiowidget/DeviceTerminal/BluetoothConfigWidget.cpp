#include "BluetoothConfigWidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtDebug>

BluetoothConfigWidget::BluetoothConfigWidget(QWidget *parent):
    QIOWidget(parent),
    theScanButton("Scan"),
    theBtStatus("Bluetooth Status: ")
{
    QVBoxLayout* largestContainer = new QVBoxLayout(this);

    QHBoxLayout* topRow = new QHBoxLayout(this);
    topRow->addWidget(&theServiceSelector);

    theScanButton.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    topRow->addWidget(&theScanButton);

    largestContainer->addLayout(topRow);

    largestContainer->addWidget(&theBtStatus);

    setLayout(largestContainer);
}

QIODevice* BluetoothConfigWidget::getQIoDevice()
{

}

void BluetoothConfigWidget::connectToDevice()
{

}
