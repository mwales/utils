#include "TcpDestinationWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QtDebug>

TcpDestinationWidget::TcpDestinationWidget(QWidget *parent) :
    QIOWidget(parent)
{
    QHBoxLayout* largestContainer = new QHBoxLayout(this);

    QVBoxLayout* hostnameContainer = new QVBoxLayout(this);
    QLabel* hostnameLabel = new QLabel("Hostname:", this);
    hostnameContainer->addWidget(hostnameLabel);
    hostnameContainer->addWidget(&theHostname);
    largestContainer->addLayout(hostnameContainer);

    QVBoxLayout* portContainer = new QVBoxLayout(this);
    QLabel* portLabel = new QLabel("Port:", this);
    portContainer->addWidget(portLabel);
    portContainer->addWidget(&thePortNumber);
    largestContainer->addLayout(portContainer);

    setLayout(largestContainer);
}


void TcpDestinationWidget::setPortNumber(int pn)
{
    thePortNumber.setText(QString::number(pn));
}

void TcpDestinationWidget::setHostname(QString hn)
{
    theHostname.setText(hn);
}

int TcpDestinationWidget::getPortNumber() const
{
    return thePortNumber.text().toInt();
}


QString TcpDestinationWidget::getHostname() const
{
    return theHostname.text();
}

QIODevice* TcpDestinationWidget::getQIoDevice()
{

}

void TcpDestinationWidget::connect()
{

}
