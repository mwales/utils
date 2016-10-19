#ifndef BLUETOOTHSERVICEWIDGET_H
#define BLUETOOTHSERVICEWIDGET_H

#include "QIOWidget.h"
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
class BluetoothServiceWidget : public QIOWidget
{

    Q_OBJECT
public:
    BluetoothServiceWidget(QWidget *parent = 0);

    virtual QIODevice* getQIoDevice();

public slots:

    virtual void connect();

protected:

    QComboBox theServiceSelector;

    QPushButton theScanButton;

    QLabel theBtStatus;
};

#endif // BLUETOOTHSERVICEWIDGET_H
