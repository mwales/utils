#ifndef MULTIWIDGET_H
#define MULTIWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QStackedWidget>
#include "TcpConfigWidget.h"
#include "BluetoothConfigWidget.h"
#include "SerialConfigWidget.h"

class QIOWidgetSelector : public QWidget
{
    Q_OBJECT
public:
    explicit QIOWidgetSelector(QWidget *parent = 0);

signals:

public slots:

private:



    QPushButton* theButton;

    QLabel* theLabel;

    QComboBox* theDeviceTypeCB;

    // Serial Port Controls
    QComboBox* theSerialPortCB;
    QComboBox* theSerialBaudRateCB;

    // Bluetooth Controls
    QComboBox* theRfcommCB;
    QPushButton* theBluetoothScanButton;

    // TCP Controls
    QLineEdit* theTcpHostname;
    QSpinBox* thePortNumber;

    TcpConfigWidget* theTcpWidget;

    BluetoothConfigWidget* theBtWidget;

    SerialConfigWidget* theSerialWidget;

    QStackedWidget* theWidgetStack;
};

#endif // MULTIWIDGET_H
