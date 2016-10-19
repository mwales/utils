#ifndef TCPDESTINATIONWIDGET_H
#define TCPDESTINATIONWIDGET_H

#include "QIOWidget.h"
#include <QWidget>
#include <QLineEdit>

class TcpDestinationWidget : public QIOWidget
{
    Q_OBJECT

    Q_PROPERTY(int PortNumber WRITE setPortNumber READ getPortNumber)
    Q_PROPERTY(QString Hostname WRITE setHostname READ getHostname)


public:
    explicit TcpDestinationWidget(QWidget *parent = 0);

    void setPortNumber(int pn);

    void setHostname(QString hostname);

    int getPortNumber() const;
    QString getHostname() const;

    virtual QIODevice* getQIoDevice();
signals:

public slots:

    virtual void connect();

protected:
    QLineEdit theHostname;

    QLineEdit thePortNumber;

};

#endif // TCPDESTINATIONWIDGET_H
