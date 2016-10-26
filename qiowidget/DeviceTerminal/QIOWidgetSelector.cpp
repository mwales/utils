#include "QIOWidgetSelector.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSizePolicy>
#include <QtDebug>

const QString SERIAL_PORT = "Serial Port";
const QString BLUETOOTH   = "Bluetooth";
const QString TCP_SOCKET  = "TCP Socket";

QIOWidgetSelector::QIOWidgetSelector(QWidget *parent) : QWidget(parent)
{

    QHBoxLayout* hbLayout = new QHBoxLayout(this);

    theDeviceTypeCB = new QComboBox(this);
    theDeviceTypeCB->addItem(TCP_SOCKET);
    theDeviceTypeCB->addItem(BLUETOOTH);
    theDeviceTypeCB->addItem(SERIAL_PORT);
    theDeviceTypeCB->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    setLayout(hbLayout);
    hbLayout->addWidget(theDeviceTypeCB);

    theTcpWidget = new TcpConfigWidget();
    hbLayout->addWidget(theTcpWidget);

    theBtWidget = new BluetoothConfigWidget();
    hbLayout->addWidget(theBtWidget);

    theSerialWidget = new SerialConfigWidget();
    hbLayout->addWidget(theSerialWidget);

    connect(theDeviceTypeCB,                SIGNAL(currentIndexChanged(int)),
            this,                           SLOT(indexChanged(int)));

    indexChanged(0);

}


void QIOWidgetSelector::indexChanged(int index)
{
    qDebug() << "Index changed to" << index;

    if (index == 0)
    {
        theTcpWidget->show();
    }
    else
    {
        theTcpWidget->hide();
    }

    if (index == 1)
    {
        theBtWidget->show();
    }
    else
    {
        theBtWidget->hide();
    }

    if (index == 2)
    {
        theSerialWidget->show();
    }
    else
    {
        theSerialWidget->hide();
    }

}
