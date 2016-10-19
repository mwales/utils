#include "TcpConfigWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QtDebug>

TcpConfigWidget::TcpConfigWidget(QWidget *parent) :
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


void TcpConfigWidget::SetPortNumber(int pn)
{
    thePortNumber.setText(QString::number(pn));
}

void TcpConfigWidget::SetHostname(QString hn)
{
    theHostname.setText(hn);
}

int TcpConfigWidget::GetPortNumber() const
{
    return thePortNumber.text().toInt();
}


QString TcpConfigWidget::GetHostname() const
{
    return theHostname.text();
}

QIODevice* TcpConfigWidget::GetQIoDevice()
{

}

void TcpConfigWidget::ConnectToDevice()
{

}
