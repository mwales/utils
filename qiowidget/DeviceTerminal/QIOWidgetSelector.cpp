#include "QIOWidgetSelector.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

const QString SERIAL_PORT = "Serial Port";
const QString BLUETOOTH   = "Bluetooth";
const QString TCP_SOCKET  = "TCP Socket";

MultiWidget::MultiWidget(QWidget *parent) : QWidget(parent)
{

    QHBoxLayout* hbLayout = new QHBoxLayout(this);
    theLabel = new QLabel(this);
    theButton = new QPushButton();

    theDeviceTypeCB = new QComboBox(this);
    theDeviceTypeCB->addItem(SERIAL_PORT);
    theDeviceTypeCB->addItem(BLUETOOTH);
    theDeviceTypeCB->addItem(TCP_SOCKET);

    setLayout(hbLayout);
    hbLayout->addWidget(theDeviceTypeCB);
    hbLayout->addWidget(theLabel);
    hbLayout->addWidget(theButton);

    theTcpWidget = new TcpDestinationWidget(this);
    theBtWidget = new BluetoothServiceWidget(this);
    theSerialWidget = new SerialConfigWidget(this);

    theWidgetStack = new QStackedWidget(this);
    theWidgetStack->addWidget(theSerialWidget);
    theWidgetStack->addWidget(theBtWidget);
    theWidgetStack->addWidget(theTcpWidget);

    connect(theDeviceTypeCB,                SIGNAL(currentIndexChanged(int)),
            theWidgetStack,                 SLOT(setCurrentIndex(int)));

    hbLayout->addWidget(theWidgetStack);
}


