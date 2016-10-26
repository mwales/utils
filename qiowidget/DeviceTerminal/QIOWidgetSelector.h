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

private slots:

    void indexChanged(int index);

private:



    QPushButton* theButton;

    QLabel* theLabel;

    QComboBox* theDeviceTypeCB;



    TcpConfigWidget* theTcpWidget;

    BluetoothConfigWidget* theBtWidget;

    SerialConfigWidget* theSerialWidget;
};

#endif // MULTIWIDGET_H
