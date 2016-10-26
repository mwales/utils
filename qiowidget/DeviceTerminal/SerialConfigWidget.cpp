#include "SerialConfigWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
SerialConfigWidget::SerialConfigWidget(QWidget *parent):
    QIOWidget(parent)
{
    QHBoxLayout* box = new QHBoxLayout(this);

    QVBoxLayout* portBox = new QVBoxLayout(this);
    QLabel* portLabel = new QLabel("Serial Port:", this);
    portBox->addWidget(portLabel);
    portBox->addWidget(&thePortCB);
    box->addLayout(portBox);

    QVBoxLayout* baudBox = new QVBoxLayout(this);
    QLabel* baudLabel = new QLabel("Baud Rate", this);
    baudBox->addWidget(baudLabel);
    baudBox->addWidget(&theBaudRateCB);
    box->addLayout(baudBox);
}

QIODevice* SerialConfigWidget::getQIoDevice()
{
    return nullptr;
}

void SerialConfigWidget::connectToDevice()
{

}
