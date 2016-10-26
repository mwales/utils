#ifndef SERIALCONFIGWIDGET_H
#define SERIALCONFIGWIDGET_H

#include "QIOWidget.h"
#include <QComboBox>

class SerialConfigWidget : public QIOWidget
{
    Q_OBJECT

public:
    SerialConfigWidget(QWidget *parent = 0);

    virtual QIODevice* getQIoDevice();

public slots:

    virtual void connectToDevice();

protected:

    QComboBox thePortCB;

    QComboBox theBaudRateCB;

};

#endif // SERIALCONFIGWIDGET_H
