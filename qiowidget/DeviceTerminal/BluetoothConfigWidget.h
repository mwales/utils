#ifndef BLUETOOTHSERVICEWIDGET_H
#define BLUETOOTHSERVICEWIDGET_H

#include "QIOWidget.h"
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
class BluetoothConfigWidget : public QIOWidget
{

    Q_OBJECT
public:
    BluetoothConfigWidget(QWidget *parent = 0);

    virtual QIODevice* GetQIoDevice();

public slots:

    virtual void ConnectToDevice();

protected:

    QComboBox theServiceSelector;

    QPushButton theScanButton;

    QLabel theBtStatus;
};

#endif // BLUETOOTHSERVICEWIDGET_H
