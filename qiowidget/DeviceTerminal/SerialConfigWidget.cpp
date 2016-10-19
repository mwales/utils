#include "SerialConfigWidget.h"
#include <QHBoxLayout>

SerialConfigWidget::SerialConfigWidget(QWidget *parent):
    QIOWidget(parent)
{
    QHBoxLayout* box = new QHBoxLayout(this);
    box->addWidget(&thePortCB);
    box->addWidget(&theBaudRateCB);
}

QIODevice* SerialConfigWidget::GetQIoDevice()
{
    return nullptr;
}

void SerialConfigWidget::ConnectToDevice()
{

}
